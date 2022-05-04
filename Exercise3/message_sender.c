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

    int file_des_to_write;
    int channel_id;
    int message_pass_len;
    char *message_to_pass;
    int mark_success;

    if(argc != 4){//invalid input
        errno = EINVAL;
        fprintf( stderr, "%s\n", strerror(errno));
        exit(1);
    }//end of if

    message_pass_len = strlen(argv[3]);

    /*if(message_pass_len == 0 || message_pass_len > 128){//the message length is incorrect
        errno = EMSGSIZE;
        fprintf( stderr, "%s\n", strerror(errno));
        exit(1);
    }*/ // this should be checked in the device_write function

    file_des_to_write = open(argv[1], O_WRONLY);

    if(file_des_to_write == -1){//error in open file
        fprintf( stderr, "%s\n", strerror(errno));
        exit(1);
    }

    channel_id = atoi(argv[2]);

    mark_success = ioctl(file_des_to_write, MSG_SLOT_CHANNEL, channel_id);

    if(mark_success < 0){
        fprintf( stderr, "%s\n", strerror(errno));
        exit(1);
    }

    mark_success = write(file_des_to_write, argv[3], message_pass_len, 0);
    if(mark_success != message_pass_len){
        fprintf( stderr, "%s\n", strerror(errno));
        exit(1);
    }

    close(file_des_to_write);

    return 0;

}//end of function main

