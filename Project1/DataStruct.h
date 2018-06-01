#ifndef DATASTRUCT
#define DATASTRUCT

typedef struct Data
{
	char* name;
	enum eRoleTypes role;
	char* typeOfVariable;
	enum eCategoryOfType categoryOfType;
	struct Data* subType;
	int sizeOfArray;
}Data;

typedef enum eRoleTypes
{
	Variable,
	UserDefinedType
}eRoleTypes;

typedef enum eCategoryOfType
{
	Basic,
	Array,
	Pointer
}eCategoryOfType;

#endif