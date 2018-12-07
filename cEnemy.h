#pragma once
#include "cSprite.h"
#include <random>

class cEnemy : 	public cSprite
{
private:
	MapRC enemyPos;
	float enemyRotation;

	/* ===========================================================
	   "In mathematics, the sign function or signum function
	   (from signum, Latin for "sign") is an odd mathematical
	   function that extracts the sign of a real number.
	   In mathematical expressions the sign function is often
	   represented as sgn."

	   https://en.wikipedia.org/wiki/Sign_function
	   =========================================================== */


	// https://helloacm.com/how-to-implement-the-sgn-function-in-c/
	template <class T>
	inline int sgn(T v) {
		return (v > T(0)) - (v < T(0));
	}

	/* Let the computer pick a random number */
	random_device rd;    // non-deterministic engine 
	mt19937 gen{ rd() }; // deterministic engine. For most common uses, std::mersenne_twister_engine, fast and high-quality.
	uniform_int_distribution<> spriteRandom{ 0, 9 };

public:
	cEnemy();
	virtual ~cEnemy();
	void update(int row, int column);
	void moveEnemy(int row, int column);
	void setMapPosition(int row, int column);
	MapRC getMapPosition();
	void setEnemyRotation(float angle);
	float getEnemyRotation();
	void genRandomPos(MapRC ship, MapRC bottle);
	bool turn;
};

