#include "FEHLCD.h"
#include "FEHUtility.h"
#include "FEHKeyboard.h"
#include "FEHImages.h"
#include <math.h>
#include <vector>
#include <string>
#include "timer.h"
#include "hitbox.h"
#include "animation.h"
#include "attack.h" // uses hitbox.h
#include "player.h" // uses hitbox.h, uses attack.h


// debug
#include <stdio.h>

// Team G25-26
// David Rubal and Charlie Limbert

int main()
{
    int frameTimeMilliseconds = 20; // time between frames
    int numGames = 0, redWins = 0, blueWins = 0;
    // program loop
    while(1){
    // menu loop
    while(1){

        LCD.SetFontColor(WHITE);
        LCD.DrawRectangle(0, 0, 320, 240);

        LCD.WriteAt("FEH Fighters", 76, 13);
        // Draw new key art for the background?
        FEHImage red;
        red.Open("./PlayerRed/Right/Idle/Idle0.png");
        red.Draw(30, 90);
        FEHImage blue;
        blue.Open("./PlayerBlue/Left/Idle/Idle0.png");
        blue.Draw(276, 90);
        FEHIcon::Icon startButton;
        startButton.SetProperties("Play", 76, 60, 168, 30, WHITE, RED);
        startButton.Draw();

        FEHIcon::Icon statsButton;
        statsButton.SetProperties("Statistics", 76, 90, 168, 30, WHITE, RED);
        statsButton.Draw();

        FEHIcon::Icon instructions;
        instructions.SetProperties("How to Play", 76, 120, 168, 30, WHITE, RED);
        instructions.Draw();

        FEHIcon::Icon credits;
        credits.SetProperties("Credits", 76, 150, 168, 30, WHITE, RED);
        credits.Draw();

        float x, y;
        while(!LCD.Touch(&x,&y)) {};
        if(startButton.Pressed(x, y, 0)){
            LCD.SetFontColor(BLACK);
            LCD.FillRectangle(0, 0, 320, 240);
            break;
        }
        if(statsButton.Pressed(x, y, 0)){
            while(1){
                LCD.SetFontColor(BLACK);
                LCD.FillRectangle(0, 0, 320, 240);
                FEHIcon::Icon backButton;
                backButton.SetProperties("Back", 10, 10, 50, 30, WHITE, RED);
                backButton.Draw();
                LCD.WriteRC("Player 1 Wins: ", 6, 9);
                LCD.WriteRC(redWins, 6, 23);
                LCD.WriteRC("Player 2 Wins: ", 7, 9);
                LCD.WriteRC(blueWins, 7, 23);
                LCD.WriteRC("Games Played: ", 8, 9);
                LCD.WriteRC(numGames, 8, 23);
                LCD.Update();
                float x1, y1;
                while(!LCD.Touch(&x1,&y1)) {};
                if(backButton.Pressed(x1, y1, 0)){
                    LCD.SetFontColor(BLACK);
                    LCD.FillRectangle(0, 0, 320, 240);
                    break;
                }

            }
        }
        if(instructions.Pressed(x, y, 0)){
            while(1){
                LCD.SetFontColor(BLACK);
                LCD.FillRectangle(0, 0, 320, 240);
                FEHIcon::Icon backButton;
                backButton.SetProperties("Back", 10, 10, 50, 30, WHITE, RED);
                backButton.Draw();
                LCD.SetFontScale(0.5);
                LCD.WriteRC("Player 1 - Move: WASD", 6, 6);
                LCD.WriteRC("Punch: X, Kick: C, Projectile: V", 7, 6);
                LCD.SetFontColor(LIGHTBLUE);
                LCD.WriteRC("Player 2 - Move: Arrow Keys", 9, 6);
                LCD.WriteRC("Punch: I, Kick: O, Projectile: P", 10, 6);
                LCD.SetFontScale(1);
                LCD.Update();
                float x1, y1;
                while(!LCD.Touch(&x1,&y1)) {};
                if(backButton.Pressed(x1, y1, 0)){
                    LCD.SetFontColor(BLACK);
                    LCD.FillRectangle(0, 0, 320, 240);
                    break;
                }
            }
        }
        if(credits.Pressed(x, y, 0)){
            while(1){
                LCD.SetFontColor(BLACK);
                LCD.FillRectangle(0, 0, 320, 240);
                FEHIcon::Icon backButton;
                backButton.SetProperties("Back", 10, 10, 50, 30, WHITE, RED);
                backButton.Draw();
                LCD.WriteRC("Developers:", 6, 9);
                LCD.WriteRC("David Rubal", 7, 9);
                LCD.WriteRC("Charlie Limbert", 8, 9);
                LCD.Update();
                float x1, y1;
                while(!LCD.Touch(&x1,&y1)) {};
                if(backButton.Pressed(x1, y1, 0)){
                    LCD.SetFontColor(BLACK);
                    LCD.FillRectangle(0, 0, 320, 240);
                    break;
                }

            }
        }
        Sleep(frameTimeMilliseconds);
        LCD.Update();
    }
    

    player Player1(KEY_A, KEY_D, KEY_W, KEY_S, KEY_X, KEY_C, KEY_V, 88, 160, RED);
    player Player2(KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN, KEY_I, KEY_O, KEY_P, 216, 160, BLUE);

    
    FEHIcon::Icon backButton;
    backButton.SetProperties("X", 10, 10, 20, 20, WHITE, RED);
    
    FEHImage background;
    background.Open("./Background/FEHBackgroundVer3.png");
    FEHImage RedUI;
    RedUI.Open("./UI/RedUI.png");
    FEHImage BlueUI;
    BlueUI.Open("./UI/BlueUI.png");
    //FEHIcon::Icon RedPercent;
    LCD.SetFontScale(0.5);
    float p1Damage = 0.0, p2Damage = 0.0;
    //RedPercent.SetProperties("0.0", 80, 200, 39, 39, TRANSPARENT, WHITE);

    //game loop
    while (1) {
        // redraw background
        background.Draw(0,0);
        // redraw UI
        RedUI.Draw(80, 200);
        
        // get player damage to display
        p1Damage = Player1.getDamage();
        std::string printP1Damage= std::to_string(p1Damage);
        int p1DecimalIndex = printP1Damage.find(".");
        printP1Damage = printP1Damage.substr(0, p1DecimalIndex) + printP1Damage.substr(p1DecimalIndex, 2);
        p2Damage = Player2.getDamage();
        std::string printP2Damage = std::to_string(p2Damage);
        int p2DecimalIndex = printP2Damage.find(".");
        printP2Damage = printP2Damage.substr(0, p2DecimalIndex) + printP2Damage.substr(p2DecimalIndex, 2);
        LCD.SetFontColor(WHITE);
        LCD.WriteAt(printP1Damage, 90, 212);
        BlueUI.Draw(201,200);
        LCD.WriteAt(printP2Damage, 211, 212);

        //drawing remaining lives for both players
        FEHImage redlifeImage, bluelifeImage;
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
        
        
        Player1.generalPlayerMovementControl();
        Player1.enactPlayerMovement();
        Player1.action();
        //Player1.updateTimers();
        Player1.manageHitboxes(&Player2);
        
        Player1.playAnimations();
        int debugColor;
        if(Player1.lagFrame > 0){
            debugColor = RED;
        }else{
            
            debugColor = WHITE;
        }
        Player1.getHitbox().debugDrawHitbox(debugColor); // debug
        Player1.resetIfOffscreen();


        // add all of the player2 function calls when done
        Player2.generalPlayerMovementControl();
        Player2.enactPlayerMovement();
        Player2.action();
        Player2.manageHitboxes(&Player1);
        Player2.playAnimations();
        Player2.resetIfOffscreen();
        Player2.getHitbox().debugDrawHitbox(WHITE);

        
        backButton.Draw();
        float x, y;
        LCD.Touch(&x, &y);
        if(backButton.Pressed(x, y, 0)){
            LCD.SetFontColor(BLACK);
            LCD.FillRectangle(0, 0, 320, 240);
            break;
        }

        // update frame
        LCD.Update();
        Sleep(frameTimeMilliseconds);

        //checks if player 2 has run out of lives
        if (Player2.gameOver)
        {
            redWins++;
            numGames++;
            LCD.SetFontColor(BLACK);
            LCD.FillRectangle(0, 0, 320, 240);
            LCD.SetFontColor(RED);
            LCD.WriteAt("Player 1 Wins!", 120, 100);
            LCD.Update();
            while(!LCD.Touch(&x,&y));
            Sleep(1000);
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
            LCD.Update();
            while(!LCD.Touch(&x,&y));
            Sleep(1000);
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