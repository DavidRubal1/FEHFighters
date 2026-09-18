FEHIcon::Icon standardBackBtn();
void mainMenu(data *gameData);
void instructions(data *gameData);
void statsScreen(data *gameData);
void credits(data *gameData);
void modeSelect(data *gameData);


FEHIcon::Icon standardBackBtn(){
    FEHIcon::Icon backBtn;
    backBtn.SetProperties("Back", 250, 10, 50, 30, WHITE, RED);
    return backBtn;
}

 void mainMenu(data *gameData){

    // start button object, breaks to gameplay
    FEHIcon::Icon startBtn;
    startBtn.SetProperties("Play", 93, 60, 146, 30, WHITE, WHITE);
    // stats button object, goes to stats menu
    FEHIcon::Icon statsBtn;
    statsBtn.SetProperties("Statistics", 93, 90, 146, 30, WHITE, WHITE);
    // instructions button object, goes to instructions menu
    FEHIcon::Icon instructionsBtn;
    instructionsBtn.SetProperties("How to Play", 93, 120, 146, 30, WHITE, WHITE);
    // credits button object, goes to credits menu
    FEHIcon::Icon creditsBtn;
    creditsBtn.SetProperties("Credits", 93, 150, 146, 30, WHITE, WHITE);
    int x, y;
    
    while(!gameData->gameStarted){
        gameData->MenuArt.Draw(0,0);
        LCD.SetFontScale(1.5);
        LCD.SetFontColor(WHITE);
        LCD.WriteAt("FEH Fighters", 10, 10);
        LCD.DrawHorizontalLine(38, 3, 190);
        LCD.SetFontScale(1);

        startBtn.Draw();
        statsBtn.Draw();
        instructionsBtn.Draw();
        creditsBtn.Draw();

        LCD.Update();

        while(!LCD.Touch(&x,&y)) {};
        if(startBtn.Pressed(x, y, 0)){
            modeSelect(gameData);
        } else if(statsBtn.Pressed(x, y, 0)){
            statsScreen(gameData);
        } else if(instructionsBtn.Pressed(x, y, 0)){
            instructions(gameData);
        } else if(creditsBtn.Pressed(x,y,0)){
            credits(gameData);
        }
    }
    
 }

 void instructions(data *gameData){

    FEHIcon::Icon backBtn = standardBackBtn();

    gameData->MenuArt.Draw(0,0);
    LCD.SetFontScale(1.5);
    LCD.SetFontColor(WHITE);
    LCD.WriteAt("FEH Fighters", 10, 10);
    LCD.DrawHorizontalLine(38, 3, 190);
    LCD.SetFontScale(1);
    backBtn.Draw();
    LCD.SetFontScale(0.5);
    LCD.WriteRC("--Player 1--", 6, 16);
    LCD.WriteRC("Move: WASD", 7, 16);
    LCD.WriteRC("Punch: X",8, 16);
    LCD.WriteRC("Kick: C", 9, 16);
    LCD.WriteRC("Cast Projectile: V", 10, 16);
    LCD.SetFontColor(LIGHTBLUE);
    LCD.WriteRC("--Player 2--", 12, 16);
    LCD.WriteRC("Move: Arrow Keys", 13, 16);
    LCD.WriteRC("Punch: I", 14, 16);
    LCD.WriteRC("Kick: O", 15, 16);
    LCD.WriteRC("Cast Projectile: P", 16, 16);
    LCD.SetFontScale(1);

    LCD.Update();

    float x, y;
    while(!(LCD.Touch(&x,&y) && backBtn.Pressed(x, y, 0))) {};
    
    
 }
 

 
void statsScreen(data *gameData){
    FEHIcon::Icon backBtn = standardBackBtn();
    gameData->MenuArt.Draw(0,0);
    LCD.SetFontScale(1.5);
    LCD.SetFontColor(WHITE);
    LCD.WriteAt("FEH Fighters", 10, 10);
    LCD.DrawHorizontalLine(38, 3, 190);
    LCD.SetFontScale(1);
    backBtn.Draw();
    LCD.SetFontScale(0.5);
    LCD.WriteRC("Player 1 Wins: ", 10, 18);
    LCD.WriteRC(gameData->redWins, 10, 35);
    LCD.SetFontColor(LIGHTBLUE);
    LCD.WriteRC("Player 2 Wins: ", 11, 18);
    LCD.WriteRC(gameData->blueWins, 11, 35);
    LCD.SetFontColor(WHITE);
    LCD.WriteRC("Games Played: ", 12, 18);
    LCD.WriteRC(gameData->numGames, 12, 35);

    LCD.Update();

    float x, y;
    while(!(LCD.Touch(&x,&y) && backBtn.Pressed(x, y, 0))) {};
 }


 void credits(data *gameData){
    FEHIcon::Icon backBtn = standardBackBtn();
    gameData->MenuArt.Draw(0,0);
    LCD.SetFontScale(1.5);
    LCD.SetFontColor(WHITE);
    LCD.WriteAt("FEH Fighters", 10, 10);
    LCD.DrawHorizontalLine(38, 3, 190);
    LCD.SetFontScale(1);
    backBtn.Draw();
    LCD.SetFontScale(0.5);
    LCD.SetFontColor(WHITE);
    LCD.WriteRC("Developers:", 10, 18);
    LCD.WriteRC("David Rubal", 11, 18);
    LCD.WriteRC("Charlie Limbert", 12, 18);
    LCD.WriteRC("Art Created Using:", 13, 18);
    LCD.WriteRC("Piskel (piskelapp.com)", 14, 18);
    LCD.WriteRC("Pixilart (pixilart.com)", 15, 18);

    LCD.Update();

    float x, y;
    while(!(LCD.Touch(&x,&y) && backBtn.Pressed(x, y, 0))) {};
 }

 void modeSelect(data *gameData){
    FEHIcon::Icon backBtn = standardBackBtn();
    gameData->MenuArt.Draw(0,0);

    FEHIcon::Icon singlePlayerButton;
    singlePlayerButton. SetProperties("Single Player",80, 100, 158, 30, WHITE, WHITE);
    FEHIcon::Icon multiplayerButton;
    multiplayerButton.SetProperties("Two Player", 80, 160, 158, 30, WHITE, WHITE);

    // still images for characters to display during countdown and during mode select
    FEHImage RedCountdown;
    RedCountdown.Open("./graphics/Animations/PlayerRed/Right/Idle/Idle0.png");
    FEHImage BlueCountdown;
    BlueCountdown.Open("./graphics/Animations/PlayerBlue/Left/Idle/Idle0.png");

    singlePlayerButton.Draw();
    multiplayerButton.Draw();
    LCD.SetFontScale(1.5);
    LCD.SetFontColor(WHITE);
    LCD.WriteAt("FEH Fighters", 10, 10);
    LCD.DrawHorizontalLine(38, 3, 190);
    LCD.SetFontScale(1);
    RedCountdown.Draw(150, 80);
    RedCountdown.Draw(140, 140);
    BlueCountdown.Draw(160, 140);
    LCD.WriteAt("Choose a Mode", 82, 50);
    backBtn.Draw();

    LCD.Update();

    int x, y;
    while(!gameData->gameStarted){
        while(!LCD.Touch(&x,&y)) {};
        if(singlePlayerButton.Pressed(x, y, 0)){
            gameData->singlePlayerMode = true;
            gameData->gameStarted = true;
        } else if(multiplayerButton.Pressed(x, y, 0)){
            gameData->gameStarted = true;
        } else if(backBtn.Pressed(x,y,0)){
            break;
        }
    }
    
 }