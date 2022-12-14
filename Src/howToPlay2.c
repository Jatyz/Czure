#include "cprocessing.h"
#include "utils.h"

#include "mainMenu.h"
#include "play.h"
#include "credits1.h"
#include "credits2.h"
#include "settings.h"
#include "howToPlay1.h"



void howToPlay2_Init()
{
    // Set the window size to a sqaure 1000 x 800
    CP_System_SetWindowSize(windowWidth, windowHeight);

   
}

void howToPlay2_Update()
{
    // Background: Black Colour
    CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));

   /* CP_Image_Draw(credits1GradientBackground, windowWidth / 2, windowHeight / 2, windowWidth, windowHeight, 255);
    CP_Image_Draw(credits1, windowWidth / 2, windowHeight / 2, windowWidth, windowHeight, 255);
    CP_Image_Draw(credits1BackToMainMenu, credits1BackButtonX, credits1ButtonY, CP_Image_GetWidth(credits1BackToMainMenu), CP_Image_GetHeight(credits1BackToMainMenu), 255);
    CP_Image_Draw(credits1NextPage, credits1NextPageButtonX, credits1ButtonY, CP_Image_GetWidth(credits1NextPage), CP_Image_GetHeight(credits1NextPage), 255);*/

// -----Checking for MouseInput-----
    //if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT) == 1)
    //{
    //    // Back To Main Menu Button
    //    if (IsAreaClicked(credits1BackButtonX, credits1ButtonY, CP_Image_GetWidth(credits1BackToMainMenu), CP_Image_GetHeight(credits1BackToMainMenu), CP_Input_GetMouseX(), CP_Input_GetMouseY()))
    //    {
    //        CP_Engine_SetNextGameStateForced(mainMenu_Init, mainMenu_Update, mainMenu_Exit);
    //    }
    //    if (IsAreaClicked(credits1NextPageButtonX, credits1ButtonY, CP_Image_GetWidth(credits1NextPage), CP_Image_GetHeight(credits1NextPage), CP_Input_GetMouseX(), CP_Input_GetMouseY()))
    //    {
    //        CP_Engine_SetNextGameStateForced(credits2_Init, credits2_Update, credits2_Exit);
    //    }
    //}
}

void howToPlay2_Exit()
{
    /*CP_Image_Free(&credits1GradientBackground);
    CP_Image_Free(&credits1);
    CP_Image_Free(&credits1BackToMainMenu);
    CP_Image_Free(&credits1NextPage);*/
}
