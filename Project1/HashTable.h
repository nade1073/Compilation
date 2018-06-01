#ifndef HashTableFile
#define HashTableFile

#include <stdio.h>
#include "Token.h"
#include <stdlib.h>
#include "DataStruct.h"

typedef struct DataItem
{
	struct Data* m_Data;
	int m_Key;
}DataItem;

typedef struct HashTable
{
	struct DataItem* m_Content;
	int m_SizeOfContent;
}HashTable;
int hashCode(int i_Size, int i_Key);
HashTable* createHashTable();
DataItem* searchInsideHashTableAndReturnItem(HashTable i_CurrentHashTable, int i_Key);
void insert(HashTable* i_CurrentHashTable, DataItem* i_Data);
void deleteItem(struct HashTable i_CurrentHashTable, DataItem* i_Item);

#endif 

