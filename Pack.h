//
//  Pack.h
//
//  Created by Nick Beeuwsaert on 5/17/14.
//  Copyright (c) 2014 Nick Beeuwsaert.
//  MIT Licensed.
//
 
#ifndef __PACK_H__
#define __PACK_H__
 
#include <stdio.h>
 
/**
* Unpacks the data from file into variables
* Supposed to work similar to how python's struct works
* > Means switch to Big Endian
* < Means switch to Little Endian
* H means read a short (uint16_t)
* I means read a int (uint32_t)
* B means read a byte
* c means read a character
* x means skip a byte
* you can prefix a format character with a number to
* mark how many of the following type to read
* unlike pythons struct, the case of the letter doesn't matter
*
* @param file The file to read data from
* @param format The format to use to read
* @param ... Variables to write out to
*/
void unpack(FILE *, const char *, ...);
/**
* Calculates the size of the fmt passed
* See unpack for fmt description
*
* @param fmt The format
*/
unsigned int calcsize(const char*);
#endif /* __PACK_H__ */