#undef __KERNEL__
#define __KERNEL__
#undef MODULE
#define MODULE


/*#include <stdio.h>
#include <stdlib.h>
#include <string.h>*/

#include <linux/kernel.h>   /* We're doing kernel work */
#include <linux/module.h>   /* Specifically, a module */
#include <linux/fs.h>       /* for register_chrdev */
#include <linux/uaccess.h>  /* for get_user and put_user */
#include <linux/string.h>   /* for memset. NOTE - not string.h!*/
#include <linux/slab.h> /* for memory use*/

//#include <linux/errno.h>
//#include <linux/kernel.h>   /* We're doing kernel work */
//#include <linux/module.h>   /* Specifically, a module */
//#include <linux/fs.h>       /* for register_chrdev */
//#include <linux/string.h>   /* for memset. NOTE - not string.h!*/
//#include <linux/uaccess.h>  /* for get_user and put_user */
//#include <linux/slab.h>


MODULE_LICENSE("GPL");//like in exercise 7

#include "message_slot.h"


/*  credits: exercise 6 && 7
    red black tree: https://www.programiz.com/dsa/red-black-tree
    free tree memory: https://stackoverflow.com/questions/9181146/freeing-memory-of-a-binary-tree-c
*/



// global array for all the trees

global_root arr_of_minor[257];




//________________________________________________________________________________________________________
//________________________________________________________________________________________________________
//__________ RED BLACK TREE _________ RED BLACK TREE _________ RED BLACK TREE ______ RED BLACK TREE ______
//________________________________________________________________________________________________________
//________________________________________________________________________________________________________

// Implementing Red-Black Tree in C





//struct rbNode *root = NULL;

// Create a red-black tree
struct rbNode *createNode(int channel_id_num) {
    struct rbNode *newnode;
    newnode = (struct rbNode *)kmalloc(sizeof(struct rbNode), GFP_KERNEL);

    if(newnode == NULL){
        return NULL;
    }

    newnode->channel_id_num = channel_id_num;
    newnode->message_len = 0;
    newnode->color = RED;
    newnode->link[0] = newnode->link[1] = NULL;
    return newnode;
}

// Insert an node
int insertion(minor *almost_root, int channel_id_num) {
  struct rbNode **stack;
  struct rbNode *ptr, *newnode, *xPtr, *yPtr;
  int dir[98];
  int ht = 0, index;
  ptr = almost_root->channels;

  stack = (struct rbNode **)kmalloc(sizeof(struct rbNode) * 98, GFP_KERNEL);

  if (!(almost_root->channels)) {
    almost_root->channels = createNode(channel_id_num);
    kfree(stack);

    if(almost_root->channels == NULL){
        return -1;
    }

    return 0;
  }

  stack[ht] = almost_root->channels;
  dir[ht++] = 0;
  while (ptr != NULL) {
    if (ptr->channel_id_num == channel_id_num) {
      //printk("Duplicates Not Allowed!!\n");
      return 0;
    }
    index = (channel_id_num - ptr->channel_id_num) > 0 ? 1 : 0;
    stack[ht] = ptr;
    ptr = ptr->link[index];
    dir[ht++] = index;
  }
  stack[ht - 1]->link[index] = newnode = createNode(channel_id_num);

    if(newnode == NULL){
        return -1;
    }

  while ((ht >= 3) && (stack[ht - 1]->color == RED)) {
    if (dir[ht - 2] == 0) {
      yPtr = stack[ht - 2]->link[1];
      if (yPtr != NULL && yPtr->color == RED) {
        stack[ht - 2]->color = RED;
        stack[ht - 1]->color = yPtr->color = BLACK;
        ht = ht - 2;
      } else {
        if (dir[ht - 1] == 0) {
          yPtr = stack[ht - 1];
        } else {
          xPtr = stack[ht - 1];
          yPtr = xPtr->link[1];
          xPtr->link[1] = yPtr->link[0];
          yPtr->link[0] = xPtr;
          stack[ht - 2]->link[0] = yPtr;
        }
        xPtr = stack[ht - 2];
        xPtr->color = RED;
        yPtr->color = BLACK;
        xPtr->link[0] = yPtr->link[1];
        yPtr->link[1] = xPtr;
        if (xPtr == almost_root->channels) {
          almost_root->channels = yPtr;
        } else {
          stack[ht - 3]->link[dir[ht - 3]] = yPtr;
        }
        break;
      }
    } else {
      yPtr = stack[ht - 2]->link[0];
      if ((yPtr != NULL) && (yPtr->color == RED)) {
        stack[ht - 2]->color = RED;
        stack[ht - 1]->color = yPtr->color = BLACK;
        ht = ht - 2;
      } else {
        if (dir[ht - 1] == 1) {
          yPtr = stack[ht - 1];
        } else {
          xPtr = stack[ht - 1];
          yPtr = xPtr->link[0];
          xPtr->link[0] = yPtr->link[1];
          yPtr->link[1] = xPtr;
          stack[ht - 2]->link[1] = yPtr;
        }
        xPtr = stack[ht - 2];
        yPtr->color = BLACK;
        xPtr->color = RED;
        xPtr->link[1] = yPtr->link[0];
        yPtr->link[0] = xPtr;
        if (xPtr == almost_root->channels) {
          almost_root->channels = yPtr;
        } else {
          stack[ht - 3]->link[dir[ht - 3]] = yPtr;
        }
        break;
      }
    }
  }
  almost_root->channels->color = BLACK;
  kfree(stack);
  return 0;

}


