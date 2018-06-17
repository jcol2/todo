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
    add_todo_item(list1, "Create TODO list");
    assert(list1->num_todos == 1);
    add_todo_item(list1, "Print TODO list");
    assert(list1->num_todos == 2);
    add_todo_item(list1, "????????");
    assert(list1->num_todos == 3);
    add_todo_item(list1, "PROFIT!!!!!!");
    assert(list1->num_todos == 4);

    printf("getting todos...\n");
    const char* item = get_todo_item(list1, 0);
    assert(strcmp(item, "Create TODO list") == 0);
    item = get_todo_item(list1, 1);
    assert(strcmp(item, "Print TODO list") == 0);
    item = get_todo_item(list1, 2);
    assert(strcmp(item, "????????") == 0);
    item = get_todo_item(list1, 3);
    assert(strcmp(item, "PROFIT!!!!!!") == 0);
    item = get_todo_item(list1, -1);
    assert(item == NULL);

    printf("removing todos...\n");
    assert(remove_todo_item(list1, 2) == 0);
    assert(get_todo_item(list1, 3) == NULL);
    assert(remove_todo_item(list1, 2) == 0);
    assert(get_todo_item(list1, 2) == NULL);
    item = get_todo_item(list1, 0);
    assert(strcmp(item, "Create TODO list") == 0);
    item = get_todo_item(list1, 1);
    assert(strcmp(item, "Print TODO list") == 0);
    item = get_todo_item(list1, 2);

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
