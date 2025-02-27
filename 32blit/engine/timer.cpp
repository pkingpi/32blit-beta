/*! \file timer.cpp
*/
#include "engine.hpp"
#include "timer.hpp"

namespace blit {
  std::vector<timer *> timers;

  timer::timer() {
  }

  /**
   * Initialize the timer.
   *
   * @param callback Callback function to trigger when timer has elapsed.
   * @param duration Duration of the timer in milliseconds.
   * @param loops Number of times the timer should repeat, -1 = forever.
   */
  void timer::init(timer_callback callback, uint32_t duration, int32_t loops = -1) {
    this->callback = callback;
    this->duration = duration;
    this->loops = loops;
    timers.push_back(this);
  }

  /**
   * Start the timer.
   */
  void timer::start() {
    this->started = blit::now();
    this->state = RUNNING;
  }

  /**
   * Stop the running timer.
   */
  void timer::stop() {
    this->state = STOPPED;
  }

  /**
   * Update all running timers, triggering any that have elapsed.
   *
   * @param time Time in milliseconds.
   */
  void update_timers(uint32_t time) {
    for (auto t: timers) {
      if (t->state == timer::RUNNING){
        if (time > (t->started + t->duration)) { // timer triggered
          t->callback(*t);

          if(t->loops == -1){
            t->started = time; // reset the start time correcting for any error
          }
          else
          {
            t->loops--;
            t->started = time;
            if (t->loops == 0){
              t->state = timer::FINISHED;
            }
          }
        }
      }
    }
  }
}