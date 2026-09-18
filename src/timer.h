// class and all methods written by David Rubal
class timer{
    public:
        timer();
        void setActiveState(bool state);
        void incrementTimer();
        void updateTimerState();
        int getCurrentTimerTime();
        bool isActive();
        void changeTimerMax(int max);
        void resetTimer();
    private:
        int max;
        int current = 0;
        bool active = false;
        int hold = 0;
};
// no args constructor
timer::timer(){
}

// sets the timer state to the boolean parameter
void timer::setActiveState(bool state){
    active = state;
}

// increments the timer time by one
void timer::incrementTimer(){
    current++;
}

// changes the timer's state based on whether the current time exceed the max
void timer::updateTimerState(){
    if(current > max){
        active = false;
    }else{
        active = true;
    }
}

// returns the timer's current time
int timer::getCurrentTimerTime(){
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
void timer::resetTimer(){
    current = 0;
    active = true;
}