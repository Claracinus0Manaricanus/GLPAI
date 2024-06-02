cmake_minimum_required(VERSION 3.29.0)
project(newEngine)
add_executable(neweng "src/main.cpp"
#math
"src/include/cm_math/operations.cpp"

#physics
"src/include/physics/collisions.cpp"

#types
"src/include/types/vectors.cpp"

#utilities
"src/include/utility/printUtil.cpp"

#window
"src/include/rendering/window/window.cpp"
)

target_link_libraries(neweng SDL2)
