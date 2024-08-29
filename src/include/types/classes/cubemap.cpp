#include "cubemap.hpp"
#include <cstdlib>

// constructors
Cubemap::Cubemap() {}

Cubemap::Cubemap(const char* filenames[6]) {
  for (int i = 0; i < 6; i++) {
    if (filenames[i] == nullptr || filenames[i] == NULL) {
      continue;
    }

    images[i].loadImage({filenames[i], 3});
  }
}

// setters
void Cubemap::loadFiles(const char* filenames[6]) {
  for (int i = 0; i < 6; i++) {
    if (filenames[i] == nullptr || filenames[i] == NULL) {
      images[i].clear();
      continue;
    }

    images[i].loadImage({filenames[i], 3});
  }
}

// getters
Image& Cubemap::getTexture(int side) { return images[side]; }
