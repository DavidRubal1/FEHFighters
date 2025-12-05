class attack {

    public:
        attack(int attackType,int hitHeight, int hitLength);
        hitbox getHitbox();
        void updateAttackPosition(int posX, int posY, int dir);
        void updateAttackHitbox();
        void updateHitboxState(bool state);
        bool checkCollision(hitbox otherHitbox);

        int getAttackType();

        int getDirection();
        float getDamage();
        float getKnockback();
        float getAngle();

        
        bool isActive();
        void deactivate();
        
    private:
        // attack properties
        int attackType;  // 0 = punch, 1 = kick, 2 = projectile.
        int direction;   // -1 = left, 1 = right
        int positionX;
        int positionY;

        float damage;
        float knockback;
        float angle;

        float velocityX;
        float velocityY;
        
        // hitbox dimensions
        int hitboxHeight;
        int hitboxLength;
        
        // attack state
        bool active = false;
        
        // hitbox for attack
        hitbox attackHitbox;
};

// Constructor
attack::attack(int attackType, int hitHeight, int hitLength)
    : attackHitbox(hitHeight, hitLength) {
    this->attackType = attackType;
    this->hitboxHeight = hitHeight;
    this->hitboxLength = hitLength;
    switch(attackType){
        case 0:
        damage = 6.0;
        knockback = 3.0;
        angle = 1.57;
        break;
        case 1:
        damage = 10.0;
        knockback = 6.0;
        angle = 0.5;
        break;
        case 2:
        damage = 3.0;
        knockback = 1.0;
        angle = 1;
        break;
    }
}

int attack::getDirection(){
    return direction;
}

float attack::getDamage(){
    return damage;
}

float attack::getKnockback(){
    return knockback;
}

float attack::getAngle(){
    return angle;
}


// Get the attack's hitbox
hitbox attack::getHitbox(){
    return attackHitbox;
}

// Update the position of the attack
void attack::updateAttackPosition(int posX, int posY, int dir){

    this->positionX = posX;
    this->positionY = posY;
    this->direction = dir;
    updateAttackHitbox();
}


void attack::updateHitboxState(bool state){
    active = state;
}

bool attack::isActive(){
    return active;
}

// Update the hitbox based on attack type and direction
void attack::updateAttackHitbox(){
    if(attackType == 0){  // punch
        if(direction == -1){
            // punch extends to the left
            attackHitbox.updateHitbox(positionX - hitboxLength, positionY);
            attackHitbox.debugDrawHitbox(RED);
        } else {
            // punch extends to the right
            attackHitbox.updateHitbox(positionX + hitboxLength, positionY);
            attackHitbox.debugDrawHitbox(WHITE);
        }
    }
    // Add other attack types here as needed
}

// Check if this attack collides with another hitbox
bool attack::checkCollision(hitbox otherHitbox){
    return attackHitbox.rectangleIntersects(otherHitbox);

}



