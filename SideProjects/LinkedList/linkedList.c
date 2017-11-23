#include <stdio.h>
#include <stdlib.h>


struct Node* node[10];
struct Node* head;

struct Node 
{
 int value;
 struct Node* next;
   
};

struct Node* createNode(int value)
{
   struct Node* node = malloc(sizeof(struct Node));
   node->value = value;
   return node;
}

struct Node* insertAtHead(struct Node* node)
{
   node->next = head;
   head = node;
   return head;
}

struct Node* deleteNode(int value)
{
   
   if (value == head->value)
     {
	struct Node* temp = head->next;
	free(head);
	head = temp;
	return head;
     }
   
   
   struct Node* currentNode = head;
   while (currentNode->next->value != -1)
     {
	if (value == currentNode->next->value)
	  {
	     struct Node* temp = currentNode->next->next;
	     free(currentNode->next);
	     currentNode->next = temp;
	     return head;
	  }
	currentNode = currentNode->next;
     }
   
}

struct Node* findNode(int value, struct Node* node)
{
   
}



int main(int argc, char** argv)
{
   int i;
   head = createNode(-1);
   for (i = 0; i < 10; i++)
     {
	node[i] = createNode(i); 
     }
   insertAtHead(node[0]);
   insertAtHead(node[1]);
   insertAtHead(node[2]);
   printf("Value of head is: %d\n", head->value);
   printf("Value of next is: %d\n", head->next->value);
   printf("Value of next next is: %d\n", head->next->next->value);
   deleteNode(1);
   printf("Value of head is: %d\n", head->value);
   printf("Value of next is: %d\n", head->next->value);
   printf("Value of next next is: %d\n", head->next->next->value);
  
   
   return 0;
}
