/*
 * =====================================================================================
 *
 *       Filename:  util.h
 *
 *    Description:  handle error's in a unified way
 *
 *        Version:  1.0
 *        Created:  09/13/2009 21:36:54 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author: ajhl 
 *
 * =====================================================================================
 */



#ifndef DOBBELEN_UTIL_H
#define DOBBELEN_UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/** macro's*/
#define clearStruct(var, type) memset(var, 0, sizeof(type))

/*-----------------------------------------------------------------------------
 *  memory management
 *-----------------------------------------------------------------------------*/
#define safeMalloc(size) __safe_malloc(size, __FILE__, __LINE__)
#define safeRealloc(ptr,size) __safe_realloc(ptr,size,__FILE__,__LINE__)

extern void * __safe_malloc(size_t size,const char * file, int line);
extern void * __safe_realloc(void * ptr,size_t size,const char * file,int line);


/*-----------------------------------------------------------------------------
 *  File management
 *-----------------------------------------------------------------------------*/
#define safeFileOpen(pf,file,par) __safe_file_open(&pf,file,par,__FILE__,__LINE__)
#define safeFileClose(pf) __safe_file_close(pf,__FILE__,__LINE__) 

extern void __safe_file_open(FILE ** pf,const char * file,const char * par,const char * src,int line);
extern void __safe_file_close(FILE *pf,const char * src,int line);

#endif
