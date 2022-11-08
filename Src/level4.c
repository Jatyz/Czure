#include "player.h"
#include "grid.h"
#include "game.h"
#include "cprocessing.h"
#include "level4.h"
#include "panels.h"
#include "levelSelect.h"
#include "enemy.h"

void level4_init(void)
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


	//for loop to go through all the tiles
	for (height = 0; height < returnBounds(Tile_Size); height++) {

		for (width = 0; width < returnBounds(Tile_Size); width++) {

			if (width < 3 || width > 6) {
				tiles[width][height].type = WALL;
			}
			else {
				tiles[width][height].type = FLOOR;

			}

		}
	}


	tiles[5][9].type = START;
	tiles[4][0].type = END;

	enemies[5][5].type = AOE_VIEW;
	enemies[5][5].isActive = 1;

	setStartGame(Tile_Size);
	player.setFOV = 0;
	gameState = PLAY;
}

void level4_update(void)
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
		drawSideBar("Level 4", player.counter);
		if (player.counter < 6)
		{
			drawSmallPanel(3 * Tile_Size, 2 * Tile_Size, 0 * Tile_Size, 1 * Tile_Size, "Enemy has their own detection system avoid walking into their vision");
			drawSmallPanel(3 * Tile_Size, 2 * Tile_Size, 7 * Tile_Size, 6 * Tile_Size, "You will lose when you walk into their vision");

		}


		if (player.counter > 6)
		{
			drawSmallPanel(3 * Tile_Size, 2 * Tile_Size, 0 * Tile_Size, 1 * Tile_Size, "Get to the red tile to proceed");

		}
		break;
	case PAUSED:
		drawFullPanel();
		checkClick(resumeGame, startLevel4, startLevelSelect);
		break;
	case WIN:
		drawFullPanel();
		checkClick(0, startLevel4, startLevelSelect);
		break;
	case LOSE:
		drawFullPanel();
		checkClick(startLevel4, startLevelSelect, 0);
		break;
	}


}

void level4_exit(void)
{

}