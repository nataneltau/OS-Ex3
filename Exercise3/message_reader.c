#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "message_slot.h"

//TODO - If no channel has been set on the file descriptor, returns -1 and errno is set to EINVAL

int main(int argc, char *argv[]){

    int file_des_to_read;
    int channel_id;
    char message_buffer[DEFAULT_SIZE];
    int mark_success;

    if(argc != 3){//invalid input
        errno = EINVAL;
        fprintf( stderr, "%s\n", strerror(errno));
        exit(1);
    }//end of if

    file_des_to_read = open(argv[1], O_WRONLY);


    if(file_des_to_read == -1){//error in open file
        fprintf( stderr, "%s\n", strerror(errno));
        exit(1);
    }

    channel_id = atoi(argv[2]);

    mark_success = ioctl(file_des_to_read, MSG_SLOT_CHANNEL, channel_id);

    if(mark_success < 0){//error occurs in ioctl()
        fprintf( stderr, "%s\n", strerror(errno));
        exit(1);
    }

    mark_success = 0;

    mark_success = read(file_des_to_read, message_buffer, DEFAULT_SIZE, 0);
    if(mark_success <= 0){//error occurs in read()
        fprintf( stderr, "%s\n", strerror(errno));
        exit(1);
    }
    //now mark_success contain the length of the message 

    close(file_des_to_read);

    if(write(STDOUT_FILENO, message_buffer, mark_success) == -1){//check if occurs error
        fprintf( stderr, "%s\n", strerror(errno));
        exit(1);
    }


    return 0;



}//end of function main