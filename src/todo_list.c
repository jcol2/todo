//
// todo Copyright (c) 2018 Jeremy Collette.
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

#include "todo/todo_list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STR_LENGTH 127
#define MAX_NUM_TODOS 255

void _strip_str(char* string) {
    for (int i = strlen(string) - 1; i >= 0; --i) {
        if (string[i] != '\0' && string[i] != '\n') {
            return;
        }
        string[i] = '\0';
    }
}

int add_todo_item(todo_list_t* list, const char* item) {
    if (list->num_todos >= MAX_NUM_TODOS) {
        fprintf(stderr, "error: todo list is full.\n");
        return 1;
    }
    if (strlen(item) > MAX_STR_LENGTH) {
        fprintf(stderr, "error: item is too long.\n");
        return 2;
    }
    list->todos[list->num_todos] = malloc(sizeof(char) * strlen(item));
    strcpy(list->todos[list->num_todos++], item);
    return 0;
}

todo_list_t* create_todo_list(const char* name) {
    todo_list_t* list = malloc(sizeof(todo_list_t));
    list->name = malloc(MAX_STR_LENGTH * sizeof(char));
    strncpy(list->name, name, MAX_STR_LENGTH);
    list->num_todos = 0;
    list->todos = malloc(sizeof(char*) * MAX_NUM_TODOS);
    return list;
}

todo_list_t* create_todo_list_from_file(FILE* f) {
    todo_list_t* list = create_todo_list("untitled");
    if (!list) {
        fprintf(stderr, "error: could not create new list.\n");
        return NULL;
    }

    // Get name
    char buf[MAX_STR_LENGTH];
    if (fgets(buf, MAX_STR_LENGTH, f) == NULL) {
        fprintf(stderr, "error: unexpected EOF while reading name.\n");
        return NULL;
    }
    _strip_str(buf);
    strncpy(list->name, buf, MAX_STR_LENGTH);

    // Get number of todos
    unsigned char num_todos;
    if (fread(&num_todos, sizeof(num_todos), 1, f) != 1) {
        fprintf(stderr, "error: unexpected error while reading num todos.\n");
        return NULL;
    }
    if (num_todos > MAX_NUM_TODOS) {
        fprintf(stderr, "error: too many todos.\n");
        return NULL;
    }

    // Consume new line
    if (fgetc(f) != '\n') {
        fprintf(stderr, "error: expected new-line.\n");
        return NULL;
    }

    // Get each todo
    for (unsigned char i = 0; i < num_todos; ++i) {
        if (fgets(buf, MAX_STR_LENGTH, f) == NULL) {
            fprintf(stderr, "error: unexpected EOF while reading todo item.\n");
            return NULL;
        }
        _strip_str(buf);

        if (add_todo_item(list, buf)) {
            fprintf(stderr, "error: failed to add todo item to list.\n");
            return NULL;
        }
    }

    // Check for EOF
    if (fgetc(f) != EOF) {
        fprintf(stderr, "error: expected EOF.\n");
        return NULL;
    }
    return list;
}

void delete_todo_list(todo_list_t* list) {
    free(list->name);
    while(list->num_todos > 0) {
        free(list->todos[--list->num_todos]);
    }
    free(list->todos);
    free(list);
}

const char* get_todo(todo_list_t* list, unsigned char index) {
    if (index >= list->num_todos) {
        return NULL;
    }
    return list->todos[index];
}

void print_todo_list(todo_list_t* list) {
    printf("%s\n", list->name);

    if (list->num_todos == 0) {
        printf("(empty)\n");
        return;
    }

    for (unsigned char i = 0; i < list->num_todos; ++i) {
        printf("%d. %s\n", i+1, get_todo(list, i));
    }
}

int remove_todo_item(todo_list_t* list, unsigned char index) {
    if (index >= list->num_todos) {
        return 1;
    }

    // Free the item at the index
    free(list->todos[index]);

    // We have to move each item up to fill the gap.
    for (unsigned char i = index; i < list->num_todos - 1; ++i) {
        list->todos[i] = list->todos[i + 1];
    }
    --list->num_todos;
    return 0;
}

int save_todo_list(todo_list_t* list, FILE* f) {
    if (!fprintf(f, "%s\n", list->name)) {
        fprintf(stderr, "error: could not write list name to file.\n");
        return 1;
    }

    if (fwrite(&list->num_todos, sizeof(list->num_todos), 1, f) != 1) {
        fprintf(stderr, "error: could not write number of todos to file.\n");
        return 2;
    }

    if (fputc('\n', f) != '\n') {
        fprintf(stderr, "error: could not write new-line to file.\n");
        return 3;
    }

    for (int i = 0; i < list->num_todos; ++i) {
        if (!fprintf(f, "%s\n", list->todos[i])) {
            fprintf(stderr, "error: could not write todo item to file.\n");
            return 4;
        }
    }
    return 0;
}
