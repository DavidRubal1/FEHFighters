class player{
    public:
        player(bool AI, Key left, Key right, Key up, Key down, Key basicAttack,Key kickAttack, Key projectileAttack, int startingX, int startingY,  int color);
        void generalPlayerMovementControl();
        void dash(int direction);
        void jump();
        void enactPlayerMovement();
        void manageHitboxes(player *otherPlayer);
        void getHit(attack* activeAttack);
        void groundPlayer(int groundYLevel);
        void updateTimers();
        timer getIntangibilityTimer();
        void playAnimations();
        void resetIfOffscreen();
        void checkAttackHits(player *otherPlayer, attack *activeAttack);
        void determineAction();
        hitbox getHitbox();
        float getDamage();
        action getCurrentAttack();
        int remainingLives = 3;
        bool gameOver = false;
        std::vector<int> getXYPosition();
        void determineAIDecisions(player *humanPlayer);


    private:
        // player attributes
        /*written by Charlie Limbert and David Rubal*/

        //flag for if the player is controlled by an AI
        bool isAI = false;
        // identifiers for the choices the AI makes
        // Horizonal direction (left/right) that the AI wants to move in.
        // -2 indicates that the current player object is not an AI and should fail all checks
        // -1 indicates  that the AI does not want to use that option
        // 0 indicates left and 1 indicates right
        int AIHorizontalDirection = -2;
        // vertical direction (jump/crouch) that the AI wants to move in
        // 0 = crouch, 1 = jump
        int AIVerticalDirection = -2;
        // Attack choice that the AI makes.
        // 0 = punch, 1 = kick, 2 = projectile cast
        int AIAttack = -2;
        timer AIReactionTimer;
        // target coordinates detail the position that the AI is trying to move towards
        int targetX, targetY;
        // ranges that the AI tries to stay out of around the player when they are attacking
        int safeRangeX = 15;
        int safeRangeY = 30;
        // player distance at which the AI will start casting projectiles
        int projectileRange = 50;


        // player position coordinates
        int startingPosX, startingPosY;
        int positionX, positionY;
        // player hitbox (and sprite) size
        int hitboxHeight = 20, hitboxLength = 14;

        // player color
        int playerColor;

        // hitbox for player, handles ground collisions and for overlap with enemy attacks
        hitbox playerHitbox;

        //attack objects. these hold info for damage, knockback, etc. and have associated hitboxes
        attack punch;
        attack kickAttack;
        attack projectileCast;
        attack *currentAttack = nullptr;
        projectile proj;
        // TODO: move these attacks to Moveset class, throw projectile spawner in there as well 
        // What do I do with you...
        //attack projectileProjectile;

        // player velocity in each axis and direction (-1 == left, 1 == right)
        float velocityX = 0, velocityY = 0;
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
        float velocityXDecayDash = 0.92;

        // grounded state, is true when the player is standing on the ground or the platform
        bool grounded = true;

        // airborne info
        // x-velocity multiplier when the player is in the air
        float airspeedMod = 0.6;
        // flag to keep track of whether the player has used double jump
        bool doubleJumpUsed = false;

        timer dashLag;
        bool jumpLag = false;
        timer hitstunTimer; // hitstun prevents the player from acting after getting hit
        timer respawnIntangibleTimer; // makes the player invincible for a short period after dying
         
        // animation player to draw the player's sprites for any given animation
        animator playerAnimator;
        
        action currentAnimationType;

        // separate animator and animation type for double jump to allow for another animation to play during double jump
        animator doubleJumpAnimator;


        // position of double jump
        int doubleJumpX, doubleJumpY;


        // change in y-velocity when jumping
        float jumpForce = 6;

        // normal gravity
        float gravity = 0.24, tempGravity = 0.24;
        // downwards force applied each frame
        float currentGravityForce = 0, groundedGravityForce = 0.3;
        // terminal velocity
        float maxGravityForce = 1.5;

        // gravity during fast fall
        float fastFallGravity = 0.20;
        // flag to tell if the player is in "fast fall", which increases their gravity
        bool inFastFall = false;


        // controls
        //movement keys followed by attack keys
        Key left, right, up, down, basic, kick, projectile;
        // damage value of player, increased damage means increased knockback taken
        float damage = 0;         
    
};

