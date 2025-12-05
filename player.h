class player{
    public:
        player(Key leftwards, Key rightwards, Key upwards, Key downwards, Key basicAttack,Key kickAttack, int startingX, int startingY,  int color);
        void generalPlayerMovementControl();
        void dash(int direction);
        void enactPlayerMovement();
        void manageHitboxes(player *otherPlayer);
        void getHit(float force,  float damageAmount, float angleRadians, int attackDirection);
        
        void playAnimations();

        void updateTimers();

        void resetIfOffscreen();

        void checkAttackHits(player *otherPlayer);
        void action();


        std::vector<int> getXYPosition();
        std::vector<float> getXYVelocity();
        void setXYVelocity(float x, float y);
        hitbox getHitbox();
        int lagFrame = 0;
        float getDamage();


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

        //attacks
        attack punch;
        attack kickAttack;
        attack projectile;

        // general player speed
        float velocityX = 0, velocityY = 0;
        int direction = 1;
        // unused maybe use this to limit speed? knockback will make this hard
        float velocityXLimit = 20, velocityYLimit = 20;

        // grounded speed stuff
        float accelerationX = 1.05;
        float dashSpeedMod = 2.1;
        float velocityXDecay = 0.92, velocityXDecayTemp = 0.92;
        float velocityXDecayDash = 0.95;
        // grounded state
        bool grounded = true;

        // airborne
        float airspeedMod = 0.75;
        bool doubleJumpUsed = false;

        // dash variables
        bool inDashLag = false;
        float dashLagTimerMax = 4; // 3 frames
        float dashLagTimer = 0;
        
        // animation timers 
        // THIS: I could also just not use the frame time and increment each frame...
        animation playerAnimator;
        animationType idleAnimation = {"/Idle/Idle", 1, true, 0, 30};
        animationType dashAnimation = {"/Dash/Dash", 8, true, 1};
        animationType crouchAnimation = {"/Crouch/Crouch", 0, true, 2};
        animationType punchAnimation;
        

        animation doubleJumpAnimator;
        animationType doubleJumpAnimation = {"./DoubleJump/doubleJumpFrame", 3, false, 3};

        int doubleJumpX, doubleJumpY;



        //jump variables
        float jumpForce = 6;
        bool inJumpLag = false;
        float jumpLagTimerMax = 3; // 3 frames
        float jumpLagTimer = 0;


        // gravity
        float fastFallGravity = 0.25;
        bool inFastFall = false;
        float gravity = 0.10, tempGravity = 0.10;
        float currentGravityForce = 0, groundedGravityForce = 0.3;
        float maxGravityForce = 1.5;

        // controls
        Key left = KEY_A, right = KEY_D, up = KEY_W,  down = KEY_S, basic = KEY_T, kick = KEY_R;
        float damage = 0;
        
        // attack animation variables
        bool inAttackAnimation = false;  // is any attack currently animating
        int currentAttackType = -1;  //(-1 = none, 0 = punch, 1 = kick, etc.)
        int attackAnimationTimer = 0;  // tracks elapsed time in current attack animation
        int attackFrame = 0;  // current frame (0-4) being displayed
        bool AttackPressedLastFrame = false;  //track previous frame's button state
        
       
        // Number of images/frames for each attack type
        int attackFrameCount[2] = {5, 5};  // punch has 5 frames, kick has 5 frames
        
        // Frame timing arrays for each attack (in number of frames)
        // punch: frames 0-4 timing
        int FrameTiming[2][5] = 
        {
            {1, 1, 3, 3, 2},
            {2, 2, 3, 3, 2}
        };
        
        //Hitbox activation arrays for each attack (which frames deal damage)
        bool FrameHasHitbox[2][5] = {
            {false, false, false, true, false},  //punch active frames
            {false, false, false, true, false}  //kick active frames
        };  
        
        bool attackHitboxActive = false;  //is the current attack's hitbox active this frame
};

