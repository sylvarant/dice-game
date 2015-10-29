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
 *         Author:  ajhl
 *
 * =====================================================================================
 */

#include <errno.h>
#include "util.h"


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  __safe_malloc
 *  Description:  malloc the memory and exit when this fails
 * =====================================================================================
 */
extern void * __safe_malloc(size_t _size,const char *_file, int _line)
{
	void * p = malloc(_size);

	if (p == NULL && _size != 0)
	{
		fprintf(stderr,"malloc %lu bytes failed Exiting At %s:%d \n",(unsigned long) _size,_file,_line);
		exit(1);
	}

	return p;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  __safe_realloc
 *  Description:  reallocate the memory, exit on failure
 * =====================================================================================
 */
extern void * __safe_realloc(void * _ptr,size_t _size,const char * _file,int _line)
{
	void * temp = realloc(_ptr,_size);
	if(temp == NULL)
	{
		fprintf(stderr,"realloc %lu bytes failed Exiting At %s:%d \n",(unsigned long) _size,_file,_line);
		exit(1);
	}
	return temp;
}


/*  
 * ===  FUNCTION  ======================================================================
 *         Name:  safeOpen
 *  Description:  open the file and handle the error according to flags
 * =====================================================================================
 */
extern void __safe_file_open(FILE ** _pf,const char * _file,const char * _param,
							const char * _src,int _line)
{
	*_pf = fopen(_file,_param);
	
	if(*_pf == NULL)
	{
		fprintf(stderr,"opening %s with %s failed Exiting At %s:%d \n",_file,_param,_src,_line);
		exit(1);
	}
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  safeClose
 *  Description:  close the file handle an error according to flags
 * =====================================================================================
 */
extern void __safe_file_close(FILE * _pf,const char * _src,int _line)
{
	if(fclose(_pf) != 0)
	{
		fprintf(stderr,"closing %#x failed, Exiting At %s:%d \n",_pf,_src,_line);
		exit(1);
	}
}

	
