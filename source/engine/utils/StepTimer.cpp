#include "StepTimer.h"

namespace Engine {

StepTimer::StepTimer()
    : m_elapsedTicks(0)
    , m_totalTicks(0)
    , m_leftOverTicks(0)
    , m_frameCount(0)
    , m_framesPerSecond(0)
    , m_framesThisSecond(0)
    , m_qpcSecondCounter(0)
    , m_isFixedTimeStep(false)
    , m_targetElapsedTicks(TicksPerSecond / 60) {
  m_qpcFrequency = SDL_GetPerformanceFrequency();
  m_qpcLastTime = SDL_GetPerformanceCounter();

  // Initialize max delta to 1/10 of a second.
  m_qpcMaxDelta = m_qpcFrequency / 10;
}

uint64_t StepTimer::GetElapsedTicks() const {
  return m_elapsedTicks;
}

double StepTimer::GetElapsedSeconds() const {
  return TicksToSeconds(m_elapsedTicks);
}

uint64_t StepTimer::GetTotalTicks() const {
  return m_totalTicks;
}

double StepTimer::GetTotalSeconds() const {
  return TicksToSeconds(m_totalTicks);
}

uint32_t StepTimer::GetFrameCount() const {
  return m_frameCount;
}

uint32_t StepTimer::GetFramesPerSecond() const {
  return m_framesPerSecond;
}

void StepTimer::SetFixedTimeStep(bool isFixedTimestep) {
  m_isFixedTimeStep = isFixedTimestep;
}

void StepTimer::SetTargetElapsedTicks(uint64_t targetElapsed) {
  m_targetElapsedTicks = targetElapsed;
}

void StepTimer::SetTargetElapsedSeconds(double targetElapsed) {
  m_targetElapsedTicks = SecondsToTicks(targetElapsed);
}

double StepTimer::TicksToSeconds(uint64_t ticks) {
  return static_cast<double>(ticks) / TicksPerSecond;
}

uint64_t StepTimer::SecondsToTicks(double seconds) {
  return static_cast<uint64_t>(seconds * TicksPerSecond);
}

void StepTimer::ResetElapsedTime() {
  m_qpcLastTime = SDL_GetPerformanceCounter();

  m_leftOverTicks = 0;
  m_framesPerSecond = 0;
  m_framesThisSecond = 0;
  m_qpcSecondCounter = 0;
}

} // namespace Engine
