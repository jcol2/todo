//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "todo/todo_list.h"
#include "todo/version.h"

#define STR_BUF_SIZE 255

int main(int argc, char** argv) {

    // Handle '--help' here
    if (argc >= 2 && (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h")
            == 0)) {
        printf("todo %s Copyright (c) Jeremy Collette 2018.\n\n"
                , TODO_VERSION_LABEL);
        printf("todo is a TODO list manager.\n");
        printf("Usage: todo [OPTION]\n\n");
        printf("Options\n");
        printf("\t--add,-a \"<item>\"\t Adds a TODO item.\n");
        printf("\t--help,-h\t\t Prints this message.\n");
        printf("\t--remove,-r <index>\t Removes the TODO item at the specified"
                " index.\n\n");
        printf("NOTE: If no options are specified, todo will display the"
                " current list.\n");
        return 0;  // We return here in case there are any bugs etc.
    }

    // Create / load default todo list
    // Get the path first
    char* home_dir = getenv("HOME");
    if (!home_dir) {  // If we couldn't get the env variable...
        struct passwd *pw = getpwuid(getuid());
        home_dir = pw->pw_dir;
    }
    const char* default_todo_name = ".default.todo";
    char default_todo_path[STR_BUF_SIZE];
    snprintf(default_todo_path, STR_BUF_SIZE, "%s/%s", home_dir
            , default_todo_name);

    // Try to open default todo list
    FILE* f = fopen(default_todo_path, "rw");
    if (!f) {  // If it doesn't exist, create it!
        printf("Creating \"%s\"...\n", default_todo_path);
        todo_list_t* list = create_todo_list("TODO");
        if (!list) {
            fprintf(stderr, "error: could not initialise list.\n");
            return 1;
        }

        f = fopen(default_todo_path, "w");
        if (!f) {
            fprintf(stderr, "error: could not create \"%s\".\n"
                    , default_todo_path);
            return 2;
        }

        if (save_todo_list(list, f)) {
            fprintf(stderr, "error: could not save default list.\n");
            return 3;
        }
        fclose(f);

        f = fopen(default_todo_path, "rw");
        if (!f) {
            fprintf(stderr, "error: could not open \"%\"."
                    , default_todo_path);
            return 4;
        }
        delete_todo_list(list);
    }

    // Read from default todo list file
    todo_list_t* list = create_todo_list_from_file(f);
    if (!list) {
        fprintf(stderr, "error: could not parse todo file.\n");
        return 5;
    }

    // If we don't have any arguments, print the default todo list
    if (argc <= 1) {
        print_todo_list(list);
    } else if (argc >= 2 && (strcmp(argv[1], "--add") == 0 || strcmp(argv[1]
            , "-a") ==  0)) {  // Parse 'add' / 'remove' arguments
        if (argc <= 2) {
            fprintf(stderr, "error: expected item to add.\n");
            return 6;
        }

        char* item = argv[2];
        if (add_todo_item(list, item)) {
            fprintf(stderr, "error: could not add list item.\n");
            return 7;
        }
        print_todo_list(list);
    } else if (argc >= 2 && (strcmp(argv[1], "--remove") == 0 || strcmp(argv[1]
            , "-r") == 0)) {
        if (argc <= 2) {
            fprintf(stderr, "error: expected item index to remove.\n");
            return 8;
        }

        int num = atoi(argv[2]);
        unsigned char index = ((char)num) - 1;
        if (remove_todo_item(list, index)) {
            fprintf(stderr, "error: could not remove list item.\n");
            return 9;
        }
        print_todo_list(list);
    } else {  // Arguments should have been handled by here
        printf("Unrecognised command-line options. Try \"todo --help\".\n");
        return 10;
    }

    // Save any changes
    fclose(f);
    f = fopen(default_todo_path, "w");
    if (save_todo_list(list, f)) {
        fprintf(stderr, "error: could not save todo list.\n");
        return 11;
    }
    fclose(f);
    delete_todo_list(list);
    return 0;
}
