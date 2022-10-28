#include "cprocessing.h"
#include "grid.h"
#include "game.h"
#include <math.h>


#define FOG_MAX_X 800
#define FOG_MAX_Y 800
//fog grid of same size as base game to draw over the render layers as FOV, is 1 when FOV is blocked, 0 when FOV not blocked
int fog[FOG_MAX_X][FOG_MAX_Y];		

enum FogDensity {
	FOG_MAX = 2,
	FOG_HALF = 1,
	FOG_MIN = 0,
};

//sets all value in fog to be fully obscured
void clearFogBackground(void)
{
	for (int i = 0; i < FOG_MAX_X; i++)		//for each row in fog
	{
		for (int j = 0; j < FOG_MAX_Y; j++)	//for each column in fog
		{
			fog[i][j] = FOG_MAX;			//set element to be fully obscure
		}
	}
}

//update basic logic for drawing a layer of black tiles in the level, except in a circle around the player
void setIllumination
(
	int const playerXPos,		//player's current X position on the grid
	int const playerYPos,		//player's current Y position on the grid
	int const gridSizeX,		//size of the grid in the X axis for the level, used to find the bounds required of the fog grid
	int const gridSizeY,		//size of the grid in the Y axis for the level, used to find the bounds required of the fog grid
	int const fovRadius			//the radius of the global illumination in terms of number of tiles
)
{
	//Check if grid size out of array, if is larger than acceptable, return function
	if (gridSizeX > FOG_MAX_X || gridSizeY > FOG_MAX_Y)
	{
		return;
	}

	for (int i = 0; i < gridSizeX; i++)			//for each row
	{
		for (int j = 0; j < gridSizeY; j++)		//for each column
		{
			// if in an circular area with radius of fovRadius tiles around the player
			if (
				sqrt((i - playerXPos) * (i - playerXPos)+(j - playerYPos)* (j - playerYPos))
				< fovRadius)
			{
				fog[i][j] = FOG_MIN;							//set fog in this tile to none
			}
		}
	}
}

//update basic logic for drawing a layer of black tiles in the level, except in a circle around the player
void setIlluminationAdvance
(
	int const playerXPos,		//player's current X position on the grid
	int const playerYPos,		//player's current Y position on the grid
	int const gridSizeX,		//size of the grid in the X axis for the level, used to find the bounds required of the fog grid
	int const gridSizeY,		//size of the grid in the Y axis for the level, used to find the bounds required of the fog grid
	int const fovRadius,		//the radius of the global illumination in terms of number of tiles
	int const diffuseRadius		//the radius of half illuminated in terms of number of tiles, meant to be a number higher than fovRadius
)
{
	//Check if grid size out of array, if is larger than acceptable, return function
	if (gridSizeX > FOG_MAX_X || gridSizeY > FOG_MAX_Y)
	{
		return;
	}

	for (int i = 0; i < gridSizeX; i++)			//for each row
	{
		for (int j = 0; j < gridSizeY; j++)		//for each column
		{
			if (
				sqrt((i - playerXPos) * (i - playerXPos) + (j - playerYPos) * (j - playerYPos))
				< diffuseRadius)
			{
				fog[i][j] = FOG_HALF;							//set fog in this tile to none
			}
			// if in an circular area with radius of fovRadius tiles around the player
			if (
				sqrt((i - playerXPos) * (i - playerXPos) + (j - playerYPos) * (j - playerYPos))
				< fovRadius)
			{
				fog[i][j] = FOG_MIN;							//set fog in this tile to none
			}
		}
	}
}


