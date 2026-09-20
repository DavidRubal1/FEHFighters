// struct to create types of animations for simple compatibility with the playAnimation methods above
struct animationProperties{
    char fileName[32];
    int finalFrameNum; //the # of the last frame of the animation
    bool looping; // does the animation loop
    int ID; // unique identifier to differentiate animations
    // TODO: should this be moved out of the properties?
    int frameLength = 1; // how long to hold the current frame for
};

 enum animationType{
     ANI_IDLE, ANI_CROUCH, ANI_DASH, ANI_BASIC, ANI_KICK, ANI_CAST, ANI_PROJECTILE, ANI_DOUBLE_JUMP
 };

 int calcTotalFrames(std::vector<int> v){
    int totalFrames = 0;
    for(int n : v){
        totalFrames += n;
    }
    return totalFrames;
 }

 animationProperties animationPropertiesLookup(animationType type){
    switch(type){
        case ANI_IDLE:
            return {"/Idle/Idle", 1, true, type, 30};
        break;
        case ANI_CROUCH:
            return {"/Crouch/Crouch", 0, true, type};
        break;
        case ANI_DASH:
            return {"/Dash/Dash", 8, true, type};
        break;
        case ANI_BASIC:
            //Frame length should be overriden
            return {"/Punch/", calcTotalFrames(attackPropertiesLookup(ATK_BASIC).frameData), false, type};
        break;
        case ANI_KICK:
            //Frame length should be overriden
            return {"/Kick/", calcTotalFrames(attackPropertiesLookup(ATK_KICK).frameData), false, type};
        break;
        case ANI_CAST:
            //Frame length should be overriden
            return {"/ProjectileCast/", calcTotalFrames(attackPropertiesLookup(ATK_PROJECTILE_CAST).frameData), false, type};
        break;
        case ANI_PROJECTILE:
            return {"/Projectile/", 1, true, 5, 1};
        break;


        case ANI_DOUBLE_JUMP:
            return {"/DoubleJump/doubleJumpFrame", 3, false, 3};
        break;
    }
 }