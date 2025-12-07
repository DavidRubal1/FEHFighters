class player{
    public:
        player(Key leftwards, Key rightwards, Key upwards, Key downwards, Key basicAttack,Key kickAttack, Key projectileAttack, int startingX, int startingY,  int color);
        void generalPlayerMovementControl();
        void dash(int direction);
        void jump();
        void enactPlayerMovement();
        void manageHitboxes(player *otherPlayer);
        void getHit(attack* activeAttack);
        void groundPlayer(int groundYLevel);
        void updateProjectile();
        void updateTimers();
        timer getIntangibilityTimer();
        void playAnimations();
        void resetIfOffscreen();
        void checkAttackHits(player *otherPlayer, attack *activeAttack);
        void action();
        attack* getCurrentAttack();
        animationType* getCurrentAttackAnimation();
        hitbox getHitbox();
        int lagFrame = 0;
        float getDamage();
        int remainingLives = 3;
        bool gameOver = false;

    private:
        // player attributes
        /*written by Charlie Limbert and David Rubal*/

        // position
        int startingPosX, startingPosY;
        int positionX = 100, positionY = 180;
        // player hitbox (and sprite) size
        int hitboxHeight = 20, hitboxLength = 14;

        // player color
        int playerColor;

        // hitbox for player, handles ground collisions and for overlap with enemy attacks
        hitbox playerHitbox;

        //attack objects
        attack punch;
        attack kickAttack;
        attack projectileCast;
        attack projectileProjectile;

        // player speed and direction (-1 == left, 1 == right)
        float velocityX = 0, velocityY = 0;
        int knockbackVelX, knockbackVelY;
        int direction;

        // grounded speed variables
        float accelerationX = 1.05;
        // speed multiplier when dashing
        float dashSpeedMod = 2;
        // maximum running speed
        float runSpeedMax = 3;
        // velocity decay when not holding a movement key
        float velocityXDecay = 0.92, velocityXDecayTemp = 0.92;
        // velocity decay when in dash
        float velocityXDecayDash = 0.95;

        // grounded state, is true when the player is standing on the ground or the platform
        bool grounded = true;

        // airborne info
        // x-velocity multiplier when the player is in the air
        float airspeedMod = 0.6;
        // flag to keep track of whether the player has used double jump
        bool doubleJumpUsed = false;

        // dash variables
        bool inDashLag = false;
        // keeps track of time spent in a dash
        float dashLagTimerMax = 4;
        float dashLagTimer = 0;
        
        // animation player to draw the player's sprites for any given animation
        animation playerAnimator;
        // animation type info for movement animations, info is passed through the animation player
        animationType idleAnimation = {"/Idle/Idle", 1, true, 0, 30};
        animationType dashAnimation = {"/Dash/Dash", 8, true, 1};
        animationType crouchAnimation = {"/Crouch/Crouch", 0, true, 2};

        // animation types for attack animations, info is passed through animation player
        animationType punchAnimation;
        animationType kickAnimation;
        animationType projectileCastAnimation;

        // separate animator and animation type for double jump to allow for another animation to play during double jump
        animation doubleJumpAnimator;
        animationType doubleJumpAnimation = {"./DoubleJump/doubleJumpFrame", 3, false, 3};
        // position of double jump
        int doubleJumpX, doubleJumpY;

        //jump variables
        // change in y-velocity when jumping
        float jumpForce = 6;
        // flag to keep track of whether the player has just jumped, used to prevent double jump from being instantly used
        bool inJumpLag = false;
        float jumpLagTimerMax = 3;
        float jumpLagTimer = 0;

        // gravity
        // gravity during fast fall
        float fastFallGravity = 0.20;
        // flag to tell if the player is in "fast fall", which increases their gravity
        bool inFastFall = false;
        // normal gravity
        float gravity = 0.08, tempGravity = 0.08;
        // downwards force applied each frame
        float currentGravityForce = 0, groundedGravityForce = 0.3;
        // terminal velocity
        float maxGravityForce = 1.2;

        // controls
        //movement keys followed by attack keys
        Key left = KEY_A, right = KEY_D, up = KEY_W,  down = KEY_S, basic = KEY_T, kick = KEY_R, projectile = KEY_Y;
        // damage value of player, increased damage means increased knockback taken
        float damage = 0;
        // timers for hitstun and intangibility when respawning
        // hitstun prevents the player from acting after getting hit
        timer hitstunTimer;
        // makes the player invincible for a short period after dying
        timer respawnIntangibleTimer;
        
        // attack animation variables
        bool inAttackAnimation = false;  // is any attack currently animating
        int currentAttackType = -1;  //(-1 = none, 0 = punch, 1 = kick, etc.)
        int attackAnimationTimer = 0;  // tracks elapsed time in current attack animation
        bool AttackPressedLastFrame = false;  //track previous frame's button state
       
        // Number of images/frames for each attack type
        int attackFrameCount[3] = {5, 5, 5};  // punch has 5 frames, kick has 5 frames, cast has 5 frames
        
        // Frame timing arrays for each attack (in number of frames)
        // punch: frames 0-4 timing, etc.
        // describes how long each frame of animation will last for
        int FrameTiming[3][5] = 
        {
            {1, 1, 2, 2, 3}, // punch
            {2, 2, 3, 3, 2}, // kick
            {4, 4, 6, 3, 3}  // cast
        };
        
        //Hitbox activation arrays for each attack (which frames deal damage)
        bool FrameHasHitbox[3][5] = {
            {false, false, false, true, false},  //punch active frames
            {false, false, false, true, false}, //kick active frames
            {false, false, false, true, false}  //cast active frames
        };  
        
        bool attackHitboxActive = false;  //is the current attack's hitbox active this frame
};

