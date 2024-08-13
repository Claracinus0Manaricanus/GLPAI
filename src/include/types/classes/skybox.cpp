#include "skybox.hpp"
extern "C" {
#define STB_IMAGE_IMPEMENTATION
#include <platform/stb_image.h>
}

// constructors
Skybox::Skybox() {}

Skybox::Skybox(Cubemap& data) { skyboxMap = data; } // needs proper copying

Skybox::Skybox(const char* filenames[6]) {

  for (int i = 0; i < 6; i++) {
    if (filenames[i] == nullptr || filenames[i] == NULL) {
      skyboxMap.textures[i].data = nullptr;
      continue;
    }

    skyboxMap.textures[i].data =
        stbi_load(filenames[i], &skyboxMap.textures[i].width,
                  &skyboxMap.textures[i].height, NULL, 3);
    skyboxMap.textures[i].channel = 3;
  }
}

// setters

// getters
TextureData& Skybox::getTexture(int side) { return skyboxMap.textures[side]; }
