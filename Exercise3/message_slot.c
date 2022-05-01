#undef __KERNEL__
#define __KERNEL__
#undef MODULE
#define MODULE


#include "mesage_slot.h"
#include <linux/uaccess.h>  /* for get_user and put_user */

/*  credits: exercise 6 && 7
    red black tree: https://www.geeksforgeeks.org/c-program-red-black-tree-insertion/
    free tree memory: https://stackoverflow.com/questions/9181146/freeing-memory-of-a-binary-tree-c
*/

// global array for all the trees
struct global_root{
    struct minor* minor_pointer;
}


global_root arr_of_minor[257];



static int device_open( struct inode* inode, struct file*  file ){
    struct minor *minor_file;
    struct node* root = NULL;

    if(arr_of_minor[iminor(inode)] != NULL){//already created data
        return SUCCESS;
    }
    
    minor_file = kmalloc(sizeof(minor), GFP_KERNEL);

    if(minor_file == NULL){
        //TODO - print a error message as said in the task
    }

    minor_file->minor_num = iminor(inode);
    arr_of_minor[minor_file->minor_num] = minor_file;
    minor_file->channels = kmalloc(sizeof(node), GFP_KERNEL);
    minor_file->channels->r = NULL;
    minor_file->channels->l =NULL;
    minor_file->channels->p = NULL;
    minor_file->channels->channel_id_num = 0;
    minor_file->last_channel = 0;
    minor_file->channels->message_len = 0;
    minor_file->channels->c = 1;

    root = bst(root, minor_file->channels);

    fixup(root, minor_file->channels);

    file->private_data = (void *)minor_file;

    return SUCCESS;

}//end of function device_open

static int device_release( struct inode* inode, struct file*  file){

    int temp = (int)file->private_data->minor_num;

    if(temp != iminor(inode)){
        //TODO - print a error message as said in the task
    }

    realese_tree(file->private_data->channels);

    return SUCCESS;

}//end of function device_release


static ssize_t device_read( struct file* file, char __user* buffer, size_t length, loff_t* offset ){

    if(length <=0 || length > 128){
        //TODO - print a error message as said in the task
    }

    struct node* root = file->private_data->channels;
    int channel_id = file->private_data->last_channel;

    struct node* channel = search(channel_id, root);

    if(length > channel->message_len){
        //TODO - print a error message as said in the task
    }

    int i = 0;

    while(i<length){

        put_user(channel->message[i], buffer[i]);
        i++;

    }//end of while

    return i;
}

static ssize_t device_write( struct file* file, const char __user* buffer, size_t length, loff_t* offset){

    if(length <=0 || length > 128){
        //TODO - print a error message as said in the task
    }

    struct node* root = file->private_data->channels;
    int channel_id = file->private_data->last_channel;

    struct node* channel = search(channel_id, root);

    int i=0;

    //fix to also check message vlidate only if valid then change(1)
    memset(channel->message, 0, DEFAULT_SIZE);

    while(i<length){

        //fix to also check message vlidate only if valid then change(1)
        channel->message[i] = buffer[i];
        i++;

    }//end of while

    channel->message_len = i;

    return i;

}


static long device_ioctl( struct   file* file,
                          unsigned int   ioctl_command_id,
                          unsigned long  ioctl_param )
{
    if(ioctl_command_id != MSG_SLOT_CHANNEL){
        //TODO - print a error message as said in the task
    }

    if(ioctl_param <=0){
        //TODO - print a error message as said in the task
    }

    struct node *channel = kmalloc(sizeof(node), GFP_KERNEL);
    channel->r = NULL;
    channel->l =NULL;
    channel->p = NULL;
    channel->channel_id_num = ioctl_param;
    channel->message_len = 0;
    channel->c = 1;

    file->private_data->last_channel = ioctl_param;

    bst(file->private_data->channels, channel);

    fixup(file->private_data->channels, channel);

    return SUCCESS;

}


// Initialize the module - Register the character device
static int __init simple_init(void){

    int rc = -1;

    // init dev struct

    // Register driver capabilities. Obtain major num
    rc = register_chrdev( MAJOR_NUM, DEVICE_RANGE_NAME, &Fops );

    // Negative values signify an error
    if( rc < 0 )
    {
        printk( KERN_ALERT "%s registraion failed for  %d\n", DEVICE_RANGE_NAME, MAJOR_NUM );
        return rc;
    }//end of if

    for(int i =0; i <257; i++){
        arr_of_minor[i] = NULL;
    }//end of for

    return SUCCESS;

}//end of function simple_init

//---------------------------------------------------------------
static void __exit simple_cleanup(void)
{
    // Unregister the device
    // Should always succeed

    for(int i =0; i<257; i++){
        
        if(arr_of_minor[i] != NULL){

            realese_tree(arr_of_minor[i]->minor_pointer->channels);//sending the root of each tree for each minor number that exist

        }//end of if

    }//end of for

    unregister_chrdev(MAJOR_NUM, DEVICE_RANGE_NAME);
  
}

//---------------------------------------------------------------
module_init(simple_init);
module_exit(simple_cleanup);

//========================= END OF FILE =========================
