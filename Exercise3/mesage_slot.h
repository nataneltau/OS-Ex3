#include <stdint.h>
#include <linux/kernel.h>   /* We're doing kernel work */
#include <linux/module.h>   /* Specifically, a module */
#include <linux/fs.h>       /* for register_chrdev */
#include <linux/string.h>   /* for memset. NOTE - not string.h!*/


#ifndef CHARDEV_H
#define CHARDEV_H

#include <linux/ioctl.h>


#define MAJOR_NUM 235
#define DEFAULT_SIZE 128
#define SUCCESS 0
#define IOCTL_SET_ENC _IOW(MAJOR_NUM, 0, unsigned int)
#define DEVICE_RANGE_NAME "message_slot"

#endif


//copied from the exercises
static ssize_t device_write( struct file*       file,
                             const char __user* buffer,
                             size_t             length,
                             loff_t*            offset);

static ssize_t device_read( struct file* file,
                            char __user* buffer,
                            size_t       length,
                            loff_t*      offset );

static int device_open( struct inode* inode,
                        struct file*  file );

static long device_ioctl( struct   file* file,
                          unsigned int   ioctl_command_id,
                          unsigned long  ioctl_param );

static int device_release( struct inode* inode,
                           struct file*  file);            



// This structure will hold the functions to be called
// when a process does something to the device we created
struct file_operations Fops =
{
  .owner	  = THIS_MODULE, // Required for correct count of module usage. This prevents the module from being removed while used.
  .read           = device_read,
  .write          = device_write,
  .open           = device_open,
  .release        = device_release,
};

struct minor{
  int minor_num;//the number of the channel id
  struct node channels;//all the channels connected to this minor number
  int last_channel;//the last channel inserted by device_ioctl
}

/** C implementation for
    Red-Black Tree Insertion
    This code is provided by
    costheta_z **/
 
// Structure to represent each
// node in a red-black tree
struct node {
    int channel_id_num; // data is minor number
    char message[DEFAULT_SIZE];
    int message_len;
    int c; // 1-red, 0-black
    struct node* p; // parent
    struct node* r; // right-child
    struct node* l; // left child
};
 
 
//FUNCTIONS OF THE RED BLACK TREE

//__________________________________________________
//__________________________________________________

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
void rightrotate(struct node* temp)
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
void leftrotate(struct node* temp)
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
                    leftrotate(parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->p;
                }
 
                /* Case : 3
                     pt is left child of its parent
                     Right-rotation required */
                rightrotate(grand_parent_pt);
                int t = parent_pt->c;
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
                    rightrotate(parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->p;
                }
 
                /* Case : 3
                     pt is right child of its parent
                     Left-rotation required */
                leftrotate(grand_parent_pt);
                int t = parent_pt->c;
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
void inorder(struct node* trav)
{
    if (trav == NULL)
        return;
    inorder(trav->l);
    printf("%d ", trav->channel_id_num);
    inorder(trav->r);
}

void realese_tree(struct node* trav){
  if(trav == NULL){
    return;
  }

  realese_tree(trav->l);
  realese_tree(trav->r);

  kfree(trav);

}

node* search(int x, node* n)
{
    if(n != NULL)
    {
        if(n.channel_id_num  == x)
                return (n)
        else if(n.channel_id_num > x)
                search(x, n.l)
        else
                search(x, n.r)
    }
    return NULL;
}
 
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