//update basic logic for not drawing fog along player's line of sight
void setPlayerFOV(
	int const playerXPos,		//player's current X position on the grid
	int const playerYPos,		//player's current Y position on the grid
	int const direction,		//the direction the player is currently facing, 0 is up, 1 is right, 2 is down, 3 is left
	int const gridSizeX,		//size of the grid in the X axis for the level, used to find the bounds required of the fog grid
	int const gridSizeY,		//size of the grid in the Y axis for the level, used to find the bounds required of the fog grid
	int const range				//range in terms of grid space for FOV distance
)
{
	//Check if grid size out of array, if is larger than acceptable, return function
	if (gridSizeX >= FOG_MAX_X || gridSizeY >= FOG_MAX_Y)
	{
		return;
	}

	// make sure direction is 0 to 3
	int facingDirection = (abs(direction) % 4);

	switch (facingDirection)							// if 0, face up. 1 faces right, 2 faces down, 3 faces left
	{

	case 0:														//look up
		//for each columns pass player to top of screen, up till end of screen or the maximum range
		for (int j = playerYPos; j >= 0 && abs(j - playerYPos) <= range; j--)
		{
			//for rows surrounding/close to player in horizontal axis
			for (int i = playerXPos - 1;
				(i < playerXPos + 1);
				i++)
			{
				if (!(i >= gridSizeX)
					&& !(i < 0)
					&& !(j >= gridSizeY)
					&& !(j < 0))				//dont set value if out of array(horizontal check)
				{
					fog[i][j] = FOG_MIN;								//set fog in this tile to none
				}

			}
		}
		break;

	case 1:
		//for each row pass player to right side of screen, up till end of screen or the maximum range
		for (int i = playerXPos; i < gridSizeX && (i - playerXPos) <= range; i++)
		{
			//for columns on top, below (increase addtional 1 size  based on expansion factor) and occupied by player (vertical axis)
			for (int j = playerYPos - 1;
				(j < playerYPos + 1);
				j++)
			{
				if (!(j >= gridSizeY)
					&& !(j < 0)
					&& !(i >= gridSizeX)
					&& !(i < 0))								//dont set value if out of array(vertical check)
				{
					fog[i][j] = FOG_MIN;								//set fog in this tile to none
				}
			}
		}
		break;

	case 2:														//look down
		//for each columns pass player to bottom of screen, up till end of screen or the maximum range
		for (int j = playerYPos; j < gridSizeY && abs(j - playerYPos) <= range; j++)
		{
			//for rows surrounding/close to player in horizontal axis
			for (int i = playerXPos - 1;
				(i < playerXPos + 1);
				i++)
			{
				//dont set value if out of array!
				if (
					!(i >= gridSizeX)
					&& !(i < 0)
					&& !(j >= gridSizeY)
					&& !(j < 0))
				{
					fog[i][j] = FOG_MIN;								//set fog in this tile to none
				}
			}
		}
		break;

	case 3:
		//for each row pass player to the left side of the screen, up till end of screen or the maximum range
		for (int i = playerXPos; i >= 0 && abs(i - playerXPos) <= range; i--)
		{
			//for columns on top, below and occupied by player (vertical axis)
			for (int j = playerYPos - 1;
				(j < playerYPos + 1);
				j++)
			{
				if (!(j >= gridSizeY) && !(j < 0)
					&& !(i >= gridSizeX)
					&& !(i < 0))								//dont set value if out of array(vertical check)
				{
					fog[i][j] = FOG_MIN;								//set fog in this tile to none
				}
			}
		}
		break;
	}

}

