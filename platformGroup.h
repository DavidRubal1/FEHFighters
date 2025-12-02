class platformGroup{
    public:
        void addPlatform(int x1, int y1, int x2, int y2, int color);
        void drawAllPlatforms();
        std::vector<platform> getPlatformList();
    private:
        std::vector<platform> platformList;

};

void platformGroup::addPlatform(int x1, int y1, int x2, int y2, int color){
    platformList.push_back(platform(x1, y1, x2, y2, color));
}

void platformGroup::drawAllPlatforms(){
    for(platform i: platformList){
        i.drawPlatform();
    }
}

std::vector<platform> platformGroup::getPlatformList(){
    return platformList;
}