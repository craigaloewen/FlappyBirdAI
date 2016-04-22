#ifndef NNVISUALIZER_H
#define NNVISUALIZER_H

#include "Bird.h"

//-----------------
//Created by Craig Loewen on February 02 2016
//-----------------

class NNVisualizer {

public:

	int windowTopX;
	int windowTopY;
	int windowWidth;
	int windowHeight;

	string inputStringList[6];
	double inputColorWeightList[6];

	double map(double inval, double inStart, double inEnd, double newStart, double newEnd);

	NNVisualizer();

	void render(bird & inBird, HDC surface);

};


#endif