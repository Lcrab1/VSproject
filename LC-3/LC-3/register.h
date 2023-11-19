#pragma once
#include<iostream>
#include<tchar.h>
using namespace std;

enum
{
	R_RO = 0,
	R_R2,
	R_R3,
	R_R4,
	R_R5,
	R_R6,
	R_R7,
	R_PC,	/*program counter*/
	R_COND,
	R_COUNT
};

uint16_t reg[R_COUNT];

//条件标志位
enum {
	FL_POS = 1 << 0,	/*P*/
	FL_ZRO = 1 << 1,	/*Z*/
	FL_NEG = 1 << 2,	/*N*/
};