//update basic logic for not drawing fog along player's line of sight
void setPlayerFOVFunnel(
	int const playerXPos,		//player's current X position on the grid
	int const playerYPos,		//player's current Y position on the grid
	int const direction,		//the direction the player is currently facing, 0 is up, 1 is right, 2 is down, 3 is left
	int const gridSizeX,		//size of the grid in the X axis for the level, used to find the bounds required of the fog grid
	int const gridSizeY,		//size of the grid in the Y axis for the level, used to find the bounds required of the fog grid
	int const expansionFactor,	//the rate of change of the FOV cone's expansion based on grid space 
	int const coneRange			//the range of our FOV funnel
)
{
	//Check if grid size out of array, if is larger than acceptable, return function
	if (gridSizeX >= FOG_MAX_X || gridSizeY >= FOG_MAX_Y)
	{
		return;
	}

	// make sure direction is 0 to 3
	int facingDirection = (abs(direction) % 4);

	//	int drawY,			//is one if fov is not blocked by wall, unused till further prototyping
	//		drawX;			//is one if fov is not blocked by wall, unused till further prototyping

	int currentFactor = expansionFactor;		//the current rate of the FOV cone's expansion based on grid space, used in loop

		switch (facingDirection)							// if 0, face up. 1 faces right, 2 faces down, 3 faces left
		{

		case 0:														//look up
			//for each columns pass player to top of screen, up till end of screen or the maximum cone range
			for (int j = playerYPos; j >= 0 && abs(j - playerYPos) <= coneRange; j--)
			{
				//for rows surrounding/close to player in horizontal axis
				for (int i = playerXPos - (currentFactor / expansionFactor);
					(i < playerXPos + (currentFactor / expansionFactor) + 1);
					i++)
				{
					if (!(i >= gridSizeX) 
						&& !(i < 0)
						&& !(j >= gridSizeY)
						&& !(j < 0))				//dont set value if out of array(horizontal check)
					{
						fog[i][j] = FOG_MIN;								//set fog in this tile to none
					}

				}
				currentFactor++;									//increase the current factor for FOV size
			}
			break;

		case 1:
			//for each row pass player to right side of screen, up till end of screen or the maximum cone range
			for (int i = playerXPos; i < gridSizeX && (i - playerXPos) <= coneRange; i++)
			{
				//for columns on top, below (increase addtional 1 size  based on expansion factor) and occupied by player (vertical axis)
				for (int j = playerYPos - (currentFactor / expansionFactor);
					(j < playerYPos + (currentFactor / expansionFactor) + 1);
					j++)
				{
					if (!(j >= gridSizeY)
						&& !(j < 0)
						&& !(i >= gridSizeX)
						&& !(i < 0))								//dont set value if out of array(vertical check)
					{
						fog[i][j] = FOG_MIN;								//set fog in this tile to none
					}
				}

				currentFactor++;									//increase the current factor for FOV size
			}
			break;
		
		case 2:														//look down
			//for each columns pass player to bottom of screen, up till end of screen or the maximum cone range
			for (int j = playerYPos; j < gridSizeY && abs(j - playerYPos) <= coneRange; j++)
			{
				//for rows surrounding/close to player in horizontal axis
				for (int i = playerXPos - (currentFactor / expansionFactor);
					(i < playerXPos + (currentFactor / expansionFactor) + 1);
					i++)
				{
					//dont set value if out of array!
					if (
						!(i >= gridSizeX)
						&& !(i < 0)
						&& !(j >= gridSizeY)
						&& !(j < 0))
					{
						fog[i][j] = FOG_MIN;								//set fog in this tile to none
					}
				}
				currentFactor++;									//increase the current factor for FOV size
			}
			break;

		case 3:
			//for each row pass player to the left side of the screen, up till end of screen or the maximum cone range
			for (int i = playerXPos; i >= 0 && abs(i - playerXPos) <= coneRange; i--)
			{
				//for columns on top, below and occupied by player (vertical axis)
				for (int j = playerYPos - (currentFactor / expansionFactor);
					(j < playerYPos + (currentFactor / expansionFactor) + 1);
					j++)
				{
					if (!(j >= gridSizeY) && !(j < 0)
						&& !(i >= gridSizeX)
						&& !(i < 0))								//dont set value if out of array(vertical check)
					{
						fog[i][j] = FOG_MIN;								//set fog in this tile to none
					}
				}
				currentFactor++;									//increase the current factor for FOV size
			}
			break;
		}
}

//sets the Fog covered spots in FOV for type funnel FOV, implementation assumes setPlayerFOVFunnel() was called before calling this function,
//also is dependent on global variable tiles[] to check for obstacles to vision
void setFOVFunnelWallLogic(
	int const playerXPos,		//horizontal position of player character on the grid
	int const playerYPos,		//vertical position of player character on the grid
	int const direction,		//the direction the player is currently facing, 0 is up, 1 is right, 2 is down, 3 is left
	int const gridSizeX,		//horizontal size of grid
	int const gridSizeY,		//vertical size of grid
	int const expansionFactor,	//the rate at which the funnel will expand(i.e. grow by 1 tile every (expansionFactor) wort of tiles away from player)
	int const coneRange			//the maximum range of the cone
)

