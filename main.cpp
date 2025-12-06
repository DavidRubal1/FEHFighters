#include "FEHLCD.h"
#include "FEHUtility.h"
#include "FEHKeyboard.h"
#include "FEHImages.h"
#include <math.h>
#include <vector>
#include <string>
#include "timer.h"
#include "hitbox.h"
#include "animation.h" // uses timer.h
#include "attack.h" // uses hitbox.h and animation.h
#include "player.h" // uses hitbox.h, attack.h, and animation.h

// Team G25-26
// David Rubal and Charlie Limbert


int main()
{
    int frameTimeMilliseconds = 20; // time between frames in milliseconds
    // variables to keep track of wins and games
    int numGames = 0, redWins = 0, blueWins = 0; 
    //Menu Objects
    /* menu coded by David Rubal*/
    FEHImage MenuArt;
    MenuArt.Open("./MenuArt/MenuKeyArt.png");
    FEHIcon::Icon startButton;
    startButton.SetProperties("Play", 93, 60, 146, 30, WHITE, WHITE);
    FEHIcon::Icon statsButton;
    statsButton.SetProperties("Statistics", 93, 90, 146, 30, WHITE, WHITE);
    FEHIcon::Icon instructions;
    instructions.SetProperties("How to Play", 93, 120, 146, 30, WHITE, WHITE);
    FEHIcon::Icon credits;
    credits.SetProperties("Credits", 93, 150, 146, 30, WHITE, WHITE);
    FEHIcon::Icon backButton;
    backButton.SetProperties("Back", 250, 10, 50, 30, WHITE, RED);
    // program loop, never exit
    while(1){

   
    // menu loop  
    while(1){
        MenuArt.Draw(0, 0);
        LCD.SetFontScale(1.5);
        LCD.SetFontColor(WHITE);
        LCD.WriteAt("FEH Fighters", 10, 10);
        LCD.DrawHorizontalLine(38, 3, 190);
        LCD.SetFontScale(1);

        startButton.Draw();
        statsButton.Draw();
        instructions.Draw();
        credits.Draw();

        float x, y;
        while(!LCD.Touch(&x,&y)) {};
        if(startButton.Pressed(x, y, 0)){
            break;
        }
        if(statsButton.Pressed(x, y, 0)){
            while(1){
                MenuArt.Draw(0,0);
                LCD.SetFontScale(1.5);
                LCD.SetFontColor(WHITE);
                LCD.WriteAt("FEH Fighters", 10, 10);
                LCD.DrawHorizontalLine(38, 3, 190);
                LCD.SetFontScale(1);
                backButton.Draw();
                LCD.SetFontScale(0.5);
                LCD.WriteRC("Player 1 Wins: ", 10, 18);
                LCD.WriteRC(redWins, 10, 35);
                LCD.SetFontColor(LIGHTBLUE);
                LCD.WriteRC("Player 2 Wins: ", 11, 18);
                LCD.WriteRC(blueWins, 11, 35);
                LCD.SetFontColor(WHITE);
                LCD.WriteRC("Games Played: ", 12, 18);
                LCD.WriteRC(numGames, 12, 35);
                LCD.Update();
                float x1, y1;
                while(!LCD.Touch(&x1,&y1)) {};
                if(backButton.Pressed(x1, y1, 0)){
                    break;
                }

            }
        }
        if(instructions.Pressed(x, y, 0)){
            while(1){
                MenuArt.Draw(0,0);
                LCD.SetFontScale(1.5);
                LCD.SetFontColor(WHITE);
                LCD.WriteAt("FEH Fighters", 10, 10);
                LCD.DrawHorizontalLine(38, 3, 190);
                LCD.SetFontScale(1);
                backButton.Draw();
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
                float x1, y1;
                while(!LCD.Touch(&x1,&y1)) {};
                if(backButton.Pressed(x1, y1, 0)){
                    break;
                }
            }
        }
        if(credits.Pressed(x, y, 0)){
            while(1){
                MenuArt.Draw(0,0);
                LCD.SetFontScale(1.5);
                LCD.SetFontColor(WHITE);
                LCD.WriteAt("FEH Fighters", 10, 10);
                LCD.DrawHorizontalLine(38, 3, 190);
                LCD.SetFontScale(1);
                backButton.Draw();
                LCD.SetFontScale(0.5);
                LCD.SetFontColor(WHITE);
                LCD.WriteRC("Developers:", 10, 18);
                LCD.WriteRC("David Rubal", 11, 18);
                LCD.WriteRC("Charlie Limbert", 12, 18);
                LCD.Update();
                float x1, y1;
                while(!LCD.Touch(&x1,&y1)) {};
                if(backButton.Pressed(x1, y1, 0)){
                    break;
                }

            }
        }
        Sleep(frameTimeMilliseconds);
        LCD.Update();
    }
    
    // create both player objects
    player Player1(KEY_A, KEY_D, KEY_W, KEY_S, KEY_X, KEY_C, KEY_V, 88, 160, RED);
    player Player2(KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN, KEY_I, KEY_O, KEY_P, 216, 160, BLUE);

    // create FEHImage objects for each background element
    FEHImage background;
    background.Open("./Background/FEHBackgroundVer3.png");
    FEHImage RedUI;
    RedUI.Open("./UI/RedUI.png");
    FEHImage BlueUI;
    BlueUI.Open("./UI/BlueUI.png");
    FEHIcon::Icon backButton;
    backButton.SetProperties("X", 10, 10, 15, 15, WHITE, RED);
    FEHImage redlifeImage, bluelifeImage;
    FEHImage RedCountdown;
    RedCountdown.Open("./PlayerRed/Right/Idle/Idle0.png");
    FEHImage BlueCountdown;
    BlueCountdown.Open("./PlayerBlue/Left/Idle/Idle0.png");


    // set player damage to 0 before game starts
    float p1Damage = 0.0, p2Damage = 0.0;
    LCD.SetFontScale(0.5);
    timer goTimer;
    goTimer.changeTimerMax(30); // timer for 30 frames when "Go!" is displays
    goTimer.resetTimer();


    LCD.SetFontScale(2);
    background.Draw(0,0);
    RedUI.Draw(80, 200);
    BlueUI.Draw(201,200);
    RedCountdown.Draw(88, 160);
    BlueCountdown.Draw(216, 160);
     LCD.SetFontColor(WHITE);
    LCD.WriteAt("3", 145, 90);
    LCD.Update();
    Sleep(1.0);
    background.Draw(0,0);
    RedUI.Draw(80, 200);
    BlueUI.Draw(201,200);
    RedCountdown.Draw(88, 160);
    BlueCountdown.Draw(216, 160);
    LCD.SetFontColor(YELLOW);
    LCD.WriteAt("2", 145, 90);
    LCD.Update();
    Sleep(1.0);
    background.Draw(0,0);
    RedUI.Draw(80, 200);
    BlueUI.Draw(201,200);
    RedCountdown.Draw(88, 160);
    BlueCountdown.Draw(216, 160);
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
        // game start countdown
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
                redlifeImage.Open("./UI/redLife3.png");
                redlifeImage.Draw(85, 209);
                break;
            case 2:
                redlifeImage.Open("./UI/redLife2.png");
                redlifeImage.Draw(85, 209);
                break;
            case 1:
                redlifeImage.Open("./UI/redLife1.png");
                redlifeImage.Draw(85, 209);
        }

        switch (Player2.remainingLives)
        {
            case 3:
                bluelifeImage.Open("./UI/blueLife3.png");
                bluelifeImage.Draw(206, 209);
                break;
            case 2:
                bluelifeImage.Open("./UI/blueLife2.png");
                bluelifeImage.Draw(206, 209);
                break;
            case 1:
                bluelifeImage.Open("./UI/blueLife1.png");
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


        // perform the same functions for player2
        Player2.generalPlayerMovementControl();
        Player2.enactPlayerMovement();
        Player2.action();
        Player2.manageHitboxes(&Player1);
        Player2.playAnimations();
        Player2.resetIfOffscreen();
        Player2.updateTimers();

        // draw the back button to exit mid-game
        backButton.Draw();
        float x, y;
        LCD.Touch(&x, &y);
        if(backButton.Pressed(x, y, 0)){
            // return to menu if pressed
            break;
        }

        // update frame
        LCD.Update();
        Sleep(frameTimeMilliseconds);

        //checks if player 2 has run out of lives
        /*coded by Charlie Limbert*/
        if (Player2.gameOver)
        {
            redWins++;
            numGames++;
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
            blueWins++;
            numGames++;
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
    LCD.SetFontScale(1);
}
    return 0;
}