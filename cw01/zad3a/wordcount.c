#include "wordcount.h"


string_table *init_table(size_t size){
    string_table *new_table = calloc(1, sizeof(string_table));
    new_table->table = calloc(size, sizeof(char*));
    new_table->size = size;
    new_table->current_block_count = 0;
    new_table->is_active = calloc(size, sizeof(bool));
    new_table->top_block = 0;
    return new_table;
}

void add_element( string_table * tab, char * pathname){
    // create a tmp file
    system("touch /tmp/OS_task1.txt");

    // save the output of the command to the file
    char command[300] = "wc ";
    strcat(command, pathname);
    strcat(command, " 1> /tmp/OS_task1.txt");
    if ( system(command) == -1){
        return;
    }
    // finding out the size of the file
    FILE * tmp_file;
    long file_size;
    if( (tmp_file = fopen("/tmp/OS_task1.txt", "r")) <0 )
        fprintf(stderr, "Failed to open a file\n");
    fseek(tmp_file, 0, SEEK_END);
    file_size = ftell(tmp_file);
    fseek(tmp_file, 0, SEEK_SET);

    // copying file content to the buffer
    char* buff = calloc(file_size, sizeof(char));
    if (fread(buff, sizeof(char), file_size, tmp_file) == 0)
        fprintf(stderr, "Failed to read from file\n");

    if( fclose(tmp_file)!=0){
        fprintf(stderr, "Failed to close the file\n");
    }

    // copying file content from buffer to the structure
    if(tab->size > tab->top_block){
        tab->table[tab->top_block] = buff; 
        tab->is_active[tab->top_block] = true;
        // increasing block counter and top counter
        tab->current_block_count ++;
        tab->top_block ++;
    }
    else{
        fprintf(stderr, "CANNOT PUSHH AT THE END\n");
    }
    
    // deleting temp file
    system("rm -rf /tmp/OS_task1.txt\n");
}

char * get_by_index(string_table * tab, int index){
    if( index >= 0 && index < tab->size){
        if(index < tab->top_block && tab->is_active[index])
            return (tab->table)[index];
        else
            fprintf(stderr, "This index was deleted or not allocated");
    }
    else{
        fprintf(stderr, "Index out of range");
    }   
    return "";
}

void delete_by_index(string_table * tab, int index){
    if(tab->is_active[index] && index < tab->top_block && index >= 0){
        free(tab->table[index]);
        tab->is_active[index] = false;
    }
    else
        fprintf(stderr, "Failed to delete block, probably wrong index\n");
}

void free_table(string_table * tab){
    int block_count = 0;
    int i = 0;
    while(block_count < tab->current_block_count){
        if(tab->is_active[i]){
            free(tab->table[i]);
            tab->is_active[i] = false;
            block_count++;
        }
        i ++;
    }
    tab->current_block_count = 0;
}

void free_struct(string_table * tab){
    free_table(tab);
    free(tab->table);
    free(tab->is_active);
    free(tab);
}