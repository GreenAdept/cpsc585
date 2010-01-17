#ifndef TIME_H
#define TIME_H


/*
  Author: Troy Alderson
  The Clock class provides functionality for tracking time.

  The getRealTime methods return the elapsed time between
  the creation of the Clock and the last call to update().

  The getGameTime methods return the amount of elapsed game
  time between the creation of the Clock and the last call
  to update(). Calls to pause() freeze the counter for game
  time, while calls to unpause() unfreeze it.

  The getElapsed methods return the elapsed time between the
  two most recent calls to update().
*/

class Clock {
    long start, elapsed, real, game;
    bool paused;

public:
    Clock ();

    long getRealTime_Micros ();
    long getRealTime_Millis ();
    long getRealTime_Seconds ();

    long getGameTime_Micros ();
    long getGameTime_Millis ();
    long getGameTime_Seconds ();

    long getElapsed_Micros ();
    long getElapsed_Millis ();
    long getElapsed_Seconds ();

    void update ();
    void reset ();
    void pause ();
    void unpause ();
};


#endif