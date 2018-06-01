
#include <stdio.h>
#include "HashTable.h"
typedef struct node
{
	struct DataItem* data;
	struct node* next;
}node;

/*
init the stack
*/
void init(node* head)
{
	head = NULL;
}

/*
push an element into stack
*/
struct node* push(struct node* head, struct DataItem* element)
{
	struct node* tmp = (struct node*)malloc(sizeof(struct node));
	if (tmp == NULL)
	{
		exit(0);
	}
	tmp->data = element;
	tmp->next = head;
	head = tmp;
	return head;
}
/*
pop an element from the stack
*/
struct node* pop(struct node *head,struct DataItem* element)
{
	struct node* tmp = head;
	element = head->data;
	head = head->next;
	free(tmp);
	return head;
}
struct node* top(struct node *head, struct DataItem* element)
{
	return head;
}
/*
returns 1 if the stack is empty, otherwise returns 0
*/
int empty(struct node* head)
{
	return head == NULL ? 1 : 0;
}

/*
display the stack content
*/
void display1(struct node* head)
{
	struct node *current;
	current = head;
	if (current != NULL)
	{
		printf("Stack: ");
		do
		{
			printf("%d ", current->data);
			current = current->next;
		} while (current != NULL);
		printf("\n");
	}
	else
	{
		printf("The Stack is empty\n");
	}

}