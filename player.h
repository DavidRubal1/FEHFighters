class player{
    public:
        player(Key leftwards, Key rightwards, Key upwards, Key downwards, Key basicAttack,Key kickAttack, Key projectileAttack, int startingX, int startingY,  int color);
        void generalPlayerMovementControl();
        void dash(int direction);
        void enactPlayerMovement();
        void manageHitboxes(player *otherPlayer);
        void jump();
        void getHit(attack* activeAttack);
        void groundPlayer(int groundYLevel);
        void updateProjectile();
        
        void playAnimations();

        void updateTimers();

        void resetIfOffscreen();

        void checkAttackHits(player *otherPlayer, attack *activeAttack);
        void action();
        attack* getCurrentAttack();
        animationType* getCurrentAttackAnimation();


        std::vector<int> getXYPosition();
        std::vector<float> getXYVelocity();
        void setXYVelocity(float x, float y);
        hitbox getHitbox();
        int lagFrame = 0;
        float getDamage();
        int remainingLives = 3;
        bool gameOver = false;


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
        attack projectileCast;
        attack projectileProjectile;

        // general player speed
        float velocityX = 0, velocityY = 0;
        int knockbackVelX, knockbackVelY;
        int direction;

        // unused maybe use this to limit speed? knockback will make this hard
        float velocityXLimit = 20, velocityYLimit = 20;

        // grounded speed stuff
        float accelerationX = 1.05;
        float dashSpeedMod = 2;
        float runSpeedMax = 3;
        float velocityXDecay = 0.92, velocityXDecayTemp = 0.92;
        float velocityXDecayDash = 0.95;
        // grounded state
        bool grounded = true;

        // airborne
        float airspeedMod = 0.6;
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

        // attack animations
        animationType punchAnimation;
        animationType kickAnimation;
        animationType projectileCastAnimation;

        
        animation doubleJumpAnimator;
        animationType doubleJumpAnimation = {"./DoubleJump/doubleJumpFrame", 3, false, 3};

        int doubleJumpX, doubleJumpY;

        //jump variables
        float jumpForce = 6;
        bool inJumpLag = false;
        float jumpLagTimerMax = 3; // 3 frames
        float jumpLagTimer = 0;


        // gravity
        float fastFallGravity = 0.20;
        bool inFastFall = false;
        float gravity = 0.08, tempGravity = 0.08;
        float currentGravityForce = 0, groundedGravityForce = 0.3;
        float maxGravityForce = 1.2;

        // controls
        Key left = KEY_A, right = KEY_D, up = KEY_W,  down = KEY_S, basic = KEY_T, kick = KEY_R, projectile = KEY_Y;
        float damage = 0;
        // TODO: WORK ON IMPLEMENTING HITSTUN
        timer hitstunTimer;
        timer respawnIntangibleTimer;
        
        // attack animation variables
        bool inAttackAnimation = false;  // is any attack currently animating
        int currentAttackType = -1;  //(-1 = none, 0 = punch, 1 = kick, etc.)
        int attackAnimationTimer = 0;  // tracks elapsed time in current attack animation
        bool AttackPressedLastFrame = false;  //track previous frame's button state
        
       
        // Number of images/frames for each attack type
        int attackFrameCount[3] = {5, 5, 5};  // punch has 5 frames, kick has 5 frames
        
        // Frame timing arrays for each attack (in number of frames)
        // punch: frames 0-4 timing
        int FrameTiming[3][5] = 
        {
            {1, 1, 2, 2, 3},
            {2, 2, 3, 3, 2},
            {4, 4, 6, 3, 3}
        };
        
        //Hitbox activation arrays for each attack (which frames deal damage)
        bool FrameHasHitbox[3][5] = {
            {false, false, false, true, false},  //punch active frames
            {false, false, false, true, false}, //kick active frames
            {false, false, false, true, false}  //fireball active frames
        };  
        
        bool attackHitboxActive = false;  //is the current attack's hitbox active this frame
};

