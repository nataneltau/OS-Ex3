#ifndef CHARDEV_H
#define CHARDEV_H

#include <linux/ioctl.h>


#define MAJOR_NUM 235

// Set the message of the device driver
#define MSG_SLOT_CHANNEL _IOW(MAJOR_NUM, 0, unsigned int)

#define DEFAULT_SIZE 128
#define SUCCESS 0
#define DEVICE_RANGE_NAME "message_slot"

#endif
     

