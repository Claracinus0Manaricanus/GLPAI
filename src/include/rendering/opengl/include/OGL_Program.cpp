#include "OGL_Program.hpp"
#include <cstdlib>
extern "C" {
#include <GL/glew.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
}

// file import function
char* getFileData(const char* path) {
  if (path == NULL)
    return NULL;

  int fd = open(path, O_RDONLY);
  if (fd == -1) {
    return NULL;
  }

  struct stat tmpStat;
  fstat(fd, &tmpStat);

  char* src = (char*)malloc(tmpStat.st_size + 1);

  read(fd, src, tmpStat.st_size);

  return src;
}

// constructor
OGL_Program::OGL_Program(OGL_ProgramData initData) {
  programID = glCreateProgram();

  // shader sources
  char* vertSrc = getFileData(initData.vertexSourcePath);
  char* fragSrc = getFileData(initData.fragSourcePath);
  char* geomSrc = getFileData(initData.geometrySourcePath);

  if (vertSrc == NULL || fragSrc == NULL) {
    errorStatus = CM_ERROR_PATH;
    return;
  }

  // vertex shader
  uint32_t vertSha = 0, fragSha = 0, geomSha = 0;
  vertSha = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertSha, 1, &vertSrc, NULL);
  glCompileShader(vertSha);

  // error check
  int tmpStatus = 0;
  glGetShaderiv(vertSha, GL_COMPILE_STATUS, &tmpStatus);
  if (tmpStatus != GL_TRUE) {
    errorStatus = CM_ERROR_VERTEX_SHADER;
    glGetShaderInfoLog(vertSha, 256, NULL, errorBuffer);
    return;
  }

  // fragment shader
  fragSha = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragSha, 1, &fragSrc, NULL);
  glCompileShader(fragSha);

  // error check
  glGetShaderiv(fragSha, GL_COMPILE_STATUS, &tmpStatus);
  if (tmpStatus != GL_TRUE) {
    errorStatus = CM_ERROR_FRAGMENT_SHADER;
    glGetShaderInfoLog(fragSha, 256, NULL, errorBuffer);
    return;
  }

  // geometry shader
  if (geomSrc != NULL) {
    geomSha = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geomSha, 1, &geomSrc, NULL);
    glCompileShader(geomSha);

    // error check
    glGetShaderiv(geomSha, GL_COMPILE_STATUS, &tmpStatus);
    if (tmpStatus != GL_TRUE) {
      errorStatus = CM_ERROR_GEOMETRY_SHADER;
      glGetShaderInfoLog(geomSha, 256, NULL, errorBuffer);
      return;
    }
  }

  // attaching shaders
  glAttachShader(programID, vertSha);
  glAttachShader(programID, fragSha);
  if (geomSrc != NULL)
    glAttachShader(programID, geomSha);
  glLinkProgram(programID);

  // error check
  glGetProgramiv(programID, GL_LINK_STATUS, &tmpStatus);
  if (tmpStatus != GL_TRUE) {
    errorStatus = CM_ERROR_PROGRAM;
    glGetProgramInfoLog(programID, 256, NULL, errorBuffer);
    return;
  }

  glDeleteShader(vertSha);
  glDeleteShader(fragSha);
  if (geomSrc != NULL)
    glDeleteShader(geomSha);

  errorStatus = CM_OK;
}

// destructor
OGL_Program::~OGL_Program() { glDeleteProgram(programID); }

// setters
int OGL_Program::setMat4(const char* uniformName, Mat4& a) {
  use();
  glUniformMatrix4fv(glGetUniformLocation(programID, uniformName), 1, GL_TRUE,
                     (float*)&a);

  return 0; // add error check
}

// utility
void OGL_Program::use() { glUseProgram(programID); }

const char* OGL_Program::getError() {
  switch (errorStatus) { // WIP
  case CM_ERROR_VERTEX_SHADER:
    return errorBuffer;
    break;

  case CM_ERROR_FRAGMENT_SHADER:
    return errorBuffer;
    break;

  case CM_ERROR_GEOMETRY_SHADER:
    return errorBuffer;
    break;

  case CM_ERROR_PROGRAM:
    return errorBuffer;
    break;

  case CM_ERROR_PATH:
    return "vertex or fragment shader path unusable!\n";
    break;
  }

  return NULL;
}