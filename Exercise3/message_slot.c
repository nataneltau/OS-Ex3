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
    red black tree: https://www.geeksforgeeks.org/c-program-red-black-tree-insertion/
    free tree memory: https://stackoverflow.com/questions/9181146/freeing-memory-of-a-binary-tree-c
*/







//________________________________________________________________________________________________________
//________________________________________________________________________________________________________
//__________ RED BLACK TREE _________ RED BLACK TREE _________ RED BLACK TREE ______ RED BLACK TREE ______
//________________________________________________________________________________________________________
//________________________________________________________________________________________________________

/** C implementation for
    Red-Black Tree Insertion
    This code is provided by
    costheta_z **/
 
// Structure to represent each
// node in a red-black tree - node are channels

 
//FUNCTIONS OF THE RED BLACK TREE

//__________________________________________________
//__________________________________________________

struct node* search_in_the_rbt(int x, struct node* n)
{
    if(n != NULL)
    {
        if(n->channel_id_num  == x)
                return (n);
        else if(n->channel_id_num > x)
                search_in_the_rbt(x, n->l);
        else
                search_in_the_rbt(x, n->r);
    }
    return NULL;
}



// function to perform BST insertion of a node
struct node* bst(struct node* trav,
                      struct node* temp)
{
    // If the tree is empty,
    // return a new node
    if (trav == NULL)
        return temp;
 
    // Otherwise recur down the tree
    if (temp->channel_id_num < trav->channel_id_num)
    {
        trav->l = bst(trav->l, temp);
        trav->l->p = trav;
    }
    else if (temp->channel_id_num > trav->channel_id_num)
    {
        trav->r = bst(trav->r, temp);
        trav->r->p = trav;
    }
 
    // Return the (unchanged) node pointer
    return trav;
}
 
// Function performing right rotation
// of the passed node
void rightrotate(struct node *root, struct node* temp)
{
    struct node* left = temp->l;
    temp->l = left->r;
    if (temp->l)
        temp->l->p = temp;
    left->p = temp->p;
    if (!temp->p)
        root = left;
    else if (temp == temp->p->l)
        temp->p->l = left;
    else
        temp->p->r = left;
    left->r = temp;
    temp->p = left;
}
 
// Function performing left rotation
// of the passed node
void leftrotate(struct node *root, struct node* temp)
{
    struct node* right = temp->r;
    temp->r = right->l;
    if (temp->r)
        temp->r->p = temp;
    right->p = temp->p;
    if (!temp->p)
        root = right;
    else if (temp == temp->p->l)
        temp->p->l = right;
    else
        temp->p->r = right;
    right->l = temp;
    temp->p = right;
}
 
