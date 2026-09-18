class data{
    public:
        data();
        enum pages{START, STATS, INSTRUCTIONS, CREDITS, MAIN, SINGLEPLAYER, MULTIPLAYER};
        // 
        FEHImage MenuArt;
        int redWins, blueWins, numGames;
};

data::data(){
    MenuArt.Open("./graphics/Backgrounds/MenuKeyArt.png");
    redWins = 0;
    blueWins = 0; 
    numGames = 0;
}
