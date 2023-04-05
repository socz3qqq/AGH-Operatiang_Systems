#include "rec_wc_bytes.h"

long long total_bytes = 0;

int process_file(const char * fpath, const struct stat * sb, int typeflag){
    if(!S_ISDIR(sb->st_mode)){
        printf("size: %ld filename: %s\n", sb->st_size, fpath);
        total_bytes += sb->st_size;
    }
    return 0;
}


int main(int argc, char * argv[]){
    if( argc != 2){
        fprintf(stderr, "Invalid number of arguments, please provide only a path to directory\n");
        return -1;
    }
    // opening a directory
    if ( ftw(argv[1], process_file, MAX_DIR) == -1){
        fprintf(stderr, "Failed to execute ftw function: %s \n", strerror(errno));
        return 2;
    }
    printf("total size of all files: %lld bytes\n", total_bytes);
    return 0;
}