// constructor
player::player(Key leftwards, Key rightwards, Key upwards, Key downwards, Key basicAttack, Key kickAttack, int startingX, int startingY, int color) 
    : playerHitbox(hitboxHeight, hitboxLength, positionX, positionY), 
    punch(0, 10, 10), kickAttack(1, 10, 10), projectile(2, 10, 10),
    playerAnimator(color), doubleJumpAnimator(color){
    left = leftwards;
    right = rightwards;
    up = upwards;
    down = downwards;
    startingPosX = startingX;
    startingPosY = startingY;
    positionX = startingX;
    positionY = startingY;
    basic = basicAttack;
    kick = kickAttack;
    playerColor = color;

    //set up attack info
    strcpy(punchAnimation.fileName, "/Punch/");
    punchAnimation.ID = 3;
    punchAnimation.looping = false;

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

void player::getHit(float force, float damageAmount, float angleRadians, int attackDirection){
    float forceX, forceY;
    // scale force based on current damage
    // TODO: divide the power by a value that makes sense later
    // TOFIX: ATTACKS HIT MULTIPLE TIMES
    force = (((0.1 * (damage / 100) + 0.05 * (damage / 100) * damageAmount))* 200) + force;

    // calculate the direction of knockback into x and y components
    // angle degrees ranges from -pi to pi, with -pi as directly down and pi is straight up
    forceX = direction * force*cos(angleRadians);
    forceY = direction * force*sin(angleRadians);
    // apply force to player velocity
    velocityX += forceX;
    velocityY -= forceY;
    damage += damageAmount;
}

// maybe I dont do this?
void player::updateTimers(){

}




void player::playAnimations(){
    // determine animation
    // if(!grounded){
    //     dashTimer.setActiveState(false);
    // }
    // if(!grounded || !Keyboard.isPressed(down) || Keyboard.areAnyPressed({left, right})){ // uncrouch when not holding down
    //     inCrouch = false;
    // }

    // determine current frame's animation


    if(!inAttackAnimation){
        if(lagFrame == 0){
            if(grounded){
                if((Keyboard.areAnyPressed({left, right}) && (!Keyboard.isPressed({left, right})))){  //dash right
                    // Dash animation
                    playerAnimator.playAnimation(dashAnimation.fileName, positionX, positionY, direction, dashAnimation.finalFrameNum, 1, dashAnimation.looping, dashAnimation.ID);
                } else if(Keyboard.isPressed(down) && !Keyboard.areAnyPressed({left, right})){
                    // Crouch animation
                    playerAnimator.playAnimation(crouchAnimation.fileName, positionX, positionY, direction, crouchAnimation.finalFrameNum, 1, crouchAnimation.looping, crouchAnimation.ID);
                }else{
                    playerAnimator.playAnimation(idleAnimation.fileName, positionX, positionY, direction, idleAnimation.finalFrameNum, idleAnimation.frameLength, idleAnimation.looping, idleAnimation.ID );
                }
            }else{
                //airborne animation, replace the placeholder idle animation when able
                playerAnimator.playAnimation(idleAnimation.fileName, positionX, positionY, direction, idleAnimation.finalFrameNum, idleAnimation.frameLength, idleAnimation.looping, idleAnimation.ID );
            }
        } else{
            playerAnimator.playAnimation(idleAnimation.fileName, positionX, positionY, direction, idleAnimation.finalFrameNum, idleAnimation.frameLength, idleAnimation.looping, idleAnimation.ID );
        }
    }


    if(doubleJumpUsed){
        doubleJumpAnimator.playAnimation(doubleJumpAnimation.fileName, doubleJumpX, doubleJumpY, doubleJumpAnimation.finalFrameNum, 2, doubleJumpAnimation.looping, doubleJumpAnimation.ID);
    }else{
        doubleJumpAnimator.resetTimer();
    }

    // TODO: clear dash animation when used, cancel when jump?
    // MAKE SURE TO UPDATE THE ATTACK POSITION updateAttackPosition
    
    //attack animation 
    /*coded by Charlie Limbert, based on existing animation code for idling by David Rubal*/
    if(inAttackAnimation){
        char filePath[64];
        
        // Determine the correct attack sprite directory based on player color, direction, and attack type
        // if(playerColor == RED)
        // {
        //     if(direction == -1)
        //     {
        //         // Punch left or other left attacks
        //         if(currentAttackType == 0){
        //             strcpy(filePath, "./PlayerRed/PlayerRedLeftPunch/");
        //         }
        //         // Add more attack types here:
        //         else if(currentAttackType == 1){
        //             strcpy(filePath, "./PlayerRed/PlayerRedLeftKick/");
        //         }
        //     }
        //     else
        //     {
        //         //right attacks
        //         if(currentAttackType == 0)
        //         {
        //             strcpy(filePath, "./PlayerRed/PlayerRedRightPunch/");
        //         }
        //         // Add more attack types here:
        //         else if(currentAttackType == 1)
        //         {
        //             strcpy(filePath, "./PlayerRed/PlayerRedRightKick/");
        //         }
        //     }
        // }
        


        // Calculate total animation duration by summing all frame timings
        int totalDuration = 0;
        for(int i = 0; i < attackFrameCount[currentAttackType]; i++)
        {
            totalDuration += FrameTiming[currentAttackType][i];
        }
        punchAnimation.finalFrameNum = totalDuration;
        
        // Determine current frame based on timer
        int elapsedTime = 0;
        attackFrame = 0;
        attackHitboxActive = false;
        
        //
        for(int i = 0; i < attackFrameCount[currentAttackType]; i++)
        {
            //determines what frame the attack animation is on
            if(attackAnimationTimer < elapsedTime + FrameTiming[currentAttackType][i])
            {
                attackFrame = i;
                punchAnimation.frameLength = FrameTiming[currentAttackType][i];
                attackHitboxActive = FrameHasHitbox[currentAttackType][i]; //sets hitbox to active or not based on frame array.
                break;
            }
            elapsedTime += FrameTiming[currentAttackType][i];
        }
        int offsetPositionX, offsetPositionY;
        if(direction == -1){
            offsetPositionX = positionX-11;
        }else{
            offsetPositionX = positionX+3;
        }
        offsetPositionY = positionY;
        playerAnimator.playAnimation(punchAnimation.fileName, offsetPositionX, offsetPositionY, direction, punchAnimation.finalFrameNum, punchAnimation.frameLength, punchAnimation.looping, punchAnimation.ID); 
        // //gets proper animation frame
        // strcat(filePath, std::to_string(attackFrame).c_str()); 
        // strcat(filePath, ".png");
        // FEHImage punchImg;
        // punchImg.Open(filePath);
        // if(direction == -1)
        // {
        // punchImg.Draw(positionX-11, positionY);
        // }
        // else
        // {
        // punchImg.Draw(positionX, positionY);

        
        
    
        
        // Update attack animation timer
        attackAnimationTimer++;
        if(attackAnimationTimer >= totalDuration){
            inAttackAnimation = false;
            currentAttackType = -1;
            attackAnimationTimer = 0;
            attackHitboxActive = false;
            lagFrame = 5;  // lag after attack ends
        }
    }
}



void player::manageHitboxes(player *otherPlayer){
    if(attackHitboxActive){
        switch(currentAttackType){
            case 0:
            punch.updateHitboxState(true);
            punch.updateAttackPosition(positionX, positionY, direction);
            break;
            case 1:
            kickAttack.updateHitboxState(true);
            kickAttack.updateAttackPosition(positionX, positionY, direction);
            break;
            case 2:
            projectile.updateHitboxState(true);
            //projectiles will move on their own
            break;
        }
        checkAttackHits(otherPlayer);
    }else{
        switch(currentAttackType){
            case 0:
            punch.updateHitboxState(false);
            break;
            case 1:
            kickAttack.updateHitboxState(false);
            break;
            case 2:
            projectile.updateHitboxState(false);
            //projectiles will move on their own
            break;
        }
    }
}

void player::checkAttackHits(player *otherPlayer){
        if(punch.checkCollision((*otherPlayer).getHitbox())){
            (*otherPlayer).getHit(punch.getKnockback(), punch.getDamage(), punch.getAngle(), punch.getDirection());
        }
        if(kickAttack.checkCollision((*otherPlayer).getHitbox())){
            (*otherPlayer).getHit(kickAttack.getKnockback(), kickAttack.getDamage(), kickAttack.getAngle(), kickAttack.getDirection());
        }
        if(projectile.checkCollision((*otherPlayer).getHitbox())){
            (*otherPlayer).getHit(projectile.getKnockback(), projectile.getDamage(), projectile.getAngle(), projectile.getDirection());
        }
}


void player::resetIfOffscreen(){
    // if player position is off-screen
    if(positionX < 0 || positionX > 319 || positionY > 239 || positionY < 0){
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

void player::action(){
    // Decrease lag frame counter
    if(lagFrame > 0){
        lagFrame--;
    }
    
    // Detect button press (transition from not pressed to pressed)
    bool buttonPressed = (Keyboard.isPressed(basic) || Keyboard.isPressed(kick));

    bool isNewPress = buttonPressed && !AttackPressedLastFrame; //checks if button was just pressed or has been held. Logic explained by Git Copilot.
    AttackPressedLastFrame = buttonPressed;  // store current frame's button state for next frame comparison
    
    // Only allow attack if: button was just pressed AND lagFrame cooldown has expired AND no attack is already playing
    if(isNewPress && lagFrame <= 0 && !inAttackAnimation)
    {
        if (Keyboard.isPressed(basic)) 
        {
            currentAttackType = 0;// punch

        }
        else if (Keyboard.isPressed(kick))
        {
            currentAttackType = 1;// kick
        }
        inAttackAnimation = true;  // start an attack animation  
        attackAnimationTimer = 0;  // reset timer to beginning of animation
        attackFrame = 0;  // start at frame 0
        lagFrame = 1;  // Set lag to prevent immediate re-triggering (will be overwritten when animation ends)

        

        
    }
}



void player::generalPlayerMovementControl(){
    if(playerColor == BLUE){
        printf("VelX = %f", velocityX);
    }
    if(grounded){
        if(lagFrame <= 0 && !inAttackAnimation && !Keyboard.isPressed({left, right})){
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
            if(Keyboard.isPressed(up)){
                currentGravityForce = 0;
                velocityY -= jumpForce;
                inJumpLag = true;
            }
            
        }
        // jump when grounded
        
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
        if(lagFrame <= 0 && !inAttackAnimation){
        if(Keyboard.isPressed(up) && !doubleJumpUsed && !inJumpLag){
            inFastFall = false;
            gravity = tempGravity;
            doubleJumpUsed = true;
            currentGravityForce = 0;
            velocityY =- (jumpForce-1);
            doubleJumpX = positionX - 3;
            doubleJumpY = positionY + hitboxHeight - 1;

        }
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


void player::enactPlayerMovement(){
    // apply velocity, typecast to int to prevent unwanted truncating of position after change
    positionX += static_cast<int>(velocityX);
    positionY += static_cast<int>(velocityY);


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
        currentGravityForce = groundedGravityForce;
        
        // set position alinged with ground
        positionY = 180 - hitboxHeight;
        // reset velocity
        velocityY = 0;
    }
    if((positionX + hitboxLength>= 106 && positionX <= 213) && velocityY >= 0 && (positionY + hitboxHeight >= 140 && positionY + hitboxHeight <= 152)  && !Keyboard.isPressed(down)){
        // Player has landed on the upper platform
        grounded = true;
        doubleJumpUsed = false;
        if(inFastFall){
            inFastFall = false;
            gravity = tempGravity;
        }
        // reset force of gravity
        currentGravityForce = groundedGravityForce;
        
        // set position alinged with ground
        positionY = 140 - hitboxHeight;
        // reset velocity
        velocityY = 0;
    }
    
    
    // update hitbox position to follow player position
    playerHitbox.updateHitbox(positionX, positionY);
}
