#include "player.h"
#include "grid.h"
#include "game.h"
#include "cprocessing.h"
#include "level7.h"
#include "panels.h"
#include "levelSelect.h"
#include "enemy.h"

void level7_init(void)
{
	CP_System_SetWindowSize(WINDOW_WIDTH, WINIDOW_HEIGHT);
	CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));
	//change this variable to change the number of tiles on the map X by X
	//Factors of 800
	// use these below
	// 20,25,32,40,50,80,100,160
	//e.g. big room = 20
	// medium room = 50
	// small room = 80
	Tile_Size = SMALL;

	//reset all arrays and variables
	resetGame(Tile_Size);

	//assign all the floors and walls
	assignTile(Tile_Size);

	int height, width;

	tiles[9][9].type = START;
	tiles[9][8].type = WALL;
	tiles[8][8].type = WALL;
	tiles[0][0].type = END;

	tiles[7][9].type = DISGUISE;
	tiles[7][9].Tile_Color = GREEN;
	tiles[5][5].type = DISGUISE;
	tiles[5][5].Tile_Color = RED;

	enemySet(5, 8, 1, 0, AOE_VIEW, GREEN);
	enemySet(8, 6, 1, 0, AOE_VIEW, GREEN);
	enemySet(3, 4, 0, 6, VERTICAL_HORIZONTAL_LOOK, 0);
	setStartGame(Tile_Size);
	player.setFOV = 0;
	gameState = PLAY;
	player.currentStage = 7;
}

void level7_update(void)
{
	switch (gameState) {
	case PLAY:
		//clears the screen so things can be redrawn
		CP_Graphics_ClearBackground(CP_Color_Create(60, 60, 60, 255));
		handlePlayerInput(Tile_Size);
		//all the game update methods that needs to be updated every frame
		renderGame();
		enemyFOV(Tile_Size);
		//End FOV logic handled area
		drawSideBar("Level 7", player.counter);
		if (player.counter < 2)
		{
			drawSmallPanel(6 * Tile_Size, 2 * Tile_Size, 1 * Tile_Size, 8 * Tile_Size, "\nDisguise tiles makes it so the enemy detection system cannot see you if you are the same color as their detection!! -->");
		}
		break;
	case PAUSED:
		drawFullPanel();
		checkClick(resumeGame, startLevel7, startLevelSelect);
		break;
	case WIN:
		drawFullPanel();
		checkClick(startLevel8, startLevel7, startLevelSelect);
		break;
	case LOSE:
		drawFullPanel();
		checkClick(startLevel7, startLevelSelect, 0);
		break;
	}


}

void level7_exit(void)
{

}