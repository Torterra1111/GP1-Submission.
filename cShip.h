#pragma once
#include "cSprite.h"
class cShip : public cSprite
{
private:
	MapRC shipPos;
	float shipRotation;

public:
	cShip();
	virtual ~cShip();
	void update(int row, int column);
	void setMapPosition(int row, int column);
	MapRC getMapPosition();
	void setShipRotation(float angle);
	float getShipRotation();
};

