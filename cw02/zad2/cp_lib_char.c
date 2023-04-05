// I am choosing standard library option implementation
#include <stdio.h>
#include <unistd.h>


typedef enum {false, true} bool;

bool path_valid(char * path){
    return true;                                                   
}

int main(int argc, char * argv[]){
    if(argc != 3){
        fprintf(stderr, "Please enter two arguments: \n1. File to copy \n2. Destination file\n");
        return 2;
    }

    const char * input_file, * output_file;

    if(path_valid(argv[1])){
        input_file = argv[1];
    }
    else
        return 2;
    
    if(path_valid(argv[2]))
        output_file = argv[2];
    else
        return 2;
    

    // opening the files
    FILE * fd_in, *fd_out;
    if ((fd_in = fopen(input_file, "r")) == NULL){
        fprintf(stderr, "Failed to open the file: %s \n", input_file);
        return 3;
    }
    if ((fd_out = fopen(output_file, "w")) == NULL){
        fprintf(stderr, "Failed to open the file: %s \n", output_file);
        return 3;
    }

    fseek(fd_in, -1, SEEK_END);


    char s;
    while( fread(&s, sizeof(char), 1, fd_in) != 0 ){
        fwrite(&s, sizeof(char), 1, fd_out);
        if (ftell(fd_in) == 1)
            break;
        else
            fseek(fd_in, -2, SEEK_CUR);
    }
    fclose(fd_in);
    fclose(fd_out);

}