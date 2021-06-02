//-------------------------------------------------------------------------------------
//   Includes:
//-------------------------------------------------------------------------------------
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//-------------------------------------------------------------------------------------
//   Definition of functions:
//-------------------------------------------------------------------------------------
void copy_mmap(int fd_from, int fd_to);
void copy_read_write(int fd_from, int fd_to);
void print_help();
void print_error(char *msg);

//-------------------------------------------------------------------------------------
//   main():
//-------------------------------------------------------------------------------------
int main(int argc, char *argv[]){
    int mflag = 0;

    if(argc == 1){
        print_help();
        exit(0);
    }

    int arg;
    while (( arg = getopt(argc, argv, ":mh" )) != -1){
        switch (arg) {
            case 'm':
                if(argc != 4){
                    print_error("Wrong no of parameter, please type -h to show usage");
                    exit(1);
                }
                mflag = 1;
                break;
            case 'h':
                if(argc > 2){
                    print_error("Wrong no of parameter, please type -h to show usage");
                    exit(1);
                }
                print_help();
                exit(0);

            case '?':
                print_error("Wrong no of parameter, please type -h to show usage");
                exit(1);
        }
    }

    if(mflag == 0 && argc != 3) {
        print_error("Wrong no of parameter, please type -h to show usage");
        return 1;
    }

    int fd_from = open(argv[1 + mflag], O_RDONLY);
    int fd_to = open(argv[2 + mflag], O_RDWR | O_CREAT, 0666);

    if(mflag == 1) {
        printf("Copying by mmap and memcpy\n");
        copy_mmap(fd_from, fd_to);
    }
    else {
        printf("Copying by read /write\n");
        copy_read_write(fd_from, fd_to);
    }
    if(close(fd_from) == 0)
        printf("Source file was closed properly\n");
    if(close(fd_to) == 0)
        printf("Destination file was closed properly\n");

    exit(0);
}

//-------------------------------------------------------------------------------------
//   copy_mmap(int fd_from, int fd_to):
//      Copying the file using read and write
//-------------------------------------------------------------------------------------
void copy_read_write(int fd_from, int fd_to){
    struct stat stat_from;
    fstat(fd_from, &stat_from);
    char buff[stat_from.st_size];

    int bytes_read, bytes_write;

    while ((bytes_read = read(fd_from, buff, stat_from.st_size)) > 0){
        bytes_write = write(fd_to, buff, bytes_read);
    }
    if(bytes_write == -1){
        print_error("Error while writting data to the file");
        exit(1);
    }

}

//-------------------------------------------------------------------------------------
//   copy_mmap(int fd_from, int fd_to):
//      Copying the file using mmap
//-------------------------------------------------------------------------------------
void copy_mmap(int fd_from, int fd_to){

    struct stat stat_from;
    fstat(fd_from, &stat_from);

    char* src_buffer;   //pointer to shared memory object
    src_buffer = mmap(NULL, stat_from.st_size, PROT_READ,MAP_SHARED,fd_from,0);

    char* dest_buffer;  //pointer to shared memory object
    dest_buffer = mmap(NULL, stat_from.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_to,0);

    ftruncate(fd_to, stat_from.st_size);    //adjusting the size of the destination file
    dest_buffer = memcpy(dest_buffer, src_buffer, stat_from.st_size);

    munmap(src_buffer, stat_from.st_size);  //unmapping the source file
    munmap(dest_buffer, stat_from.st_size); //unmapping the destination file
}

//-------------------------------------------------------------------------------------
//   Printing helpers functions:
//-------------------------------------------------------------------------------------
void print_help(){
    printf("Usage:\n");
    printf("\t./copy [-m] <file_path> <new file_path>\n");
    printf("\t./copy [-h]\n");

    printf("Explenation:\n\t-h argument\t- triggers the help prompter\n\t-m argument\t- triggers the use of mmap()\n");
}

void print_error(char *msg){
    printf("[Error]:\t");
    printf(msg);
    printf("\n\n");
    print_help();
}