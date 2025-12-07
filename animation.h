// class and all functions written by David Rubal
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
        FEHImage drawAnimation;
};

// Constructor, sets the color of the object to be animated
animation::animation(int color)
: animationTimer(), holdTime(){
    this->color = color;
}

// returns a copy of the animationTimer
timer animation::getTimer(){
    return animationTimer;
}
// resets the animation timer
void animation::resetTimer(){
    animationTimer.resetTimer();
}

// plays a frame of animation given info about the animation
// animation path must follow ./Player(Color)/(Direction)/
void animation::playAnimation(char fileBaseName[], int posX, int posY, int direction, int finalFrameNum, int frameLength, bool looping, int ID){
    // gradually builds the file path
    char filePath[64] = "";
    // if the animation ID has changed, the reset the animation Timer
    if(currentAnimationID != ID){
        animationTimer.resetTimer();
        animationTimer.changeTimerMax(finalFrameNum);
        holdTime.resetTimer();
        currentAnimationID = ID;
    }
    // update the timer each frame
    animationTimer.updateTimerState();
    // reset the timer if it is looping and has become inactive
    if(looping && !animationTimer.isActive()){
        animationTimer.resetTimer();
    }
    // when the animation timer is active
    if(animationTimer.isActive()){
        // add the player directory to the file path
        if(color == RED){
            strcat(filePath, "./PlayerRed");
        }else{
            strcat(filePath, "./PlayerBlue");
        }
        // add the direction directory to the file path
        if(direction == -1){
            strcat(filePath, "/Left");
        }else{
            strcat(filePath, "/Right");
        }
        // add the given file name to the file path
        strcat(filePath, fileBaseName);
        // add the number indicator given for the frame of animation 
        strcat(filePath, std::to_string(animationTimer.getCurrentTimerTime()).c_str());
        // add the .png file type
        strcat(filePath, ".png"); 
        //draw the given animation at the provided location
        drawAnimation.Open(filePath);
        drawAnimation.Draw(posX, posY);
        // determine if the currnet frame # should be held for the next frame
        if(holdTime.getCurrentTimerTime() < frameLength - 1){
            holdTime.incrementTimer();
        }else{
            animationTimer.incrementTimer();
            holdTime.resetTimer();
        }
        
    }

}

// plays a frame of animation given info about the animation
// made for non-player-bound directionless animations (double jump)
void animation::playAnimation(char fileBaseName[], int posX, int posY, int finalFrameNum, int frameLength, bool looping, int ID){
    //same functionality as the function above, but without the player color and direction directories
    char filePath[64] = "";
    if(currentAnimationID != ID){
        animationTimer.resetTimer();
        animationTimer.changeTimerMax(finalFrameNum);
        holdTime.resetTimer();
        currentAnimationID = ID;
    }
    animationTimer.updateTimerState();
    if(looping && !animationTimer.isActive()){
        if(!animationTimer.isActive()){
            animationTimer.resetTimer();
        }
    }
    if(animationTimer.isActive()){
        strcat(filePath, fileBaseName);
        strcat(filePath, std::to_string(animationTimer.getCurrentTimerTime()).c_str());
        strcat(filePath, ".png");
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


// struct to create types of animations for simple compatibility with the playAnimation methods above
struct animationType{
    char fileName[32];
    int finalFrameNum; //the # of the last frame of the animation
    bool looping; // does the animation loop
    int ID; // unique identifier to differentiate animations
    int frameLength = 1; // how long to hold the current frame for
};