// constructor for player. Assigns each movement and attack key, starting position, player color,
// constructs player hitbox object, constructs the four attack objects with ID, size and position offset,
// and constructs the animator objects that will play the player's animations
/* written by David Rubal*/
player::player(Key leftwards, Key rightwards, Key upwards, Key downwards, Key basicAttack, Key kickAttack, Key projectileAttack, int startingX, int startingY, int color) 
    : playerHitbox(hitboxHeight, hitboxLength, positionX, positionY), 
    punch(0, 15, 10, 5, 4), kickAttack(1, 10, 12, 3, 4), projectileCast(2, 10, 5, 3, 6), projectileProjectile(3, 9, 8, -5, 8, 2.5),
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
        direction = -1; // Blue starts facing left
    }else{
        direction = 1;  // Red starts facing right
    }

    //set up partial info for attack animations
    strcpy(punchAnimation.fileName, "/Punch/");
    punchAnimation.ID = 3;
    punchAnimation.looping = false;

    strcpy(kickAnimation.fileName, "/Kick/");
    kickAnimation.ID = 4;
    kickAnimation.looping = false;

    strcpy(projectileCastAnimation.fileName, "/ProjectileCast/");
    projectileCastAnimation.ID = 5;
    projectileCastAnimation.looping = false;
}

// returns a copy of the player's hitbox
/* written by David Rubal*/
hitbox player::getHitbox(){
    return playerHitbox;
}
// returns the player's current damage
/* written by David Rubal*/
float player::getDamage(){
    return damage;
}


// enact knockback and hitstun on player when hit, and increase damage counter
/* written by David Rubal*/
void player::getHit(attack* activeAttack){
    float forceX, forceY;
    // scale force based on current damage and given knockback
    float force = (((0.1 * (damage / 100)))* 50 * (*activeAttack).getKBScaling()) + (*activeAttack).getKnockback();
    // reset timing varibles before entering hitstun, or move those timers into a separate function
    lagFrame = 0;
    hitstunTimer.resetTimer();
    // set hitstun time based on scaling
    hitstunTimer.changeTimerMax((*activeAttack).getHitstun()  + (*activeAttack).getHitstunScaling()  * damage);
    // calculate the direction of knockback into x and y components
    // angle degrees ranges from -pi to pi, with -pi as directly down and pi is straight up
    forceX = (*activeAttack).getDirection() * force*cos((*activeAttack).getAngle());
    forceY = force*sin((*activeAttack).getAngle());
    // apply force to player's velocity components
    velocityX = forceX;
    velocityY = -1 * forceY;
    currentGravityForce = 0;
    // increase player's damage
    damage += (*activeAttack).getDamage();
}

