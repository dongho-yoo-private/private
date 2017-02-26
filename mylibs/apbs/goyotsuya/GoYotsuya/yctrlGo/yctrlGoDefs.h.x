#pragma once

#ifndef __GODEFS__
#define __GODEFS__

enum eGoLineCount
{
	LINE_19 = 19,
	LINE_13 = 13,
	LINE_9 = 9,
	LINE_DEFAULT = LINE_19
};

enum eStoneType
{
	STONE_NONE=0,
	STONE_BLACK,
	STONE_WHITE,
	STONE_CHAR,
	STONE_BLACK_CHAR,
	STONE_WHITE_CHAR,
	STONE_WHITE_PATH,
	STONE_BLACK_PATH,
	STONE_DEAD_STONE,
	STONE_INVALID=0x0000007F
};


#define MAX_GO_BLOCK_COUNT 361


typedef unsigned int stone_t;

typedef stone_t aStoneList[MAX_GO_BLOCK_COUNT];



#define STONE_POS_X(s)	(0x000000FF&s)
#define STONE_POS_Y(s)	((0x0000FF00&s)>>8)
#define STONE_ORDER(s)	((0x0001FF00&s)>>16)
#define STONE_TYPE(s)	((eStoneType)(s>>25))
#define STONE_EOF		0xFFFFFFFF



#define MAKESTONE(type, order, x, y) (((type)<<25)|((order)<<16) | ((y)<<8) | (x))

#endif