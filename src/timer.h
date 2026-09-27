// class and all methods written by David Rubal
class timer{
    public:
        timer();
        timer(int max);
        void setActiveState(bool state);
        bool incrementTimer();
        int getCurrentTime();
        bool isActive();
        void changeTimerMax(int max);
        void resetTimer(bool active = true);
    private:
        int max;
        int current = 0;
        bool active = false;
        int hold = 0;
};
// no args constructor
timer::timer(){
}

timer::timer(int max){
    this->max = max;
}

// sets the timer state to the boolean parameter
void timer::setActiveState(bool state){
    active = state;
}

// increments the timer time by one
bool timer::incrementTimer(){
    return active = ++current <= max;
}

// returns the timer's current time
int timer::getCurrentTime(){
    return current;
}

// returns the state of the timer
bool timer::isActive(){
    return active;
}

// updates the timer's maximum time
void timer::changeTimerMax(int max){
    this->max = max;
}

// resets the current time to 0 and reactivates the timer
void timer::resetTimer(bool active = true){
    current = 0;
    this->active = active;
}