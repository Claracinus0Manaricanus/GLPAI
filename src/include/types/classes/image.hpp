#ifndef CM_IMAGE
#define CM_IMAGE

struct ImageData {
  const char* filename;
  int desired_channels; // put 0 here to get all the channels available
};

class Image {
private:
  unsigned char* pixelData;
  int width, height, channels;

public:
  // constructors
  Image();
  Image(ImageData data);

  // destructors
  ~Image();

  // setters
  int loadImage(ImageData data);
  int clear();

  // filters
  // maybe implement some filters here?
  // example:
  // void gaussianBlur();

  // getters
  unsigned char* getDataPointer();
  int getWidth();
  int getHeight();
  int getChannels();
  float getAspectRatio();

  // static functions for import purposes
  static unsigned char* ImportImage(const char* path, int* x, int* y,
                                    int* channels_in_file,
                                    int desired_channels);
};

#endif
