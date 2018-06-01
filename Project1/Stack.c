#include "Stack.h"

void push(StackOfHashTables* i_Stack, HashTable* i_Table)
{
	NodeOfStack *tempNode= (NodeOfStack*)malloc(sizeof(NodeOfStack));
	if (tempNode == NULL)
	{
		exit(0);
	}
	tempNode->m_HashTable = i_Table;
	tempNode->m_NextNode = i_Stack->head;
	i_Stack->head = tempNode;
}

void pop(StackOfHashTables* i_Stack)
{
	NodeOfStack *tempNode = i_Stack->head;
	i_Stack->head = i_Stack->head->m_NextNode;
	free(tempNode);
}

int isEmpty(StackOfHashTables* i_Stack)
{
	return i_Stack->head == NULL ? 1 : 0;
}

HashTable* top(StackOfHashTables* i_Stack)
{
	return i_Stack->head->m_HashTable;
}

StackOfHashTables* createStack()
{
	StackOfHashTables* stackToReturn = (StackOfHashTables*)malloc(sizeof(StackOfHashTables));
	stackToReturn->head = NULL;
	return stackToReturn;
}
