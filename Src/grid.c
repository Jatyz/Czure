#include "cprocessing.h"
#include "grid.h"
#include "game.h"

void drawTile() {

	int height, width;
	//for loop to go through all the tiles
	for (height = 0; height < HORIZONTAL_COUNT; height++) {

		for (width = 0; width < VERTICAL_COUNT; width++) {
			//depending on the tile the color of the tile is different
			switch (tiles[width][height].type) {
			case WALL:
				CP_Settings_Stroke(CP_Color_Create(125, 125, 125, 255));
				CP_Settings_Fill(CP_Color_Create(125, 125, 125, 255));
				break;
			case FLOOR:
				CP_Settings_Stroke(CP_Color_Create(75, 75, 75, 255));
				CP_Settings_Fill(CP_Color_Create(75, 75, 75, 255));
				break;
			case START:
				CP_Settings_Fill(CP_Color_Create(0, 150, 75, 255));
				break;
			case END:
				CP_Settings_Fill(CP_Color_Create(200, 0, 0, 255));
				break;
			}
			//draw the tile
			CP_Graphics_DrawRect(width * TILE_SIZE, height * TILE_SIZE, TILE_SIZE, TILE_SIZE);
		}

	}

}

void assignTile() {
	int height, width;
	//need to check tht there is only 1 start tile
	for (height = 0; height < HORIZONTAL_COUNT; height++) {

		for (width = 0; width < VERTICAL_COUNT; width++) {

			tiles[width][height].type = 1; //set everything to floor
		}

	}
}