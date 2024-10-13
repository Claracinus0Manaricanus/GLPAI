#include <chrono>
#include <platform/cm_time.hpp>

using namespace std::chrono;

// constructors
cm_Time::cm_Time() {
  lastDeltaTimeCall = high_resolution_clock::now();
  startTime = high_resolution_clock::now();
}

// destructors

// getters
float cm_Time::getTimeSinceStart() {
  high_resolution_clock::time_point current = high_resolution_clock::now();
  return duration_cast<duration<float>>(current - startTime).count();
}

float cm_Time::getDeltaTime() {

  std::chrono::high_resolution_clock::time_point t1 = lastDeltaTimeCall;
  lastDeltaTimeCall = std::chrono::high_resolution_clock::now();

  return duration_cast<duration<float>>(lastDeltaTimeCall - t1).count();
}
