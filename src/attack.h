
/*class written by Charlie Limbert and David Rubal*/
class attack {
    public:
        attack(action type, int hitHeight, int hitLength, int offsetX, int offsetY);
        hitbox getHitbox();
        void updateAttackPosition(int posX, int posY, int dir, bool attackHitboxActive);
        void updateAttackHitbox(bool attackHitboxActive);
        void consumeHitbox();
        // void updateActiveState(bool state);
        bool checkCollision(hitbox otherHitbox);


        action getAttackType();
        int getDirection();
   
        bool isActive();
        attackProperties getProperties();
        int getCurrentFrame();
        void incrementFrame();
        void resetFrameCounters();
        
    private:
        // attack properties
        action type; 
        int direction;   // -1 = left, 1 = right
        int positionX;
        int positionY;
        int offX, offY; // given offsets for attack positions
        // TODO: integrate this into the posx and posy sent initially?
        int playerHitboxLength = 14; // used to offset attack position

        // hitbox dimensions
        int hitboxHeight;
        int hitboxLength;
        
        // whether attack is being used or not
        // Attack should know its current frame being played 
        // bool active = false;

        attackProperties properties;

        int currentAnimationFrame = 0;
        
        // hitbox for attack
        hitbox attackHitbox;
        bool hitboxConsumed = false;
};

// Constructor
/* written by Charlie Limbert and David Rubal*/
attack::attack(action type, int hitHeight, int hitLength, int offsetX, int offsetY)
    : attackHitbox(hitHeight, hitLength){
    this->type = type;
    this->hitboxHeight = hitHeight;
    this->hitboxLength = hitLength;
    this->offX = offsetX;
    this->offY = offsetY;

    this->properties = attackPropertiesLookup(type);    
}

// returns the attack's type (0 = punch, 1 = kick, 2 = cast)
action attack::getAttackType(){
    return type;
}
// gets the direction of the attack (used for kb calculation)
//TODO: change this to be the player's facing direction???
int attack::getDirection(){
    return direction;
}

attackProperties attack::getProperties(){
    return properties;
}

int attack::getCurrentFrame(){
    return currentAnimationFrame;
}

void attack::incrementFrame(){
    hitboxConsumed = false;
    currentAnimationFrame++;
}

void attack::resetFrameCounters(){
    currentAnimationFrame = 0;
}

// returns a copy of the attack's hitbox
hitbox attack::getHitbox(){
    return attackHitbox;
}

void attack::consumeHitbox(){
    hitboxConsumed = true;
}


// Update the position of the attack
/*coded by Charlie Limbert*/
void attack::updateAttackPosition(int posX, int posY, int dir, bool attackHitboxActive){
    // moves the attack's position to be an certain distance away from the player
    if(dir == 1){
        this->positionX = posX + playerHitboxLength - offX;
    }else{
        this->positionX = posX - hitboxLength + offX;
    }
    // apply y-offset
    this->positionY = posY + offY;
    this->direction = dir;
    updateAttackHitbox(attackHitboxActive);
}



// // changes the active state of the attack to the parameter's state
// void attack::updateActiveState(bool state){
//     active = state;
// }

// returns whether this attack is active
bool attack::isActive(){
    return !hitboxConsumed && properties.activeFrames[currentAnimationFrame];
}

// Update the hitbox based on attack type and direction
/*coded by Charlie Limbert*/
void attack::updateAttackHitbox(bool attackHitboxActive){


    //TODO: Why is this like it is?
        if(direction == -1){
            // attack extends to the left
            attackHitbox.updateHitbox(positionX, positionY);
        } else {
            // attack extends to the right
            attackHitbox.updateHitbox(positionX, positionY);
        }
        // debug code for viewing active attack hitboxes
        // intentionally left commented for future debugging
        if(attackHitboxActive){
            if(isActive()){
                attackHitbox.debugDrawHitbox(RED);
            }else{
                attackHitbox.debugDrawHitbox(WHITE);
            }
        }
}

// Check if this attack collides with another hitbox, used for testing player collison with attacks
bool attack::checkCollision(hitbox otherHitbox){
    return attackHitbox.rectangleIntersects(otherHitbox);
}
