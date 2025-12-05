class animation{
    public:
        animation(int color);
        timer getTimer();
        void playAnimation(char fileBaseName[], int posX, int posY, int direction, int lastFrame, int frameLength, bool looping, int ID);
        void playAnimation(char fileBaseName[], int posX, int posY, int finalFrameNum, int frameLength, bool looping, int ID);
        void resetTimer();

    private:
        int currentAnimationID = -1;
        int color;
        timer animationTimer;
        timer holdTime;
};

animation::animation(int color)
: animationTimer(), holdTime(){
    this->color = color;
}

timer animation::getTimer(){
    return animationTimer;
}
void animation::resetTimer(){
    animationTimer.resetTimer();
}

// player animations
void animation::playAnimation(char fileBaseName[], int posX, int posY, int direction, int finalFrameNum, int frameLength, bool looping, int ID){
    char filePath[64] = "";
    if(currentAnimationID != ID){
        animationTimer.resetTimer();
        animationTimer.changeTimerMax(finalFrameNum);
        holdTime.resetTimer();
        currentAnimationID = ID;
    }
    animationTimer.updateTimerState();
    if(looping && !animationTimer.isActive()){
        animationTimer.resetTimer();
    }
    if(animationTimer.isActive()){
        if(color == RED){
            strcat(filePath, "./PlayerRed");
        }else{
            strcat(filePath, "./PlayerBlue");
        }
        if(direction == -1){
            strcat(filePath, "/Left");
        }else{
            strcat(filePath, "/Right");
        }
        strcat(filePath, fileBaseName);
        strcat(filePath, std::to_string(animationTimer.getCurrentTimerTime()).c_str());
        strcat(filePath, ".png"); 
        FEHImage drawAnimation;
        drawAnimation.Open(filePath);
        drawAnimation.Draw(posX, posY);
        if(holdTime.getCurrentTimerTime() < frameLength - 1){
            holdTime.incrementTimer();
        }else{
            animationTimer.incrementTimer();
            holdTime.resetTimer();
        }
        
    }

}

// non-player-bound directionless animations (double jump, etc)
void animation::playAnimation(char fileBaseName[], int posX, int posY, int finalFrameNum, int frameLength, bool looping, int ID){
    char filePath[64] = "";
    if(currentAnimationID != ID){
        animationTimer.resetTimer();
        animationTimer.changeTimerMax(finalFrameNum);
        holdTime.resetTimer();
        currentAnimationID = ID;
    }
    animationTimer.updateTimerState();
    if(looping){
        if(!animationTimer.isActive()){
            animationTimer.resetTimer();
        }
    }
    if(animationTimer.isActive()){
        strcat(filePath, fileBaseName);
        strcat(filePath, std::to_string(animationTimer.getCurrentTimerTime()).c_str());
        strcat(filePath, ".png");
        FEHImage drawAnimation;
        drawAnimation.Open(filePath);
        drawAnimation.Draw(posX, posY);
        if(holdTime.getCurrentTimerTime() < frameLength - 1){
            holdTime.incrementTimer();
        }else{
            animationTimer.incrementTimer();
            holdTime.resetTimer();
        }
    }
}


// structs to create types of animations for easy compatibility with the playAnimation methods above
struct animationType{
    char fileName[32];
    int finalFrameNum; //the # of the last frame of the animation
    bool looping;
    int ID;
    int frameLength = 1;
};


