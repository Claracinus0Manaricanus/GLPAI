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
  // flips c_one and c_two, 1 2 3 4 resembles r g b a
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