// Print the inorder traversal of the tree
/*void inorderTraversal(struct rbNode *node) {
  if (node) {
    inorderTraversal(node->link[0]);
    printf("%d  ", node->channel_id_num);
    inorderTraversal(node->link[1]);
  }
  return;
}*/

void realese_tree(rbNode* trav){
  if(trav == NULL){
    return;
  }

  realese_tree(trav->link[0]);
  realese_tree(trav->link[1]);

  kfree(trav);

}

struct rbNode* search_in_the_rbt(int x, struct rbNode* n)
{
    if(n != NULL)
    {
        if(n->channel_id_num  == x)
                return (n);
        else if(n->channel_id_num > x)//go left
                return search_in_the_rbt(x, n->link[0]);
        else//go right
                return search_in_the_rbt(x, n->link[1]);
    }
    return NULL;
}

// Driver code
/*int main() {
  int ch, data;
  while (1) {
    printf("1. Insertion\t2. Deletion\n");
    printf("3. Traverse\t4. Exit");
    printf("\nEnter your choice:");
    scanf("%d", &ch);
    switch (ch) {
      case 1:
        printf("Enter the element to insert:");
        scanf("%d", &data);
        insertion(data);
        break;
      case 2:
        printf("Enter the element to delete:");
        scanf("%d", &data);
        deletion(data);
        break;
      case 3:
        inorderTraversal(root);
        printf("\n");
        break;
      case 4:
        exit(0);
      default:
        printf("Not available\n");
        break;
    }
    printf("\n");
  }
  return 0;
}*/

//________________________________________________________________________________________________________
//________________________________________________________________________________________________________
//__________ RED BLACK TREE _________ RED BLACK TREE _________ RED BLACK TREE ______ RED BLACK TREE ______
//________________________________________________________________________________________________________
//________________________________________________________________________________________________________






//global_root *arr_of_minor = (global_root*)kmalloc(sizeof(global_root)*257, GFP_KERNEL);



