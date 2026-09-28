// class and all methods written by David Rubal
// Timer is active when current < max [0 < current <= max]. Timer becomes inactive once current == max.
class timer{
    public:
        timer();
        timer(int max);
        void activate();
        void stop();
        void increment();
        int getCurrentTime();
        bool isActive();
        void setMax(int max);
        int getMax();
        void reset();
    private:
        int max;
        int current = 0;
        bool active = false;
};
// no args constructor
timer::timer(){
}

timer::timer(int max){
    this->max = max;
}

// sets the timer state to the boolean parameter
void timer::activate(){
    active = true;
}

void timer::stop(){
    active = false;
}

// Increments timer and updates state accordingly
void timer::increment(){
    active = ++current < max;
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
void timer::setMax(int max){
    this->max = max;
}

int timer::getMax(){
    return max;
}

// resets the current time to 0
void timer::reset(){
    current = 0;
}