#ifndef CM_FILE_IMPORT
#define CM_FILE_IMPORT

extern "C" {
#include <sys/types.h>
}

char* readFile(const char* path, size_t* readBytes);

#endif