static int device_open( struct inode* inode, struct file*  file ){
    minor *minor_file;
    //rbNode* temp;
    //rbNode* root = NULL;

    if((arr_of_minor[iminor(inode)].minor_pointer) != NULL){//already created data
        file->private_data = (void *)(arr_of_minor[iminor(inode)].minor_pointer);
        //arr_of_minor[iminor(inode)]->counter++;
        return SUCCESS;
    }
    
    minor_file = (minor *)kmalloc(sizeof(struct minor), GFP_KERNEL);

    if(minor_file == NULL){
        return -ENOMEM;
    }

    minor_file->minor_num = iminor(inode);
    minor_file->channels = NULL;
    arr_of_minor[minor_file->minor_num].minor_pointer = minor_file;
    //arr_of_minor[minor_file->minor_num]->counter = 1;
    //minor_file->channels = kmalloc(sizeof(struct rbNode), GFP_KERNEL);

    /*if(minor_file->channels == NULL){
        return -ENOMEM;
    }*/

    /*temp = createNode(0);

    minor_file->channels = temp;

    root = bst(root, minor_file->channels);

    fixup(root, minor_file->channels);*///need that?

    if(insertion(minor_file, 0) < 0){
        return -ENOMEM;
    }

    minor_file->last_channel = 0;

    file->private_data = (void *)minor_file;

    return SUCCESS;

}//end of function device_open

static int device_release( struct inode* inode, struct file*  file){

    minor *minor_file;
    int temp;

    minor_file = (minor *)(file->private_data);
    temp = minor_file->minor_num;

    if(temp != iminor(inode)){
        return -EINVAL;
    }

    //if(arr_of_minor[temp]->couner > 1){
        //arr_of_minor[temp]->couner--;
        //return SUCCESS;
    //}

    realese_tree((minor_file->channels));
    kfree(arr_of_minor[temp].minor_pointer);
    arr_of_minor[temp].minor_pointer = NULL;

    return SUCCESS;

}//end of function device_release


static ssize_t device_read( struct file* file, char __user* buffer, size_t length, loff_t* offset){

    struct rbNode* channel;
    int i, num;
    struct rbNode* root;
    int channel_id;
    minor *minor_file;
    char *the_message;

    minor_file = (minor *)(file->private_data);
    
    root = minor_file->channels;
    channel_id = minor_file->last_channel;

    if(channel_id <= 0 || buffer == NULL){//no channel has been set on the file descriptor or user buffer invalid
        return -EINVAL;
    }

    
    channel = search_in_the_rbt(channel_id, root);

    if(channel == NULL){
        return -EINVAL;
    }

    if(channel->message_len == 0){//no message exists on the channel
        return -EWOULDBLOCK;
    }


    if(length <=0 || length > 128 || length < channel->message_len){
        return -ENOSPC;
    }

    i = 0;
    the_message = channel->message;
    num = channel->message_len;

    while(i<num){

      if (put_user(the_message[i], buffer+i) != 0) {//check if put_user failed
        return -EFAULT;
		  }
      i++;

    }//end of while

    return i;
}

static ssize_t device_write( struct file* file, const char __user* buffer, size_t length, loff_t* offset){

    struct rbNode* root;
    int channel_id;
    rbNode* channel;
    minor *minor_file;
    char *safe_buff;
    int i = 0;

    minor_file = (minor *)(file->private_data);

    channel_id = minor_file->last_channel;
    root = minor_file->channels;

    if(channel_id <= 0 || buffer == NULL){//no channel has been set on the file descriptor or user buffer invalid
        return -EINVAL;
    }

    if(length <=0 || length > 128){
        return -EMSGSIZE;
    }

    safe_buff = kmalloc(length * sizeof(char *), GFP_KERNEL);

    if(safe_buff == NULL){
        return -ENOMEM;
    }

    channel = search_in_the_rbt(channel_id, root);

    if(channel == NULL){
        return -EINVAL;
    }

    i=0;

    while(i<length){
        if(get_user(safe_buff[i], &buffer[i]) != 0){
            return -EFAULT;
        }
        i++;
    }

    i = 0;
    //the message is valid, now can replace
    kfree(channel->message);

    channel->message = kmalloc(length * sizeof(char *), GFP_KERNEL);

    if(channel->message == NULL){
        return -ENOMEM;
    }


    while(i<length){

        //fix to also check message vlidate only if valid then change(1)
        channel->message[i] = safe_buff[i];
        i++;

    }//end of while

    channel->message_len = i;

    kfree(safe_buff);

    return i;

}


