// struct to create types of animations for simple compatibility with the playAnimation methods above
struct animationProperties{
    char fileName[32];
    int finalFrameNum; //the # of the last frame of the animation
    bool looping; // does the animation loop
    int ID; // unique identifier to differentiate animations
    // TODO: should this be moved out of the properties?
    int frameLength = 1; // how long to hold the current frame for
};



 int calcTotalFrames(std::vector<int> v){
    int totalFrames = 0;
    for(int n : v){
        totalFrames += n;
    }
    return totalFrames;
 }

 animationProperties animationPropertiesLookup(action type){
    switch(type){
        case IDLE:
            return {"/Idle/Idle", 1, true, type, 30};
        break;
        case CROUCH:
            return {"/Crouch/Crouch", 0, true, type};
        break;
        case DASH:
            return {"/Dash/Dash", 8, true, type};
        break;
        case BASIC:
            //Frame length should be overriden
            return {"/Punch/", attackPropertiesLookup(BASIC).frameData.size(), false, type};
        break;
        case KICK:
            //Frame length should be overriden
            return {"/Kick/", attackPropertiesLookup(KICK).frameData.size(), false, type};
        break;
        case CAST:
            //Frame length should be overriden
            return {"/ProjectileCast/", attackPropertiesLookup(CAST).frameData.size(), false, type};
        break;
        case PROJECTILE:
            return {"/Projectile/", 1, true, 5, 1};
        break;


        case DOUBLE_JUMP:
            return {"/DoubleJump/doubleJumpFrame", 3, false, 3};
        break;
    }
 }