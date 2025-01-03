#ifndef CM_OGL_PROGRAM
#define CM_OGL_PROGRAM

#include "OGL_Types.hpp"
#include <cstdint>
#include <types/vectors.hpp>

#define CM_OK 0
#define CM_ERROR_VERTEX_SHADER 1
#define CM_ERROR_FRAGMENT_SHADER 2
#define CM_ERROR_GEOMETRY_SHADER 3
#define CM_ERROR_PROGRAM 4
#define CM_ERROR_PATH 5

class OGL_Program {
protected:
  // instance vars
  uint32_t programID;
  char errorBuffer[256];
  int errorStatus;

public:
  // constructor
  OGL_Program(OGL_ProgramData initData);

  // destuctor
  ~OGL_Program();

  // setters
  int setMat4(const char* uniformName, Mat4& a);
  int setVec4(const char* uniformName, Vec4& a);
  int setVec3(const char* uniformName, Vec3& a);
  int setFloat(const char* uniformName, float a);
  int setUnsignedInt(const char* uniformName, unsigned int a);
  int setInt(const char* uniformName, int a);

  // utility
  void use();
  const char* getError(); // returns NULL if everything is fine
};

#endif
