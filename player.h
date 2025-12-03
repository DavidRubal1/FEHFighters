class player{
    public:
        player(Key leftwards, Key rightwards, Key upwards, Key downwards, Key basicAttack,Key kickAttack, int startingX, int startingY,  int color);
        void generalPlayerMovementControl(int frameTime);
        void dash(int direction);
        void enactPlayerMovement(int frameTime);
        void getHit(float force,  float damageAmount, int angleDegrees, int attackDirection);
        void playAnimations(int frameTime);
        bool inAnimation();
        void resetIfOffscreen();
        void action(int frameTime);
        std::vector<int> getXYPosition();
        std::vector<float> getXYVelocity();
        void setXYVelocity(float x, float y);
        hitbox getHitbox();
        int lagFrame = 0;
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
        float accelerationX = 1.25;
        float dashSpeedMod = 1.75;
        float velocityXDecay = 0.92, velocityXDecayTemp = 0.92;
        float velocityXDecayDash = 0.95;
        // grounded state
        bool grounded = true;

        // airborne
        float airspeedMod = 0.75;
        bool doubleJumpUsed = false;

        // dash variables
        bool inDashLag = false;
        float dashLagTimerMax = 55;
        float dashLagTimer = 0;
        
        // animation timers 
        bool inDashAnimation = false;
        int dashAnimationTimerMax = 128; // 9 frames * 16 milliseconds
        int dashAnimationTimer = 0;
        bool inIdle = false;
        int idleTimerMax = 1024; // 2 frames * 512 milliseconds
        int idleTimer = 0;

        //jump variables
        float jumpForce = 6;
        bool inJumpLag = false;
        float jumpLagTimerMax = 55;
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
        
        // Frame timing arrays for each attack (in milliseconds per frame)
        // punch: frames 0-4 timing
        int FrameTiming[2][5] = 
        {
            {15, 15, 40, 40, 20},
            {20, 20, 50, 50, 30}
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
    kick = kickAttack;
    playerColor = color;
}

hitbox player::getHitbox(){
    return playerHitbox;
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
    forceX = force*cos(angleRadians);
    forceY = force*sin(angleRadians);
    // apply force to player velocity
    velocityX += forceX;
    velocityY -= forceY;
    
    damage += damageAmount;
}

void player::playAnimations(int frameTime){
    if(!grounded){
        inDashAnimation = false;
    }
    
    //attack animation 
    /*coded by Charlie Limbert, based on existing animation code for idling by David Rubal*/
    if(inAttackAnimation){
        char filePath[64];
        
        // Determine the correct attack sprite directory based on player color, direction, and attack type
        if(playerColor == RED)
        {
            if(direction == -1)
            {
                // Punch left or other left attacks
                if(currentAttackType == 0){
                    strcpy(filePath, "./PlayerRed/PlayerRedLeftPunch/");
                }
                // Add more attack types here:
                else if(currentAttackType == 1){
                    strcpy(filePath, "./PlayerRed/PlayerRedLeftKick/");
                }
            }
            else
            {
                //right attacks
                if(currentAttackType == 0)
                {
                    strcpy(filePath, "./PlayerRed/PlayerRedRightPunch/");
                }
                // Add more attack types here:
                else if(currentAttackType == 1)
                {
                    strcpy(filePath, "./PlayerRed/PlayerRedRightKick/");
                }
            }
        }
        
    
        
        // Calculate total animation duration by summing all frame timings
        int totalDuration = 0;
        for(int i = 0; i < attackFrameCount[currentAttackType]; i++)
        {
            totalDuration += FrameTiming[currentAttackType][i];
        }
        
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
                attackHitboxActive = FrameHasHitbox[currentAttackType][i]; //sets hitbox to active or not based on frame array.
                break;
            }
            elapsedTime += FrameTiming[currentAttackType][i];
        }
        //gets proper animation frame
        strcat(filePath, std::to_string(attackFrame).c_str()); 
        strcat(filePath, ".png");
        FEHImage punchImg;
        punchImg.Open(filePath);
        if(direction == -1)
        {
        punchImg.Draw(positionX-11, positionY);
        }
        else
        {
        punchImg.Draw(positionX, positionY);
    }
        
        // Update attack animation timer
        attackAnimationTimer += frameTime;
        if(attackAnimationTimer >= totalDuration){
            inAttackAnimation = false;
            currentAttackType = -1;
            attackAnimationTimer = 0;
            attackHitboxActive = false;
            lagFrame = 20;  // lag after attack ends
        }
    }
    // idle animation
    else if(!inAnimation()){
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
        strcat(filePath, std::to_string(idleTimer/513).c_str());
        strcat(filePath, ".png");
        idle.Open(filePath);
        idle.Draw(positionX, positionY);
    }
    // dash animation
    if(grounded && (Keyboard.areAnyPressed({left, right}) || inDashAnimation)){  //dash right
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
        strcat(filePath, std::to_string(dashAnimationTimer/frameTime).c_str());
        strcat(filePath, ".png");
        drawDash.Open(filePath);
        drawDash.Draw(positionX, positionY);
    }

    if(inDashAnimation){
        if(grounded){
            if(dashAnimationTimer < dashAnimationTimerMax){
                dashAnimationTimer += frameTime;
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
                idleTimer += frameTime;
            }else{
                idleTimer = 0;
            }
        }else{
            inIdle = false;
            idleTimer = 0;
        }
        
    }
    
}

bool player::inAnimation(){
    return inDashAnimation || inAttackAnimation;
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

void player::action(int frameTime){
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
                // normal acceleration
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
        dashLagTimer += frameTime;
        if(dashLagTimer >= dashLagTimerMax){
            inDashLag = false;
            dashLagTimer = 0;
            velocityXDecay = velocityXDecayTemp;
        }
    }
    // jump lag timer, prevents double jump from instantly being used
    if(inJumpLag){
        jumpLagTimer += frameTime;
        if(jumpLagTimer >= jumpLagTimerMax  && !Keyboard.isPressed(up)){
            inJumpLag = false;
            jumpLagTimer = 0;
        }
    }


}


void player::enactPlayerMovement(int frameTime){
    // remove previous frame
    LCD.SetFontColor(BLACK);
    LCD.FillRectangle(positionX-5, positionY, hitboxLength+10, hitboxHeight);

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