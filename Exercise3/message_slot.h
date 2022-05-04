#ifndef CHARDEV_H
#define CHARDEV_H

#include <linux/ioctl.h>


#define MAJOR_NUM 235
#define DEFAULT_SIZE 128
#define SUCCESS 0
#define IOCTL_SET_ENC _IOW(MAJOR_NUM, 0, unsigned int)
#define DEVICE_RANGE_NAME "message_slot"

#endif
     

