//
//  Pack.cpp
//
//  Created by Nick Beeuwsaert on 5/17/14.
//  Copyright (c) 2014 Nick Beeuwsaert.
//  MIT Licensed.
//
 
#include "Pack.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "endian.h"
 
 
 
void unpack(FILE* f, const char *fmt, ...){
 
    va_list ap;
    bool big_endian = __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__;
    va_start(ap, fmt);
    for(;*fmt != '\0'; fmt++){
        unsigned long int length=1;
 
        if(isdigit(*fmt)){
            length = strtoul(fmt, (char**)&fmt, 10);
        }
        switch(*fmt){
            case '>':
            case '<':
                big_endian = *fmt=='>';
            break;
            case 'I':
            case 'i':
            {
                uint32_t *data = va_arg(ap, uint32_t*);
                while(length-->0){
                    //file.read((char*)data, sizeof(uint32_t));
                    fread(data, sizeof(uint32_t), 1, f);
                    *(data++) = big_endian?be32toh(*data):le32toh(*data);
                }
            }break;
            case 'H':
            case 'h':
            {
                uint16_t *data = va_arg(ap, uint16_t*);
                while(length-->0){
                    //file.read((char*)data, sizeof(uint16_t));
                    fread(data, sizeof(uint16_t), 1, f);
                    *(data++) = big_endian?be16toh(*data):le16toh(*data);
                }
            }break;
            // B and c could probably be to same thing....
            case 'B':
            case 'b':
            {
                uint8_t *str_bytes = va_arg(ap, uint8_t*);
                fread(str_bytes, sizeof(uint8_t), length, f);
                //file.read((char*)str_bytes, length);
            }break;
            case 'c':{
                char *str = va_arg(ap, char*);
                fread(str, sizeof(char), length, f);
                //file.read(str, length);
            }break;
            case 'x':
                //file.seekg(length, file.cur);
                fseek(f, length, SEEK_CUR);
                continue;
        }
    }
    va_end(ap);
}
unsigned int calcsize(const char *fmt){
    unsigned int size = 0;
    for(;*fmt != '\0'; fmt++){
        unsigned long int length=1;
        if(isdigit(*fmt)){
            length = strtoul(fmt, (char**)&fmt, 10);
        }
        switch(*fmt){
            case 'I':
            case 'i':
                size += length * 4;
            break;
            case 'H':
            case 'h':
                size += length * 2;
            break;
            case 'B':
            case 'b':
            case 'c':
            case 'x':
                size += length;
            break;
        }
    }
    return size;
}