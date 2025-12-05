class attack {

    public:
        attack(int attackType, int attackDirection, int posX, int posY);
        hitbox getHitbox();
        void updateAttackPosition(int posX, int posY);
        void updateAttackHitbox();
        bool checkCollision(hitbox otherHitbox);
        
    private:
        // attack properties
        int attackType;  // 0 = punch, 1 = kick, etc.
        int direction;   // -1 = left, 1 = right
        int positionX;
        int positionY;
        
        // hitbox dimensions
        int hitboxHeight;
        int hitboxLength;
        
        // attack state
        bool active;
        
        // hitbox for attack
        hitbox attackHitbox;
};

// Constructor
attack::attack(int attackType, int attackDirection, int posX, int posY)
    : attackHitbox(10, 10, posX, posY, true) {
    this->attackType = attackType;
    this->direction = attackDirection;
    this->positionX = posX;
    this->positionY = posY;
    this->hitboxHeight = 10;
    this->hitboxLength = 10;
    this->active = true;
}

// Get the attack's hitbox
hitbox attack::getHitbox(){
    return attackHitbox;
}

// Update the position of the attack
void attack::updateAttackPosition(int posX, int posY){
    this->positionX = posX;
    this->positionY = posY;
    updateAttackHitbox();
}

// Update the hitbox based on attack type and direction
void attack::updateAttackHitbox(){
    if(attackType == 0){  // punch
        if(direction == -1){
            // punch extends to the left
            attackHitbox.updateHitbox(positionX - hitboxLength, positionY);
        } else {
            // punch extends to the right
            attackHitbox.updateHitbox(positionX + hitboxLength, positionY);
        }
    }
    // Add other attack types here as needed
}

// Check if this attack collides with another hitbox
bool attack::checkCollision(hitbox otherHitbox){
    return attackHitbox.rectangleIntersects(otherHitbox);
}


