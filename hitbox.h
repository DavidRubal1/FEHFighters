// class and all methods written by David Rubal
class hitbox{
    public:
        hitbox(int heightIn, int lengthIn, int posX, int posY);
        hitbox(int heightIn, int lengthIn);
        void updateHitbox(int posX1, int posY1);
        void debugDrawHitbox(int color);
        bool rectangleIntersects(hitbox element);
        std::vector<int> getCoordinates();
    private:
        int x1, y1, x2, y2;
        int height, length;        
};

/*************************************************************************
 * Hitbox Info
 * - The "Position" of a hitbox lies at the top-left corner of the hitbox
 * - x1 and y1 are coordinates for the top-left corner
 * - x2 and y2 are coordinates for the bottom-right corner
 * - height and length are vertical and horizontal descriptors for how long 
 *      the hitbox spans from its Position in the positive x and y directions (right and down)
 * - There are two constructors for this class, the first takes the coordinates for 
 *      the top-left and bottom-right, the other takes the position, height, length, 
 *      and a filler value that allows a second constructor
 * 
 * 
 ***************************************************************************/


// constructor #1, for players
hitbox::hitbox(int heightIn, int lengthIn, int posX, int posY){
    x1 = posX;
    y1 = posY;
    x2 = posX + lengthIn;
    y2 = posY + heightIn;
    length = lengthIn;
    height = heightIn;
}

// constructor #2, for attacks
hitbox::hitbox(int heightIn, int lengthIn){
    height = heightIn;
    length = lengthIn;
}

// returns a vector array of the hitbox's top-left coordinates
std::vector<int> hitbox::getCoordinates(){
    return {x1, y1, x2, y2};
}

// updates the hitbox coordinates based on given player position
void hitbox::updateHitbox(int posX, int posY){
    x1 = posX;
    y1 = posY;
    x2 = posX + length;
    y2 = posY + height;
}

//funtions for viewing hitboxes, used for debugging
void hitbox::debugDrawHitbox(int color){
    LCD.SetFontColor(color);
    LCD.DrawRectangle(x1, y1, length, height);
}

// tests intersection of this and element, assuming that both are rectangluar
bool hitbox::rectangleIntersects(hitbox element){
    std::vector<int> elementCoords = element.getCoordinates();
    // x1 and y1 and the upper left corners, x2 and y2 are the lower right corners

    // The following 3 lines of code adapted from StackExchange user davin
    // Url: https://math.stackexchange.com/questions/99565/simplest-way-to-calculate-the-intersect-area-of-two-rectangles
    int x_overlap = fmax(0, fmin(x2, elementCoords[2]) - fmax(x1, elementCoords[0]));
    int y_overlap = fmax(0, fmin(y2, elementCoords[3]) - fmax(y1, elementCoords[1]));
    int overlapArea = x_overlap * y_overlap;

    return overlapArea > 0;
}

