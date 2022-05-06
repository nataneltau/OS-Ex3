#ifndef MESSAGE_SLOT_H
#define MESSAGE_SLOT_H

#include <linux/ioctl.h>


#define MAJOR_NUM 235

// Set the message of the device driver
#define MSG_SLOT_CHANNEL _IOW(MAJOR_NUM, 0, unsigned int)

#define DEFAULT_SIZE 128
#define SUCCESS 0
#define DEVICE_RANGE_NAME "message_slot"


typedef struct node {
    int channel_id_num; // data is minor number
    char *message;
    int message_len;
    int c; // 1-red, 0-black
    struct node* p; // parent
    struct node* r; // right-child
    struct node* l; // left child
}node;
 


typedef struct minor{
  int minor_num;//the number of the channel id
  struct node *channels;//all the channels connected to this minor number
  int last_channel;//the last channel inserted by device_ioctl
}minor;


typedef struct global_root{
    struct minor* minor_pointer;
    //int counter; //should add? it's neccerey if there is several files with the same minor num, that way device_release with not make bug, only add if it make a bug
}global_root;//arr_of_minor[257];

#endif
     