{
	//Check if grid size out of array, if is larger than acceptable, return function
	if (gridSizeX >= FOG_MAX_X || gridSizeY >= FOG_MAX_Y)
	{
		return;
	}

	// make sure direction is 0 to 3
	int facingDirection = (abs(direction) % 4);

	int drawLowerBounds = -1,					//use to check if any fog should be drawn, for elements higher than player position
		drawUpperBounds = FOG_MAX_X;			//use to check if any fog should be drawn, for elements lower than player position

	//assumes size of fog X and Y axis are similiar, stores if the element in one axis is a wall, 1 means wall, 0 means not a wall
	//by default, all elements are assumed to not be a wall
	int wallArray[FOG_MAX_X] = { 0 };
	for (int* p = wallArray, *q = wallArray + FOG_MAX_X;
		p < q;
		*p = 0, p++);

	int currentFactor = expansionFactor;		//the current rate of the FOV cone's expansion based on grid space, used in loop

	switch (facingDirection)							// if 0, face up. 1 faces right, 2 faces down, 3 faces left
	{

	case 0:														//look up
		//for each columns pass player to top of screen, up till end of screen or the maximum cone range
		for (int j = playerYPos; j >= 0 && abs(j - playerYPos) <= coneRange; j--)
		{
			//for rows surrounding/close to player in horizontal axis
			for (int i = playerXPos - (currentFactor / expansionFactor);
				(i < playerXPos + (currentFactor / expansionFactor) + 1);
				i++)
			{
				if (!(i >= gridSizeX)
					&& !(i < 0)
					&& !(j >= gridSizeY)
					&& !(j < 0))								//dont set value if out of array(horizontal check)
				{
					if (i < drawLowerBounds)						//if x axis is further lower than the last known left side wall position touching the edge of the player's FOV
					{
						fog[i][j] = FOG_MAX;							//set fog to true
						continue;								//skip further checks to save on memory and processes, proceed with next loop check
					}
					if (i > drawUpperBounds)					//if x axis is further higher than the last known right side wall position touching the edge of the player's FOV
					{
						fog[i][j] = FOG_MAX;							//set fog to true
						continue;								//skip further checks to save on memory and processes, proceed with next loop check
					}
					if (wallArray[i]) { fog[i][j] = FOG_MAX; }			//if previously checked to be a wall, set fog to true
					if (tiles[i][j].type == WALL)				//if the current element is a type of wall in the global tiles array
					{
						//int variable used to add one to following checks to disable edge case of corner peeking
						//change variable to 0 to enable corner peeking
						int ifPreviouslyExpand = (currentFactor / expansionFactor) - ((currentFactor - 1) / expansionFactor);

						//Lower Bounds check for FOV
						//if the checked tile is a wall and is in the leftmost corner plus one in the FOV, only on rows that are larger than the previous row
						if (i - ifPreviouslyExpand == (playerXPos - (currentFactor / expansionFactor)))
						{
							drawLowerBounds = i;				//make all tiles beyond this row that is left of the current tile have fog
							fog[i - 1][j] = FOG_MAX;					//set the corner to be covered by fog to prevent peeking
						}
						//if the checked tile is a wall and is in the furthest left corner in the FOV
						else if (i == (playerXPos - (currentFactor / expansionFactor)))
						{
							drawLowerBounds = i;				//make all tiles beyond this row that is left of the current tile have fog
						}

						//Upper Bounds check for FOV
						//if checked tile is a wall and is in the furthest right corner minus one in the FOV, only on rows that are larger than the previous row
						if (i + ifPreviouslyExpand == (playerXPos + (currentFactor / expansionFactor)))
						{	//no need to set fog to next row as setting upper bound here prevents next row from being set to clear
							drawUpperBounds = i;				//make all tiles beyond this row that is right of the current tile have fog
						}
						//if the checked tile is a wall and is in the furthest right corner in the FOV
						else if (i == (playerXPos + (currentFactor / expansionFactor)))
						{
							drawUpperBounds = i;				//make all tiles beyond this row that is right of the current tile have fog
						}
						wallArray[i] = 1;						//make every row behind the current wall have fog added on further checks
						//if the next element behind the wall is another wall, let it be not covered in fog
						if (tiles[i][j + 1].type == WALL) { wallArray[j] = 0; }
					}
				}
			}
			currentFactor++;									//increase the current factor for FOV size
		}
		break;

	case 1:														//look right
		//for each row pass player to right side of screen, up till end of screen or the maximum cone range
		for (int i = playerXPos; i < gridSizeX && (i - playerXPos) <= coneRange; i++)
		{
			//for columns on top, below (increase addtional 1 size  based on expansion factor) and occupied by player (vertical axis)
			for (int j = playerYPos - (currentFactor / expansionFactor);
				(j < playerYPos + (currentFactor / expansionFactor) + 1);
				j++)
			{
				if (!(j >= gridSizeY)
					&& !(j < 0)
					&& !(i >= gridSizeX)
					&& !(i < 0))								//dont set valuae if out of array(vertical check)
				{
					if (j < drawLowerBounds)					//if y axis is further lower than the last known left side wall position touching the edge of the player's FOV
					{
						fog[i][j] = FOG_MAX;							//set fog to true
						continue;								//skip further checks to save on memory and processes, proceed with next loop check
					}
					if (j > drawUpperBounds)					//if y axis is further higher than the last known right side wall position touching the edge of the player's FOV
					{
						fog[i][j] = FOG_MAX;							//set fog to true
						continue;								//skip further checks to save on memory and processes, proceed with next loop check
					}
					if (wallArray[j]) { fog[i][j] = FOG_MAX; }		//if previously checked to be a wall, set fog to true
					if (tiles[i][j].type == WALL)				//if the current element is a type of wall in the global tiles array
					{
						//int variable used to add one to following checks to disable edge case of corner peeking
						//change variable to 0 to enable corner peeking
						int ifPreviouslyExpand = (currentFactor / expansionFactor) - ((currentFactor - 1) / expansionFactor);

						//Lower Bounds check for FOV
						//if the checked tile is a wall and is in the top-most corner plus one in the FOV, only on rows that are larger than the previous columns
						if (j - ifPreviouslyExpand == (playerYPos - (currentFactor / expansionFactor)))
						{
							drawLowerBounds = j;				//make all tiles beyond this column that is above(minus y axis) of the current tile have fog
							fog[i][j - 1] = FOG_MAX;					//set the corner to be covered by fog to prevent peeking
						}
						//if the checked tile is a wall and is in the furthest top corner in the FOV
						else if (j == (playerYPos - (currentFactor / expansionFactor)))
						{
							drawLowerBounds = j;				//make all tiles beyond this row that is left of the current tile have fog
						}

						//Upper Bounds check for FOV
						//if checked tile is a wall and is in the furthest bottom corner in FOV
						if (j + 1 + ifPreviouslyExpand == (playerYPos + (currentFactor / expansionFactor) + 1))
						{
							drawUpperBounds = j;				//make all tiles beyond this row that is bottom of the current tile have fog
						}
						//if checked tile is a wall and is in the furthest bottom corner in FOV
						else if (j + 1 == (playerYPos + (currentFactor / expansionFactor) + 1))
						{
							drawUpperBounds = j;				//make all tiles beyond this row that is bottom of the current tile have fog
						}
						wallArray[j] = 1;						//make every row behind the wall have fog added on further checks
						//if the next element behind the wall is another wall, let it be not covered in fog
						if (tiles[i + 1][j].type == WALL) { wallArray[j] = 0; }
					}
				}
			}

			currentFactor++;									//increase the current factor for FOV size
		}
		break;

	case 2:														//look down
		//for each columns pass player to bottom of screen, up till end of screen or the maximum cone range
		for (int j = playerYPos; j < gridSizeY && abs(j - playerYPos) <= coneRange; j++)
		{
			//for rows surrounding/close to player in horizontal axis
			for (int i = playerXPos - (currentFactor / expansionFactor);
				(i < playerXPos + (currentFactor / expansionFactor) + 1);
				i++)
			{
				//dont set value if out of array!
				if (
					!(i >= gridSizeX)
					&& !(i < 0)
					&& !(j >= gridSizeY)
					&& !(j < 0))
				{
					if (i < drawLowerBounds)						//if x axis is further lower than the last known left side wall position touching the edge of the player's FOV
					{
						fog[i][j] = FOG_MAX;							//set fog to true
						continue;								//skip further checks to save on memory and processes, proceed with next loop check
					}
					if (i > drawUpperBounds)					//if x axis is further higher than the last known right side wall position touching the edge of the player's FOV
					{
						fog[i][j] = FOG_MAX;							//set fog to true
						continue;								//skip further checks to save on memory and processes, proceed with next loop check
					}
					if (wallArray[i]) { fog[i][j] = FOG_MAX; }			//if previously checked to be a wall, set fog to true
					if (tiles[i][j].type == WALL)				//if the current element is a type of wall in the global tiles array
					{
						//int variable used to add one to following checks to disable edge case of corner peeking
						//change variable to 0 to enable corner peeking
						int ifPreviouslyExpand = (currentFactor / expansionFactor) - ((currentFactor - 1) / expansionFactor);

						//Lower Bounds check for FOV
						//if the checked tile is a wall and is in the leftmost corner plus one in the FOV, only on rows that are larger than the previous row
						if (i - ifPreviouslyExpand == (playerXPos - (currentFactor / expansionFactor)))
						{
							drawLowerBounds = i;				//make all tiles beyond this row that is left of the current tile have fog
							fog[i - 1][j] = FOG_MAX;					//set the corner to be covered by fog to prevent peeking
						}
						//if the checked tile is a wall and is in the furthest left corner in the FOV
						else if (i == (playerXPos - (currentFactor / expansionFactor)))
						{
							drawLowerBounds = i;				//make all tiles beyond this row that is left of the current tile have fog
						}

						//Upper Bounds check for FOV
						//if checked tile is a wall and is in the furthest right corner minus one in the FOV, only on rows that are larger than the previous row
						if (i + ifPreviouslyExpand == (playerXPos + (currentFactor / expansionFactor)))
						{	//no need to set fog to next row as setting upper bound here prevents next row from being set to clear
							drawUpperBounds = i;				//make all tiles beyond this row that is right of the current tile have fog
						}
						//if the checked tile is a wall and is in the furthest right corner in the FOV
						else if (i == (playerXPos + (currentFactor / expansionFactor)))
						{
							drawUpperBounds = i;				//make all tiles beyond this row that is right of the current tile have fog
						}
						wallArray[i] = 1;						//make every row behind the current wall have fog added on further checks
						//if the next element behind the wall is another wall, let it be not covered in fog
						if (tiles[i][j - 1].type == WALL) { wallArray[j] = 0; }
					}

				}

			}
			currentFactor++;									//increase the current factor for FOV size
		}
		break;

	case 3:														//look left
		//for each row pass player to the left side of the screen, up till end of screen or the maximum cone range
		for (int i = playerXPos; i >= 0 && abs(i - playerXPos) <= coneRange; i--)
		{
			//for columns on top, below and occupied by player (vertical axis)
			for (int j = playerYPos - (currentFactor / expansionFactor);
				(j < playerYPos + (currentFactor / expansionFactor) + 1);
				j++)
			{
				if (!(j >= gridSizeY) && !(j < 0)
					&& !(i >= gridSizeX)
					&& !(i < 0))								//dont set value if out of array(vertical check)
				{
					if (j < drawLowerBounds)					//if y axis is further lower than the last known left side wall position touching the edge of the player's FOV
					{
						fog[i][j] = FOG_MAX;							//set fog to true
						continue;								//skip further checks to save on memory and processes, proceed with next loop check
					}
					if (j > drawUpperBounds)					//if y axis is further higher than the last known right side wall position touching the edge of the player's FOV
					{
						fog[i][j] = FOG_MAX;							//set fog to true
						continue;								//skip further checks to save on memory and processes, proceed with next loop check
					}
					if (wallArray[j]) { fog[i][j] = FOG_MAX; }		//if previously checked to be a wall, set fog to true
					if (tiles[i][j].type == WALL)				//if the current element is a type of wall in the global tiles array
					{						//int variable used to add one to following checks to disable edge case of corner peeking
						//change variable to 0 to enable corner peeking
						int ifPreviouslyExpand = (currentFactor / expansionFactor) - ((currentFactor - 1) / expansionFactor);

						//Lower Bounds check for FOV
						//if the checked tile is a wall and is in the top-most corner plus one in the FOV, only on rows that are larger than the previous columns
						if (j - ifPreviouslyExpand == (playerYPos - (currentFactor / expansionFactor)))
						{
							drawLowerBounds = j;				//make all tiles beyond this column that is above(minus y axis) of the current tile have fog
							fog[i][j - 1] = FOG_MAX;					//set the corner to be covered by fog to prevent peeking
						}
						//if the checked tile is a wall and is in the furthest top corner in the FOV
						else if (j == (playerYPos - (currentFactor / expansionFactor)))
						{
							drawLowerBounds = j;				//make all tiles beyond this row that is left of the current tile have fog
						}

						//Upper Bounds check for FOV
						//if checked tile is a wall and is in the furthest bottom corner in FOV
						if (j + 1 + ifPreviouslyExpand == (playerYPos + (currentFactor / expansionFactor) + 1))
						{
							drawUpperBounds = j;				//make all tiles beyond this row that is bottom of the current tile have fog
						}
						//if checked tile is a wall and is in the furthest bottom corner in FOV
						else if (j + 1 == (playerYPos + (currentFactor / expansionFactor) + 1))
						{
							drawUpperBounds = j;				//make all tiles beyond this row that is bottom of the current tile have fog
						}
						wallArray[j] = 1;						//make every row behind the wall have fog added on further checks
						//if the next element behind the wall is another wall, let it be not covered in fog
						if (tiles[i - 1][j].type == WALL) { wallArray[j] = 0; }
					}
				}
			}
			currentFactor++;									//increase the current factor for FOV size
		}
		break;
	}
}

