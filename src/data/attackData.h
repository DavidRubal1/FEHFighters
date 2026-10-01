struct attackProperties{
    // how much damage the attack deals
    float damage;
    // base amount of how much the attack will launch the opponent away
    float knockback;
    // angle of knockback
    float angle;
    // how much the knockback scale with the other player's damage
    float KBscaling;
    // how many frames of hitstun the opponent will be subjected to 
    int hitstunFramesBase;
    // how much the opponent's damage will increase the hitstun they recieve
    float hitstunScaling;
    std::vector<int> frameData;
    std::vector<bool> activeFrames;
};

attackProperties attackPropertiesLookup(action type){
    switch(type){
        case BASIC: // punch
        return {
            5.0, // Damage
            5.0, // KB
            .89, // Angle
            1.0, // KB Scaling
            6,   // Hitstun Base
            0.22,// Hitstun Scaling
            {1, 1, 2, 2, 4}, // Frame data
            {false, false, false, true, false} // Active Frames
        };
        break;
        case KICK: // kick
        return{
            8.5,
            3, 
            0.4,
            2.5,
            5,
            0.2,
            {2, 3, 1, 3, 7},
            {false, false, false, true, false}
        };
        break; // projectile cast
        case CAST:
        return{
            3.0,
            4.0,
            0.6,
            1.5,
            5,
            0.1,
            {4, 6, 3, 3, 6},
            {false, false, false, true, false}
        };
        break; // Projectile object
        case PROJECTILE:
            return{
                6.5,
                3.0,
                0.9,
                2.7,
                13,
                0.1,
                {1},
                {true}
            };
        break;
    }
}
