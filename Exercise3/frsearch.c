#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RED 0
#define BLACK 1


typedef struct rbNode {
  int color;
  int channel_id_num; // channel_id_num is minor number
  char *message;
  int message_len;
  struct rbNode *link[2];
}rbNode;

typedef struct minor{
  int minor_num;//the number of the channel id
  struct rbNode *channels;//all the channels connected to this minor number
  int last_channel;//the last channel inserted by device_ioctl
}minor;




// Create a red-black tree
struct rbNode *createNode(int channel_id_num) {
    struct rbNode *newnode;
    newnode = (struct rbNode *)malloc(sizeof(struct rbNode));

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

  stack = (struct rbNode **)malloc(sizeof(struct rbNode) * 98);

  if (!almost_root->channels) {
    almost_root->channels = createNode(channel_id_num);

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
  free(stack);
  return 0;

}


// Print the inorder traversal of the tree
void inorderTraversal(struct rbNode *node) {
  if (node) {
    inorderTraversal(node->link[0]);
    printf("%d  ", node->channel_id_num);
    inorderTraversal(node->link[1]);
  }
  return;
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



int main() {
    int ch, channel_id_num;


        minor *minor_file;
        rbNode* temp_node;
        //rbNode* temp;
        //rbNode* root = NULL;

        
        minor_file = (minor*)malloc(sizeof(struct minor));

        /*if(minor_file == NULL){
            return -ENOMEM;
        }*/

        minor_file->minor_num = 17;
    
        minor_file->channels = NULL;

        /*if(insertion(minor_file, 0) < 0){
            return -ENOMEM;
        }

        if(minor_file->channels == NULL){
            printf("hi\n");
            return 0;
        }

        minor_file->last_channel = 0;*/

        insertion(minor_file, 1);
        insertion(minor_file, 2);
        insertion(minor_file, 3);
        insertion(minor_file, 4);

        temp_node = search_in_the_rbt(1, minor_file->channels);

        if(temp_node == NULL){
            printf("No such node\n");
        }
        else{
            printf("Found\n");
        }

        //insertion(minor_file, 2);*/

        //inorderTraversal(minor_file->channels);

    while (1) {
        printf("1. Insertion\t2. Deletion\n");
        printf("3. Traverse\t4. Exit\t5. search");
        printf("\nEnter your choice:");
        scanf("%d", &ch);
        switch (ch) {
        case 1:
            printf("Enter the element to insert:");
            scanf("%d", &channel_id_num);
            insertion(minor_file, channel_id_num);
            break;
        case 2:
            printf("Enter the element to delete:");
            scanf("%d", &channel_id_num);
            //deletion(channel_id_num);
            break;
        case 3:
            inorderTraversal(minor_file->channels);
            printf("\n");
            break;
        case 4:
            exit(0);

        case 5:
            printf("Enter the element to search:");
            scanf("%d", &channel_id_num);
            temp_node = search_in_the_rbt(channel_id_num ,minor_file->channels);
            printf("\n");
            if(temp_node == NULL){
                printf("No such node\n");
            }
            else{
                printf("Found\n");
            }
            break;
        default:
            printf("Not available\n");
            break;
        }
        printf("\n");
    }
    return 0;
}