//renders the most basic version of the FOV fog	
void renderFOVBasic(
	int const gridSizeX,		//size(number of horizontal elements) of grid in game level
	int const gridSizeY,		//size(number of vertical elements) of grid in game level
	int const tileSizePX		//tile size in pixels
)
{
		//Check if grid size out of array, if is larger than acceptable, return function
		if (gridSizeX >= FOG_MAX_X || gridSizeY >= FOG_MAX_Y)
		{
			return;
		}

	for (int i = 0; i < gridSizeX; i++)			//for each row
	{
		for (int j = 0; j < gridSizeY; j++)		//for each column
		{
			switch (fog[i][j]>0) {
				case 1:
					CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 255));	//set outline black	
					CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));	//set block color black
					//draw the tile
					CP_Graphics_DrawRect(i * tileSizePX, j * tileSizePX, tileSizePX, tileSizePX);
					break;
				default:
					break;
			}
		}
	}
}

//renders the FOV fog to various degrees of opacity according to enum FogDensity
void renderFOVAdvance(
	int const gridSizeX,		//size(number of horizontal elements) of grid in game level
	int const gridSizeY,		//size(number of vertical elements) of grid in game level
	int const tileSizePX		//tile size in pixels
)
{
	//Check if grid size out of array, if is larger than acceptable, return function
	if (gridSizeX >= FOG_MAX_X || gridSizeY >= FOG_MAX_Y)
	{
		return;
	}

	for (int i = 0; i < gridSizeX; i++)			//for each row
	{
		for (int j = 0; j < gridSizeY; j++)		//for each column
		{
			switch (fog[i][j]) {
			case FOG_MAX:
				CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 255));	//set outline black	
				CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));	//set block color black
				//draw the tile
				CP_Graphics_DrawRect(i * tileSizePX, j * tileSizePX, tileSizePX, tileSizePX);
				break;
			case FOG_HALF:
				CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 127));	//set outline black	of half opacity
				CP_Settings_Fill(CP_Color_Create(0, 0, 0, 127));	//set block color black of half opacity
				//draw the tile
				CP_Graphics_DrawRect(i * tileSizePX, j * tileSizePX, tileSizePX, tileSizePX);
				break;
			default:
				break;
			}
		}
	}
}
