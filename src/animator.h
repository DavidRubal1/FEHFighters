
// class and all functions written by David Rubal
class animator{
    public:
        animator(int color);
        int getHoldTime();
        int getAnimationTime();
        bool isAnimationOver();
        action getAnimationType();
        int playAnimation(animationProperties properties, int posX, int posY, int direction);
        // directionless
        int playAnimation(animationProperties properties, int posX, int posY);
        void resetTimers();
    private:
        int type = -1;
        int color, direction = -2;
        animationProperties currentAnimation;
        char baseFilePath[64] = "./graphics/Animations";
        timer animationTimer;
        timer holdTimer;
        FEHImage drawAnimation;
};

// Constructor, sets the color of the object to be animated
animator::animator(int color)
: animationTimer(64), holdTimer(64){
    this->color = color;
}

int animator::getHoldTime(){
    return holdTimer.getCurrentTime();
}

int animator::getAnimationTime(){
    return animationTimer.getCurrentTime();
}

bool animator::isAnimationOver(){
    return !animationTimer.isActive();
}

action animator::getAnimationType(){
    return currentAnimation.type;
}

// resets the animation timer
void animator::resetTimers(){
    animationTimer.reset();
    holdTimer.reset();
    animationTimer.activate();
    holdTimer.activate();
}


// plays a frame of animation given info about the animation
// animation path must follow ./graphics/Animations/Player(Color)/(Direction)/
// returns current animation frame
int animator::playAnimation(animationProperties properties, int posX, int posY, int direction){
    // Construct file path of frame
    char filePath[64];
    strcpy(filePath, baseFilePath);

    // if the animation ID has changed, the reset the animation Timer
    //TODO: probably replace this with a state of one or both timers instead of comparing type
    if(type != properties.type || this->direction != direction){
        currentAnimation = properties;
        animationTimer.setMax(properties.frameLengths.size());
        holdTimer.setMax(properties.frameLengths.at(0));
        resetTimers();
        type = properties.type;
        this->direction = direction;
    }

    // reset the timer if it is looping and has become inactive
    if(properties.looping && !animationTimer.isActive()){
        animationTimer.reset();
        animationTimer.activate();
    }

    if(animationTimer.isActive()){
        // add the player directory to the file path
        color == RED ? strcat(filePath, "/PlayerRed"): strcat(filePath, "/PlayerBlue");

        // add the direction directory to the file path
        direction == -1 ? strcat(filePath, "/Left"): strcat(filePath, "/Right");

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
        holdTimer.increment();
        if(!holdTimer.isActive()){
            animationTimer.increment();
            holdTimer.reset();
            if(animationTimer.isActive()){
                holdTimer.setMax(properties.frameLengths.at(animationTimer.getCurrentTime()));
                holdTimer.activate();
            }
        }
    }
    return animationTimer.getCurrentTime();

}

// plays a frame of animation given info about the animation
// made for non-player-bound directionless animations (double jump)
int animator::playAnimation(animationProperties properties, int posX, int posY){
    
    char filePath[64];
    strcpy(filePath, baseFilePath);
    //same functionality as the function above, but without the player color and direction directories
    if(type != properties.type){
        currentAnimation = properties;
        animationTimer.setMax(properties.frameLengths.size());
        holdTimer.setMax(properties.frameLengths.at(0));
        resetTimers();
        type = properties.type;
    }
    if(properties.looping && !animationTimer.isActive()){
        animationTimer.reset();
        animationTimer.activate();
    }
    if(animationTimer.isActive()){
        strcat(filePath, properties.fileName);
        strcat(filePath, std::to_string(animationTimer.getCurrentTime()).c_str());
        strcat(filePath, ".png");
        drawAnimation.Open(filePath);
        drawAnimation.Draw(posX, posY);
        holdTimer.increment();
        if(!holdTimer.isActive()){
            animationTimer.increment();
            holdTimer.reset();
            if(animationTimer.isActive()){
                holdTimer.setMax(properties.frameLengths.at(animationTimer.getCurrentTime()));
                holdTimer.activate();
            }
        }
    }
     return animationTimer.getCurrentTime();
}
