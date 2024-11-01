cmake_minimum_required(VERSION 3.29.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_C_COMPILER gcc)

project(newEngine)
add_executable(neweng "src/main.cpp"
#math
"src/include/cm_math/operations.cpp"

#physics
"src/include/physics/collisions.cpp"
"src/include/physics/classes/hitVolumes.cpp"
"src/include/physics/classes/physicsEngine.cpp"

#types
"src/include/types/vectors.cpp"

#types/classes
"src/include/types/classes/transform.cpp"
"src/include/types/classes/mesh.cpp"
"src/include/types/classes/camera.cpp"
"src/include/types/classes/gameObject.cpp"
"src/include/types/classes/material.cpp"
"src/include/types/classes/scene.cpp"
"src/include/types/classes/light.cpp"
"src/include/types/classes/cubemap.cpp"
"src/include/types/classes/image.cpp"

#types/data_structures

#utilities
"src/include/utility/printUtil.cpp"

#window
"src/include/rendering/window/sdl/window.cpp"
"src/include/rendering/window/glfw/window.cpp"

#opengl
"src/include/rendering/opengl/cm_opengl.cpp"
"src/include/rendering/opengl/include/OGL_Program.cpp"

#osDependent
"src/include/platform/linux/filesystem/fileImport.cpp"
"src/include/platform/linux/time/cm_time.cpp"
)

target_link_libraries(neweng SDL2 GL GLEW assimp m stdc++ glfw)
target_include_directories(neweng PUBLIC src/include)
