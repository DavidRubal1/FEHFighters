#include "FEHLCD.h"
#include "FEHUtility.h"
#include "FEHKeyboard.h"
#include "FEHImages.h"
#include <math.h>
#include <vector>
#include <string>
#include "hitbox.h"
#include "player.h" // uses hitbox.h
#include "platform.h"  // uses hitbox.h
#include "platformGroup.h" // usess platform.h 
#include "animation.h"

// debug
#include <stdio.h>

// Team G25-26
// David Rubal and Charlie Limbert

int main()
{
    int frameTimeMilliseconds = 16; // default should be around 15-16
    bool inMenu = true;
    // TODO: put title screen stuff here
    while(1){
    while(inMenu){
        LCD.SetFontColor(WHITE);
        LCD.DrawRectangle(0, 0, 320, 240);

        LCD.WriteAt("FEH Fighters", 76, 13);

        FEHIcon::Icon startButton;
        startButton.SetProperties("Play", 76, 60, 136, 30, WHITE, RED);
        startButton.Draw();

        FEHIcon::Icon statsButton;
        statsButton.SetProperties("Statistics", 76, 90, 136, 30, WHITE, RED);
        statsButton.Draw();

        FEHIcon::Icon instructions;
        instructions.SetProperties("How to Play", 76, 120, 136, 30, WHITE, RED);
        instructions.Draw();

        FEHIcon::Icon credits;
        credits.SetProperties("Credits", 76, 150, 136, 30, WHITE, RED);
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
    platformGroup stage1;
    stage1.addPlatform(50, 180, 268, 240, 0x2e2e2e);
    stage1.addPlatform(106, 140, 213, 145, WHITE);
    FEHIcon::Icon backButton;
    backButton.SetProperties("X", 10, 10, 20, 20, WHITE, RED);
    
    while (1) {
        // draw stage platforms and border
        stage1.drawAllPlatforms();
        LCD.SetFontColor(0x313030);
        LCD.DrawRectangle(0,0, 320, 240);

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
}
    return 0;
}