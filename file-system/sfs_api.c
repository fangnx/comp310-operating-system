/**
 * sfs_api.c
 *
 * McGill COMP 310
 * Mountable Simple File System
 *
 * @author nxxinf
 * @github https://github.com/fangnx
 * @created 2019-11-20 20:42:06
 * @last-modified 2019-12-01 11:16:47
 */

#include "sfs_api.h"

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Initialize constants.
static const DIR_ENTRIES_PER_BLOCK = BLOCK_SIZE / sizeof(dir_entry);
static const NULL_BLOCK_PTR = {
    .block_id = -1,
};
static const NULL_INODE = -1;

// Initialize data structures for SFS.
static open_file_entry file_descriptor_table[100];

static inode inode_cache[NUM_DATA_BLOCKS];
static dir_entry dir_entry_cache[NUM_DATA_BLOCKS];

static block_ptr block_buffer;

static superblock sfs_superblock = {
    .block_size = BLOCK_SIZE,
    .num_data_blocks = NUM_DATA_BLOCKS,
    .num_inode_blocks = sizeof(inode) * NUM_DATA_BLOCKS / BLOCK_SIZE,
    .file_system_size =
        1 + sizeof(inode) * NUM_DATA_BLOCKS / BLOCK_SIZE + NUM_DATA_BLOCKS};

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// SFS helper functions.

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
 * Check if the file exists.
 */
int file_exists(char *fname) {
  for (int i = 0; i < sfs_superblock.num_data_blocks; i++) {
    if (strcmp(fname, directory_cache[i].fname) == 0) {
      return i;
    }
  }
  return -1;
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
    // Initialize the first inode to represent the root directory.
    inode_cache[1].file_size = 0;
    inode_cache[1].eof = 0;
    inode_cache[1].link_count = 1;
    block_ptr data_blocks[NUM_DATA_BLOCKS_PER_INODE];
    for (int i = 0; i < NUM_DATA_BLOCKS_PER_INODE; i++) {
      data_blocks[i] = NULL_BLOCK_PTR;
    }
    memcpy(inode_cache[1].data_blocks, data_blocks, sizeof(int) * 12);
    inode_cache[1].singly_indirect_ptr = NULL_BLOCK_PTR;

    // Initialize entries in the file descriptor table to NULL.
    for (int i = 0; i < sfs_superblock.num_data_blocks; i++) {
      file_descriptor_table[i].inode_index = NULL_INODE;
    }
    // Initialize entries in the directory to NULL.
    for (int i = 0; i < sfs_superblock.num_data_blocks; i++) {
      dir_entry_cache[i].inode_index = NULL_INODE;
    }
  }
  // The file system should be opened from the disk.
  else {
    // Initialize an existing disk.
    init_disk(sfs_superblock.signature, sfs_superblock.num_data_blocks,
              sfs_superblock.file_system_size);
  }
  for (int i = 0; i < sfs_superblock.num_data_blocks; i++) {
    dir_entry_cache[i].inode_index = NULL_INODE;
  }
  for (int i = 0; i < sfs_superblock.num_data_blocks; i++) {
    file_descriptor_table[i].inode_index = NULL_INODE;
  }
}

/**
 * Copy the name of the next file in the directory -> fname
 * Return non-zero if there is a new file.
 */
int sfs_getnextfilename(char *fname) {}

/**
 * Get the size of the given file.
 */
int sfs_getfilesize(const char *path) {
  for (int i = 0; i < sfs_superblock.num_data_blocks; i++) {
    if (!strcmp(path, dir_entry_cache[i])) {
      return inode_cache[dir_entry_cache[i].inode_index].file_size;
    }
  }
  return -1;
}

/**
 * Open/create a file.
 * Return an integer that corresponds to the index of the entry.
 */
