class player{
    public:
        player(Key leftwards, Key rightwards, Key upwards, Key downwards, Key basicAttack, int startingX, int startingY,  int color);
        void generalPlayerMovementControl(int frameTime);
        void dash(int direction);
        void enactPlayerMovement(int frameTime);
        void getHit(float force,  float damageAmount, int angleDegrees, int attackDirection);
        
        void playAnimations(int frameTime);
        void dashAnimation(int frameTime);
        void crouchAnimation(int frameTime);
        void idleAnimation(int frameTime);
        
        void doubleJumpAnimation(int frameTime);

        bool inAnimation();
        void resetIfOffscreen();
        void action(int frameTime);
        std::vector<int> getXYPosition();
        std::vector<float> getXYVelocity();
        void setXYVelocity(float x, float y);
        hitbox getHitbox();
        float getDamage();
        // implement animation players to work
        //animation animationPlayer;

    private:
        // player attributes
        // position
        int startingPosX, startingPosY;
        int positionX = 100, positionY = 180;
        // size
        int hitboxHeight = 20, hitboxLength = 14;

        // player color
        int playerColor;

        // hitbox for player
        hitbox playerHitbox;

        // general player speed
        float velocityX = 0, velocityY = 0;
        int direction = 1;
        // unused maybe use this to limit speed? knockback will make this hard
        float velocityXLimit = 20, velocityYLimit = 20;

        // grounded speed stuff
        float accelerationX = 1.10;
        float dashSpeedMod = 1.80;
        float velocityXDecay = 0.90, velocityXDecayTemp = 0.90;
        float velocityXDecayDash = 0.95;
        // grounded state
        bool grounded = true;

        // airborne
        float airspeedMod = 0.75;
        bool doubleJumpUsed = false;

        // dash variables
        bool inDashLag = false;
        float dashLagTimerMax = 3; // 3 frames
        float dashLagTimer = 0;
        
        // animation timers 
        // THIS: I could also just not use the frame time and increment each frame...
        bool inDashAnimation = false;
        int dashAnimationTimerMax = 8; // 9 frames, change every frame
        int dashAnimationTimer = 0;
        bool inIdle = false;
        int idleTimerMax = 59; // 2 frames, change every 30 frames
        int idleTimer = 0;

        int doubleJumpTimerMax = 7; // 4 frames, change every other frame
        int doubleJumpTimer = 0;
        bool doubleJumpAnimationEnd = false;
        int doubleJumpX, doubleJumpY;

        bool inCrouch = false;

        //jump variables
        float jumpForce = 6;
        bool inJumpLag = false;
        float jumpLagTimerMax = 3; // 3 frames
        float jumpLagTimer = 0;

        // unused, possibly use for animation related stuff
        float animationTimer = 0;

        // gravity
        float fastFallGravity = 0.60;
        bool inFastFall = false;
        float gravity = 0.1, tempGravity = 0.1;
        float currentGravityForce = 0;
        float maxGravityForce = 1.5;

        // controls
        Key left = KEY_A, right = KEY_D, up = KEY_W,  down = KEY_S, basic = KEY_T;
        float damage = 0;
};

// constructor
player::player(Key leftwards, Key rightwards, Key upwards, Key downwards, Key basicAttack, int startingX, int startingY, int color) 
    : playerHitbox(hitboxHeight, hitboxLength, positionX, positionY, true){
    left = leftwards;
    right = rightwards;
    up = upwards;
    down = downwards;
    startingPosX = startingX;
    startingPosY = startingY;
    positionX = startingX;
    positionY = startingY;
    basic = basicAttack;
    playerColor = color;
}

hitbox player::getHitbox(){
    return playerHitbox;
}
float player::getDamage(){
    return damage;
}
std::vector<int> player::getXYPosition(){
    return {positionX, positionY};
}
std::vector<float> player::getXYVelocity(){
    return {velocityX, velocityY};
}

void player::setXYVelocity(float x, float y){
    velocityX = x;
    velocityY = y;
}

