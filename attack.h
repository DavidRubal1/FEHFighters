class attack {

    public:
        attack(int attackType,int hitHeight, int hitLength, int offsetX, int offsetY);
        attack(int attackType, int hitHeight, int hitLength, int offsetX, int offsetY, float velX);
        hitbox getHitbox();
        void updateAttackPosition(int posX, int posY, int dir, bool attackHitboxActive);
        void updateAttackHitbox(bool attackHitboxActive);
        void updateActiveState(bool state);
        bool checkCollision(hitbox otherHitbox);
        void moveProjectile(float velX);
        float getXVelocity();

        int getAttackType();

        int getDirection();
        float getDamage();
        float getKnockback();
        float getAngle();
        float getKBScaling();
        int getHitstun();
        float getHitstunScaling();
        void playProjectileAnimation(int color);

        
        bool isActive();
        
    private:
        // attack properties
        int attackType;  // 0 = punch, 1 = kick, 2 = projectile.
        int direction;   // -1 = left, 1 = right
        int positionX;
        int positionY;
        int offX, offY;
        int magicCorrectingFactor = 14;

        float damage;
        float knockback;
        float angle;
        float KBscaling;
        int hitstunFramesBase;
        float hitstunScaling;

        float velocityX;
        float velocityY;
        
        // hitbox dimensions
        int hitboxHeight;
        int hitboxLength;
        
        // attack state
        bool active = false;
        
        // hitbox for attack
        hitbox attackHitbox;
        animationType projectile = {"/Projectile/", 1, true, 5, 1};
};

// Constructor
attack::attack(int attackType, int hitHeight, int hitLength, int offsetX, int offsetY)
    : attackHitbox(hitHeight, hitLength){
    this->attackType = attackType;
    this->hitboxHeight = hitHeight;
    this->hitboxLength = hitLength;
    this->offX = offsetX;
    this->offY = offsetY;
    switch(attackType){
        case 0: // punch
        damage = 5.0;
        knockback = 5.0;
        angle = .82;
        KBscaling = 1.0;
        hitstunFramesBase = 12;
        hitstunScaling = 0.2;
        break;
        case 1: // kick
        damage = 8.5;
        knockback = 3;
        angle = 0.4;
        KBscaling = 2.5;
        hitstunFramesBase = 5;
        hitstunScaling = 0.2;
        break; // projectile cast
        case 2:
        damage = 3.0;
        knockback = 4.0;
        angle = 0.6;
        KBscaling = 1.5;
        hitstunFramesBase = 5;
        hitstunScaling = 0.1;
        break;
    }
}

// projectile constructor
attack::attack(int attackType, int hitHeight, int hitLength, int offsetX, int offsetY, float velX)
    : attackHitbox(hitHeight, hitLength){
    this->attackType = attackType;
    this->hitboxHeight = hitHeight;
    this->hitboxLength = hitLength;
    this->offX = offsetX;
    this->offY = offsetY;
    this->velocityX = velX;
    damage = 6.5;
    knockback = 3.0;
    angle = 0.9;
    KBscaling = 2.7;
    hitstunFramesBase = 13;
    hitstunScaling = 0.1;

    strcpy(projectile.fileName, "/Projectile/");
    projectile.looping = true;
        
}

void attack::playProjectileAnimation(int color){
    animation projectileAnimator(color);
    projectileAnimator.playAnimation(projectile.fileName, positionX, positionY, direction, projectile.finalFrameNum, projectile.frameLength, projectile.looping, projectile.ID);
}

int attack::getAttackType(){
    return attackType;
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
float attack::getKBScaling(){
    return KBscaling;
}
int attack::getHitstun(){
    return hitstunFramesBase;
}
float attack::getHitstunScaling(){
    return hitstunScaling;
}

// Get the attack's hitbox
hitbox attack::getHitbox(){
    return attackHitbox;
}

float attack::getXVelocity(){
    return velocityX;
}

// Update the position of the attack
void attack::updateAttackPosition(int posX, int posY, int dir, bool attackHitboxActive){
    printf("PosX Start: %d\n", posX);
    if(dir == 1){
        this->positionX = posX + magicCorrectingFactor - offX;
    }else{
        this->positionX = posX - hitboxLength + offX;
    }
    this->positionY = posY + offY;
    this->direction = dir;
    updateAttackHitbox(attackHitboxActive);
}

void attack::moveProjectile(float velX){
    if(positionX > 0 && positionX < 320){
        positionX += velX * direction;
        updateAttackHitbox(true);
    }else{
        active = false;
    }
}

void attack::updateActiveState(bool state){
    active = state;
}

bool attack::isActive(){
    return active;
}


// Update the hitbox based on attack type and direction
void attack::updateAttackHitbox(bool attackHitboxActive){

        if(direction == -1){
            // punch extends to the left
            attackHitbox.updateHitbox(positionX, positionY);
        } else {
            // punch extends to the right
            attackHitbox.updateHitbox(positionX, positionY);
        }
        if(attackHitboxActive){
            if(active){
                attackHitbox.debugDrawHitbox(RED);
            }else{
                attackHitbox.debugDrawHitbox(WHITE);
            }
        }
    // Add other attack types here as needed
}

// Check if this attack collides with another hitbox
bool attack::checkCollision(hitbox otherHitbox){
    return attackHitbox.rectangleIntersects(otherHitbox);

}