// move and draw projectile
/* written by David Rubal*/
void player::updateProjectile(){
    // change projectile position by the projectile's velocity
    projectileProjectile.moveProjectile(projectileProjectile.getXVelocity());
    // draw projectile
    projectileProjectile.playProjectileAnimation(playerColor);
}

// increments timers and updates the player state accordingly
/*written by Charlie Limbert and David Rubal*/
void player::updateTimers(){
    // dash lag timer, keeps track of when the player started a dash
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
    // lagFrame timer, prevents the player from acting after attacking
    if(lagFrame > 0){
        lagFrame--;
    }
    // hitstun timer, prevents the player from acting after being attacked
    if(hitstunTimer.isActive()){
        hitstunTimer.incrementTimer();
        if(velocityY > 2.0){
            hitstunTimer.setActiveState(false);
        }else{
            hitstunTimer.updateTimerState();
        }
    }
    // respawn intangibility timer, provided invincibility after respawn
    if(respawnIntangibleTimer.isActive()){
        respawnIntangibleTimer.incrementTimer();
        respawnIntangibleTimer.updateTimerState();
    }
}

// returns a copy of the intangibility timer
/* written by David Rubal*/
timer player::getIntangibilityTimer(){
    return respawnIntangibleTimer;
}

// determines the current animation for the player and plays it
/*coded by Charlie Limbert and David Rubal*/
void player::playAnimations(){
    // written by David rubal
    // if not in hitstun
    if(!hitstunTimer.isActive()){
        // if not attacking
        if(!inAttackAnimation){
            // if not in attack lag
            if(lagFrame == 0){
                // if standing on ground
                if(grounded){
                    // if holding left or right and not crouch but not both left and right
                    if((Keyboard.areAnyPressed({left, right}) & !Keyboard.isPressed(down) && (!Keyboard.isPressed({left, right})))){ 
                        // Dash animation
                        playerAnimator.playAnimation(dashAnimation.fileName, positionX, positionY, direction, dashAnimation.finalFrameNum, 1, dashAnimation.looping, dashAnimation.ID);
                    } else if(Keyboard.isPressed(down)){
                        // Crouch animation
                        playerAnimator.playAnimation(crouchAnimation.fileName, positionX, positionY, direction, crouchAnimation.finalFrameNum, 1, crouchAnimation.looping, crouchAnimation.ID);
                    }else{
                        // idle animation
                        playerAnimator.playAnimation(idleAnimation.fileName, positionX, positionY, direction, idleAnimation.finalFrameNum, idleAnimation.frameLength, idleAnimation.looping, idleAnimation.ID );
                    }
                }else{
                    //airborne, still using idle animation
                    playerAnimator.playAnimation(idleAnimation.fileName, positionX, positionY, direction, idleAnimation.finalFrameNum, idleAnimation.frameLength, idleAnimation.looping, idleAnimation.ID );
                }
            } else{
                // in end lag, using idle animation
                playerAnimator.playAnimation(idleAnimation.fileName, positionX, positionY, direction, idleAnimation.finalFrameNum, idleAnimation.frameLength, idleAnimation.looping, idleAnimation.ID );
            }
        }
    }else{
        // in hitstun, using idle animation
        playerAnimator.playAnimation(idleAnimation.fileName, positionX, positionY, direction, idleAnimation.finalFrameNum, idleAnimation.frameLength, idleAnimation.looping, idleAnimation.ID );
    }
    
    // play double jump animation
    if(doubleJumpUsed && !hitstunTimer.isActive()){
        doubleJumpAnimator.playAnimation(doubleJumpAnimation.fileName, doubleJumpX, doubleJumpY, doubleJumpAnimation.finalFrameNum, 2, doubleJumpAnimation.looping, doubleJumpAnimation.ID);
    }else{
        // resets the animator when player is grounded
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
        // gets the current attack animation type, used to play attack animation
        animationType* currentAttackAnimation = getCurrentAttackAnimation();
        // total number of frames in attack animation
        (*currentAttackAnimation).finalFrameNum = totalDuration;

        // Determine current frame based on timer
        int elapsedTime = 0;
        attackHitboxActive = false;

        for(int i = 0; i < attackFrameCount[currentAttackType]; i++)
        {
            //determines what frame the attack animation is on
            if(attackAnimationTimer < elapsedTime + FrameTiming[currentAttackType][i])
            {
                //gets the length of the current animation frame
                (*currentAttackAnimation).frameLength = FrameTiming[currentAttackType][i];
                attackHitboxActive = FrameHasHitbox[currentAttackType][i]; //sets hitbox to active or not based on frame array.
                break;
            }
            elapsedTime += FrameTiming[currentAttackType][i];
        }
        // offsets the attack by a certain amount to align the animation with the player's hitbox
        int offsetPositionX, offsetPositionY;
        if(direction == -1){
            offsetPositionX = positionX-11;
        }else{
            offsetPositionX = positionX;
        }
        offsetPositionY = positionY;
        // plays the attack animation
        playerAnimator.playAnimation((*currentAttackAnimation).fileName, offsetPositionX, offsetPositionY, direction, (*currentAttackAnimation).finalFrameNum, (*currentAttackAnimation).frameLength, (*currentAttackAnimation).looping, (*currentAttackAnimation).ID); 
    
        // Update attack animation
        attackAnimationTimer++;
        if(attackAnimationTimer >= totalDuration){
            // put the player in lag after an attack
            if(currentAttackType == 0){
                lagFrame = 5;  // set lag after attack ends
            }else if(currentAttackType == 1){
                lagFrame = 7;
            }else if(currentAttackType == 2){
                lagFrame = 3;
            }
            // update attack info after it has finished
            inAttackAnimation = false;
            currentAttackType = -1;
            attackAnimationTimer = 0;
            attackHitboxActive = false;
        }else{
            // create separate projectile after casting, only allow one
            if(currentAttackType == 2 && attackAnimationTimer == 19){
                projectileProjectile.updateAttackPosition(positionX, positionY, direction, true);
                projectileProjectile.updateActiveState(true);
            }
        }
    }
}

