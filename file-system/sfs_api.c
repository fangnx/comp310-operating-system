/**
 * sfs_api.c
 *
 * McGill COMP 310
 * Mountable Simple File System
 *
 * @author nxxinf
 * @github https://github.com/fangnx
 * @created 2019-11-20 20:42:06
 * @last-modified 2019-12-03 00:14:42
 */

#include "sfs_api.h"

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Initialize constants.
static const int DIR_ENTRIES_PER_BLOCK = BLOCK_SIZE / sizeof(dir_entry);

static const block_ptr NULL_BLOCK_PTR = {
    .block_id = -1,
};
static const int NULL_INODE = -1;

// Initialize data structures for SFS.
static uint8_t bitmap[NUM_DATA_BLOCKS];
static file_table_entry file_descriptor_table[100];
static inode inode_arr[NUM_DATA_BLOCKS];
static dir_entry dir_entry_arr[NUM_DATA_BLOCKS];

// Initialize temp buffers.
static block block_buffer;
static block empty_buffer;

// Directory state tracker.
static int dir_curr;

static superblock sfs_superblock = {
    .block_size = BLOCK_SIZE,
    .num_data_blocks = NUM_DATA_BLOCKS,
    .num_inode_blocks = sizeof(inode) * NUM_DATA_BLOCKS / BLOCK_SIZE,
    .file_system_size =
        1 + sizeof(inode) * NUM_DATA_BLOCKS / BLOCK_SIZE + NUM_DATA_BLOCKS};

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// SFS helper functions.

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

/**
 * Allocate a free block.
 */
int alloc_block() {
  int free_block_index = NULL_BLOCK_PTR.block_id;
  for (int i = 0; i < sfs_superblock.num_data_blocks; i++) {
    if (bitmap[i] == 0) {
      free_block_index = i;
      break;
    }
  }
  if (free_block_index == NULL_BLOCK_PTR.block_id) {
    return -1;
  }
  bitmap[free_block_index] = 1;
  return free_block_index;
}

/**
 * Find file index in the directory entry arr, by fname.
 * Return the index, or -1 if not found.
 */
int find_dir_index_by_fname(char *fname) {
  for (int i = 0; i < sfs_superblock.num_data_blocks; i++) {
    if (strcmp(fname, dir_entry_arr[i].fname) == 0) {
      return i;
    }
  }
  return -1;
}

/**
 * Find file index in the file descriptor table, by inode_index.
 * Return the index, or -1 if not found.
 */
int find_fdt_index_by_inode(int inode_index) {
  for (int i = 0; i < sfs_superblock.num_data_blocks; i++) {
    if (inode_index == file_descriptor_table[i].inode_index) {
      return i;
    }
  }
  return -1;
}

int find_free_inode() {
  // Start from index 2, since index 1 is the root dir.
  for (int i = 2; i < sfs_superblock.num_data_blocks; i++) {
    if (inode_arr[i].link_count < 1) {
      return i;
    }
  }
  return -1;
}

int find_free_dir_entry() {
  for (int i = 0; i < sfs_superblock.num_data_blocks; i++) {
    if (dir_entry_arr[i].inode_index == NULL_INODE) {
      return i;
    }
  }
  return -1;
}

int find_free_fdt_entry() {
  for (int i = 0; i < sfs_superblock.num_data_blocks; i++) {
    if (file_descriptor_table[i].inode_index == NULL_INODE) {
      return i;
    }
  }
  return -1;
}

/**
 * Return the start address of the given block_id.
 */
int parse_start_addr(int block_id) {
  return 1 + sfs_superblock.num_inode_blocks + block_id;
}