int sfs_fopen(char *name) {
  int file_index = file_exists(name);
  // If a file does not exist -> create a new file.
  if (file_index < 0) {
    if (strlen(name) > MAX_FNAME_LENGTH) {
      return -1;
    }
    int valid_dir_entry_index, valid_inode_index;
    valid_dir_entry_index = -1;
    valid_inode_cache_index = -1;
    // Find an empty dir entry.
    for (int i = 0; i < sfs_superblock.num_data_blocks; i++) {
      if (dir_entry_cache[i].inode_index == -1) {
        valid_dir_entry_index = i;
        break;
      }
    }
    // Find an available inode.
    // Start from index 2, since index 1 is the root dir.
    for (int i = 2; i < sfs_superblock.num_data_blocks; i++) {
      if (inode_cache[i].link_count < 1) {
        valid_inode_index = i;
        break;
      }
    }
    if (valid_dir_entry_index == -1) {
      return -1;
    }
    // Init new file.
    block_ptr data_blocks[NUM_DATA_BLOCKS_PER_INODE];
    for (int i = 0; i < NUM_DATA_BLOCKS_PER_INODE; i++) {
      data_blocks[i] = NULL_BLOCK_PTR;
    }
    memcpy(inode_cache[valid_inode_index].data_blocks, data_blocks,
           sizeof(int) * 12);
    inode_cache[valid_inode_index].singly_indirect_ptr = NULL_BLOCK_PTR;
    inode_cache[valid_inode_index].file_size = 0;
    inode_cache[valid_inode_index].link_count = 1;

    // Allocate a valid directory entry to the newly created file.
    int block_num ==
        valid_dir_entry_index / DIR_ENTRIES_PER_BLOCK;  // From 1 to 13.
    // If dir_entry_index is within the range pointed by the 12 direct block
    // pointers.
    if (block_num < 12) {
      block_ptr alloc_data_block = inode_cache[1].data_blocks[block_num];
      // If that data block is null -> allocate a new one.
      if (alloc_data_block.block_id == NULL_BLOCK_PTR) {
        inode_cache[1].data_blocks[block_num].block_id = alloc_block();
        if (inode_cache[1].data_blocks[block_num].block_id ==
            NULL_BLOCK_PTR.block_id) {
          return -1;
        }
        // Init all dir entries in the newly created block NULL.
        for (int i = 0; i < DIR_ENTRIES_PER_BLOCK; i++) {
          dir_entry_cache[valid_dir_entry_index + i].inode_index = NULL_INODE;
        }
      }
    }
    // If dir_entry_index is in the range of the singly indirect pointer.
    else {
      // If the singly indirect pointer is null -> allocate a new one.
      if (inode_cache[1].singly_indirect_ptr.block_id ==
          NULL_BLOCK_PTR.block_id) {
        inode_cache[1].singly_indirect_ptr.block_id = alloc_block();
        int start_addr = 1 + sfs_superblock.num_inode_blocks +
                         inode_cache[1].index_block.block_id;

        read_blocks(start_addr, 1, $block_buffer);
        for (int i = 0; i < DIR_ENTRIES_PER_BLOCK; i++) {
          block_buffer.content.index[i] = NULL_BLOCK_PTR;
        }
        write_blocks(start_addr, 1, &block_buffer);
      }

      read_blocks(start_addr, 1, &block_buffer);
      if (block_buffer.content.index[file_dir_index / DIR_ENTRIES_PER_BLOCK]
              .block_id == NULL_BLOCK_PTR) {
        block_buffer.content.index[file_dir_index / DIR_ENTRIES_PER_BLOCK]
            .block_id = alloc_block();
        if (block_buffer.content
                .index[file_dir_index / DIR_ENTRIES_PER_BLOCK] ==
            NULL_BLOCK_PTR) {
          return -1;
        }
        for (int i = 0; i < DIR_ENTRIES_PER_BLOCK; i++) {
          dir_entry_cache[file_dir_index + i].inode_index = NULL_INODE;
        }
      }
      write_blocks(start_addr, 1, &block_buffer);
    }
  }
  // Open file in append mode.
  int file_entry_index, file_dir_index;
  file_entry_index = NULL_INODE;
  file_dir_index = NULL_INODE;
  // Find an empty slot in the file descriptor table.
  for (int i = 0; i < sfs_superblock.num_data_blocks; i++) {
    if (file_descriptor_table[i].inode_index == NULL_INODE) {
      file_entry_index = i;
      break;
    }
  }
  // If the file descriptor table is full.
  if (file_entry_index == NULL_INODE) {
    return -1;
  }
  // Find an empty slot in the dir entries.
  for (int i = 0; i < sfs_superblock.num_data_blocks; i++) {
    if (strcmp) {
    }
  }
  // If no empty slot in dir entries.
  if (file_dir_index == NULL_INODE) {
    return -1;
  }
  // Update the file descriptor table.
  // Read file ptr -> beginning of the file,
  // Write file ptr -> end of the file.
  file_descriptor_table[file_entry_index].inode_index =
      dir_entry_cache[file_dir_index].inode_index;
  file_descriptor_table[file_entry_index].read_ptr = 0;
  file_descriptor_table[file_entry_index].write_ptr =
      inode_cache[file_descriptor_table[file_entry_index].inode_index].eof;

  return file_entry_index;
}

/**
 * Close a file.
 * Remove the entry from the open file descriptor table.
 */
int sfs_fclose(int fileID) {}

/**
 * Move the read ptr.
 */
int sfs_frseek(int fileID, int loc) {}

/**
 * Move the write ptr.
 */
int sfs_fwsdeek(int fileID, int loc) {}

int sfs_fwrite(int fileID, char *buf, int length) {}

int sfs_fread(int fileID, char *buf, int length) {}

int sfs_remove(char *file) {}