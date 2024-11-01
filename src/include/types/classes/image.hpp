#ifndef CM_IMAGE
#define CM_IMAGE

struct ImageData {
  const char* filename;
  int desired_channels; // put 0 here to get all the channels available
};

struct LoadedImageData {
  unsigned char* data;
  int width, height;
  int channels;
};

class Image {
protected:
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
  int loadImage(LoadedImageData data, int clearPrevious = 1);
  int clear();

  // filters
  // flips c_one and c_two, 1 2 3 4 resembles r g b a
  // if the same channel is given for both c_one and c_two it gets annihilated
  int flipChannels(int c_one, int c_two);

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