// constructor
player::player(Key leftwards, Key rightwards, Key upwards, Key downwards, Key basicAttack, Key kickAttack, Key projectileAttack, int startingX, int startingY, int color) 
    : playerHitbox(hitboxHeight, hitboxLength, positionX, positionY), 
    punch(0, 15, 10, 5, 4), kickAttack(1, 10, 12, 3, 4), projectileCast(2, 10, 10, 0, 0), projectileProjectile(3, 9, 8, -5, 8, 2.5),
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
    projectile = projectileAttack;
    playerColor = color;
    if(color == BLUE){
        direction = -1;
    }else{
        direction = 1;
    }

    //set up attack info
    strcpy(punchAnimation.fileName, "/Punch/");
    punchAnimation.ID = 3;
    punchAnimation.looping = false;

    strcpy(kickAnimation.fileName, "/Kick/");
    kickAnimation.ID = 4;
    kickAnimation.looping = false;

    strcpy(projectileCastAnimation.fileName, "/ProjectileCast/");
    projectileCastAnimation.ID = 5;
    projectileCastAnimation.looping = false;
    // maybe add a separate variable to indicate that this should be separated from the player.
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
// TODO: rework this
void player::getHit(attack* activeAttack){
    float forceX, forceY;
    float hitstunMult = 1;
    // scale force based on current damage
    // TODO: scale the power by a value that makes sense later
    float force = (((0.1 * (damage / 100)))* 50 * (*activeAttack).getKBScaling()) + (*activeAttack).getKnockback();
    // printf("FORCE: %f", force);
    // if(force < 3.5){
    //     hitstunMult = 0.75;
    //     force /= 2;
    // }

    //TODO: calculate htistun scaling
    // reset timing varibles before entering hitstun, or move those timers into a separate function
    lagFrame = 0;
    hitstunTimer.resetTimer();
    hitstunTimer.changeTimerMax((*activeAttack).getHitstun() * hitstunMult  + (*activeAttack).getHitstunScaling()  * damage);
    hitstunTimer.isActive();

    // calculate the direction of knockback into x and y components
    // angle degrees ranges from -pi to pi, with -pi as directly down and pi is straight up
    forceX = (*activeAttack).getDirection() * force*cos((*activeAttack).getAngle());
    forceY = force*sin((*activeAttack).getAngle());
    // apply force to player velocity

    velocityX = forceX;
    velocityY = -1 * forceY;
    currentGravityForce = 0;
    
    damage += (*activeAttack).getDamage();
}

// maybe I dont do this?
void player::updateTimers(){

}

void player::updateProjectile(){
    projectileProjectile.moveProjectile(projectileProjectile.getXVelocity());
    projectileProjectile.playProjectileAnimation(playerColor);
}




