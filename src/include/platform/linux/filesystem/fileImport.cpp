#include "../../fileIO.hpp"

#include <cstdlib>

extern "C" {
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
}

char* readFile(const char* path, size_t* readBytes) {
  if (path == NULL)
    return NULL;

  int fd = open(path, O_RDONLY);
  if (fd == -1)
    return NULL;

  struct stat tmpStat;
  fstat(fd, &tmpStat);

  char* src = (char*)malloc(tmpStat.st_size + 1);

  read(fd, src, tmpStat.st_size);
  src[tmpStat.st_size] = 0;

  if (readBytes != NULL)
    (*readBytes) = tmpStat.st_size;

  close(fd);
  return src;
}

unsigned char* readImageFile(const char* path, int* x, int* y,
                             int* channels_in_file, int desired_channels) {
  return stbi_load(path, x, y, channels_in_file, desired_channels);
}