void player::getHit(float force, float damageAmount, int angleRadians, int attackDirection){
    float forceX, forceY;
    // scale force based on current damage
    // TODO: divide the power by a value that makes sense later
    force = pow(force, damage/5);

    // calculate the direction of knockback into x and y components
    // angle degrees ranges from -pi to pi, with -pi as directly down and pi is straight up
    forceX = direction * force*cos(angleRadians);
    forceY = direction * force*sin(angleRadians);
    // apply force to player velocity
    velocityX += forceX;
    velocityY -= forceY;
    
    damage += damageAmount;
}

    void player::dashAnimation(int frameTime){
        char filePath[64];
        if(playerColor == RED){
            if(direction == -1){
                strcpy(filePath, "./PlayerRed/DashLeft/RedDashLeft");
            }else{
                strcpy(filePath, "./PlayerRed/DashRight/RedDashRight");
            }
        }else{
            if(direction == -1){
                strcpy(filePath, "./PlayerBlue/DashLeft/BlueDashLeft");
            }else{
                strcpy(filePath, "./PlayerBlue/DashRight/BlueDashRight");
            }
        }
        inDashAnimation = true;
        FEHImage drawDash;
        strcat(filePath, std::to_string(dashAnimationTimer).c_str());
        strcat(filePath, ".png");
        drawDash.Open(filePath);
        drawDash.Draw(positionX, positionY);
        
    }
    void player::crouchAnimation(int frameTime){
        char filePath[64];
        inDashAnimation = false;
        if(playerColor == RED){
            if(direction == -1){
                strcpy(filePath, "./PlayerRed/CrouchLeft/RedCrouchLeft");
            }else{
                strcpy(filePath, "./PlayerRed/CrouchRight/RedCrouchRight");
            }
        }else{
            if(direction == -1){
                strcpy(filePath, "./PlayerBlue/CrouchLeft/BlueCrouchLeft");
            }else{
                strcpy(filePath, "./PlayerBlue/CrouchRight/BlueCrouchRight");
            }
        }
        inCrouch = true;
        FEHImage drawDash;
        strcat(filePath, ".png");
        drawDash.Open(filePath);
        drawDash.Draw(positionX, positionY);
    }
    
    void player::idleAnimation(int frameTime){
            inIdle = true;
            char filePath[64];
            //play idle animation
            FEHImage idle;
            if(playerColor == RED){
                if(direction == -1){
                    strcpy(filePath, "./PlayerRed/Left/PlayerRedLeft");
                }else{
                    strcpy(filePath, "./PlayerRed/Right/PlayerRedRight");
                }
            }else{
                if(direction == -1){
                    strcpy(filePath, "./PlayerBlue/Left/PlayerBlueLeft");
                }else{
                    strcpy(filePath, "./PlayerBlue/Right/PlayerBlueRight");
                }
            }
            strcat(filePath, std::to_string(idleTimer/30).c_str());
            strcat(filePath, ".png");
            idle.Open(filePath);
            idle.Draw(positionX, positionY);
    }

void player::doubleJumpAnimation(int frameTime){
    char filePath[64];
    //play doublejump animation
    FEHImage doubleJump;
    strcpy(filePath, "./Double Jump Frames/doubleJumpFrame");
    strcat(filePath, std::to_string(doubleJumpTimer/2).c_str());
    strcat(filePath, ".png");
    doubleJump.Open(filePath);
    doubleJump.Draw(doubleJumpX - 3, doubleJumpY+hitboxHeight);
}

void player::playAnimations(int frameTime){
    
    if(!grounded){
        inDashAnimation = false;
    }
    if(!grounded || !Keyboard.isPressed(down) || Keyboard.areAnyPressed({left, right})){ // uncrouch when not holding down
            inCrouch = false;
    }

    if(grounded && (Keyboard.areAnyPressed({left, right}) || inDashAnimation)){  //dash right
        dashAnimation(frameTime);
    }else if(grounded && Keyboard.isPressed(down) && !Keyboard.areAnyPressed({left, right})){
        crouchAnimation(frameTime);
    }else{
        idleAnimation(frameTime);
    }

    if(doubleJumpUsed && !doubleJumpAnimationEnd){
        doubleJumpAnimation(frameTime);
    }
    
    // countdown timers
    if(inDashAnimation){
        if(grounded){
            if(dashAnimationTimer < dashAnimationTimerMax){
                dashAnimationTimer++;
            }else{
                inDashAnimation = false;
                dashAnimationTimer = 0;
            }
        }else{
            inDashAnimation = false;
            dashAnimationTimer = 0;
        }
    }

    if(inIdle){
        if(grounded || !inAnimation()){
            if(idleTimer < idleTimerMax){
                idleTimer++;
            }else{
                idleTimer = 0;
            }
        }else{
            inIdle = false;
            idleTimer = 0;
        }
        
    }

    if(doubleJumpUsed){
        if(!grounded && doubleJumpTimer < doubleJumpTimerMax){
            doubleJumpTimer++;
        }else{
            doubleJumpTimer = 0;
            doubleJumpAnimationEnd = true;
        }
    }else{
        doubleJumpTimer = 0;
        doubleJumpAnimationEnd = true;
    }

    
}

bool player::inAnimation(){
    return inDashAnimation || inCrouch;
}

void player::resetIfOffscreen(){
    // if playe position is off-screen
    if(positionX < 0 || positionX > 319 || positionY > 239 || positionY < 0){
        // undraw player
        LCD.SetFontColor(BLACK);
        LCD.FillRectangle(positionX, positionY, hitboxLength, hitboxHeight);
        // resets position
        positionX = startingPosX;
        positionY = startingPosY;
        velocityX = 0;
        velocityY = 0;
        damage = 0;
    }
}

void player::dash(int direction){
    velocityX += direction * accelerationX * dashSpeedMod;
    inDashLag = true;
    velocityXDecay = velocityXDecayDash;
}

