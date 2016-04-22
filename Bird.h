#ifndef BIRD_H
#define BIRD_H

//-----------------
//Created by Craig Loewen on February 02 2016
//-----------------


#include "CNeuralNet.h"


struct obstacle {
 
	int xpos; 
	int holeTopy;
	int holeBottomy;
	int id;

	obstacle(int starty, int length) {

		xpos = CParams::WindowWidth;
		holeTopy = starty;
		holeBottomy = starty + length;
		id = 0;

	}

	void randomize() {

		holeTopy = RandFloat()*(CParams::randomObstacleWidth) + CParams::topLimit;
		holeBottomy = holeTopy + CParams::iObstacleLength;

	}


};


class bird {

public: 

	CNeuralNet brain;

	double ySpeed;
	double xSpeed;

	double yPosition; 
	double xPosition;

	double fitness;

	double yThrust;
	double xThrust;

	int lastObstacle;
	int timeSinceFlap;
	bool alive; 

	vector<double> inputs;

	//Functions

	bird();

	bool update(vector<obstacle> &obstacleList);

	int findNextObstacle(vector<obstacle> &obstacleList);


	void reset();

	void incrementFitness() { fitness++; }

	int getFitness() { return fitness; }

	void PutWeights(vector<double> &w) { brain.PutWeights(w); }

	int GetNumberOfWeights()const { return brain.GetNumberOfWeights(); }




};


#endif
