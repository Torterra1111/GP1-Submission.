/*
=================
cTileMap.h
- Header file for class definition - SPECIFICATION
- Header file for the tileMap class which is a child of cSprite class
=================
*/
#ifndef _CTILEMAP_H
#define _CTILEMAP_H
#include "cSprite.h"


class cTileMap: public cSprite
{
protected:
	int tileMap[10][12]; //Size of the map has been upped to 10 12.

private:
	SDL_Point mapStartXY;
	SDL_Point tileClickedRC;
	cSprite aTile;
	void initialiseMap();		// Set the initial values for the map
	float tileRotation;

public:
	cTileMap();

	void render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, cTextureMgr* theTxtMgr, vector<LPCSTR> theTxt);		// Default render function
	void renderGridLines(SDL_Renderer* theRenderer, SDL_Rect theRect, SDL_Color theColour); // Draw grid lines
	void update(SDL_Point theMapAreaClicked, int theTileToPlace);
	void update(MapRC mapPos, int theItemToPlace, float rotation);
	void setMapStartXY(SDL_Point startPosXY);
	SDL_Point getMapStartXY();
	void setTileRotation(float rotation);
	float getTileRotation();
};
#endif