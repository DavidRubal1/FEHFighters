
/*class written by Charlie Limbert and David Rubal*/
class attack {
    public:
        attack(action type, int hitHeight, int hitLength, int offsetX, int offsetY);
        hitbox getHitbox();
        void updateAttackPosition(int posX, int posY, int dir);
        void updateAttackHitbox();
        void consumeHitbox();
        // void updateActiveState(bool state);
        bool checkCollision(hitbox otherHitbox);

        action getAttackType();
        int getDirection();
        std::vector<int>* getPosition();
   
        bool isActive();
        attackProperties getProperties();
        int getCurrentFrame();
        void setCurrentFrame(int i);
        
    private:
        // attack properties
        action type; 
        int direction;   // -1 = left, 1 = right
        std::vector<int> position = {0, 0};
        int offX, offY; // given offsets for attack positions
        // TODO: integrate this into the posx and posy sent initially?
        int playerHitboxLength = 14; // used to offset attack position

        // hitbox dimensions
        int hitboxHeight;
        int hitboxLength;

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

std::vector<int>* attack::getPosition(){
    return &position;
}

int attack::getCurrentFrame(){
    return currentAnimationFrame;
}

void attack::setCurrentFrame(int i){
    hitboxConsumed = false;
    currentAnimationFrame = i;
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
void attack::updateAttackPosition(int posX, int posY, int dir){
    // moves the attack's position to be an certain distance away from the player
    if(dir == 1){
        position[0] = posX + playerHitboxLength - offX;
    }else{
        position[0] = posX - hitboxLength + offX;
    }
    // apply y-offset
    position[1] = posY + offY;
    this->direction = dir;
    updateAttackHitbox();
}


// returns whether this attack is active
bool attack::isActive(){
    // TODO:  This causes attacks that are active on frame 0 to always be active since currentAnimationFrame is 0 be default
    return !hitboxConsumed && properties.activeFrames[currentAnimationFrame];
}

// Update the hitbox based on attack type and direction
/*coded by Charlie Limbert*/
void attack::updateAttackHitbox(){

        attackHitbox.updateHitbox(position[0], position[1]);
       
        // debug hitbox display 
        // if(isActive()){
        //     attackHitbox.debugDrawHitbox(RED);
        // }else{
        //     attackHitbox.debugDrawHitbox(WHITE);
        // }
        
}

// Check if this attack collides with another hitbox, used for testing player collison with attacks
bool attack::checkCollision(hitbox otherHitbox){
    return attackHitbox.rectangleIntersects(otherHitbox);
}