void player::generalPlayerMovementControl(int frameTime){

    if(grounded){
        if(!Keyboard.isPressed({left, right})){
            if(!inDashLag){
                // move left
                if(Keyboard.isPressed(left)){
                    direction = -1;
                    // dash if turning around or stationary
                    if(velocityX >= 0){
                        dash(direction);
                    }
                    // normal acceleration
                    velocityX -= accelerationX;
                }
                // move right
                if(Keyboard.isPressed(right)){
                    direction = 1;
                    if(velocityX <= 0){
                        dash(direction);
                    }
                    velocityX += accelerationX;
                }
            }
            
        }
        // jump when grounded
        if(Keyboard.isPressed(up)){
            velocityY -= jumpForce;
            inJumpLag = true;
        }
    }
    else{ 
        // airborne movement (grouded == false)
        if(!Keyboard.isPressed({left, right})){
            if(Keyboard.isPressed(left)){
                velocityX -= accelerationX * airspeedMod;
            }
            if(Keyboard.isPressed(right)){
                velocityX += accelerationX * airspeedMod;
            }
        }
        // fast fall
        if(Keyboard.isPressed(down) && !inJumpLag){
            // increase gravity for fast fall
            gravity = fastFallGravity;
            // set fast fall state to true
            inFastFall = true;
        }
        // use double jump
        if(Keyboard.isPressed(up) && !doubleJumpUsed && !inJumpLag){
            inFastFall = false;
            gravity = tempGravity;
            doubleJumpUsed = true;
            currentGravityForce = 0;
            velocityY =- jumpForce;
            doubleJumpAnimationEnd = false;
            doubleJumpX = positionX;
            doubleJumpY = positionY;
            // TODO: animate double jump
            //FEHImage doubleJump;
            //doubleJump.Open("doubleJumpFrames\)
        }
    }

    // other

    // currently unused
    // limit velocity values before movement
    if(velocityX > velocityXLimit){
        velocityX = velocityXLimit;
    }else if(velocityX < (velocityXLimit * -1)){
        velocityX = velocityXLimit * -1;
    }


    // dash lag timer
    if(inDashLag){
        dashLagTimer++;
        if(dashLagTimer >= dashLagTimerMax){
            inDashLag = false;
            dashLagTimer = 0;
            velocityXDecay = velocityXDecayTemp;
        }
    }
    // jump lag timer, prevents double jump from instantly being used
    if(inJumpLag){
        jumpLagTimer++;
        if(jumpLagTimer >= jumpLagTimerMax  && !Keyboard.isPressed(up)){
            inJumpLag = false;
            jumpLagTimer = 0;
        }
    }


}


void player::enactPlayerMovement(int frameTime){
    // apply velocity
    positionX += velocityX;
    positionY += velocityY;


    // lower limit for speed, prevents sliding at low speed
    if(grounded && abs(velocityX) < 1){
        velocityX = 0;
    }

    // decay X-velocity exponentially
    velocityX *= pow(velocityXDecay, abs(velocityX));
    

    // custom-bake conditions to fit the dimensions of the platform(s)
    // apply gravity (decay y-velocity) and handle grounded state
    if(positionY < 180 - hitboxHeight || (positionX <= 50 - hitboxLength || positionX >= 263)){
        // player is in the air
        grounded = false;
        if(currentGravityForce < maxGravityForce){
            currentGravityForce += gravity;
        }
        velocityY += currentGravityForce;  
    }else if((positionY > 185) && (positionX >= 50 - hitboxLength && positionX <= 263)){
        // player is against the sides of the ground
        if(positionX <= 50 + hitboxLength){  //left side
            positionX = 50 - hitboxLength;
            velocityX = 0;
        }else if(positionX >= 268 - hitboxLength){ // right side
            positionX = 263;
            velocityX = 0;
        }else{
            positionY = 180;
            velocityY = 0;
        }
        
    }
    else{
        // player has landed on the ground
        // reset grounded state
        grounded = true;
        doubleJumpUsed = false;
        // reset fast fall state
        if(inFastFall){
            inFastFall = false;
            gravity = tempGravity;
        }
        // reset force of gravity
        currentGravityForce = 0;
        
        // set position alinged with ground
        positionY = 180 - hitboxHeight;
        // reset velocity
        velocityY = 0;
    }
    if((positionX + hitboxLength>= 106 && positionX <= 213) && velocityY >= 0 && (positionY + hitboxHeight >= 140 && positionY + hitboxHeight <= 150)  && !Keyboard.isPressed(down)){
        // Player has landed on the upper platform
        grounded = true;
        doubleJumpUsed = false;
        if(inFastFall){
            inFastFall = false;
            gravity = tempGravity;
        }
        // reset force of gravity
        currentGravityForce = 0;
        
        // set position alinged with ground
        positionY = 140 - hitboxHeight;
        // reset velocity
        velocityY = 0;
    }
    
    
    // update hitbox position to follow player position
    playerHitbox.updateHitbox(positionX, positionY);
}