// returns pointer to current attack
/* written by David Rubal*/
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

// returns pointer to current attack animation
/* written by David Rubal*/
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

//updates position of attack hitboxes and checks for overlap with other player
/* written by David Rubal*/
void player::manageHitboxes(player *otherPlayer){
    if(currentAttackType != -1){
        // gets the current attack to update position and check for overlap with other player
        attack* currentAttack = getCurrentAttack();
        (*currentAttack).updateAttackPosition(positionX, positionY, direction, attackHitboxActive);
        if(attackHitboxActive){
            checkAttackHits(otherPlayer, currentAttack);
        }
    }
    // checks for projectile overlap with other player, separate because projectiles have separate movement
    if(projectileProjectile.isActive()){
        checkAttackHits(otherPlayer, &projectileProjectile);
    }
    
}

// check if the current attack overlaps with the other player and hit if true
/* written by David Rubal*/
void player::checkAttackHits(player *otherPlayer, attack *activeAttack){
    // if the other player is not intangible, the current attack is active, and the attack collides with the other player
        if(!(*otherPlayer).getIntangibilityTimer().isActive() && (*activeAttack).isActive() && (*activeAttack).checkCollision((*otherPlayer).getHitbox())){
            // the other player takes the hit
            (*otherPlayer).getHit(activeAttack);
            // disable attack to prevent attack from hitting multiple times in the following active frames
            (*activeAttack).updateActiveState(false);
        }
}

// the player is sent offscreen and a life is lost
/*coded by Charlie Limbert and David Rubal*/
void player::resetIfOffscreen(){
    // if player position is off-screen
    if(positionX < 0 || positionX > 319 || positionY > 239 || positionY < 0){
        // give intangibility towards incoming attacks when respawned
        respawnIntangibleTimer.resetTimer();
        respawnIntangibleTimer.changeTimerMax(30);
        // resets position, velocity, and damage
        positionX = startingPosX;
        positionY = startingPosY -10; // player starts slightly above starting position
        velocityX = 0;
        velocityY = 0;
        damage = 0;
        remainingLives--; // jump lag to prevent instant double jump after respawning
        inJumpLag = true;

        // checks for game over when a player has run out of lives
        if (remainingLives == 0)
        {
            gameOver = true;
        }
    }
}

