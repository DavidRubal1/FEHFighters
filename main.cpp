#include "FEHLCD.h"
#include "FEHUtility.h"
#include "FEHKeyboard.h"
#include "FEHImages.h"
#include <math.h>
#include <vector>
#include <string>
#include "hitbox.h"
#include "player.h" // uses hitbox.h
#include "animation.h"

// debug
#include <stdio.h>

// Team G25-26
// David Rubal and Charlie Limbert

int main()
{
    int frameTimeMilliseconds = 19; // time between frames
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
        red.Open("./PlayerRed/Right/PlayerRedRight0.png");
        red.Draw(30, 90);
        FEHImage blue;
        blue.Open("./PlayerBlue/Left/PlayerBlueLeft0.png");
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
                LCD.WriteRC("Player 1 Wins: 6", 6, 9);
                LCD.WriteRC("Player 2 Wins: 7", 7, 9);
                LCD.WriteRC("Games Played 13", 8, 9);
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
                LCD.WriteRC("Player 1 - Move: WASD, Atatck: T", 6, 9);
                LCD.WriteRC("Player 2 - Move: Arrow Keys, Atatck: M", 7, 9);
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
    

    player Player1(KEY_A, KEY_D, KEY_W, KEY_S, KEY_T, 160, 160, RED);
    player Player2(KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN, KEY_M, 230, 160, BLUE);
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
    while (1) {
        // redraw background
        background.Draw(0,0);
        // redraw UI
        RedUI.Draw(80, 200);
        // TODO: update percent with damage
        p1Damage = Player1.getDamage();
        std::string printP1Damage= std::to_string(p1Damage);
        int p1DecimalIndex = printP1Damage.find(".");
        printP1Damage = printP1Damage.substr(0, p1DecimalIndex) + printP1Damage.substr(p1DecimalIndex, 2);
        p2Damage = Player2.getDamage();
        std::string printP2Damage= std::to_string(p2Damage);
        int p2DecimalIndex = printP2Damage.find(".");
        printP2Damage = printP2Damage.substr(0, p2DecimalIndex) + printP2Damage.substr(p2DecimalIndex, 2);
        LCD.SetFontColor(WHITE);
        LCD.WriteAt(printP1Damage, 90, 212);
        BlueUI.Draw(201,200);
        LCD.WriteAt(printP2Damage, 211, 212);
        


 
        
        // draw stage platforms and border
        //stage1.drawAllPlatforms();
        //LCD.SetFontColor(0x313030);
        //LCD.DrawRectangle(0,0, 320, 240);

        // get an array of [XPOS, YPOS] for player 1
        //std::vector<int> XYPos_Player1 = Player1.getXYPosition();
        //std::vector<float> XYVel_Player1 = Player1.getXYVelocity();
        
        Player1.generalPlayerMovementControl(frameTimeMilliseconds);
        Player1.enactPlayerMovement(frameTimeMilliseconds);
        Player1.playAnimations(frameTimeMilliseconds);
        Player1.resetIfOffscreen();
        Player2.generalPlayerMovementControl(frameTimeMilliseconds);
        Player2.enactPlayerMovement(frameTimeMilliseconds);
        Player2.playAnimations(frameTimeMilliseconds);
        Player2.resetIfOffscreen();

        
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
    }
    LCD.SetFontScale(1);
}
    return 0;
}