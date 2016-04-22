#include "Bird.h"

//-----------------
//Created by Craig Loewen on February 02 2016
//-----------------

bird::bird()
{
	alive = true;
	xSpeed = 0;
	xPosition = RandFloat()*20+5;
	xThrust = 0;

	ySpeed = 0;
	yPosition = CParams::topLimit + 25 + RandFloat()*(CParams::bottomLimit - CParams::topLimit - 50); //CParams::topLimit + (CParams::bottomLimit - CParams::topLimit) / 2;
	yThrust = 0;

	fitness = 0; 
	lastObstacle = 0;
	
	timeSinceFlap = CParams::flapRate;
	
}

bool bird::update(vector<obstacle> &obstacleList)
{
	
	int closestObstacle = findNextObstacle(obstacleList);


	bool inDangerZone = obstacleList[closestObstacle].xpos <= (xPosition + 20);
	bool belowTop = yPosition >= obstacleList[closestObstacle].holeTopy; 
	bool aboveBottom = (yPosition+CParams::birdSize) <= obstacleList[closestObstacle].holeBottomy;

	bool hittingObstacle = inDangerZone && (!belowTop || !aboveBottom);



	//Check if dead
	if ( hittingObstacle || yPosition <= CParams::topLimit || (yPosition+CParams::birdSize) >= (CParams::bottomLimit)) {
		reset();
		alive = false;
		return true;
	}

	if (closestObstacle != lastObstacle && alive) {
		fitness += 100;
		lastObstacle = closestObstacle;

	}
	
	//Make Decision

	inputs.clear();

	inputs.push_back(yPosition - obstacleList[closestObstacle].holeTopy);
	inputs.push_back(obstacleList[closestObstacle].holeBottomy - yPosition);
	inputs.push_back(timeSinceFlap);
	inputs.push_back(yPosition - CParams::topLimit);
	inputs.push_back(CParams::bottomLimit - yPosition);
	inputs.push_back(ySpeed);

	vector<double> outputs;

	vector<double> inputSave = inputs;
	outputs = brain.Update(inputs);
	inputs = inputSave;

	yThrust = outputs[0];

	if (yThrust >= 0.5 && timeSinceFlap >= CParams::flapRate) {
		ySpeed -= CParams::flapForce;
		timeSinceFlap = 0;
	}


	ySpeed += CParams::gravity;

	//Propagate Speed
	yPosition += ySpeed;


	//Fitter for surviving longer
	fitness += 0.01;
	timeSinceFlap++;

	return true;
}

int bird::findNextObstacle(vector<obstacle>& obstacleList)
{
	vector<obstacle>::iterator vecIterator;

	int closestXDistance = 9999999;
	int obstacleVal = 0;

	for (int i = 0; i < obstacleList.size(); i++) {

		int obstacleDistance = (obstacleList[i].xpos - xPosition);

		if (obstacleDistance > 0) {
			if (obstacleDistance < closestXDistance) {
				closestXDistance = obstacleDistance;
				obstacleVal = i;
			}
		}
	}

	return obstacleVal;
}


void bird::reset()
{
	alive = true;
	xSpeed = 0;
	xPosition = RandFloat() * 20 + 5;
	xThrust = 0;
	lastObstacle = 0;

	ySpeed = 0;
	yPosition = CParams::topLimit + 25 + RandFloat()*(CParams::bottomLimit - CParams::topLimit - 50); //CParams::topLimit + (CParams::bottomLimit-CParams::topLimit)/2;
	yThrust = 0;

	timeSinceFlap = CParams::flapRate;
}
