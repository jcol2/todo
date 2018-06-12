#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "todo/todo_list.h"

void compare_lists(todo_list_t* a, todo_list_t* b) {
    assert(strcmp(a->name, b->name) == 0);
    assert(a->num_todos == b->num_todos);

    for (unsigned char i = 0; i < a->num_todos && i < b->num_todos; ++i) {
        assert(strcmp(a->todos[i], b->todos[i]) == 0);
    }
}

void test1() {
    printf("todo tests.\n");

    printf("creating list...\n");
    todo_list_t* list1 = create_todo_list("test list");
    assert(list1);
    assert(list1->num_todos == 0);

    printf("adding todos...\n");
    add_todo(list1, "Create TODO list");
    assert(list1->num_todos == 1);
    add_todo(list1, "Print TODO list");
    assert(list1->num_todos == 2);

    printf("saving to file...\n");
    FILE* f = fopen("test.todo", "wr");
    assert(f);
    assert(save_todo_list(list1, f) == 0);
    fclose(f);

    printf("reading from file...\n");
    f = fopen("test.todo", "r");
    todo_list_t* list2 = create_todo_list_from_file(f);
    assert(list2);
    printf("comparing list from memory and from file...\n");
    compare_lists(list1, list2);

    printf("deleting lists...\n");
    delete_todo_list(list1);
    delete_todo_list(list2);

    printf("success!\n");
}

int main(int argc, char** argv) {
    test1();
    return 0;
}
