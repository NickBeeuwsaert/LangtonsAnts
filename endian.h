//
//  endian.h
//
//  Created by Nick Beeuwsaert on 5/17/14.
//  Copyright (c) 2014 Nick Beeuwsaert.
//  MIT Licensed.
//
 
#ifndef __ENDIAN_H__
#define __ENDIAN_H__
 
#if defined(__linux__) || defined(__CYGWIN__)
#    include <endian.h>
#elif defined(__APPLE__)
#include <libkern/OSByteOrder.h>
 
#define le16toh(x) OSSwapLittleToHostInt16(x)
#define htole16(x) OSSwapHostToLittleInt16(x)
#define be16toh(x) OSSwapBigToHostInt16(x)
#define htobe16(x) OSSwapHostToBigInt16(x)
 
#define le32toh(x) OSSwapLittleToHostInt32(x)
#define htole32(x) OSSwapHostToLittleInt32(x)
#define be32toh(x) OSSwapBigToHostInt32(x)
#define htobe32(x) OSSwapHostToBigInt32(x)
 
#define le64toh(x) OSSwapLittleToHostInt64(x)
#define htole64(x) OSSwapHostToLittleInt64(x)
#define be64toh(x) OSSwapBigToHostInt64(x)
#define htobe64(x) OSSwapHostToBigInt64(x)
 
#elif defined(__OpenBSD__)
#include <sys/endian.h>
#elif defined(_WIN32) || defined(__MINGW32__)
#if BYTE_ORDER == BIG_ENDIAN
//XBox 360
#define htobe16(x) (x)
#define htole16(x) __builtin_bswap16(x)
#define be16toh(x) (x)
#define le16toh(x) __builtin_bswap16(x)
 
#define htobe32(x) (x)
#define htole32(x) __builtin_bswap32(x)
#define be32toh(x) (x)
#define le32toh(x) __builtin_bswap32(x)
 
#define htobe64(x) (x)
#define htole64(x) __builtin_bswap64(x)
#define be64toh(x) (x)
#define le64toh(x) __builtin_bswap64(x)
 
#elif BYTE_ORDER == LITTLE_ENDIAN
#define htobe16(x) htons(x)
#define htole16(x) (x)
#define be16toh(x) ntohs(x)
#define le16toh(x) (x)
 
#define htobe32(x) htonl(x)
#define htole32(x) (x)
#define be32toh(x) ntohl(x)
#define le32toh(x) (x)
 
#define htobe64(x) htonll(x)
#define htole64(x) (x)
#define be64toh(x) ntohll(x)
#define le64toh(x) (x)
 
#else
#error bad byte order
#endif
#else
#error platform not supported
#endif
 
 
#endif