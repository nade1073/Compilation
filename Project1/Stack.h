#ifndef Stack
#define Stack

#include <stdio.h>
#include "HashTable.h"

typedef struct NodeOfStack
{
	struct HashTable* m_HashTable;
	struct NodeOfStack* m_NextNode;
}NodeOfStack;

typedef struct StackOfHashTables
{
	struct NodeOfStack* head;
}StackOfHashTables;

int isEmpty( StackOfHashTables* i_Stack);
void push(StackOfHashTables* i_Stack, HashTable* i_Table);
void pop(StackOfHashTables* i_Stack);
HashTable* top(StackOfHashTables*);
StackOfHashTables* createStack();

#endif