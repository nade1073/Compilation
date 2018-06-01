#ifndef HashTable
#define HashTable

#include <stdio.h>
#include "Token.h"
#include "DataStruct.h"
typedef struct DataItem;
struct DataItem* createHashTable();
int hashCode(int key);
struct DataItem *search(int key);
void insert(int key, Data* data);
struct DataItem* deleteItem(struct DataItem* item);
void display();
#endif 

