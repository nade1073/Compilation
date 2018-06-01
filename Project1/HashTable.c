#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "DataStruct.h"

#define SIZE 5000

typedef struct DataItem
{
	struct Data* data;
	int key;
}DataItem;

DataItem* hashArray[SIZE];
DataItem* dummyItem;
DataItem* item;

int hashCode(int key)
{
	return key % SIZE;
}

struct DataItem* createHashTable()
{
	DataItem* Dataitem = (DataItem*)malloc(sizeof(DataItem));
	return Dataitem;
}

struct DataItem *search(int key)
{
	//get the hash 
	int hashIndex = hashCode(key);

	//move in array until an empty 
	while (hashArray[hashIndex] != NULL)
	{
		if (hashArray[hashIndex]->key == key)
		{
			return hashArray[hashIndex];
		}

		//go to next cell
		++hashIndex;

		//wrap around the table
		hashIndex %= SIZE;
	}

	return NULL;
}

void insert(int key, Data* data)
{
	//get the hash 
	int hashIndex = hashCode(key);

	DataItem *item = (DataItem*)malloc(sizeof(DataItem));
	item->data = data;
	item->key = key;

	//move in array until an empty or deleted cell
	while (hashArray[hashIndex] != NULL && hashArray[hashIndex]->key != -1)
	{
		//go to next cell
		++hashIndex;

		//wrap around the table
		hashIndex %= SIZE;
	}

	hashArray[hashIndex] = item;
}

struct DataItem* deleteItem(DataItem* item)
{
	int key = item->key;

	//get the hash 
	int hashIndex = hashCode(key);

	//move in array until an empty
	while (hashArray[hashIndex] != NULL)
	{

		if (hashArray[hashIndex]->key == key)
		{
			DataItem* temp = hashArray[hashIndex];

			//assign a dummy item at deleted position
			hashArray[hashIndex] = dummyItem;
			return temp;
		}

		//go to next cell
		++hashIndex;

		//wrap around the table
		hashIndex %= SIZE;
	}

	return NULL;
}

void display()
{
	int i = 0;

	for (i = 0; i<SIZE; i++)
	{
		if (hashArray[i] != NULL)
		{
			printf(" (%d,%d)", hashArray[i]->key, hashArray[i]->data);
		}
		else
		{
			printf(" ~~ ");
		}
	}

	printf("\n");
}