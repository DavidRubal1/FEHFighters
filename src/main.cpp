#include "FEHLCD.h"
#include "FEHUtility.h"
#include "FEHKeyboard.h"
#include "FEHImages.h"
#include <math.h>
#include <vector>
#include <string>
#include "data/data.h"
#include "timer.h"
#include "hitbox.h"
#include "animation.h" // uses timer.h
#include "attack.h" // uses hitbox.h and animation.h
#include "player.h" // uses hitbox.h, attack.h, and animation.h
#include "screens.h"

// Team G25-26
// David Rubal and Charlie Limbert


int main()
{
    int frameTimeMilliseconds = 20; // time between frames in milliseconds
    // variables to keep track of wins and games
    data gameData;
    //Menu Objects
    /* menu coded by David Rubal*/

    float x, y;

    // program loop, never exit
    while(1){
   
    // menu proper loop, allows for user to return back to the menu after 
    // selecting play but without selecting a mode
    
    while(1){
        // menu inner loop
        while(1){
        // display main menu and title

        mainMenu(gameData);

        }
        // display mode selection screen, allows for singleplayer or multiplayer
        while(1){
        
    }
    // start the game if the a mode has been selected.
    // return to the main menu otherwise

    }
    

    /* written by David Rubal*/
    // create both player objects
    // Player 1 is the red character. Player 1 can move with WASD and attack with XCV.
    // Player 2 is the blue character. Player 2 can move with the Arrow Keys and attack with IOP.
    // in multiplayer, player 1 and player 2 are to be controlled by two separate people in competition 
    // if singleplayer is selected, player 2 is controlled by an AI
    player Player1(false, KEY_A, KEY_D, KEY_W, KEY_S, KEY_X, KEY_C, KEY_V, 88, 160, RED);
    player Player2(gameData.singlePlayerMode, KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN, KEY_I, KEY_O, KEY_P, 216, 160, BLUE);


    // create FEHImage objects for each background element
    // background art
    FEHImage background;
    background.Open("./graphics/Backgrounds/FEHBackgroundVer3.png");
    // red diamond image
    FEHImage RedUI;
    RedUI.Open("./graphics/Gui/RedUI.png");
    // blue diamond image
    FEHImage BlueUI;
    BlueUI.Open("./graphics/Gui/BlueUI.png");
    // change back button to an "X"
    //////////backButton.SetProperties("X", 10, 10, 15, 15, WHITE, RED);
    FEHImage redlifeImage, bluelifeImage;

    // set player damage to 0 before game starts
    float p1Damage = 0.0, p2Damage = 0.0;
    LCD.SetFontScale(0.5);
    timer goTimer;
    goTimer.changeTimerMax(30); // timer set to display "Go!" for 30 frames after the game has started
    goTimer.resetTimer();

    // game start countdown 
    LCD.SetFontScale(2);
    background.Draw(0,0);
    RedUI.Draw(80, 200);
    BlueUI.Draw(201,200);
    //////////RedCountdown.Draw(88, 160);
    //////////BlueCountdown.Draw(216, 160);
     LCD.SetFontColor(WHITE);
    LCD.WriteAt("3", 145, 90);
    LCD.Update();
    Sleep(1.0);
    background.Draw(0,0);
    RedUI.Draw(80, 200);
    BlueUI.Draw(201,200);
    //////////RedCountdown.Draw(88, 160);
    //////////BlueCountdown.Draw(216, 160);
    LCD.SetFontColor(YELLOW);
    LCD.WriteAt("2", 145, 90);
    LCD.Update();
    Sleep(1.0);
    background.Draw(0,0);
    RedUI.Draw(80, 200);
    BlueUI.Draw(201,200);
    //////////RedCountdown.Draw(88, 160);
    //////////BlueCountdown.Draw(216, 160);
    LCD.SetFontColor(ORANGE);
    LCD.WriteAt("1", 145, 90);
    LCD.Update();
    Sleep(1.0);


    //game loop
    while (1) {

        /* written by David Rubal*/
        // redraw background
        background.Draw(0,0);
        // redraw UI
        RedUI.Draw(80, 200);
        BlueUI.Draw(201,200);
        //display "Go!" for as long as the goTimer is active
        if(goTimer.isActive()){
            LCD.SetFontScale(2);
            LCD.WriteAt("Go!", 130, 90);
            goTimer.incrementTimer();
            goTimer.updateTimerState();
            LCD.SetFontScale(0.5);
        }
        // get player1 damage to display
        // converts damage value into a printable string with one decimal place
        p1Damage = Player1.getDamage();
        std::string printP1Damage= std::to_string(p1Damage);
        int p1DecimalIndex = printP1Damage.find(".");
        printP1Damage = printP1Damage.substr(0, p1DecimalIndex) + printP1Damage.substr(p1DecimalIndex, 2);
        // get player2 damage to display
        p2Damage = Player2.getDamage();
        std::string printP2Damage = std::to_string(p2Damage);
        int p2DecimalIndex = printP2Damage.find(".");
        printP2Damage = printP2Damage.substr(0, p2DecimalIndex) + printP2Damage.substr(p2DecimalIndex, 2);
        // display current damage
        LCD.SetFontColor(WHITE);
        LCD.WriteAt(printP1Damage, 90, 212);
        LCD.WriteAt(printP2Damage, 211, 212);

        //drawing remaining lives for both players
        /*coded by Charlie Limbert*/
        switch (Player1.remainingLives)
        {
            case 3:
                redlifeImage.Open("./graphics/Gui/redLife3.png");
                redlifeImage.Draw(85, 209);
                break;
            case 2:
                redlifeImage.Open("./graphics/Gui/redLife2.png");
                redlifeImage.Draw(85, 209);
                break;
            case 1:
                redlifeImage.Open("./graphics/Gui/redLife1.png");
                redlifeImage.Draw(85, 209);
        }

        switch (Player2.remainingLives)
        {
            case 3:
                bluelifeImage.Open("./graphics/Gui/blueLife3.png");
                bluelifeImage.Draw(206, 209);
                break;
            case 2:
                bluelifeImage.Open("./graphics/Gui/blueLife2.png");
                bluelifeImage.Draw(206, 209);
                break;
            case 1:
                bluelifeImage.Open("./graphics/Gui/blueLife1.png");
                bluelifeImage.Draw(206, 209);
        }
        
        // get input for player movement
        Player1.generalPlayerMovementControl();
        // move the player and handle solid collisions
        Player1.enactPlayerMovement();
        // get input for attacks
        Player1.action();
        // check for attack collision with other player
        Player1.manageHitboxes(&Player2);
        // play current player animation
        Player1.playAnimations();
        // reset the player to their starting point after they fall
        Player1.resetIfOffscreen();
        Player1.updateTimers();

        // perform the same player functions above for player2
        // if single player mode is active, call a function to determine the actions that 
        // player 2 takes. in multiplayer mode this is deactivated.
        if(gameData.singlePlayerMode){
            // the player 1 object is sent in to allow the AI to see player 1's position
            Player2.determineAIDecisions(&Player1);
        }
        Player2.generalPlayerMovementControl();
        Player2.enactPlayerMovement();
        Player2.action();
        Player2.manageHitboxes(&Player1);
        Player2.playAnimations();
        Player2.resetIfOffscreen();
        Player2.updateTimers();

        // draw the back button to exit mid-game
        //////////backButton.Draw();
        // exit the game if the back button is hovered
        float x, y;
        LCD.Touch(&x, &y);
        //////////////////////////////////////////////////
        //////////if(backButton.Pressed(x, y, 0)){
        //////////    // return to menu if pressed
        //////////    break;
        //////////}

        // update frame
        LCD.Update();
        // pause for the frame time
        Sleep(frameTimeMilliseconds);

        //checks if player 2 has run out of lives
        /*coded by Charlie Limbert*/
        if (Player2.gameOver)
        {
            //display player 1 victory screen
            //////////redWins++;
            //////////numGames++;
            LCD.SetFontColor(BLACK);
            LCD.FillRectangle(0, 0, 320, 240);
            LCD.SetFontColor(RED);
            LCD.WriteAt("Player 1 Wins!", 120, 100);
            LCD.WriteAt("Tap to Continue", 115, 115);
            LCD.Update();
            while(!LCD.Touch(&x,&y));
            Sleep(750);
            LCD.SetFontColor(BLACK);
            LCD.FillRectangle(0, 0, 320, 240);
            break;
        }
        //checks if player 1 has run out of lives
        if (Player1.gameOver)
        {
            //display player 2 victory screen
            //////////blueWins++;
            //////////numGames++;
            LCD.SetFontColor(BLACK);
            LCD.FillRectangle(0, 0, 320, 240);
            LCD.SetFontColor(BLUE);
            LCD.WriteAt("Player 2 Wins!", 120, 100);
            LCD.WriteAt("Tap to Continue", 115, 115);
            LCD.Update();
            while(!LCD.Touch(&x,&y));
            Sleep(750);
            LCD.SetFontColor(BLACK);
            LCD.FillRectangle(0, 0, 320, 240);
            break;
            Sleep(1000);
        }
    }
    // reset font size for the menu
    LCD.SetFontScale(1);
}
    return 0;
}
