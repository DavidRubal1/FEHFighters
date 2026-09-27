// struct to create types of animations for simple compatibility with the playAnimation methods above
struct animationProperties{
    char fileName[32];
    bool looping; // does the animation loop
    action type; // unique identifier to differentiate animations
    // TODO: should this be moved out of the properties?
    std::vector<int> frameLengths = {1}; // how long to hold the current frame for
};



 int calcTotalFrames(std::vector<int> v){
    int totalFrames = 0;
    for(int n : v){
        totalFrames += n;
    }
    return totalFrames;
 }

 std::vector<int> createVectorWithSameElements(int size, int num){
    std::vector<int> v(size, num);
    return v;   
 }

 animationProperties animationPropertiesLookup(action type){
    switch(type){
        case IDLE:
            return {"/Idle/Idle", true, type, createVectorWithSameElements(2, 30)};
        break;
        case CROUCH:
            return {"/Crouch/Crouch", true, type};
        break;
        case DASH:
            return {"/Dash/Dash", true, type, createVectorWithSameElements(9, 1)};
        break;
        case BASIC:
            //Frame length should be overriden
            return {"/Punch/", false, type, attackPropertiesLookup(type).frameData};
        break;
        case KICK:
            //Frame length should be overriden
            return {"/Kick/", false, type, attackPropertiesLookup(type).frameData};
        break;
        case CAST:
            //Frame length should be overriden
            return {"/ProjectileCast/", false, type, attackPropertiesLookup(type).frameData};
        break;
        case PROJECTILE:
            return {"/Projectile/", true, type};
        break;


        case DOUBLE_JUMP:
            return {"/DoubleJump/doubleJumpFrame", false, type, createVectorWithSameElements(4, 1)};
        break;
    }
 }