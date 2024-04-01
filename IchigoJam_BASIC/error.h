// Copyright 2014-2024 the IchigoJam authors. All rights reserved. MIT license.

#ifndef __ERROR_H__
#define __ERROR_H__

static const char* ERR_MESSAGES = // 36byte -> 88byte , -52byte
	"\0"
	"Syntax error" "\0"			// 13
	"Out of memory" "\0"		// 14
	"Stack overflow" "\0"		// 15
	"Not match" "\0"			// 10
	"Line error" "\0"			// 11
	"Divide by 0" "\0"			// 12
	"Index out of range" "\0"	// 19
	"File error" "\0"			// 11
	"Segmentation Fault" "\0"	// 19 = 
	"Complex expression" "\0"	// 19
	"Illegal argument" "\0"		// 17
	"Break";

enum {
	ERR_NO_ERROR,
	ERR_SYNTAX_ERROR = ERR_NO_ERROR + 1,
	ERR_OUT_OF_MEMORY = ERR_SYNTAX_ERROR + 13,
	ERR_STACK_OVERFLOW = ERR_OUT_OF_MEMORY + 14,
	ERR_NOT_MATCH = ERR_STACK_OVERFLOW + 15,
	ERR_UNDEFINED_LINE = ERR_NOT_MATCH + 10,
	ERR_DIVIDE_BY_ZERO = ERR_UNDEFINED_LINE + 11,
	ERR_INDEX_OUT_OF_RANGE = ERR_DIVIDE_BY_ZERO + 12,
	ERR_FILE_ERROR = ERR_INDEX_OUT_OF_RANGE + 19,
	ERR_SEGMENTATION_FAULT = ERR_FILE_ERROR + 11,
	ERR_COMPLEX_EXPRESSION = ERR_SEGMENTATION_FAULT + 19,
	ERR_ILLEGAL_ARGUMENT = ERR_COMPLEX_EXPRESSION + 19,
	ERR_BREAK = ERR_ILLEGAL_ARGUMENT + 17,
};


/*
enum { // 88byte節約
	ERR_NO_ERROR,
	ERR_SYNTAX_ERROR,
	ERR_OUT_OF_MEMORY,
	ERR_STACK_OVERFLOW,
	ERR_NOT_MATCH,
	ERR_UNDEFINED_LINE,
	ERR_DIVIDE_BY_ZERO,
	ERR_INDEX_OUT_OF_RANGE,
	ERR_FILE_ERROR,
	ERR_SEGMENTATION_FAULT,
	ERR_COMPLEX_EXPRESSION,
	ERR_ILLEGAL_ARGUMENT,
	ERR_BREAK,
};

static const char* ERR_MESSAGES[] = {
//	"",
	"Syntax error",
	"Out of memory",
	"Stack overflow",
	"Not match",
	"Line error",
	"Divide by 0",
	"Index out of range",
	"File error",
	"Segmentation Fault",
	"Complex expression",
	"Illegal argument",
	"Break",
};
*/


#endif	// ERROR_H_
