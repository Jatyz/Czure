#include "player.h"
#include "grid.h"
#include "game.h"
#include "cprocessing.h"
#include "level9.h"
#include "panels.h"
#include "levelSelect.h"
#include "enemy.h"
#include "FOV.h"

void level9_init(void)
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

	tiles[1][1].type = WALL;
	tiles[2][1].type = WALL;
	tiles[3][1].type = WALL;
	tiles[4][1].type = WALL;
	tiles[5][1].type = WALL;
	tiles[6][1].type = WALL;
	tiles[7][1].type = WALL;
	tiles[8][1].type = WALL;
	tiles[1][2].type = WALL;
	tiles[1][3].type = WALL;
	tiles[1][4].type = WALL;
	tiles[1][5].type = WALL;
	tiles[1][6].type = WALL;
	tiles[1][7].type = WALL;
	tiles[1][8].type = WALL;
	tiles[2][8].type = WALL;
	tiles[3][8].type = WALL;
	tiles[6][8].type = WALL;
	tiles[7][8].type = WALL;
	tiles[8][8].type = WALL;
	tiles[8][7].type = WALL;
	tiles[8][6].type = WALL;
	tiles[8][5].type = WALL;
	tiles[8][4].type = WALL;
	tiles[8][3].type = WALL;
	tiles[8][2].type = WALL;
	tiles[8][1].type = WALL;
	tiles[3][3].type = WALL;
	tiles[4][3].type = WALL;
	tiles[5][3].type = WALL;
	tiles[6][3].type = WALL;
	tiles[6][4].type = WALL;
	tiles[6][5].type = WALL;
	tiles[6][6].type = WALL;
	tiles[3][4].type = WALL;
	tiles[3][5].type = WALL;
	tiles[3][6].type = WALL;
	tiles[4][6].type = WALL;
	tiles[5][6].type = WALL;

	tiles[5][5].type = END;
	tiles[0][9].type = START;


	gates[0].Door = &tiles[5][3];
	gates[0].Switch = &tiles[9][0];

	tiles[0][0].type = DISGUISE;
	tiles[0][0].Tile_Color = RED;

	enemySet(2, 7, 0, 3, VERTICAL_HORIZONTAL_LOOK, RED);
	setStartGame(Tile_Size);
	player.setFOV = 1;
	doorLightRange = 2;
	gameState = PLAY;
	player.currentStage = 10;
}

void level9_update(void)
{
	//need this for light shine on door
	if (doorLightCounter > 0) {
		doorLightCounter -= CP_System_GetDt();
		return;
	}
	else {
		switch (gameState) {
		case PLAY:
			//clears the screen so things can be redrawn
			CP_Graphics_ClearBackground(CP_Color_Create(60, 60, 60, 255));
			enemyFOV(Tile_Size);
			//all the game update methods that needs to be updated every frame
			if (player.setFOV) {
				clearFogBackground();
				setIlluminationWallLogicOnce(player.x, player.y, returnBounds(Tile_Size), returnBounds(Tile_Size), 4);

			}
			handlePlayerInput(Tile_Size);
			renderGame();

			if (player.counter < 10)
			{
				drawSmallPanel(4 * Tile_Size, 2 * Tile_Size, 3 * Tile_Size, 4 * Tile_Size, "The enemy base is really dark.");

			}
			//End FOV logic handled area
			drawSideBar("Level 9", player.counter);

			break;
		case PAUSED:
			drawFullPanel();
			checkClick(resumeGame, startLevel9, startLevelSelect);
			break;
		case WIN:
			drawFullPanel();
			checkClick(startLevel10, startLevel9, startLevelSelect);
			break;
		case LOSE:
			drawFullPanel();
			checkClick(startLevel9, startLevelSelect, 0);
			break;
		}

	}
}

void level9_exit(void)
{

}
