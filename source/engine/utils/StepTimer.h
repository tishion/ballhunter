/*
 * File: StepTimer.h
 * Project: engine
 * Created: Tuesday, 17th May 2022 4:17:09 pm
 * Author: Sheen Tian
 */
#ifndef STEPTIMER_H
#define STEPTIMER_H

#pragma once

#include <cstdint>

#include <SDL_timer.h>

namespace Engine {

class StepTimer {
public:
  StepTimer();

  // Get elapsed time since the previous Update call.
  uint64_t GetElapsedTicks() const;
  double GetElapsedSeconds() const;

  // Get total time since the start of the program.
  uint64_t GetTotalTicks() const;
  double GetTotalSeconds() const;

  // Get total number of updates since start of the program.
  uint32_t GetFrameCount() const;

  // Get the current framerate.
  uint32_t GetFramesPerSecond() const;

  // Set whether to use fixed or variable timestep mode.
  void SetFixedTimeStep(bool isFixedTimestep);

  // Set how often to call Update when in fixed timestep mode.
  void SetTargetElapsedTicks(uint64_t targetElapsed);
  void SetTargetElapsedSeconds(double targetElapsed);

  // Integer format represents time using 10,000,000 ticks per second.
  static const uint64_t TicksPerSecond = 10000000;

  static double TicksToSeconds(uint64_t ticks);
  static uint64_t SecondsToTicks(double seconds);

  // After an intentional timing discontinuity (for instance a blocking IO operation)
  // call this to avoid having the fixed timestep logic attempt a set of catch-up
  // Update calls.

  void ResetElapsedTime();

  template <typename TUpdate>
  void Tick(const TUpdate& update) {
    // Query the current time.
    uint64_t currentTime = SDL_GetPerformanceCounter();
    uint64_t timeDelta = currentTime - m_qpcLastTime;

    m_qpcLastTime = currentTime;
    m_qpcSecondCounter += timeDelta;

    // Clamp excessively large time deltas (e.g. after paused in the debugger).
    if (timeDelta > m_qpcMaxDelta) {
      timeDelta = m_qpcMaxDelta;
    }

    // Convert QPC units into a canonical tick format. This cannot overflow due to the previous
    // clamp.
    timeDelta *= TicksPerSecond;
    timeDelta /= m_qpcFrequency;

    uint32_t lastFrameCount = m_frameCount;

    if (m_isFixedTimeStep) {
      // Fixed timestep update logic

      // If the app is running very close to the target elapsed time (within 1/4 of a millisecond)
      // just clamp the clock to exactly match the target value. This prevents tiny and irrelevant
      // errors from accumulating over time. Without this clamping, a game that requested a 60 fps
      // fixed update, running with vsync enabled on a 59.94 NTSC display, would eventually
      // accumulate enough tiny errors that it would drop a frame. It is better to just round
      // small deviations down to zero to leave things running smoothly.

      if (static_cast<uint64_t>(std::abs(static_cast<int64_t>(timeDelta - m_targetElapsedTicks))) <
          TicksPerSecond / 4000) {
        timeDelta = m_targetElapsedTicks;
      }

      m_leftOverTicks += timeDelta;

      while (m_leftOverTicks >= m_targetElapsedTicks) {
        m_elapsedTicks = m_targetElapsedTicks;
        m_totalTicks += m_targetElapsedTicks;
        m_leftOverTicks -= m_targetElapsedTicks;
        m_frameCount++;

        update();
      }
    } else {
      // Variable timestep update logic.
      m_elapsedTicks = timeDelta;
      m_totalTicks += timeDelta;
      m_leftOverTicks = 0;
      m_frameCount++;

      update();
    }

    // Track the current framerate.
    if (m_frameCount != lastFrameCount) {
      m_framesThisSecond++;
    }

    if (m_qpcSecondCounter >= m_qpcFrequency) {
      m_framesPerSecond = m_framesThisSecond;
      m_framesThisSecond = 0;
      m_qpcSecondCounter %= m_qpcFrequency;
    }
  }

private:
  // Source timing data uses QPC units.
  uint64_t m_qpcFrequency;
  uint64_t m_qpcLastTime;
  uint64_t m_qpcMaxDelta;

  // Derived timing data uses a canonical tick format.
  uint64_t m_elapsedTicks;
  uint64_t m_totalTicks;
  uint64_t m_leftOverTicks;

  // Members for tracking the framerate.
  uint32_t m_frameCount;
  uint32_t m_framesPerSecond;
  uint32_t m_framesThisSecond;
  uint64_t m_qpcSecondCounter;

  // Members for configuring fixed timestep mode.
  bool m_isFixedTimeStep;
  uint64_t m_targetElapsedTicks;
};
} // namespace Engine

#endif
