/*
=================
- cShip.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cEnemy.h"

/*
=================================================================
Defualt Constructor
=================================================================
*/
cEnemy::cEnemy()
{
	this->setMapPosition(3, 3);
}

void cEnemy::update(int row, int column)
{
	this->setMapPosition(column, row);
}

void cEnemy::moveEnemy(int row, int column)
{
	// New enemy position
	this->setMapPosition(this->enemyPos.R + sgn(row - this->enemyPos.R), this->enemyPos.C + sgn(column - this->enemyPos.C));

}

void cEnemy::setMapPosition(int row, int column)
{
	this->enemyPos = { column, row };
}

MapRC cEnemy::getMapPosition()
{
	return this->enemyPos;
}

void cEnemy::setEnemyRotation(float angle)
{
	this->enemyRotation = angle;
	this->setSpriteRotAngle(angle);
}

float cEnemy::getEnemyRotation()
{
	return this->enemyRotation;
}

void cEnemy::genRandomPos(MapRC ship, MapRC bottle)
{
	this->setMapPosition(spriteRandom(gen), spriteRandom(gen));
	while (this->getMapPosition() == ship && this->getMapPosition() == bottle)
	{
		this->setMapPosition(this->spriteRandom(gen), this->spriteRandom(gen));
	}
}

/*
=================================================================
Defualt Destructor
=================================================================
*/

cEnemy::~cEnemy()
{
}
