#GLPAI
GLPAI is a simple game engine that is still in it's early stages of development. It aims to be a library style engine. The goal for this project is to be modular. To be able to change any part of the engine you want without affecting others.

#Current
Currently the engine is used through the main file with a single thread and only OpenGL is supported. For window management both SDL2 and GLFW3 is supported. For assets ASSIMP is implemented on a really basic level just to add meshes(Look at the scene.cpp file). For image imports stb_image is used. No audio is supported at the current moment. The code has only been compiled and tested on linux with glibc 2.40+r16+gaa533d58ff-2.

#Plans
- Overall fixes to modularity
- Vulkan implementation
- Performance improvements
- Better GameObject support with ECS(Entity Component System)