void player::playAnimations(){
    // determine the current animation to play and play it
    if(!hitstunTimer.isActive()){
        if(!inAttackAnimation){
        if(lagFrame == 0){
            if(grounded){
                if((Keyboard.areAnyPressed({left, right}) & !Keyboard.isPressed(down) && (!Keyboard.isPressed({left, right})))){  //dash right
                    // Dash animation
                    playerAnimator.playAnimation(dashAnimation.fileName, positionX, positionY, direction, dashAnimation.finalFrameNum, 1, dashAnimation.looping, dashAnimation.ID);
                } else if(Keyboard.isPressed(down)){
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
    }else{
        //potentially add a "hit" animation
        playerAnimator.playAnimation(idleAnimation.fileName, positionX, positionY, direction, idleAnimation.finalFrameNum, idleAnimation.frameLength, idleAnimation.looping, idleAnimation.ID );
    }
    

    // play double jump animation
    if(doubleJumpUsed && !hitstunTimer.isActive()){
        doubleJumpAnimator.playAnimation(doubleJumpAnimation.fileName, doubleJumpX, doubleJumpY, doubleJumpAnimation.finalFrameNum, 2, doubleJumpAnimation.looping, doubleJumpAnimation.ID);
    }else{
        doubleJumpAnimator.resetTimer();
    }
    
    //attack animation 
    /*coded by Charlie Limbert, based on existing animation code for idling by David Rubal*/
    if(inAttackAnimation && !hitstunTimer.isActive()){
        char filePath[64];
        
        // Calculate total animation duration by summing all frame timings
        int totalDuration = 0;
        for(int i = 0; i < attackFrameCount[currentAttackType]; i++)
        {
            totalDuration += FrameTiming[currentAttackType][i];
        }
        // change this to work with other attacks as well
        animationType* currentAttackAnimation = getCurrentAttackAnimation();
        (*currentAttackAnimation).finalFrameNum = totalDuration;

        
        // Determine current frame based on timer
        int elapsedTime = 0;
        attackHitboxActive = false;
        
        //
        for(int i = 0; i < attackFrameCount[currentAttackType]; i++)
        {
            //determines what frame the attack animation is on
            if(attackAnimationTimer < elapsedTime + FrameTiming[currentAttackType][i])
            {
                (*currentAttackAnimation).frameLength = FrameTiming[currentAttackType][i];
                attackHitboxActive = FrameHasHitbox[currentAttackType][i]; //sets hitbox to active or not based on frame array.
                break;
            }
            elapsedTime += FrameTiming[currentAttackType][i];
        }
        int offsetPositionX, offsetPositionY;
        if(direction == -1){
            offsetPositionX = positionX-11;
        }else{
            offsetPositionX = positionX;
        }
        offsetPositionY = positionY;
        playerAnimator.playAnimation((*currentAttackAnimation).fileName, offsetPositionX, offsetPositionY, direction, (*currentAttackAnimation).finalFrameNum, (*currentAttackAnimation).frameLength, (*currentAttackAnimation).looping, (*currentAttackAnimation).ID); 
    
        // Update attack animation timer
        attackAnimationTimer++;
        if(attackAnimationTimer >= totalDuration){
            if(currentAttackType == 0){
                lagFrame = 5;  // lag after attack ends
            }else if(currentAttackType == 1){
                lagFrame = 7;
            }else if(currentAttackType == 2){
                lagFrame = 3;
            }
            
            inAttackAnimation = false;
            currentAttackType = -1;
            attackAnimationTimer = 0;
            attackHitboxActive = false;
            
            
        }else{
            if(currentAttackType == 2 && attackAnimationTimer == 19){
                projectileProjectile.updateAttackPosition(positionX, positionY, direction, true);
                projectileProjectile.updateActiveState(true);
            }
        }
    }
}

attack* player::getCurrentAttack(){
    switch(currentAttackType){
        case 0: 
        return &punch;
        break;
        case 1: 
        return &kickAttack;
        break;
        case 2:
        return &projectileCast;
        break;
    }
}

animationType* player::getCurrentAttackAnimation(){
    switch(currentAttackType){
        case 0: 
        return &punchAnimation;
        break;
        case 1: 
        return &kickAnimation;
        break;
        case 2:
        return &projectileCastAnimation;
        break;
    }
}

void player::manageHitboxes(player *otherPlayer){
    // manage projectiles differently that other attacks
    if(currentAttackType != -1){
        attack* currentAttack = getCurrentAttack();
        (*currentAttack).updateAttackPosition(positionX, positionY, direction, attackHitboxActive);
        // TODO: the attack is only active on the last frame now
        if(attackHitboxActive){
            checkAttackHits(otherPlayer, currentAttack);
        }
    }
    if(projectileProjectile.isActive()){
        checkAttackHits(otherPlayer, &projectileProjectile);
    }
    
}

void player::checkAttackHits(player *otherPlayer, attack *activeAttack){
        if((*activeAttack).isActive() && (*activeAttack).checkCollision((*otherPlayer).getHitbox())){
            (*otherPlayer).getHit(activeAttack);
            // disable attack to prevent attack from hitting multiple times in consecutive frames
            (*activeAttack).updateActiveState(false);
        }

}

// TODO: rework this to make it less instant
void player::resetIfOffscreen(){
    // if player position is off-screen
    if(positionX < 0 || positionX > 319 || positionY > 239 || positionY < 0){
        // resets position, velocity, and damage
        positionX = startingPosX;
        positionY = startingPosY;
        velocityX = 0;
        velocityY = 0;
        damage = 0;
        remainingLives--;

        // checks for game over
        
        if (remainingLives == 0)
        {
            gameOver = true;
        }
    }
    
}

void player::dash(int direction){
    velocityX = direction * runSpeedMax * 0.9;
    inDashLag = true;
    velocityXDecay = velocityXDecayDash;
}

/*coded by Charlie Limbert*/
void player::action(){
    if(projectileProjectile.isActive()){
        updateProjectile();
    }
    // jump-cancel projectileCast attack after projectile has been created
    // this does not work properly
    // if(((inAttackAnimation && (attackAnimationTimer > 14 || lagFrame != 0) && (*getCurrentAttack()).getAttackType() == 2)) && Keyboard.isPressed(up) && grounded){
    //     lagFrame = 0;
    //     currentAttackType = -1;
    //     inAttackAnimation = false;
        
    // }
    // Decrease lag frame counter
    if(lagFrame > 0){
        lagFrame--;
    }
    if(!hitstunTimer.isActive()){
        // Detect button press (transition from not pressed to pressed)
        bool buttonPressed = (Keyboard.isPressed(basic) || Keyboard.isPressed(kick) ||Keyboard.isPressed(projectile));

        //prevents holding of attacks.
        bool isNewPress = buttonPressed && !AttackPressedLastFrame; //checks if button was just pressed or has been held.
        AttackPressedLastFrame = buttonPressed;  // store current frame's button state for next frame comparison
        
        // Only allow attack if: button was just pressed AND lagFrame cooldown has expired AND no attack is already playing
        if(isNewPress && lagFrame <= 0 && !inAttackAnimation)
        {
            if (Keyboard.isPressed(basic)) 
            {
                currentAttackType = 0;// punch
                punch.updateActiveState(true);

            }
            else if (Keyboard.isPressed(kick))
            {
                currentAttackType = 1; //kick
                kickAttack.updateActiveState(true);
            }
            else if (Keyboard.isPressed(projectile))
            {
                currentAttackType = 2; //projectile
                projectileCast.updateActiveState(true);
            }
            inAttackAnimation = true;  //indicates attack animation is being played 
            attackAnimationTimer = 0;  // reset timer to beginning of animation
            lagFrame = 1;  // Set lag to prevent immediate re-triggering (will be overwritten when animation ends)

        }
    }
    
    
}

void player::jump(){
    currentGravityForce = 0;
    velocityY -= jumpForce;
    inJumpLag = true;
}

void player::generalPlayerMovementControl(){
    if(!hitstunTimer.isActive()){
        // grounded movement
        if(grounded){
            if(lagFrame <= 0 && !inAttackAnimation){
                if(!Keyboard.isPressed(down) && !Keyboard.isPressed({left, right})){
                    if(!inDashLag){
                        // move left
                        if(Keyboard.isPressed(left)){
                            direction = -1;
                            if(velocityX >= 0){
                                // dash if turning around or stationary
                                dash(direction);
                            }else{
                                velocityX -= accelerationX;
                            }
                        }
                        // move right
                        if(Keyboard.isPressed(right)){
                            direction = 1;
                            if(velocityX <= 0){
                                dash(direction);
                            }else{
                                velocityX += accelerationX;
                            }
                            
                        }
                    }else{
                        //turn around mid-dash
                        if(Keyboard.isPressed(right)){
                            direction = 1;
                        }else if(Keyboard.isPressed(left)){
                            direction = -1;
                        }
                    }
                    
                }
                //jump when on ground
                if(Keyboard.isPressed(up)){
                    jump();
                }
            }
        }else{ 
            // airborne movement (grouded == false)
            if(!Keyboard.isPressed({left, right})){
                if(Keyboard.isPressed(left)){
                    velocityX -= accelerationX * airspeedMod;
                }
                if(Keyboard.isPressed(right)){
                    velocityX += accelerationX * airspeedMod;
                }
            }
            // fast fall when down is pressed
            if(Keyboard.isPressed(down) && !inJumpLag){
                // increase gravity for fast fall
                gravity = fastFallGravity;
                // set fast fall state to true
                inFastFall = true;
            }
            // use double jump when jumping in air
            if(lagFrame <= 0 && !inAttackAnimation){
                if(Keyboard.isPressed(up) && !doubleJumpUsed && !inJumpLag){
                    inFastFall = false;
                    gravity = tempGravity;
                    doubleJumpUsed = true;
                    currentGravityForce = 0;
                    velocityY =- (jumpForce-1);
                    doubleJumpX = positionX - 3;
                    doubleJumpY = positionY + hitboxHeight - 1;
                    if(!Keyboard.isPressed({left, right})){
                        if(Keyboard.isPressed(left)){
                            direction = -1;
                            velocityX = 2.0 * direction;
                        }
                        if(Keyboard.isPressed(right)){
                            direction = 1;
                            velocityX = 2.0 * direction;
                            
                        }
                    }

                    }

                }
            }
        }
    
    

    // other functionality, TODO: move these to a separate function or something

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

void player::groundPlayer(int groundYLevel){
    grounded = true;
    doubleJumpUsed = false;
    // reset fast fall state
    if(inFastFall){
        inFastFall = false;
        gravity = tempGravity;
    }
    // reset force of gravity
    currentGravityForce = groundedGravityForce;
    // reset velocity
    velocityY = 0;
    // set position alinged with ground
    positionY = groundYLevel - hitboxHeight;
}


void player::enactPlayerMovement(){
    // apply velocity, typecast to int to prevent unwanted truncating of position after change
    // ex: 100 + 2.4 = 102, but 100 - 2.4 = 97, rounding down works against us with negative vel
    positionX += static_cast<int>(velocityX);
    positionY += static_cast<int>(velocityY);

    // lower limit for speed, prevents sliding at low speed
    if(grounded && abs(velocityX) < 1){
        velocityX = 0;
    }

    if(!hitstunTimer.isActive()){
            // decay X-velocity exponentially when not moving horizontally
            // THIS DOES NOT WORK WHEN HOLDING CROUCH
        if(!Keyboard.areAnyPressed({left,right}) || inAttackAnimation){
            velocityX *= pow(velocityXDecay, abs(velocityX));
        }else{
            if(velocityX > runSpeedMax){
                velocityX = runSpeedMax;
            }else if(velocityX < (runSpeedMax * -1)){
                velocityX = runSpeedMax * -1;
            }
        }
    }else{
        //have a separate x-velocity decay when in hitstun
        velocityX *= velocityXDecay;
    }

    

    // custom-bake conditions to fit the dimensions of the platform(s)
    // apply gravity (decay y-velocity) and handle grounded state
    if(positionY < 180 - hitboxHeight || (positionX <= 50 - hitboxLength || positionX >= 263)){
        // player is in the air
        grounded = false;
        // increase force of gravity when in air
        if(currentGravityForce < maxGravityForce){
            currentGravityForce += gravity;
        }
        // apply force of gravity
        velocityY += currentGravityForce;  
    }else if((positionY > 185) && (positionX >= 50 - hitboxLength && positionX <= 263)){
        // player is against the sides of the stage
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
        groundPlayer(180);

    }
    if((positionX + hitboxLength>= 106 && positionX <= 213) && velocityY >= 0 && (positionY + hitboxHeight >= 140 && positionY + hitboxHeight <= 152)  && !Keyboard.isPressed(down)){
        // Player has landed on the upper platform
        // reset grounded state
        groundPlayer(140);
    }
    
    // update hitbox position to follow player position
    playerHitbox.updateHitbox(positionX, positionY);
    if(hitstunTimer.isActive()){
        hitstunTimer.incrementTimer();
        if(velocityY > 2.0){
            hitstunTimer.setActiveState(false);
        }else{
            hitstunTimer.updateTimerState();
        }
        
    }
}
