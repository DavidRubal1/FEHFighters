
// class and all functions written by David Rubal
class animator{
    public:
        animator(int color);
        int getHoldTime();
        int getAnimationTime();
        action getLastFrameType();
        int playAnimation(animationProperties properties, int posX, int posY, int direction);
        // directionless
        int playAnimation(animationProperties properties, int posX, int posY);
        void resetTimer();

    private:
        int type = -1;
        int color;
        animationProperties currentAnimation;
        timer animationTimer;
        timer holdTimer;
        FEHImage drawAnimation;
};

// Constructor, sets the color of the object to be animated
animator::animator(int color)
: animationTimer(), holdTimer(){
    this->color = color;
}

// returns a copy of the animationTimer
// timer animator::getAnimationTimer(){
//     return animationTimer;
// }

int animator::getHoldTime(){
    return holdTimer.getCurrentTime();
}

int animator::getAnimationTime(){
    return animationTimer.getCurrentTime();
}

action animator::getLastFrameType(){
    return currentAnimation.type;
}

// resets the animation timer
void animator::resetTimer(){
    animationTimer.resetTimer();
    holdTimer.resetTimer();
}

// plays a frame of animation given info about the animation
// animation path must follow ./graphics/Animations/Player(Color)/(Direction)/
// returns current animation frame
int animator::playAnimation(animationProperties properties, int posX, int posY, int direction){
    currentAnimation = properties;

    // gradually builds the file path
    char filePath[64] = "./graphics/Animations";
    // if the animation ID has changed, the reset the animation Timer
    if(type != properties.type){
        animationTimer.resetTimer();
        animationTimer.changeTimerMax(properties.finalFrameNum);
        holdTimer.resetTimer();
        type = properties.type;
    }
    // update whether the time is active or not (has reached max val or not)
    animationTimer.updateTimerState();
    // reset the timer if it is looping and has become inactive
    if(properties.looping && !animationTimer.isActive()){
        animationTimer.resetTimer();
    }

    if(animationTimer.isActive()){
        // add the player directory to the file path
        if(color == RED){
            strcat(filePath, "/PlayerRed");
        }else{
            strcat(filePath, "/PlayerBlue");
        }
        // add the direction directory to the file path
        if(direction == -1){
            strcat(filePath, "/Left");
        }else{
            strcat(filePath, "/Right");
        }
        // add the given file name to the file path
        strcat(filePath, properties.fileName);
        // add the number indicator given for the frame of animation 
        strcat(filePath, std::to_string(animationTimer.getCurrentTime()).c_str());
        // add the .png file type
        strcat(filePath, ".png"); 
        //draw the given animation at the provided location
        drawAnimation.Open(filePath);
        drawAnimation.Draw(posX, posY);
        // determine if the current frame # should be held for the next frame
        if(holdTimer.getCurrentTime() < properties.frameLength - 1){
            holdTimer.incrementTimer();
        }else{
            animationTimer.incrementTimer();
            holdTimer.resetTimer();
        }
        
    }
    return animationTimer.getCurrentTime();

}

// plays a frame of animation given info about the animation
// made for non-player-bound directionless animations (double jump)
int animator::playAnimation(animationProperties properties, int posX, int posY){
    currentAnimation = properties;

    //same functionality as the function above, but without the player color and direction directories
    char filePath[64] = "./graphics/Animations";
    // if the animation has changed
    if(type != properties.type){
        animationTimer.resetTimer();
        animationTimer.changeTimerMax(properties.finalFrameNum);
        holdTimer.resetTimer();
        type = properties.type;
    }
    animationTimer.updateTimerState();
    if(properties.looping && !animationTimer.isActive()){
        if(!animationTimer.isActive()){
            animationTimer.resetTimer();
        }
    }
    if(animationTimer.isActive()){
        strcat(filePath, properties.fileName);
        strcat(filePath, std::to_string(animationTimer.getCurrentTime()).c_str());
        strcat(filePath, ".png");
        drawAnimation.Open(filePath);
        drawAnimation.Draw(posX, posY);
        if(holdTimer.getCurrentTime() < properties.frameLength - 1){
            holdTimer.incrementTimer();
        }else{
            animationTimer.incrementTimer();
            holdTimer.resetTimer();
        }
    }
     return animationTimer.getCurrentTime();
}



