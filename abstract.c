#include <iostream>
#include <cstring>
#include <ctime>
#include <cstdlib>

#define MAX_FILES 100
#define MAX_BLOCKS 1000
#define BLOCK_SIZE 512
#define MAX_FILENAME_LENGTH 64

struct DirectoryEntry {
    char name[MAX_FILENAME_LENGTH];
    size_t size;
    time_t creation_time;
    int inode_index;
};

struct Directory {
    DirectoryEntry entries[MAX_FILES];
    int entry_count;
};

struct Inode {
    int size;
    int first_block;
    char name[MAX_FILENAME_LENGTH];
};

struct Block {
    int next_block;
};

struct Superblock {
    int num_inodes;
    int num_blocks;
    int block_size;
};

Directory root_directory;
Superblock superblock;
Inode inodes[MAX_FILES];
Block blocks[MAX_BLOCKS];

void initialize_storage() {
    root_directory.entry_count = 0;

    superblock.num_inodes = MAX_FILES;
    superblock.num_blocks = MAX_BLOCKS;
    superblock.block_size = BLOCK_SIZE;

    for (int i = 0; i < MAX_FILES; ++i) {
        inodes[i].size = -1;
        inodes[i].first_block = -1;
        strcpy(inodes[i].name, "empty");
    }

    for (int i = 0; i < MAX_BLOCKS; ++i) {
        blocks[i].next_block = -1;
    }
}

int create_file(const char *filename, int inode_index) {
    if (filename == NULL || strlen(filename) == 0 || strlen(filename) >= MAX_FILENAME_LENGTH) {
        return -1; // Invalid filename
    }

    if (inode_index < 0 || inode_index >= MAX_FILES) {
        return -4; // Invalid inode index
    }

    if (inodes[inode_index].size != -1) {
        return -2; // Inode already in use
    }

    for (int i = 0; i < root_directory.entry_count; ++i) {
        if (strcmp(root_directory.entries[i].name, filename) == 0) {
            return -3; // File already exists
        }
    }

    if (root_directory.entry_count >= MAX_FILES) {
        return -5; // Directory is full
    }

    DirectoryEntry new_entry;
    strncpy(new_entry.name, filename, MAX_FILENAME_LENGTH);
    new_entry.size = 0;
    new_entry.creation_time = time(NULL);
    new_entry.inode_index = inode_index;

    root_directory.entries[root_directory.entry_count++] = new_entry;

    inodes[inode_index].size = 0;
    inodes[inode_index].first_block = -1;
    strncpy(inodes[inode_index].name, filename, MAX_FILENAME_LENGTH);

    return 0; // Success
}

int save_file(const char *filename, const char *data, size_t size, int inode_index) {
    for (int i = 0; i < root_directory.entry_count; ++i) {
        if (strcmp(root_directory.entries[i].name, filename) == 0) {
            root_directory.entries[i].size = size;
            root_directory.entries[i].creation_time = time(NULL);
            root_directory.entries[i].inode_index = inode_index;

            inodes[inode_index].size = size;
            // Here you should set the actual data block index
            // For simplicity, we're setting a dummy first block
            inodes[inode_index].first_block = i % MAX_BLOCKS;
            strncpy(inodes[inode_index].name, filename, MAX_FILENAME_LENGTH);

            // Here we should save data to storage, but this is just a simulation
            return 0; // Success
        }
    }
    return -1; // File not found
}

int load_file(const char *filename, char **data, size_t *size) {
    for (int i = 0; i < root_directory.entry_count; ++i) {
        if (strcmp(root_directory.entries[i].name, filename) == 0) {
            *size = root_directory.entries[i].size;
            *data = (char*)malloc(*size + 1);
            if (*data == NULL) {
                return -2; // Memory allocation failed
            }
            // Here we should load data from storage, but this is just a simulation
            strncpy(*data, "Dummy data content", *size);
            (*data)[*size] = '\0';
            return 0; // Success
        }
    }
    return -1; // File not found
}

int delete_file(const char *filename) {
    for (int i = 0; i < root_directory.entry_count; ++i) {
        if (strcmp(root_directory.entries[i].name, filename) == 0) {
            root_directory.entries[i].size = 0;
            inodes[root_directory.entries[i].inode_index].size = -1;
            inodes[root_directory.entries[i].inode_index].first_block = -1;
            strcpy(inodes[root_directory.entries[i].inode_index].name, "empty");
            return 0; // Success
        }
    }
    return -1; // File not found
}

int rename_file(const char *old_filename, const char *new_filename){
    if (new_filename == NULL || strlen(new_filename) == 0 || strlen(new_filename) >= MAX_FILENAME_LENGTH){
        return -1;
    }

    for (int i = 0; i < root_directory.entry_count; ++i){
        if (strcmp(root_directory.entries[i].name, old_filename) == 0){
            strncpy(root_directory.entries[i].name, new_filename, MAX_FILENAME_LENGTH);
            strncpy(inodes[root_directory.entries[i].inode_index].name, new_filename, MAX_FILENAME_LENGTH);
            return 0;
        }
    }
    return -1; // File not found
}

void list_files(const Directory *directory) {
    std::cout << "Directory Listing:\n";
    for (int i = 0; i < directory->entry_count; ++i) {
        std::cout << directory->entries[i].name << " (size: " << directory->entries[i].size 
                  << ", created: " << ctime(&directory->entries[i].creation_time)
                  << ", inode: " << directory->entries[i].inode_index << ")\n";
    }
}

void show_disk_storage() {
    std::cout << "Superblock info:\n";
    std::cout << "  num inodes: " << superblock.num_inodes << "\n";
    std::cout << "  num blocks: " << superblock.num_blocks << "\n";
    std::cout << "  block size: " << superblock.block_size << "\n\n";

    std::cout << "inodes\n";
    for (int i = 0; i < superblock.num_inodes; ++i) {
        std::cout << "  size: " << inodes[i].size << " block: " << inodes[i].first_block 
                  << " name: " << inodes[i].name << "\n";
    }
    for (int i = 0; i < superblock.num_blocks; ++i) {
        std::cout << "  block num: " << i << " next block: " << blocks[i].next_block << "\n";
    }
}

int main() {
    initialize_storage();

    create_file("file1.txt", 0);
    create_file("file2.txt", 1);

    save_file("file1.txt", "Hello, World!", 13, 0);

    char *data;
    size_t size;
    if (load_file("file1.txt", &data, &size) == 0) {
        std::cout << "Loaded file content: " << data << "\n";
        free(data);
    }

    list_files(&root_directory);

    show_disk_storage();

    delete_file("file1.txt");
    rename_file("file2.txt", "renamed_file.txt");

    list_files(&root_directory);

    return 0;
}
