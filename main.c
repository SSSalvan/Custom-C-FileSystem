#include "abstract.h"

void print_menu() {
    printf("\nSensen's Operating System\n");
    printf("1. Create File\n");
    printf("2. Load File\n");
    printf("3. Rename File\n");
    printf("4. Delete File\n");
    printf("5. List Files\n");
    printf("6. Show Disk Storage\n");
    printf("7. Exit\n");
    printf("Enter your choice: ");
}

int main() {
    initialize_storage();
    printf("Welcome to Sensen's Operating System!\n");

    int choice;
    char filename[MAX_FILENAME_LENGTH];
    char new_filename[MAX_FILENAME_LENGTH];
    char data[1024];
    char *loaded_data;
    size_t loaded_size;
    int inode_index;

    while (1) {
        print_menu();
        scanf("%d", &choice);
        getchar(); // Consume newline character

        switch (choice) {
            case 1:
                printf("Enter filename to create: ");
                fgets(filename, MAX_FILENAME_LENGTH, stdin);
                filename[strcspn(filename, "\n")] = '\0'; // Remove newline character
                printf("Enter inode index to use: ");
                scanf("%d", &inode_index);
                getchar(); // Consume newline character
                if (create_file(filename, inode_index - 1) == 0) {
                    printf("File created successfully.\n");
                } else {
                    printf("Failed to create file.\n");
                }
                break;
            case 2:
                printf("Enter filename to load: ");
                fgets(filename, MAX_FILENAME_LENGTH, stdin);
                filename[strcspn(filename, "\n")] = '\0'; // Remove newline character
                if (load_file(filename, &loaded_data, &loaded_size) == 0) {
                    printf("Loaded file: %s\n", loaded_data);
                    free(loaded_data);
                } else {
                    printf("Failed to load file.\n");
                }
                break;
            case 3:
                printf("Enter old filename to rename: ");
                fgets(filename, MAX_FILENAME_LENGTH, stdin);
                filename[strcspn(filename, "\n")] = '\0'; // Remove newline character
                printf("Enter new filename: ");
                fgets(new_filename, MAX_FILENAME_LENGTH, stdin);
                new_filename[strcspn(new_filename, "\n")] = '\0'; // Remove newline character
                if (rename_file(filename, new_filename) == 0){
                    printf("File renamed successfully.\n");
                } else{
                    printf("Failed to rename file.\n");
                }
                break;
            case 4:
                printf("Enter filename to delete: ");
                fgets(filename, MAX_FILENAME_LENGTH, stdin);
                filename[strcspn(filename, "\n")] = '\0'; // Remove newline character
                if (delete_file(filename) == 0) {
                    printf("File deleted successfully.\n");
                } else {
                    printf("Failed to delete file.\n");
                }
                break;
            case 5:
                list_files(&root_directory);
                break;
            case 6:
                show_disk_storage();
                break;
            case 7:
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