// This function fixes violations
// caused by BST insertion
void fixup(struct node* root, struct node* pt)
{
    struct node* parent_pt = NULL;
    struct node* grand_parent_pt = NULL;
    int t;
 
    while ((pt != root) && (pt->c != 0)
           && (pt->p->c == 1))
    {
        parent_pt = pt->p;
        grand_parent_pt = pt->p->p;
 
        /*  Case : A
             Parent of pt is left child
             of Grand-parent of
           pt */
        if (parent_pt == grand_parent_pt->l)
        {
 
            struct node* uncle_pt = grand_parent_pt->r;
 
            /* Case : 1
                The uncle of pt is also red
                Only Recoloring required */
            if (uncle_pt != NULL && uncle_pt->c == 1)
            {
                grand_parent_pt->c = 1;
                parent_pt->c = 0;
                uncle_pt->c = 0;
                pt = grand_parent_pt;
            }
 
            else {
 
                /* Case : 2
                     pt is right child of its parent
                     Left-rotation required */
                if (pt == parent_pt->r) {
                    leftrotate(root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->p;
                }
 
                /* Case : 3
                     pt is left child of its parent
                     Right-rotation required */
                rightrotate(root, grand_parent_pt);
                t = parent_pt->c;
                parent_pt->c = grand_parent_pt->c;
                grand_parent_pt->c = t;
                pt = parent_pt;
            }
        }
 
        /* Case : B
             Parent of pt is right
             child of Grand-parent of
           pt */
        else {
            struct node* uncle_pt = grand_parent_pt->l;
 
            /*  Case : 1
                The uncle of pt is also red
                Only Recoloring required */
            if ((uncle_pt != NULL) && (uncle_pt->c == 1))
            {
                grand_parent_pt->c = 1;
                parent_pt->c = 0;
                uncle_pt->c = 0;
                pt = grand_parent_pt;
            }
            else {
                /* Case : 2
                   pt is left child of its parent
                   Right-rotation required */
                if (pt == parent_pt->l) {
                    rightrotate(root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->p;
                }
 
                /* Case : 3
                     pt is right child of its parent
                     Left-rotation required */
                leftrotate(root, grand_parent_pt);
                t = parent_pt->c;
                parent_pt->c = grand_parent_pt->c;
                grand_parent_pt->c = t;
                pt = parent_pt;
            }
        }
    }
 
    root->c = 0;
}
 
// Function to print inorder traversal
// of the fixated tree
/*void inorder(struct node* trav)
{
    if (trav == NULL)
        return;
    inorder(trav->l);
    printf("%d ", trav->channel_id_num);
    inorder(trav->r);
}*/

void realese_tree(struct node* trav){
  if(trav == NULL){
    return;
  }

  realese_tree(trav->l);
  realese_tree(trav->r);

  kfree(trav);

}



/*void insert_and_fix(struct node* root, struct node* node_to_insert){
    // calling function that performs bst insertion of
    // this newly created node
    root = bst(root, node_to_insert);
 
    // calling function to preserve properties of rb
    // tree
    fixup(root, node_to_insert);
}*/
 
// driver code
/*int main()
{
    int n = 7;
    int a[7] = { 7, 6, 5, 4, 3, 2, 1 };
 
    for (int i = 0; i < n; i++) {
 
        // allocating memory to the node and initializing:
        // 1. color as red
        // 2. parent, left and right pointers as NULL
        // 3. data as i-th value in the array
        struct node* temp
            = (struct node*)malloc(sizeof(struct node));
        temp->r = NULL;
        temp->l = NULL;
        temp->p = NULL;
        temp->channel_id_num = a[i];
        temp->c = 1;
 
        // calling function that performs bst insertion of
        // this newly created node
        root = bst(root, temp);
 
        // calling function to preserve properties of rb
        // tree
        fixup(root, temp);
    }
 
    printf("Inorder Traversal of Created Tree\n");
    inorder(root);
 
    return 0;
}*/


//________________________________________________________________________________________________________
//________________________________________________________________________________________________________
//__________ RED BLACK TREE _________ RED BLACK TREE _________ RED BLACK TREE ______ RED BLACK TREE ______
//________________________________________________________________________________________________________
//________________________________________________________________________________________________________





// global array for all the trees

global_root arr_of_minor[257];


//global_root *arr_of_minor = (global_root*)kmalloc(sizeof(global_root)*257, GFP_KERNEL);



static int device_open( struct inode* inode, struct file*  file ){
    minor *minor_file;
    node* root = NULL;

    if((arr_of_minor[iminor(inode)].minor_pointer) != NULL){//already created data
        file->private_data = (void *)(arr_of_minor[iminor(inode)].minor_pointer);
        //arr_of_minor[iminor(inode)]->counter++;
        return SUCCESS;
    }
    
    minor_file = kmalloc(sizeof(struct minor), GFP_KERNEL);

    if(minor_file == NULL){
        return -ENOMEM;
    }

    minor_file->minor_num = iminor(inode);
    arr_of_minor[minor_file->minor_num].minor_pointer = minor_file;
    //arr_of_minor[minor_file->minor_num]->counter = 1;
    minor_file->channels = kmalloc(sizeof(struct node), GFP_KERNEL);

    if(minor_file->channels == NULL){
        return -ENOMEM;
    }

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


static ssize_t device_read( struct file* file, char __user* buffer, size_t length, loff_t* offset ){

    struct node* channel;
    int i;
    struct node* root;
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

    while(i<length){

        if (put_user(the_message[i], buffer+i) != 0) {//check if put_user failed
            return -EFAULT;
		}
        i++;

    }//end of while

    return i;
}

static ssize_t device_write( struct file* file, const char __user* buffer, size_t length, loff_t* offset){

    struct node* root;
    int channel_id;
    node* channel;
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
    node *channel;

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

    
    channel = kmalloc(sizeof(node), GFP_KERNEL);

    if(channel == NULL){
        return -ENOMEM;
    }

    channel->r = NULL;
    channel->l =NULL;
    channel->p = NULL;
    channel->channel_id_num = ioctl_param;
    channel->message_len = 0;
    channel->c = 1;

    minor_file->last_channel = ioctl_param;

    minor_file->channels = bst(minor_file->channels, channel);

    fixup(minor_file->channels, channel);//check for sure if after that file->private_data->channels point to the root

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
