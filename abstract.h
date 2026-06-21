#ifndef ABSTRACT_H
#define ABSTRACT_H

#include <stddef.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_FILENAME_LENGTH 255
#define MAX_FILES 10
#define MAX_BLOCKS 100
#define BLOCK_SIZE 512

typedef struct DirectoryEntry {
    char name[MAX_FILENAME_LENGTH];
    size_t size;
    time_t creation_time;
    int inode_index;
} DirectoryEntry;

typedef struct Directory {
    DirectoryEntry entries[MAX_FILES];
    int entry_count;
} Directory;

typedef struct {
    int size;
    int first_block;
    char name[MAX_FILENAME_LENGTH];
} Inode;

typedef struct {
    int num_inodes;
    int num_blocks;
    int block_size;
} Superblock;

typedef struct {
    int next_block;
} Block;

extern Directory root_directory;
extern Superblock superblock;
extern Inode inodes[MAX_FILES];
extern Block blocks[MAX_BLOCKS];

void initialize_storage();
int create_file(const char *filename, int inode_index);
int save_file(const char *filename, const char *data, size_t size, int inode_index);
int load_file(const char *filename, char **data, size_t *size);
int delete_file(const char *filename);
int rename_file(const char *old_filename, const char *new_filename);
void list_files(const Directory *directory);
void show_disk_storage();

#endif // ABSTRACT_H
