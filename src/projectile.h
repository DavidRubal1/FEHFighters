class projectile : public attack{
    public:
        projectile(action type, int hitHeight, int hitLength, int offsetX, int offsetY, float velX, int color);
        void playProjectileAnimation();
        void updateProjectilePosition();
        float getXVelocity();
    private:
        // projectile speed variables
        float velocityX;
        float velocityY;
        animator projectileAnimator;

};

// projectile constructor
projectile::projectile(action type, int hitHeight, int hitLength, int offsetX, int offsetY, float velX, int color)
    : attack(type, hitHeight, hitLength, offsetX, offsetY),
        projectileAnimator(color){
    this->velocityX = velX;     
    //start inactive
    consumeHitbox();   
}


// plays the animation of the projectile given the player color
void projectile::playProjectileAnimation(){
    projectileAnimator.playAnimation(animationPropertiesLookup(PROJECTILE), getPosition()->at(0), getPosition()->at(1), getDirection());
}

// gets the velocity of the attack (projecile only)
float projectile::getXVelocity(){
    return velocityX;
}

void projectile::updateProjectilePosition(){
    if(getPosition()->at(0) > 0 && getPosition()->at(0) < 320){
        getPosition()->at(0) += velocityX * getDirection();
        updateAttackHitbox();
    }else{
        consumeHitbox();
    }
}