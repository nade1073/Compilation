#ifndef Stack
#define Stack

#include <stdio.h>
#include "HashTable.h"

typedef struct node;
int empty(struct node *s);
struct node* push(struct node *s, struct DataItem* element);
struct node* pop(struct node *s, struct DataItem* element);
void init(struct node* s);
void display1(struct node* head);

#endif