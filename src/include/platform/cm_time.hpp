#ifndef CM_TIME
#define CM_TIME

#include <chrono>

class cm_Time {
private:
  std::chrono::high_resolution_clock::time_point lastDeltaTimeCall;
  std::chrono::high_resolution_clock::time_point startTime;

public:
  // constructor
  cm_Time();

  // these functions return values in seconds
  float getTimeSinceStart();
  float getDeltaTime(); // returns the time difference between two calls
};

#endif
