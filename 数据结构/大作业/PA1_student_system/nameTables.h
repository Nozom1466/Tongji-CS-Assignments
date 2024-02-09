#pragma once
#include "hashTable.hpp"
#include "student.h"
#include "curriculum.h"

class NameTables
{
public:

	HashTable<Student> studentHashTable = HashTable<Student>(3000);
	HashTable<Curriculum> curriculumHashTable = HashTable<Curriculum>(40000);
};
