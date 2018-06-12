#ifndef TODO_TODOLIST_H_
#define TODO_TODOLIST_H_

#include <stdio.h>

///
/// Holds a todo list.
///
typedef struct todo_list_t_ {
    char* name;                 ///< Holds list name
    unsigned char num_todos;    ///< Holds number of todos
    char** todos;               ///< Holds todos
} todo_list_t;

///
/// Adds an item to a todo list.
///
/// \param list The list to add the item to.
/// \param item The item to add.
///
/// \return 0 indicates success. Non-zero indicates failure.
///
int add_todo_item(todo_list_t* list, const char* item);

///
/// Creates and initialises a todo list.
///
/// \return A pointer to the newly created todo list. If creation or
/// initialisation failed, returns NULL.
///
todo_list_t* create_todo_list(const char* name);

///
/// Reads a todo list from a file.
///
/// \param f The file to read from.
///
/// \return A pointer to the newly created todo list. If creation or
/// initialisation failed, returns NULL.
///
todo_list_t* create_todo_list_from_file(FILE* f);

///
/// Gets the todo list item at the specified index (if it exists).
///
/// \param list The list to get the item from.
/// \param index The index of the item to retrieve.
///
/// \return A pointer to the item if it exists. Otherwise, returns NULL.
///
const char* get_todo_item(todo_list_t* list, unsigned char index);

///
/// Deletes a todo list.
///
/// \param list The list to delete.
///
void delete_todo_list(todo_list_t* list);

///
/// Prints todo list items to stdout.
///
/// \param list The list to print.
///
void print_todo_list(todo_list_t* list);

///
/// Removes the todo list item at the specified index.
///
/// \param list The list to remove the item from.
/// \param index The index to remove the item at.
///
/// \return 0 indicates success. Non-zero indicates failure.
///
int remove_todo_item(todo_list_t* list, unsigned char index);

///
/// Saves todo list to file.
///
/// \param list The list to save.
/// \param f The file to write to.
///
/// \return 0 indicates success. Non-zero indicates failure.
///
int save_todo_list(todo_list_t* list, FILE* f);

#endif