// give the player a dash of speed in the direction
/* written by David Rubal*/
void player::dash(int direction){
    // increase x-velocity
    velocityX = direction * runSpeedMax * 0.9;
    inDashLag = true;
    // decrease velocity decay for sliding
    velocityXDecay = velocityXDecayDash;
}

// get input for attacks and activate the respective attack 
/*coded by Charlie Limbert*/
void player::action(){
    if(projectileProjectile.isActive()){
        updateProjectile();
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
                currentAttackType = 2; //projectile cast
                projectileCast.updateActiveState(true);
            }
            inAttackAnimation = true;  //indicates attack animation is being played 
            attackAnimationTimer = 0;  // reset timer to beginning of animation
            lagFrame = 1;  // Set lag to prevent immediate re-triggering (will be overwritten when animation ends)

        }
    }
}

// the player jumps
/* written by David Rubal*/
void player::jump(){
    // reset downwards force
    currentGravityForce = 0;
    // decrease y-velocity (decrease means upwards motion)
    velocityY -= jumpForce;
    inJumpLag = true;
}

// general input handler for player movement
/* written by David Rubal*/
void player::generalPlayerMovementControl(){
    // if not in hitstun, allow movement inputs
    if(!hitstunTimer.isActive()){
        // grounded movement
        if(grounded){
            // not in attack or endlag
            if(lagFrame <= 0 && !inAttackAnimation){
                // if not crouching or holding both left and right
                if(!Keyboard.isPressed(down) && !Keyboard.isPressed({left, right})){
                    // if not right after a dash
                    if(!inDashLag){
                        // move left
                        if(Keyboard.isPressed(left)){
                            direction = -1;
                            if(velocityX >= 0){
                                // dash if turning around or stationary
                                dash(direction);
                            }else{
                                // continue left
                                velocityX -= accelerationX;
                            }
                        }
                        // move right
                        if(Keyboard.isPressed(right)){
                            direction = 1;
                            if(velocityX <= 0){
                                dash(direction);
                            }else{
                                // continue right
                                velocityX += accelerationX;
                            }
                            
                        }
                    }else{
                        // allow for changing direction faced mid-dash
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
                inFastFall = true;
            }
            // if not in lag or in an attack
            if(lagFrame <= 0 && !inAttackAnimation){
                // use double jump when jumping in air
                if(Keyboard.isPressed(up) && !doubleJumpUsed && !inJumpLag){
                    inFastFall = false;
                    //increase gravity
                    gravity = tempGravity;
                    doubleJumpUsed = true;
                    currentGravityForce = 0;
                    velocityY =- (jumpForce-1); // double jump is slightly weaker than regular jump
                    // save position to use for double jump rings animation
                    doubleJumpX = positionX - 3;
                    doubleJumpY = positionY + hitboxHeight - 1;
                    // give a burst of speed in held direction
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
}

// resets the player to the grounded state
/* written by David Rubal*/
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
    // set position aligned with ground
    positionY = groundYLevel - hitboxHeight;
}

// move the player and alter velocity
/* written by David Rubal*/
void player::enactPlayerMovement(){
    // apply velocity, typecast to int to prevent unwanted truncating of position after change
    // ex: 100 + 2.4 = 102, but 100 - 2.4 = 97, rounding down works against us with negative vel
    positionX += static_cast<int>(velocityX);
    positionY += static_cast<int>(velocityY);

    // lower limit for speed, prevents sliding at low speed
    if(grounded && abs(velocityX) < 1){
        velocityX = 0;
    }
    // if the player is not in hitstun
    if(!hitstunTimer.isActive()){
        // decay X-velocity exponentially when not moving horizontally
        if(grounded && Keyboard.isPressed(down) || !Keyboard.areAnyPressed({left,right}) || inAttackAnimation){
            velocityX *= pow(velocityXDecay, abs(velocityX));
        }else{
            // the player is moving, do not decay speed until movement has stopped
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

    // custom-baked conditions to fit the dimensions of the platforms for collisions
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
            positionY = 180; // player has fallen inside the stage, reset position
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
}
