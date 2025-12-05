class timer{
    public:
        timer();
        void setActiveState(bool state);
        void incrementTimer();
        void incrementTimer(int amount);
        void updateTimerState();
        int getCurrentTimerTime();
        bool isActive();
        void changeTimerMax(int max);
        void resetTimer();
    private:
        int max;
        int current = 0;
        bool active = false;
};

timer::timer(){
}



void timer::setActiveState(bool state){
    active = state;
}

void timer::incrementTimer(){
    current++;
}
void timer::incrementTimer(int amount){
    current+=amount;
}

void timer::updateTimerState(){
    if(current > max){
        active = false;
    }else{
        active = true;
    }
}

int timer::getCurrentTimerTime(){
    return current;
}

bool timer::isActive(){
    return active;
}

void timer::changeTimerMax(int max){
    this->max = max;
}

void timer::resetTimer(){
    current = 0;
    active = true;
}