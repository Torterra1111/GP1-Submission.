/*
=================
- cShip.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cShip.h"

/*
=================================================================
Defualt Constructor
=================================================================
*/
cShip::cShip()
{
	this->setMapPosition( 0, 0 );
}

void cShip::update(int row, int column)
{
	this->setMapPosition(column, row );
}

void cShip::setMapPosition(int row, int column)
{
	this->shipPos = { column, row };
}

MapRC cShip::getMapPosition()
{
	return this->shipPos;
}

void cShip::setShipRotation(float angle)
{
	this->shipRotation = angle;
	this->setSpriteRotAngle(angle);
}

float cShip::getShipRotation()
{
	return this->shipRotation;
}

/*
=================================================================
Defualt Destructor
=================================================================
*/
cShip::~cShip()
{
}
