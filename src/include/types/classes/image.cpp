#include "image.hpp"
#include <cstdlib>
#include <platform/fileIO.hpp>

// constructors
Image::Image() {
  pixelData = nullptr;
  width = 0;
  height = 0;
  channels = 0;
}

Image::Image(ImageData data) {
  pixelData = readImageFile(data.filename, &width, &height, &channels,
                            data.desired_channels);
  if (data.desired_channels != 0)
    channels = data.desired_channels;
}

// destructors
Image::~Image() { free(pixelData); }

// setters
int Image::loadImage(ImageData data) {
  free(pixelData);

  pixelData = readImageFile(data.filename, &width, &height, &channels,
                            data.desired_channels);

  if (data.desired_channels != 0)
    channels = data.desired_channels;

  return -(pixelData == NULL);
}

int Image::clear() {
  free(pixelData);

  pixelData = nullptr;
  width = 0;
  height = 0;
  channels = 0;

  return 0;
}

// getters
unsigned char* Image::getDataPointer() { return pixelData; }

int Image::getWidth() { return width; }

int Image::getHeight() { return height; }

int Image::getChannels() { return channels; }

float Image::getAspectRatio() { return (float)height / width; }

// static functions for import purposes
unsigned char* Image::ImportImage(const char* path, int* x, int* y,
                                  int* channels_in_file, int desired_channels) {
  return readImageFile(path, x, y, channels_in_file, desired_channels);
}
