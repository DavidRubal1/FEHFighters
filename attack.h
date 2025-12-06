/*class written by Charlie Limbert and David Rubal*/
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
        int playerHitboxLength = 14;
        // attack properties
        float damage;
        float knockback;
        float angle;
        float KBscaling;
        int hitstunFramesBase;
        float hitstunScaling;
        // projectile speed variables
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
/* written by Charlie Limbert and David Rubal*/
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
// "getters" written by David Rubal

// plays the animation of the projectile given the player color
void attack::playProjectileAnimation(int color){
    animation projectileAnimator(color);
    projectileAnimator.playAnimation(projectile.fileName, positionX, positionY, direction, projectile.finalFrameNum, projectile.frameLength, projectile.looping, projectile.ID);
}

// returns the attack's type (0 = punch, 1 = kick, 2 = cast)
int attack::getAttackType(){
    return attackType;
}
// gets the direction of the attack (used for kb calculation)
int attack::getDirection(){
    return direction;
}

// gets the damage of the attack
float attack::getDamage(){
    return damage;
}

// gets the knockback of the attack
float attack::getKnockback(){
    return knockback;
}

// gets the angle of the attack
float attack::getAngle(){
    return angle;
}
// gets the knockback scaling of the attack
float attack::getKBScaling(){
    return KBscaling;
}
// gets the hitstun amount of the attack
int attack::getHitstun(){
    return hitstunFramesBase;
}
// gets the hitstun scaling  of the attack
float attack::getHitstunScaling(){
    return hitstunScaling;
}

// returns a copy of the attack's hitbox
hitbox attack::getHitbox(){
    return attackHitbox;
}

// gets the velocity of the attack (projecile only)
float attack::getXVelocity(){
    return velocityX;
}

// Update the position of the attack
/*coded by Charlie Limbert*/
void attack::updateAttackPosition(int posX, int posY, int dir, bool attackHitboxActive){
    printf("PosX Start: %d\n", posX);
    if(dir == 1){
        this->positionX = posX + playerHitboxLength - offX;
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
/*coded by Charlie Limbert*/
void attack::updateAttackHitbox(bool attackHitboxActive){

        if(direction == -1){
            // punch extends to the left
            attackHitbox.updateHitbox(positionX, positionY);
        } else {
            // punch extends to the right
            attackHitbox.updateHitbox(positionX, positionY);
        }
        // debug code for viewing active attack hitboxes
        // intentionally left commented for future debugging
        // if(attackHitboxActive){
        //     if(active){
        //         attackHitbox.debugDrawHitbox(RED);
        //     }else{
        //         attackHitbox.debugDrawHitbox(WHITE);
        //     }
        // }
    // Add other attack types here as needed
}

// Check if this attack collides with another hitbox
bool attack::checkCollision(hitbox otherHitbox){
    return attackHitbox.rectangleIntersects(otherHitbox);

}
