#include "HashTable.h"

#define SIZE 5000



int hashCode(int i_Size,int i_Key)
{
	return i_Key%i_Size;
}

HashTable* createHashTable()
{
	HashTable* hashTableToReturn = (HashTable*)malloc(sizeof(HashTable));
	hashTableToReturn->m_SizeOfContent = SIZE;
	hashTableToReturn->m_Content = (DataItem**)malloc(sizeof(DataItem*));
	return hashTableToReturn;
}

struct DataItem* searchInsideHashTableAndReturnItem(HashTable i_CurrentHashTable, int i_Key)
{
	int hashIndex = hashCode(i_CurrentHashTable.m_SizeOfContent,i_Key);
	while (   (i_CurrentHashTable).m_Content[hashIndex]   != NULL)
	{
		if ((i_CurrentHashTable.m_Content[hashIndex])->m_Key == i_Key)
		{
			return i_CurrentHashTable.m_Content[hashIndex];
		}
		//go to next cell
		++hashIndex;
		//wrap around the table
		hashIndex %= (i_CurrentHashTable).m_SizeOfContent;
	}
	return NULL;
}
 
void insert(HashTable i_CurrentHashTable, DataItem* i_Data)
{
	//get the hash 
	int hashIndex = hashCode(i_CurrentHashTable.m_SizeOfContent,i_Data->m_Key);

	//move in array until an empty or deleted cell
	while ((i_CurrentHashTable).m_Content[hashIndex] != NULL)
	{
		//go to next cell
		++hashIndex;
		//wrap around the table
		hashIndex %= (i_CurrentHashTable).m_SizeOfContent;
	}
	i_CurrentHashTable.m_Content[hashIndex] = i_Data;
}

void deleteItem(HashTable i_CurrentHashTable,DataItem* i_Item)
{
	int key = i_Item->m_Key;

	//get the hash 
	int hashIndex = hashCode(i_CurrentHashTable.m_SizeOfContent,key);

	//move in array until an empty
	while ((i_CurrentHashTable).m_Content[hashIndex] != NULL)
	{

		if ((i_CurrentHashTable.m_Content[hashIndex])->m_Key == key)
		{
			//i_CurrentHashTable.m_Content[hashIndex] = NULL;
			free(i_CurrentHashTable.m_Content[hashIndex]);
			//Or Free
			break;
		}
		//go to next cell
		++hashIndex;
		//wrap around the table
		hashIndex %= (i_CurrentHashTable).m_SizeOfContent;
	}
}

//void display()
//{
//	int i = 0;
//
//	for (i = 0; i<SIZE; i++)
//	{
//		if (hashArray[i] != NULL)
//		{
//			printf(" (%d,%d)", hashArray[i]->key, hashArray[i]->data);
//		}
//		else
//		{
//			printf(" ~~ ");
//		}
//	}
//
//	printf("\n");
//}