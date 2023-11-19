#pragma once
#include<iostream>
#include<tchar.h>
using namespace std;


//LC-3有65565个内存位置
#define MEMORY_MAX (1<<16)		//2^16次方	c++中整数字面量(如1)的默认数据类型是 int类型
uint16_t memory[MEMORY_MAX]		/*65536的大小*/