class platform{
    public:
        platform(int posX1, int posY1, int posX2, int posY2, int platColor);
        hitbox getHitbox();
        void drawPlatform();
    private:
        int x1, y1, x2, y2;
        int color;
        hitbox platformHitbox;
};  

platform::platform(int posX1, int posY1, int posX2, int posY2, int platColor)
    : platformHitbox(posX1, posY1, posX2, posY2){
    x1 = posX1;
    y1 = posY1;
    x2 = posX2;
    y2 = posY2;
    color = platColor;
}

hitbox platform::getHitbox(){
    return platformHitbox;
}

void platform::drawPlatform(){
    LCD.SetFontColor(color);
    // LCD.DrawRectangle(x1, y1, x2-x1, y2-y1);
    LCD.FillRectangle(x1, y1, x2-x1, y2-y1);
}