// constructor for player. Assigns each movement and attack key, starting position, player color,
// constructs player hitbox object, constructs the four attack objects with ID, size and position offset,
// and constructs the animator objects that will play the player's animations
/* written by David Rubal*/
player::player(bool AI, Key left, Key right, Key up, Key down, Key basicAttack, Key kickAttack, Key projectileAttack, int startingX, int startingY, int color) 
    : playerHitbox(hitboxHeight, hitboxLength, positionX, positionY), 
        punch(BASIC, 15, 10, 5, 4), 
        kickAttack(KICK, 10, 12, 3, 4), 
        projectileCast(CAST, 10, 5, 3, 6), 
        proj(PROJECTILE, 9, 8, -5, 8, 3, color),
        //projectileProjectile(PROJECTILE, 9, 8, -5, 8, 2.5),
        playerAnimator(color), doubleJumpAnimator(color),
        dashLag(3), respawnIntangibleTimer(30){
    this->left = left;
    this->right = right;
    this->up = up;
    this->down = down;
    startingPosX = startingX;
    startingPosY = startingY;
    positionX = startingX;
    positionY = startingY;
    basic = basicAttack;
    kick = kickAttack;
    projectile = projectileAttack;
    playerColor = color;
    isAI = AI;
    if(AI){
        AIReactionTimer.setMax(11);
    }
    color == BLUE ? direction = -1: direction = 1;


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

// returns a 2d vector of the player's current xy position
/* written by David Rubal*/
 std::vector<int> player::getXYPosition(){
    return {positionX, positionY};
 }


// enact knockback and hitstun on player when hit, and increase damage counter
/* written by David Rubal*/
void player::getHit(attack* activeAttack){
    float forceX, forceY;
    attackProperties properties = activeAttack->getProperties();
    // scale force based on current damage and given knockback
    float force = (((0.1 * (damage / 100)))* 50 * properties.KBscaling) + properties.knockback;
    // reset timing varibles before entering hitstun, or move those timers into a separate function
    hitstunTimer.activate();
    // set hitstun time based on scaling
    hitstunTimer.setMax(properties.hitstunFramesBase  + properties.hitstunScaling  * damage);
    // calculate the direction of knockback into x and y components
    // angle degrees ranges from -pi to pi, with -pi as directly down and pi is straight up
    forceX = (*activeAttack).getDirection() * force*cos(properties.angle);
    forceY = force*sin(properties.angle);
    // apply force to player's velocity components
    velocityX = forceX;
    velocityY = -1 * forceY;
    currentGravityForce = 0;
    // increase player's damage
    damage += properties.damage;
}

// move and draw projectile
// ******** MOVE OUT OF PLAYER **********
/* written by David Rubal*/

// increments timers and updates the player state accordingly
/*written by Charlie Limbert and David Rubal*/
void player::updateTimers(){
    // dash lag timer, keeps track of when the player is in a dash
    if(dashLag.isActive()){
        dashLag.increment();
        if(!dashLag.isActive()){
            dashLag.reset();
            dashLag.stop();
            velocityXDecay = velocityXDecayTemp;
        }
    } 

    //TODO: move this out of timers into another method
    // jump lag timer, prevents double jumping with same input as jump
    jumpLag = jumpLag && Keyboard.isPressed(up);

    // hitstun timer, prevents the player from acting after being attacked
    if(hitstunTimer.isActive()){
        hitstunTimer.increment();
        if( velocityY > 2.75 || !hitstunTimer.isActive()) {
            hitstunTimer.reset();
            hitstunTimer.stop();
        }
    }

    // respawn intangibility timer, provided invincibility after respawn
    if(respawnIntangibleTimer.isActive()){
        respawnIntangibleTimer.increment();
        if(!respawnIntangibleTimer.isActive()){
            respawnIntangibleTimer.reset();
            respawnIntangibleTimer.stop();
        }
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
    //Default animation
    currentAnimationType = IDLE;
    // if not in hitstun
    if(!hitstunTimer.isActive() && currentAttack == nullptr && grounded){
        // if not attacking
        // if not in attack lag or on ground
        // if holding left or right and not crouch but not both left and right
        if((Keyboard.areAnyPressed({left, right}) & !Keyboard.isPressed(down) && (!Keyboard.isPressed({left, right})))
            || AIHorizontalDirection > -1){ 
            
            currentAnimationType = DASH;
        } else if(Keyboard.isPressed(down) || AIVerticalDirection == 0){
            
            currentAnimationType = CROUCH;
        }
    }

    animationProperties currentAnimationProperties = animationPropertiesLookup(currentAnimationType);

    // play double jump animation
    if(doubleJumpUsed){
        doubleJumpAnimator.playAnimation(animationPropertiesLookup(DOUBLE_JUMP), doubleJumpX, doubleJumpY);
    }else{
        // resets the animator when player gets double jump back
        doubleJumpAnimator.resetTimers();
    }
    
    int offsetX = positionX;
    
    //attack animation 
    /*coded by Charlie Limbert, based on existing animation code for idling by David Rubal*/
    /* Updated by David Rubal */
    if(currentAttack != nullptr && !hitstunTimer.isActive()){

        if(!playerAnimator.isAnimationOver() || currentAttack->getAttackType() != playerAnimator.getAnimationType()){
            currentAnimationType = currentAttack->getAttackType();
            currentAnimationProperties = animationPropertiesLookup(currentAnimationType);  
            if(currentAttack->getCurrentFrame() != playerAnimator.getAnimationTime()){
                
                currentAttack->setCurrentFrame(playerAnimator.getAnimationTime());
            }
            
            //TODO fix offset weirdness across the board
            // offsets the attack by a certain amount to align the animation with the player's hitbox
            if(direction == -1){
            offsetX -= 10;
            } else{
                offsetX -= 1;
            }
        } else{
            // End attack, go with previously declared animation
            currentAttack->setCurrentFrame(0);
            currentAttack = nullptr;
        }

        //TOOD: move this out of the animation function
        // spawn projectile at player at the last frame of cast animation
        if(currentAttack != nullptr){
            if(currentAttack->getAttackType() == CAST 
            && playerAnimator.getAnimationTime() ==  currentAttack->getProperties().frameData.size() - 1 
            && playerAnimator.getHoldTime() == currentAttack->getProperties().frameData[currentAttack->getProperties().frameData.size() - 1] - 1){
                proj.setCurrentFrame(0);
                proj.updateAttackPosition(positionX, positionY, direction);
            }
        
        }
        
    }
    playerAnimator.playAnimation(currentAnimationProperties, offsetX, positionY, direction);
}


//updates position of attack hitboxes and checks for overlap with other player
/* written by David Rubal*/
void player::manageHitboxes(player *otherPlayer){
    if(currentAttack != nullptr){
        currentAttack->updateAttackPosition(positionX, positionY, direction);
        if(currentAttack->isActive()){
            checkAttackHits(otherPlayer, currentAttack);
        }
    }
    // checks for projectile overlap with other player, separate because projectiles have separate movement
    if(proj.isActive()){
        // change projectile position by the projectile's velocity
        proj.updateProjectilePosition();
        // draw projectile
        proj.playProjectileAnimation();
        checkAttackHits(otherPlayer, &proj);
        
    }
}

// check if the current attack overlaps with the other player and hit if true
/* written by David Rubal*/
void player::checkAttackHits(player *otherPlayer, attack *atk){
    // if the other player is not intangible, the current attack is active, and the attack collides with the other player
        if(!otherPlayer->getIntangibilityTimer().isActive() && atk->checkCollision(otherPlayer->getHitbox())){
            // the other player takes the hit
            otherPlayer->getHit(atk);
            // disable attack to prevent attack from hitting multiple times in the following active frames
            atk->consumeHitbox();
        }
}

// the player is sent offscreen and a life is lost
/*coded by Charlie Limbert and David Rubal*/
void player::resetIfOffscreen(){
    // if player position is off-screen
    if(positionX < 0 - hitboxLength || positionX > 319 || positionY > 239 || positionY < 0 - hitboxHeight){
        // give intangibility towards incoming attacks when respawned
        // resets position, velocity, and damage
        positionX = startingPosX;
        positionY = startingPosY -10; // player starts slightly above starting position
        velocityX = 0;
        velocityY = 0;
        damage = 0;
        remainingLives--; 
        jumpLag = true; // jump lag to prevent instant double jump after respawning
        respawnIntangibleTimer.activate();

        // checks for game over when a player has run out of lives
        if (remainingLives == 0)
        {

            //TODO; make this a hoisted function that tells main to stop end the game
            gameOver = true;
        }
    }
}

// give the player a dash of speed in the direction
/* written by David Rubal*/
void player::dash(int direction){
    // increase x-velocity
    velocityX = direction * runSpeedMax * 1.25;
    dashLag.activate();
    // decrease velocity decay for sliding
    velocityXDecay = velocityXDecayDash;
}

// get input for attacks and activate the respective attack 
// TODO: Combine this with movement action function?
/*coded by Charlie Limbert*/
void player::determineAction(){

    //TODO: lift this out of here into another function to cover more at once
    if(hitstunTimer.isActive())return;

    if(currentAttack == nullptr) {
        if (Keyboard.isPressed(basic) || AIAttack == 0) {
            currentAttack = &punch;
        }
        else if (Keyboard.isPressed(kick) || AIAttack == 1) {
            currentAttack = &kickAttack;
        }
        else if (Keyboard.isPressed(projectile) || AIAttack == 2) {
            currentAttack = &projectileCast;
        }
        
    }
   
    
}

// Returns the current attack action type if the player is attacking. If not, returns the IDLE action
action player::getCurrentAttack(){
    if(currentAttack == nullptr){
        return IDLE;
    } else {
        return currentAttack->getAttackType();
    } 
}

// the player jumps upwards
/* written by David Rubal*/
void player::jump(){
    // reset downwards force
    currentGravityForce = 0;
    // decrease y-velocity (upwards motion)
    velocityY -= jumpForce;
    jumpLag= true;
}

// general input handler for player movement
//TODO: make this make sense
/* written by David Rubal*/
void player::generalPlayerMovementControl(){
    // if in hitstun, no movement allowed
    if(hitstunTimer.isActive()) return;
    
    // grounded movement
    if(grounded){
        // not in attack or endlag
        if(currentAttack == nullptr){
            // if not crouching or holding both left and right
            if(!Keyboard.isPressed(down) && !Keyboard.isPressed({left, right})
                || (AIVerticalDirection == -1 || AIVerticalDirection == 1)){
                // if not right after a dash
                if(!dashLag.isActive()){
                    // move left
                    if(Keyboard.isPressed(left) || AIHorizontalDirection == 0){
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
                    if(Keyboard.isPressed(right) || AIHorizontalDirection == 1){
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
                    if(Keyboard.isPressed(right) || AIHorizontalDirection == 1){
                        direction = 1;
                    }else if(Keyboard.isPressed(left) || AIHorizontalDirection == 0){
                        direction = -1;
                    }
                }
            }
            //jump when on ground
            if(Keyboard.isPressed(up) || AIVerticalDirection == 1){
                jump();
            }
        }
    }else{ 
        // airborne movement (grouded == false)
        if(!Keyboard.isPressed({left, right})){
            if(Keyboard.isPressed(left) || AIHorizontalDirection == 0){
                velocityX -= accelerationX * airspeedMod;
            }
            if(Keyboard.isPressed(right) || AIHorizontalDirection == 1){
                velocityX += accelerationX * airspeedMod;
            }
        }
        // fast fall when down is pressed
        if((Keyboard.isPressed(down) || AIVerticalDirection == 0) && !jumpLag){
            // increase gravity for fast fall
            gravity = fastFallGravity;
            inFastFall = true;
        }
        // if not in lag or in an attack
        if(currentAttack == nullptr){
            // use double jump when jumping in air
            if((Keyboard.isPressed(up) || AIVerticalDirection == 1) && !doubleJumpUsed && !jumpLag){
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
                    //TODO: Rework double jump momentum
                    if(Keyboard.isPressed(left) || AIHorizontalDirection == 0){
                        direction = -1;
                        velocityX = 2.0 * direction;
                    }
                    if(Keyboard.isPressed(right) || AIHorizontalDirection == 1){
                        direction = 1;
                        velocityX = 2.0 * direction;  
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
        if(grounded && (Keyboard.isPressed(down) || AIVerticalDirection == 0) || ( !isAI && !Keyboard.areAnyPressed({left,right}) || AIHorizontalDirection == -1)  || currentAttack != nullptr){
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
    if((positionX + hitboxLength>= 106 && positionX <= 213) && velocityY >= 0 && (positionY + hitboxHeight >= 140 && positionY + hitboxHeight <= 152)  && ( (!isAI && !Keyboard.isPressed(down)) || (isAI && AIVerticalDirection != 0))){
        // Player has landed on the upper platform
        // reset grounded state
        groundPlayer(140);
    }
    
    // update hitbox position to follow player position
    playerHitbox.updateHitbox(positionX, positionY);
}

// determines the actions of the ai player for the current frame given the human player's position
/* written by David Rubal */
void player::determineAIDecisions(player *humanPlayer){
    // stores both x and y coordinates of the human player in a vector
    std::vector<int> p1Position = humanPlayer->getXYPosition();
    // resets each decision each frame, -1 means no action
    AIHorizontalDirection = -1;
    AIVerticalDirection = -1;
    AIAttack = -1;
    // random value to base some decisions off of
    int randomness = rand() % 30;
    // store both x and y variables into two ints
    int playerX = p1Position.at(0);
    int playerY = p1Position.at(1);
    // determine the distance from the AI's current position with the player's current position
    int distanceToPlayerX = positionX - playerX;
    int distanceToPlayerY = positionY - playerY;
    // once every 12 frames, set the targeted position to the player's position
    if(AIReactionTimer.getCurrentTime() == 0){
        targetX = playerX;
        targetY = playerY;
    }
    // if the human player is attacking, see if the AI can react and move away out of the player's range
    action humanAction = humanPlayer->getCurrentAttack();
    if(humanAction != IDLE && !AIReactionTimer.isActive()){
        if(humanAction == BASIC || humanAction == KICK){
            if(distanceToPlayerX > 0){
                targetX = targetX + safeRangeX;
            }else{
                targetX = targetX - safeRangeX;
            }
        }
        // if the player is casting a projectile, jump to avoid it
        if(grounded && humanAction == CAST){
            // avoid the y-level that the projectile was fired from
            if(playerY > 150){
                targetY = 130;
            }
        }
    }
    
    // if the targeted position is up at the platform, go towards the center of the platform
    if(targetY == 140){
        targetX = 160;
    }
    // move towards the player until the AI has reached the safe range surrounding the player
    if(positionX > targetX + safeRangeX){
        AIHorizontalDirection = 0;
    }else if(positionX <= targetX - safeRangeX){
         AIHorizontalDirection = 1;
    }else{
        // if the AI is within the player's range, keep moving in the current direction
        if(direction == -1){
            AIHorizontalDirection = 0;
        }else{
            AIHorizontalDirection = direction;
        }
    }
    // if the player is above the AI by a substantial amount (determined by safeRangeY), jump to meet them
    // otherwise, if the player is below the AI, crouch/fastfall to meet them
    if(positionY < targetY - safeRangeY && (positionX > targetX - safeRangeX || positionX < targetX + safeRangeX)){
        AIVerticalDirection = 0;
    }else if(!jumpLag && positionY - safeRangeY > targetY && (positionX > targetX - safeRangeX || positionX < targetX + safeRangeX)){
        AIVerticalDirection = 1;
    }
    // if the AI is within range of the player, attack
    if(abs(distanceToPlayerX) < safeRangeX ){
        // perform a punch or a kick (less likely) if the player has less than 50 damage
        // if they have more than 50 damage, kick only
        if(humanPlayer->getDamage() < 50 && randomness < 25){
            AIAttack = 0; //punch
        }else{
            AIAttack = 1; // kick
        }
        // if the player is outside of the range determined by Projectile range, have a chance to cast a projectile
    }else if(abs(distanceToPlayerX) > projectileRange && distanceToPlayerY < safeRangeY && randomness == 1){
        AIAttack = 2; //cast
    }

    // survival instincts come last to override any other choices
    // these only happen if the AI is offstage
    if(positionX < 50){
        AIHorizontalDirection = 1;
        AIAttack = -1;
    }
    else if(positionX > 263){
        AIHorizontalDirection = 0;
        AIAttack = -1;
    }
    if(positionY > 180){
        AIVerticalDirection = 1;
        AIAttack = -1;
    }
    //increment the reaction time
    AIReactionTimer.increment();
    if(!AIReactionTimer.isActive()){
        AIReactionTimer.reset();
    }
    
}