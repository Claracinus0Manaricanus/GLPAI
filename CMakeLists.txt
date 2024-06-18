cmake_minimum_required(VERSION 3.29.0)
project(newEngine)
add_executable(neweng "src/main.cpp"
#math
"src/include/cm_math/operations.cpp"

#physics
"src/include/physics/collisions.cpp"

#types
"src/include/types/vectors.cpp"

#types/classes
"src/include/types/classes/transform.cpp"
"src/include/types/classes/mesh.cpp"
"src/include/types/classes/camera.cpp"
"src/include/types/classes/gameObject.cpp"
"src/include/types/classes/material.cpp"

#utilities
"src/include/utility/printUtil.cpp"

#window
"src/include/rendering/window/window.cpp"

#opengl
"src/include/rendering/opengl/cm_opengl.cpp"
"src/include/rendering/opengl/include/OGL_Program.cpp"
)

target_link_libraries(neweng SDL2 GL GLEW)
