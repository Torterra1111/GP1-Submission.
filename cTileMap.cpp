/*
=================
- cTileMap.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cTileMap.h"

/*
=================================================================
 Defualt Constructor
=================================================================
*/
cTileMap::cTileMap() : cSprite()
{
	this->initialiseMap();
}


void cTileMap::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, cTextureMgr* theTxtMgr, vector<LPCSTR> theTxt)
{
	SDL_Rect tilePosXY;
	tilePosXY = { mapStartXY.x,  mapStartXY.y, 0 , 0 };
	// determine number of rows and columns for array
	int numRows = sizeof(tileMap) / sizeof(tileMap[0]);
	int numCols = sizeof(tileMap[0]) / sizeof(tileMap[0][0]);
	int txtToUse = 0;
	for (int row = 0; row < numRows; row++)
	{
		for (int column = 0; column < numCols; column++)
		{
			switch (tileMap[row][column])
			{
			case 1:
				txtToUse = 0;
				break;
			case 2:
				txtToUse = 1;
				break;
			case 3:
				txtToUse = 2;
				break;
			case 4:
				txtToUse = 3;
				break;
			case 5:
				txtToUse = 4;
				break;
			case 6:
				txtToUse = 5;
				break;
			case 7:
				txtToUse = 6;
				break;
			case 8:
				txtToUse = 7;
				break;
			}
			
			aTile.setTexture(theTxtMgr->getTexture(theTxt[txtToUse]));
			aTile.setSpriteDimensions(theTxtMgr->getTexture(theTxt[txtToUse])->getTWidth(), theTxtMgr->getTexture(theTxt[txtToUse])->getTHeight());
			tilePosXY.w = aTile.getSpritePos().w;
			tilePosXY.h = aTile.getSpritePos().h;
			// Render tile
			if (tileMap[row][column] == 3)
			{
				aTile.render(theRenderer, &aTile.getSpriteDimensions(), &tilePosXY, this->getTileRotation(), &aTile.getSpriteCentre(), aTile.getSpriteScale());
			}
			else
			{
				aTile.render(theRenderer, &aTile.getSpriteDimensions(), &tilePosXY, aTile.getSpriteRotAngle(), &aTile.getSpriteCentre(), aTile.getSpriteScale());
			}
			tilePosXY.x += aTile.getSpritePos().w;
		}
		tilePosXY.x = mapStartXY.x;
		tilePosXY.y += aTile.getSpritePos().h;
	}

}

void cTileMap::renderGridLines(SDL_Renderer* theRenderer, SDL_Rect theRect, SDL_Color theColour)
{
	SDL_Rect tilePosXY;
	tilePosXY = { mapStartXY.x, mapStartXY.y, theRect.w, theRect.h };
	// determine number of rows and columns for array
	int numRows = sizeof(tileMap) / sizeof(tileMap[0]);
	int numCols = sizeof(tileMap[0]) / sizeof(tileMap[0][0]);
	SDL_SetRenderDrawColor(theRenderer, theColour.r, theColour.g, theColour.b, theColour.a);
	for (int row = 0; row < numRows; row++)
	{
		for (int column = 0; column < numCols; column++)
		{
			SDL_RenderDrawRect(theRenderer, &tilePosXY);
			tilePosXY.x += theRect.w;
		}
		tilePosXY.x = mapStartXY.x;
		tilePosXY.y += theRect.h;
	}
	SDL_SetRenderDrawColor(theRenderer, 0, 100, 0, 0); //???
}

/*
=================================================================
Update the sprite position
=================================================================
*/

void cTileMap::update(SDL_Point theMapAreaClicked, int theTileToPlace)
{
	SDL_Point areaClicked = theMapAreaClicked;
	if (theTileToPlace > -1)
		{
			if (areaClicked.x > this->mapStartXY.x && areaClicked.x < 612 && areaClicked.y > this->mapStartXY.y && areaClicked.y < 612)
			{
				this->tileClickedRC.x = (int)(areaClicked.x - this->mapStartXY.x) / this->aTile.getSpriteDimensions().w;
				this->tileClickedRC.y = (int)(areaClicked.y - this->mapStartXY.y) / this->aTile.getSpriteDimensions().h;
				this->tileMap[this->tileClickedRC.y][this->tileClickedRC.x] = theTileToPlace;
			}
		}
}

void cTileMap::update(MapRC mapPos, int theItemToPlace, float rotation)
{
	this->tileMap[mapPos.R][mapPos.C] = theItemToPlace;
	this->setTileRotation(rotation);
}
/*
=================================================================
- Set the initial values for the map
=================================================================
*/
void cTileMap::initialiseMap()
{
	// determine number of rows and columns for array
	int numRows = sizeof(tileMap) / sizeof(tileMap[0]);
	int numCols = sizeof(tileMap[0]) / sizeof(tileMap[0][0]);

	for (int row = 0; row < numRows; row++)
	{
		for (int column = 0; column < numCols; column++)
		{
			tileMap[row][column] = 1;
		}
	}

}
/*
=================================================================
- set start position for tile map
=================================================================
*/
void cTileMap::setMapStartXY(SDL_Point startPosXY)
{
	mapStartXY = startPosXY;
}
/*
=================================================================
- get start pposition for tile map
=================================================================
*/
SDL_Point cTileMap::getMapStartXY()
{
	return mapStartXY;
}

void cTileMap::setTileRotation(float rotation)
{
	this->tileRotation = rotation;
}

float cTileMap::getTileRotation()
{
	return this->tileRotation;
}
