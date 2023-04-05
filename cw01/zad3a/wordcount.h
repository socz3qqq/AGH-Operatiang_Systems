#ifndef _WORDCOUNT_H
#define _WORDCOUNT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE 2048

typedef enum { false, true } bool;

typedef struct string_table
{
    char ** table;
    size_t size;
    int current_block_count;
    bool * is_active;
    int top_block;
} string_table;


string_table *init_table(size_t size);
void add_element( string_table * tab, char * pathname);
char * get_by_index(string_table * tab, int i);
void delete_by_index(string_table * tab, int index);
void free_table(string_table * tab);
void free_struct(string_table * tab);

#endif