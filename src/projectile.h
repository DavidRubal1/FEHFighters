class projectile : public attack{
    public:
        projectile(action type, int hitHeight, int hitLength, int offsetX, int offsetY, float velX, int color);
        void playProjectileAnimation(int color);
        void updateProjectilePosition(float velX);
        float getXVelocity();
    private:
        // projectile speed variables
        float velocityX;
        float velocityY;
        int positionX, positionY;
        animator projectileAnimator;

};

// projectile constructor
projectile::projectile(action type, int hitHeight, int hitLength, int offsetX, int offsetY, float velX, int color)
    : attack(type, hitHeight, hitLength, offsetX, offsetY),
        projectileAnimator(color){
    this->velocityX = velX;        
}


// plays the animation of the projectile given the player color
void projectile::playProjectileAnimation(int color){
    projectileAnimator.playAnimation(animationPropertiesLookup(PROJECTILE), positionX, positionY, getDirection());
}

// gets the velocity of the attack (projecile only)
float projectile::getXVelocity(){
    return velocityX;
}

void projectile::updateProjectilePosition(float velX){
    if(positionX > 0 && positionX < 320){
        positionX += velX * getDirection();
        updateAttackHitbox();
    }else{
        // active = false;
    }
}