static long device_ioctl( struct   file* file, unsigned int ioctl_command_id, unsigned long  ioctl_param){

    minor *minor_file;
    rbNode *channel;

    minor_file = (minor *)(file->private_data);


    if(ioctl_command_id != MSG_SLOT_CHANNEL){
        return -EINVAL;
    }

    if(ioctl_param <=0){
        return -EINVAL;
    }

    if(search_in_the_rbt(ioctl_param, minor_file->channels) != NULL){//there is already a node in the tree with this id
        minor_file->last_channel = ioctl_param;
        return SUCCESS;
    }


    if(insertion(minor_file, ioctl_param) < 0){
        return -ENOMEM;
    }
    

    channel = search_in_the_rbt(ioctl_param, minor_file->channels);

    channel->message = kmalloc(2 * sizeof(char *), GFP_KERNEL);


    /*channel = kmalloc(sizeof(node), GFP_KERNEL);

    if(channel == NULL){
        return -ENOMEM;
    }

    channel->r = NULL;
    channel->l =NULL;
    channel->p = NULL;
    channel->channel_id_num = ioctl_param;
    channel->message_len = 0;
    channel->c = 1;*/

    minor_file->last_channel = ioctl_param;

    //minor_file->channels = bst(minor_file->channels, channel);

    //fixup(minor_file->channels, channel);//check for sure if after that file->private_data->channels point to the root

    return SUCCESS;

}

// This structure will hold the functions to be called
// when a process does something to the device we created
struct file_operations Fops =
{
  .owner	      = THIS_MODULE, // Required for correct count of module usage. This prevents the module from being removed while used.
  .read           = device_read,
  .write          = device_write,
  .unlocked_ioctl = device_ioctl,
  .open           = device_open,
  .release        = device_release,
};


// Initialize the module - Register the character device
static int __init simple_init(void){

    int rc;
    int i;

    rc = -1;

    // init dev struct

    // Register driver capabilities. Obtain major num
    rc = register_chrdev( MAJOR_NUM, DEVICE_RANGE_NAME, &Fops );

    // Negative values signify an error
    if( rc < 0 )
    {
        printk( KERN_ERR "%s registraion failed for  %d\n", DEVICE_RANGE_NAME, MAJOR_NUM );
        return rc;
    }//end of if

    for(i =0; i <257; i++){
        if(arr_of_minor[i].minor_pointer != NULL){

            realese_tree(arr_of_minor[i].minor_pointer->channels);//sending the root of each tree for each minor number that exist

            kfree(arr_of_minor[i].minor_pointer);
            arr_of_minor[i].minor_pointer = NULL;

        }//end of if

    }//end of for

    return SUCCESS;

}//end of function simple_init

//---------------------------------------------------------------
static void __exit simple_cleanup(void)
{

    int i;    

    for(i =0; i<257; i++){
        
        if(arr_of_minor[i].minor_pointer != NULL){

            realese_tree(arr_of_minor[i].minor_pointer->channels);//sending the root of each tree for each minor number that exist

            kfree(arr_of_minor[i].minor_pointer);
            arr_of_minor[i].minor_pointer = NULL;

        }//end of if

    }//end of for

    //kfree(arr_of_minor);

    // Unregister the device
    // Should always succeed
    unregister_chrdev(MAJOR_NUM, DEVICE_RANGE_NAME);
  
}

//---------------------------------------------------------------
module_init(simple_init);
module_exit(simple_cleanup);

//========================= END OF FILE =========================
