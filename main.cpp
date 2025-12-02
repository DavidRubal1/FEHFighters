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

    // TODO: put title screen stuff here

    player Player1(KEY_A, KEY_D, KEY_W, KEY_S, KEY_T, 160, 160, RED);
    player Player2(KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN, KEY_M, 230, 160, BLUE);
    platformGroup stage1;
    stage1.addPlatform(50, 180, 268, 240, 0x2e2e2e);
    stage1.addPlatform(106, 140, 213, 145, WHITE);
    
    
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

        
        // update frame
        LCD.Update();
        Sleep(frameTimeMilliseconds);
    }
    return 0;
}