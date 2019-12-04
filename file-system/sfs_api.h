/**
 * sfs_api.h
 *
 * @author nxxinf
 * @github https://github.com/fangnx
 * @created 2019-11-20 20:42:51
 * @last-modified 2019-12-03 19:18:36
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "disk_emu.h"

// Constants.
#define MAX_FNAME_LENGTH 28
#define MAX_FILE_EXTENSION_LENGTH 4
#define BLOCK_SIZE 1024
#define NUM_DATA_BLOCKS 4096
#define NUM_DATA_BLOCKS_PER_INODE 12
// #define MAX_FILE_SIZE (NUM_DATA_BLOCKS_PER_INODE + 1) * BLOCK_SIZE

// Type definitions.
typedef struct type_block_ptr {
  int block_id;
  int block_end;
} block_ptr;

typedef struct type_inode {
  int file_size;
  int file_end;
  block_ptr data_blocks[12];      // 12 direct pointers.
  block_ptr singly_indirect_ptr;  // 1 singly indirect pointer.
  int link_count;
} inode;

typedef struct type_dir_entry {
  // sizeof(dir_entry): 4 + 60 = 64 Bytes.
  int inode_index;
  char fname[MAX_FNAME_LENGTH];
} dir_entry;

typedef struct type_block {
  // A block can store different types of things.
  union type_block_store {
    inode inodes[BLOCK_SIZE / sizeof(inode)];
    block_ptr block_ptrs[BLOCK_SIZE / sizeof(block_ptr)];
    dir_entry dir_entires[BLOCK_SIZE / sizeof(dir_entry)];
    uint8_t data[BLOCK_SIZE / sizeof(uint8_t)]
  } store;
} block;

typedef struct type_superblock {
  char signature[MAX_FNAME_LENGTH];
  int inode_root_index;
  int block_size;
  int file_system_size;
  int num_data_blocks;
  int num_inode_blocks;
} superblock;

typedef struct type_file_table_entry {
  char fname[MAX_FNAME_LENGTH];
  int read_ptr, write_ptr, inode_index;
} file_table_entry;

// SFS API
void mksfs(int fresh);
int sfs_getnextfilename(char *fname);
int sfs_getfilesize(const char *path);
int sfs_fopen(char *name);
int sfs_fclose(int fileID);
int sfs_frseek(int fileID, int loc);
int sfs_fwseek(int fileID, int loc);
int sfs_fwrite(int fileID, char *buf, int length);
int sfs_fread(int fileID, char *buf, int length);
int sfs_remove(char *file);