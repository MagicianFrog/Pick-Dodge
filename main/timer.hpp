#ifndef TIMER_HPP_INCLUDED
#define TIMER_HPP_INCLUDED

class Timer {
    private:
        int startTime;
        int lastPause;
        int lastUnpause;
        int freezeTime;
        bool isPausing;
    public:
        Timer();
        void start();
        void pause();
        void unpause();
        int elapsedTime(); // return in miliseconds
};


#endif // TIMER_HPP_INCLUDED
