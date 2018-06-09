#include "HashTable.h"

#define SIZE 5000

int hashCode(int i_Size,int i_Key)
{
	//return i_Key%i_Size;
	if (i_Key < 0)
	{
		while (i_Key < 0)
		{
			i_Key += i_Size;
		}
	}
	else
	{
		return i_Key%i_Size;
	}
	return i_Key;
}

HashTable* createHashTable()
{
	HashTable* hashTableToReturn = (HashTable*)malloc(sizeof(HashTable));
	hashTableToReturn->m_SizeOfContent = SIZE;
	hashTableToReturn->m_Content = (DataItem*)calloc(SIZE, sizeof(DataItem));
	return hashTableToReturn;
}

DataItem* searchInsideHashTableAndReturnItem(HashTable* i_CurrentHashTable, int i_Key)
{
	int hashIndex = hashCode((*i_CurrentHashTable).m_SizeOfContent,i_Key);

	while ((*i_CurrentHashTable).m_Content[hashIndex].m_Data != NULL)
	{
		if ((*i_CurrentHashTable).m_Content[hashIndex].m_Key == i_Key)
		{
			return &(i_CurrentHashTable->m_Content[hashIndex]);
		}

		//go to next cell
		++hashIndex;
		//wrap around the table
		hashIndex %= (*i_CurrentHashTable).m_SizeOfContent;
	}
	return NULL;
}
 
void insert(HashTable* i_CurrentHashTable, DataItem* i_Data)
{
	//get the hash 
	int hashIndex = hashCode((*i_CurrentHashTable).m_SizeOfContent,i_Data->m_Key);

	//move in array until an empty or deleted cell
	while ((*i_CurrentHashTable).m_Content[hashIndex].m_Data != NULL)
	{
		//go to next cell
		++hashIndex;
		//wrap around the table
		hashIndex %= (*i_CurrentHashTable).m_SizeOfContent;
	}
	DataItem* currentDataItem = &((*i_CurrentHashTable).m_Content[hashIndex]);
	currentDataItem->m_Data = (Data*)malloc(sizeof(Data));
	currentDataItem->m_Key = i_Data->m_Key;
	currentDataItem->m_Data->name = i_Data->m_Data->name;
	currentDataItem->m_Data->categoryOfType = i_Data->m_Data->categoryOfType;
	currentDataItem->m_Data->role = i_Data->m_Data->role;
	currentDataItem->m_Data->sizeOfArray = i_Data->m_Data->sizeOfArray;
	currentDataItem->m_Data->subType = i_Data->m_Data->subType;
	currentDataItem->m_Data->basicSubTypeName = i_Data->m_Data->basicSubTypeName;
	currentDataItem->m_Data->typeOfVariable = i_Data->m_Data->typeOfVariable;
}

int StringToIntHash(const char* s) {
	int h = 0;
	while (*s) {
		h = 31 * h + (*s++);
	}
	return h;
}
