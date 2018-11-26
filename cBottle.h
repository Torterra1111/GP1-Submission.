/*
=================
- cBottle.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#pragma once
#include "cSprite.h"
#include <random>

class cBottle :	public cSprite
{
private:
	MapRC bottlePos;
	float bottleRotation;
	/* Let the computer pick a random number */
	random_device rd;    // non-deterministic engine 
	mt19937 gen{ rd() }; // deterministic engine. For most common uses, std::mersenne_twister_engine, fast and high-quality.
	uniform_int_distribution<> spriteRandomR{ 0, 9 }; //THIS IS FOR THE Y DIRECTION.
	uniform_int_distribution<> spriteRandomC{ 0, 11 };//THIS IS FOR THE X DIRECTION.

public:
	cBottle();
	virtual ~cBottle();
	void update(int row, int column);
	void setMapPosition(int row, int column);
	MapRC getMapPosition();
	void setBottleRotation(float angle);
	float getBottleRotation();
	void genRandomPos(int row, int column);
};

