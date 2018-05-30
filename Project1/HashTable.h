#ifndef HashTable
#define HashTable

#include <stdio.h>
#include "Token.h"
typedef struct DataItem;
int hashCode(int key);
DataItem *search(int key);
void insert(int key, int data);
DataItem* deleteItem(DataItem* item);
void display();

#endif