void init_new_file(int inode_index) {
  block_ptr data_blocks[NUM_DATA_BLOCKS_PER_INODE];
  for (int i = 0; i < NUM_DATA_BLOCKS_PER_INODE; i++) {
    data_blocks[i] = NULL_BLOCK_PTR;
  }
  memcpy(inode_arr[inode_index].data_blocks, data_blocks, sizeof(int) * 12);
  inode_arr[inode_index].singly_indirect_ptr = NULL_BLOCK_PTR;
  inode_arr[inode_index].file_size = 0;
  inode_arr[inode_index].file_end = 0;
  inode_arr[inode_index].link_count = 1;
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// SFS API implementations.

/**
 * Initialize the simple file system.
 */
void mksfs(int fresh) {
  // The file system should be created from scratch.
  if (fresh) {
    // Initialize a new disk.
    init_fresh_disk(sfs_superblock.signature, sfs_superblock.block_size,
                    sfs_superblock.file_system_size);
    // Initialize the inode of index 1 to represent the root directory.
    inode_arr[1].file_size = 0;
    inode_arr[1].file_end = 0;
    inode_arr[1].link_count = 1;
    block_ptr data_blocks[NUM_DATA_BLOCKS_PER_INODE];
    for (int i = 0; i < NUM_DATA_BLOCKS_PER_INODE; i++) {
      data_blocks[i] = NULL_BLOCK_PTR;
    }
    memcpy(inode_arr[1].data_blocks, data_blocks, sizeof(int) * 12);
    inode_arr[1].singly_indirect_ptr = NULL_BLOCK_PTR;
  }
  // The file system should be opened from the disk.
  else {
    // Initialize an existing disk.
    init_disk(sfs_superblock.signature, sfs_superblock.num_data_blocks,
              sfs_superblock.file_system_size);
  }

  // Initialize bitmap.
  for (int i = 0; i < sfs_superblock.num_data_blocks; i++) {
    bitmap[i] = 0;
  }
  // Initialize directory entries.
  for (int i = 0; i < sfs_superblock.num_data_blocks; i++) {
    dir_entry_arr[i].inode_index = NULL_INODE;
  }
  // Initialize file descriptor table entries.
  for (int i = 0; i < sfs_superblock.num_data_blocks; i++) {
    file_descriptor_table[i].inode_index = NULL_INODE;
  }
}

/**
 * Copy the name of the next file in the directory -> fname
 * Return non-zero if there is a new file.
 */
int sfs_getnextfilename(char *fname) {
  dir_entry file_dir_entry;
  while (dir_curr < sfs_superblock.num_inode_blocks) {
    file_dir_entry = dir_entry_arr[dir_curr];
    if (file_dir_entry.inode_index != NULL_INODE) {
      // Copy the file name.
      strcpy(fname, file_dir_entry.fname);
      dir_curr += 1;
      return 1;
    }
    dir_curr += 1;
  }
  return 0;
}

/**
 * Return the size of the given file.
 */
int sfs_getfilesize(const char *path) {
  inode file_inode;
  for (int i = 0; i < sfs_superblock.num_data_blocks; i++) {
    // Find a match for file/path name in directory entry arr.
    if (strcmp(path, dir_entry_arr[i].fname) == 0) {
      file_inode = inode_arr[dir_entry_arr[i].inode_index];
      return file_inode.file_size;
    }
  }
  return -1;
}

/**
 * Open/create a file.
 * Return an integer that corresponds to the index of the entry.
 */
int sfs_fopen(char *name) {
  block_ptr block_to_open;
  int file_index = find_dir_index_by_fname(name);

  // Find free slot of dir entry.
  int valid_dir_entry_index;
  valid_dir_entry_index = find_free_dir_entry();
  // valid_inode_index = find_free_inode();
  // valid_fdt_entry_index = find_free_fdt_entry();
  if (valid_dir_entry_index == -1) {
    return -1;
  }

  int inode_index;
  inode_index = dir_entry_arr[valid_dir_entry_index].inode_index;

  // If a file does not exist -> create a new file.
  if (file_index < 0) {
    if (strlen(name) > MAX_FNAME_LENGTH) {
      return -1;
    }
    // Init new file.
    init_new_file(inode_index);

    // Allocate a valid directory entry to the newly created file.
    int block_num =
        valid_dir_entry_index / DIR_ENTRIES_PER_BLOCK;  // From 1 to 13.
    // Case: block is pointed by the 12 direct block
    if (block_num < 12) {
      block_to_open = inode_arr[1].data_blocks[block_num];
      // If that data block is null -> allocate a new one.
      if (block_to_open.block_id == NULL_BLOCK_PTR.block_id) {
        inode_arr[1].data_blocks[block_num].block_id = alloc_block();
        if (inode_arr[1].data_blocks[block_num].block_id ==
            NULL_BLOCK_PTR.block_id) {
          return -1;
        }
        // Init all dir entries in the newly created block NULL.
        for (int i = 0; i < DIR_ENTRIES_PER_BLOCK; i++) {
          dir_entry_arr[valid_dir_entry_index + i].inode_index = NULL_INODE;
        }
      }
    }
    // Case: block is pointed by the singly indirect block pointer.
    else {
      block_to_open = inode_arr[1].singly_indirect_ptr;
      int start_addr = parse_start_addr(block_to_open.block_id);
      // If the singly indirect pointer is null -> allocate a new one.
      if (inode_arr[1].singly_indirect_ptr.block_id ==
          NULL_BLOCK_PTR.block_id) {
        inode_arr[1].singly_indirect_ptr.block_id = alloc_block();

        read_blocks(start_addr, 1, &block_buffer);
        for (int i = 0; i < DIR_ENTRIES_PER_BLOCK; i++) {
          block_buffer.store.block_ptrs[i] = NULL_BLOCK_PTR;
        }
        write_blocks(start_addr, 1, &block_buffer);
      }

      int valid_block_index = valid_dir_entry_index / DIR_ENTRIES_PER_BLOCK;
      read_blocks(start_addr, 1, &block_buffer);
      // If block ptr is NULL -> allocate a new block.
      if (block_buffer.store.block_ptrs[valid_block_index].block_id ==
          NULL_BLOCK_PTR.block_id) {
        block_buffer.store.block_ptrs[valid_block_index].block_id =
            alloc_block();
        if (block_buffer.store.block_ptrs[valid_block_index].block_id ==
            NULL_BLOCK_PTR.block_id) {
          return -1;
        }
        for (int i = 0; i < DIR_ENTRIES_PER_BLOCK; i++) {
          dir_entry_arr[valid_dir_entry_index + i].inode_index = NULL_INODE;
        }
      }
      write_blocks(start_addr, 1, &block_buffer);
    }

    // Update directory info with the newly created file.
    strcpy(dir_entry_arr[valid_dir_entry_index].fname, name);
    dir_entry_arr[valid_dir_entry_index].inode_index = inode_index;
  }

  // Update the file descriptor table.
  // Read file ptr -> beginning of the file,
  // Write file ptr -> end of the file.
  int fdt_index;
  fdt_index = find_fdt_index_by_inode(inode_index);
  if (fdt_index == -1) {
    return -1;
  }

  file_descriptor_table[fdt_index].inode_index = inode_index;
  file_descriptor_table[fdt_index].read_ptr = 0;
  file_descriptor_table[fdt_index].write_ptr = inode_arr[inode_index].file_end;

  return fdt_index;
}

/**
 * Close the file.
 * Remove the entry from the open file descriptor table.
 */
int sfs_fclose(int fileID) {
  if (fileID < 0) {
    return -1;
  }
  if (file_descriptor_table[fileID].inode_index != NULL_INODE) {
    // Clear the fdt entry.
    memset(&file_descriptor_table[fileID], 0, sizeof(file_table_entry));
    file_descriptor_table[fileID].read_ptr = 0;
    file_descriptor_table[fileID].write_ptr = 0;
    file_descriptor_table[fileID].inode_index = NULL_INODE;

    return 0;
  }
  return -1;
}

/**
 * Move the read pointer.
 */
int sfs_frseek(int fileID, int loc) {
  if (fileID < 0) {
    return -1;
  }
  file_table_entry file_entry = file_descriptor_table[fileID];
  if (file_entry.inode_index != NULL_INODE) {
    int file_inode_index = file_descriptor_table[fileID].inode_index;
    // Check if loc is valid.
    if (loc < 0 || loc > MAX_FILE_SIZE ||
        loc > inode_arr[file_inode_index].file_size) {
      return -1;
    }
    file_descriptor_table[fileID].read_ptr = loc;
    return 0;
  }
  return -1;
}

/**
 * Move the write pointer.
 */
int sfs_fwseek(int fileID, int loc) {
  if (fileID < 0) {
    return -1;
  }
  file_table_entry file_entry = file_descriptor_table[fileID];
  if (file_descriptor_table[fileID].inode_index != NULL_BLOCK_PTR.block_id &&
      0 <= loc && loc <= BLOCK_SIZE * (12 + BLOCK_SIZE / sizeof(block_ptr))) {
    return 0;
  }
  return -1;
}

/**
 * Return the number of bytes written.
 */
int sfs_fwrite(int fileID, char *buf, int length) {
  memset(&block_buffer, 0, sizeof(block_ptr));

  // Check if file is present in fdt.
  if (file_descriptor_table[fileID].inode_index == NULL_INODE) {
    return 0;
  }
  int num_bytes_written, num_bytes_to_write;
  block_ptr block_to_write;
  num_bytes_written = 0;
  num_bytes_to_write = 0;

  // Find the file inode.
  inode file_inode = inode_arr[file_descriptor_table[fileID].inode_index];

  while (num_bytes_written < length) {
    // ?
    if (file_descriptor_table[fileID].write_ptr >
        BLOCK_SIZE * (12 + BLOCK_SIZE / sizeof(block_ptr))) {
      break;
    }
    // block_num: index of block wrt table.
    // block_index: index inside that block.
    int block_num = file_descriptor_table[fileID].write_ptr / BLOCK_SIZE;
    int block_index = file_descriptor_table[fileID].write_ptr % BLOCK_SIZE;
    int num_bytes_to_write =
        MIN(BLOCK_SIZE - block_index, length - num_bytes_written);
    // Case: block is pointed by one of the 12 direct blocks.
    if (block_num < 12) {
      block_ptr assigned_block = file_inode.data_blocks[block_num];
      // If allocated block is NULL -> allocate a new one.
      if (assigned_block.block_id == NULL_BLOCK_PTR.block_id) {
        assigned_block.block_id = alloc_block();
        if (assigned_block.block_id == NULL_BLOCK_PTR.block_id) {
          break;
        }
      }
      // Update block end marker.
      assigned_block.block_end =
          MAX(block_index + num_bytes_to_write, assigned_block.block_end);
      block_to_write = assigned_block;
    }
    // Case: block is pointed by the singly indirect block pointer.
    else {
      block_ptr assigned_block = file_inode.singly_indirect_ptr;
      int start_addr = parse_start_addr(assigned_block.block_id);
      // If the allocated block is NULL -> allocate a new one.
      if (assigned_block.block_id == NULL_BLOCK_PTR.block_id) {
        assigned_block.block_id = alloc_block();
        if (assigned_block.block_id == NULL_BLOCK_PTR.block_id) {
          break;
        }

        // Init all block pointers in the block to NULL.
        for (int i = 0; i < (BLOCK_SIZE / sizeof(block_ptr)); i++) {
          block_buffer.store.block_ptrs[i] = NULL_BLOCK_PTR;
        }
        if ((write_blocks(start_addr, 1, &block_buffer) != 1)) {
          break;
        };
      }
      // Read block from the disk -> block_buffer.
      if ((read_blocks(start_addr, 1, &block_buffer) != 1)) {
        break;
      }
      // If data block pointed by the index block is NULL -> allocate a new one.
      if (block_buffer.store.block_ptrs[block_num - 12].block_id =
              NULL_BLOCK_PTR.block_id) {
        block_buffer.store.block_ptrs[block_num - 12].block_id = alloc_block();
        if (block_buffer.store.block_ptrs[block_num - 12].block_id ==
            NULL_BLOCK_PTR.block_id) {
          break;
        }
      }
      // Update block end marker.
      // for (int i = 0; i < (BLOCK_SIZE / sizeof(block_ptr)); i++) {
      //   block_buffer.store.block_ptrs[i - 12].block_id = alloc_block();
      // }
      block_buffer.store.block_ptrs[block_num - 12].block_end =
          MAX(block_index + num_bytes_to_write,
              block_buffer.store.block_ptrs[block_num - 12].block_end);
      // Write block from block_buffer -> disk.
      if ((write_blocks(start_addr, 1, &block_buffer) != -1)) {
        break;
      }
    }

    int start_addr = parse_start_addr(block_to_write.block_id);
    if ((read_blocks(start_addr, 1, &block_buffer)) != 1) {
      break;
    }
    // Write the actual data from the given buffer -> disk.
    memcpy(&(block_buffer.store.data[block_index]), buf + num_bytes_written,
           num_bytes_to_write);
    if ((write_blocks(start_addr, 1, &block_buffer)) != 1) {
      break;
    }
    // Update after each iteration.
    num_bytes_written += num_bytes_to_write;
    file_descriptor_table[fileID].write_ptr += num_bytes_to_write;
  }

  // Update file descriptor table.
  inode_arr[file_descriptor_table[fileID].inode_index].file_size = 1;
  inode_arr[file_descriptor_table[fileID].inode_index].file_end =
      MAX(file_descriptor_table[fileID].write_ptr,
          inode_arr[file_descriptor_table[fileID].inode_index].file_end);

  return num_bytes_written;
}

/**
 * Return number of bytes read.
 */
int sfs_fread(int fileID, char *buf, int length) {
  memset(&block_buffer, 0, sizeof(block));

  if (fileID < 0 || fileID > sfs_superblock.num_data_blocks - 1) {
    return 0;
  }
  if (file_descriptor_table[fileID].inode_index == NULL_INODE) {
    return 0;
  }
  int num_bytes_read, num_bytes_to_read;
  int block_index, block_num;
  block_ptr block_to_read;
  num_bytes_read = 0;
  num_bytes_to_read = 0;

  inode file_inode = inode_arr[file_descriptor_table[fileID].inode_index];

  while (num_bytes_read < length) {
    if (file_descriptor_table[fileID].read_ptr >= file_inode.file_end) {
      break;
    }
    // block_num: index of block wrt table.
    // block_index: index inside that block.
    block_index = file_descriptor_table[fileID].read_ptr % BLOCK_SIZE;
    block_num = file_descriptor_table[fileID].read_ptr / BLOCK_SIZE;
    num_bytes_to_read =
        MIN(MIN(BLOCK_SIZE - block_index, length - num_bytes_read),
            file_inode.file_end - file_descriptor_table[fileID].read_ptr);
    // Case: block is pointed by one of the 12 direct blocks.
    if (block_num < 12) {
      if (file_inode.data_blocks[block_num].block_id ==
          NULL_BLOCK_PTR.block_id) {
        memset(buf + num_bytes_read, 0, sizeof(char) * num_bytes_to_read);
      } else {
        block_ptr assigned_block = file_inode.data_blocks[block_num];
        int start_addr = parse_start_addr(assigned_block.block_id);
        read_blocks(start_addr, 1, &block_buffer);
        // With block in block_buffer, copy data from block_buffer to the given
        // buffer.
        memcpy(buf + num_bytes_read, &block_buffer.store.data[block_index],
               num_bytes_to_read);
      }
    }
    // Case: block is pointed by the singly indirect block pointer.
    else {
      if (file_inode.singly_indirect_ptr.block_id == NULL_BLOCK_PTR.block_id) {
        memset(buf + num_bytes_read, 0, sizeof(char) * num_bytes_to_read);
      } else {
        block_ptr assigned_block = file_inode.singly_indirect_ptr;
        int start_addr = parse_start_addr(assigned_block.block_id);

        read_blocks(start_addr, 1, &block_buffer);
        if (block_buffer.store.block_ptrs[block_num - 12].block_id ==
            NULL_BLOCK_PTR.block_id) {
          memset(buf + num_bytes_read, 0, sizeof(char) * num_bytes_to_read);
        }
        // With block in block_buffer, copy data from block_buffer to the given
        // buffer.
        read_blocks(start_addr, 1, &block_buffer);
        memcpy(buf + num_bytes_read, &block_buffer.store.data[block_index],
               num_bytes_to_read);
      }
    }
    // Update info after each iter.
    num_bytes_read += num_bytes_to_read;
    // Update read pointer in fdt.
    file_descriptor_table[fileID].read_ptr += num_bytes_to_read;
  }
  return num_bytes_read;
}

/**
 * Remove the file from the directory entry.
 * Release the file from file descriptor table & release the data blocks used by
 * file.
 */
int sfs_remove(char *file) {
  memset(&empty_buffer, 0, sizeof(block));

  int file_index, fdt_index, start_addr;
  // Get the file index in the directory.
  file_index = find_dir_index_by_fname(file);
  if (file_index >= 0) {
    // Locate the dir_entry & inode.
    dir_entry file_dir_entry = dir_entry_arr[file_index];
    int file_inode_index = file_dir_entry.inode_index;
    inode file_inode = inode_arr[file_inode_index];
    // Clear the data blocks, and update bitmap.
    // Case: first 12 direct blocks.
    int temp_bid;
    for (int i = 0; i < 12; i++) {
      temp_bid = file_inode.data_blocks[i].block_id;
      if (temp_bid != NULL_BLOCK_PTR.block_id) {
        start_addr = parse_start_addr(temp_bid);
        write_blocks(start_addr, 1, &empty_buffer);

        bitmap[temp_bid] = 0;
      }
    }
    // Case: the singly indirect block ptr.
    temp_bid = file_inode.singly_indirect_ptr.block_id;
    if (temp_bid != NULL_BLOCK_PTR.block_id) {
      start_addr = parse_start_addr(temp_bid);
      read_blocks(start_addr, 1, &block_buffer);

      for (int i = 0; i < (BLOCK_SIZE / sizeof(block_ptr)); i++) {
        temp_bid = block_buffer.store.block_ptrs[i].block_id;
        if (temp_bid != NULL_BLOCK_PTR.block_id) {
          start_addr = parse_start_addr(temp_bid);
          write_blocks(start_addr, 1, &empty_buffer);

          bitmap[temp_bid] = 0;
        }
      }
    }
    // Reset inode.
    memset(&file_inode, 0, sizeof(inode));
    // Reset dir entry.
    memset(&dir_entry_arr[file_index], 0, sizeof(dir_entry));
    dir_entry_arr[file_index].inode_index = NULL_INODE;

    // Close the file if opened.
    fdt_index = find_fdt_index_by_inode(file_inode_index);
    if (fdt_index < 0) {
      sfs_fclose(fdt_index);
    }

    return 0;
  }
  return -1;
}