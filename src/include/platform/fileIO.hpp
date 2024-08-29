#ifndef CM_FILE_IO
#define CM_FILE_IO

extern "C" {
#include <sys/types.h>
}

char* readFile(const char* path, size_t* readBytes);
unsigned char* readImageFile(const char* path, int* x, int* y,
                             int* channels_in_file, int desired_channels);

#endif
