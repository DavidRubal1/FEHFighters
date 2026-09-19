
/*class written by Charlie Limbert and David Rubal*/
class attack {
    public:
        attack(attackType type ,int hitHeight, int hitLength, int offsetX, int offsetY);
        attack(attackType type, int hitHeight, int hitLength, int offsetX, int offsetY, float velX);
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
        attackType type; 
        int direction;   // -1 = left, 1 = right
        int positionX;
        int positionY;
        int offX, offY; // given offsets for attack positions
        // TODO: integrate this into the posx and posy sent initially?
        int playerHitboxLength = 14; // used to offset attack position

        // projectile speed variables
        float velocityX;
        float velocityY;
        
        // hitbox dimensions
        int hitboxHeight;
        int hitboxLength;
        
        // attack state
        //TODO: REPLACE THIS WITH THE ACTIVE FRAMES VECTOR
        bool active = false;

        attackProperties properties;
        
        // hitbox for attack
        hitbox attackHitbox;

        // TODO: figure out why this is here
        // -> this should be a separate entity from the player
        animationType projectile = {"/Projectile/", 1, true, 5, 1};
};

// Constructor
/* written by Charlie Limbert and David Rubal*/
attack::attack(attackType type, int hitHeight, int hitLength, int offsetX, int offsetY)
    : attackHitbox(hitHeight, hitLength){
    this->type = type;
    this->hitboxHeight = hitHeight;
    this->hitboxLength = hitLength;
    this->offX = offsetX;
    this->offY = offsetY;

    this->properties = assignAttackProperties(type);    
}

// projectile constructor
attack::attack(attackType type, int hitHeight, int hitLength, int offsetX, int offsetY, float velX)
    : attackHitbox(hitHeight, hitLength){
    this->type = type;
    this->hitboxHeight = hitHeight;
    this->hitboxLength = hitLength;
    this->offX = offsetX;
    this->offY = offsetY;
    this->velocityX = velX;
    

    //TODO: find out why this code it here and what it does
    strcpy(projectile.fileName, "/Projectile/");
    projectile.looping = true;
        
}
// "getters" written by David Rubal

// plays the animation of the projectile given the player color
void attack::playProjectileAnimation(int color){
    animation projectileAnimator(color);
    projectileAnimator.playAnimation(projectile, positionX, positionY, direction);
}

// returns the attack's type (0 = punch, 1 = kick, 2 = cast)
int attack::getAttackType(){
    return type;
}
// gets the direction of the attack (used for kb calculation)
int attack::getDirection(){
    return direction;
}

attackProperties attack::getProperties(){
    return properties;
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

void attack::moveProjectile(float velX){
    if(positionX > 0 && positionX < 320){
        positionX += velX * direction;
        updateAttackHitbox(true);
    }else{
        active = false;
    }
}

// changes the active state of the attack to the parameter's state
void attack::updateActiveState(bool state){
    active = state;
}

// returns whether this attack is active
bool attack::isActive(){
    return active;
}

// Update the hitbox based on attack type and direction
/*coded by Charlie Limbert*/
void attack::updateAttackHitbox(bool attackHitboxActive){

        if(direction == -1){
            // attack extends to the left
            attackHitbox.updateHitbox(positionX, positionY);
        } else {
            // attack extends to the right
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
}

// Check if this attack collides with another hitbox, used for testing player collison with attacks
bool attack::checkCollision(hitbox otherHitbox){
    return attackHitbox.rectangleIntersects(otherHitbox);
}
