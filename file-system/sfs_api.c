#include <stdio.h>
#include <stdlib.h>

#define MAX_FNAME_LENGTH 16
#define MAX_FILE_EXTENSION_LENGTH 4

#define BLOCK_SIZE 1024;
#define NUM_DATA_BLOCKS 8192;

#define NUM_DATA_BLOCKS_PER_INODE 12;

// Declare types.
typedef struct type_block_ptr {
  int block_id;
} block_ptr;

typedef struct type_block {
  int data[BLOCK_SIZE / sizeof(int)];
} block;

typedef struct type_inode {
  int file_size;
  block_ptr data_blocks[NUM_DATA_BLOCKS_PER_INODE];
  block_ptr singly_indirect_ptr;
  int link_count;
} inode;

typedef struct type_dir_entry {
  int inode_index;
  char fname[MAX_FNAME_LENGTH];
} dir_entry;

typedef struct type_superblock {
  char signature[MAX_FNAME_LENGTH];
  int inode_root_index;

  int num_inode_blocks;
  int num_data_blocks;
  int block_size;
  int file_system_size;
} superblock;

typedef struct type_open_file_entry {
  char fname[MAX_FNAME_LENGTH];
  int read_ptr, write_ptr, inode_index;
} open_file_entry;

// Initialize constants.
static const NULL_BLOCK_PTR = {
    .block_id = -1,
};

// Initialize data structures for SFS.
static open_file_entry open_file_table[100];

static inode inode_cache[NUM_DATA_BLOCKS];
static dir_entry dir_entry_cache[NUM_DATA_BLOCKS];

static superblock sfs_superblock = {
    .block_size = BLOCK_SIZE,
    .num_data_blocks = NUM_DATA_BLOCKS,
    .num_inode_blocks = sizeof(inode) * NUM_DATA_BLOCKS / BLOCK_SIZE,
    .file_system_size = 10000};

void mksfs(int fresh) {}

/**
 * Copy the name of the next file in the directory -> fname
 * Return non-zero if there is a new file.
 */
int sfs_getnextfilename(char *fname) {}

int sfs_getfilesize(const char *path) {}

// Check if the file exists.
int file_exists(char *fname) {
  for (int i = 0; i < sfs_superblock.num_data_blocks; i++) {
    if (strcmp(fname, directory_cache[i].fname) == 0) {
      return i;
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
  // If file exists -> open file in append mode.
  // Align write ptr -> end of file,
  //        read ptr -> start of file.
  if (file_index >= 0) {
    //
  }
  // If a file does not exist -> create a new file.
  else {
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
    for (int i = 0; i < sfs_superblock.num_data_blocks; i++) {
      if (inode_cache[i].link_count```9 ` < 1) {
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
    // Put the newly created file to the valid dir entry.

  }
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
int sfs_fwseek(int fileID, int loc) {}

int sfs_fwrite(int fileID, char *buf, int length) {}

int sfs_fread(int fileID, char *buf, int length) {}

int sfs_remove(char *file) {}