class data{
    public:
        data();
        
        FEHImage MenuArt;
        int redWins, blueWins, numGames;
        bool singlePlayerMode, gameStarted;
};

data::data(){
    MenuArt.Open("./graphics/Backgrounds/MenuKeyArt.png");
    redWins = 0;
    blueWins = 0; 
    numGames = 0;
    gameStarted = false;
    singlePlayerMode = false;
}
