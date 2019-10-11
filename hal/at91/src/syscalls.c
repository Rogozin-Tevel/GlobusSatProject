/*!
 * @file	syscalls.c
 * Implements some base-level for libc for heap management and printing on the debug port.
 * Functions such as printf for printing and reading from the debug port are implemented
 * in AT91lib/utility. Therefore, functions such as _read and _write are provided solely
 * for the user to be able to use standard libc implementations if they want.
 * Using the AT91lib implementations is recommended as it significantly reduces code size.
 */

#include "at91/utility/exithandler.h"
#include "at91/peripherals/dbgu/dbgu.h"
#include "at91/commons.h"

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>

#if USE_AT91LIB_STDIO_AND_STRING

int _read(int file, void *ptr, size_t len) {
	(void)file;
	(void)ptr;
	(void)len;
	return -1;
}

int _write(int file, const void *ptr, size_t len) {
	(void)file;
	(void)ptr;
	(void)len;
	return -1;
}

#else // #if USE_AT91LIB_STDIO_AND_STRING

#define AUTO_RETURN_AFTER_NEWLINE	1

// _write currently adds \n after \r automatically.
// If the At91lib\utility\stdio.c printf implementation is not used anymore:
// remove if there are spurious newlines!

int _read(int file, void *ptr, size_t len) {
	(void)len;
	if(file <= STDERR_FILENO) {
		*(char *) ptr = DBGU_GetChar();
#if AUTO_RETURN_AFTER_NEWLINE
		if((*(char *) ptr != '\n') && (*(char *) ptr != '\r')) {
			DBGU_PutChar(*(char *) ptr);
		}
		else if(*(char *) ptr == '\r') {
			DBGU_PutChar('\n');
			DBGU_PutChar('\r');
		}
#endif
		return 1;
	}
	else {
		return -1;
	}

}

int _write(int file, const void *ptr, size_t len) {
	size_t i;
	if(file <= STDERR_FILENO) {
		for(i=0; i<len; i++) {
#if AUTO_RETURN_AFTER_NEWLINE
			if(((const char *) ptr)[i]=='\n' && ((const char *) ptr)[i+1]!='\r') {
				DBGU_PutChar('\r');
			}
#endif
			DBGU_PutChar(((const char *) ptr)[i]);
		}
		return len;
	} else {
		return -1;
	}

}
#endif //#if USE_AT91LIB_STDIO_AND_STRING

int _lseek (int file, int ptr, int dir) {
	(void)file;
	(void)ptr;
	(void)dir;
	return 0;
}

int _close(int file) {
	(void)file;
	return -1;
}

int _open(const char *name, int flags, ...) {
	(void)name;
	(void)flags;
	return -1;
}

int _link(const char *oldpath, const char *newpath) {
	(void)oldpath;
	(void)newpath;
	return -1;
}

int _rename(const char *oldpath, const char *newpath) {
	(void)oldpath;
	(void)newpath;
	return -1;
}

int _unlink(const char *pathname) {
	(void)pathname;
	return -1;
}

int fsync(int fd) {
	(void)fd;
	return -1;
}

int _fstat(int fd, struct stat *st) {
	(void)fd;
	(void)st;
	return -1;
}

int _stat(const char *path, struct stat *st) {
	(void)path;
	(void)st;
	return -1;
}

int _isatty(int fd) {
	if(fd <= STDERR_FILENO) {
		return 1;
	}
	else {
		return 0;
	}
}

void exit(int n) {
	printf("\n\r EXITING WITH CODE: %u \n\r", n);
	restart();
	while(1);
}

void _exit(int n) {
	printf("\n\r EXITING WITH CODE: %u \n\r", n);
	restart();
	while(1);
}

// Register name faking - works in collusion with the linker.
register char * stack_ptr asm ("sp");

void *_sbrk(ptrdiff_t increment) {

	extern char _sheap_, _eheap_;
	static void *cur_heap_pos = 0;

	if(cur_heap_pos == 0) {
		cur_heap_pos = &_sheap_;
	}

	//printf("_sheap_ 0x%x, _eheap_ 0x%x \n\r", (unsigned int)&_sheap_, (unsigned int)&_eheap_);
	//printf("cur_heap_pos 0x%x\n\r", (unsigned int)cur_heap_pos);

	if((cur_heap_pos + increment) > (void *) &_eheap_) {
		printf("\n\r _sbrk: Out of heap-space! \n\r");
		errno = ENOMEM;
		return (void *) -1;
	}

	// No collision checking as FreeRTOS places task stack inside the heap.
//	if((char*)(_eheap_ + increment) > stack_ptr) {
//		// Some of the libstdc++-v3 tests rely upon detecting
//		//out of memory errors, so do not abort here.
//#if 0
//		printf("\n\r _sbrk: Heap and stack collision! RESTARTING! \n\r");
//		restart();
//#else
//		printf("\n\r _sbrk: Heap and stack collision! \n\r");
//		errno = ENOMEM;
//		return (void *) -1;
//#endif
//	}

	void * old_heap_pos = cur_heap_pos;
	cur_heap_pos += increment;
	return old_heap_pos;
}

int _kill() {
	return -1;
}

int _getpid() {
	return 1;
}

int _gettimeofday() {
	return -1;
}
