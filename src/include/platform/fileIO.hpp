#ifndef CM_FILE_IO
#define CM_FILE_IO

extern "C" {
#include <sys/types.h>
}

char* readFile(const char* path, size_t* readBytes);

#endif
