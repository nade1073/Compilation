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

HashTable* pop(StackOfHashTables* i_Stack)
{
	NodeOfStack *tempNode = i_Stack->head;
	i_Stack->head = i_Stack->head->m_NextNode;
	return tempNode->m_HashTable;
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

void pushAllItemFromFirstStackToSecondStack(StackOfHashTables* i_First, StackOfHashTables* i_Second)
{
	while (!isEmpty(i_First))
	{
		push(i_Second,pop(i_First));
	}
}