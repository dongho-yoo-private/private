#ifndef __yi_hash_func_h__
#define __yi_hash_func_h__

#include "yi.h"
//
unsigned int yihf_general(const char* p, int max_shift);

//
unsigned int yihf_additional(const char* p, int max_shift);


//
unsigned int yihf_sub(const char* p, int max_shift);

//
unsigned int yihf_encoded_additional(const char* p, int max_shift);

//
unsigned int yihf_shift_additional(const char* p, int max_shift);

#endif
