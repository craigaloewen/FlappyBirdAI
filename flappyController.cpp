#include "flappyController.h"

//-----------------
//Created by Craig Loewen on February 02 2016
//-----------------

flappyController::flappyController(HWND hwndMain)
{
	m_bFastRender = false;
	m_showAIBrain = false;
	birdsAlive = true;
	numAlive = CParams::iNumBirds;
	winHandle = hwndMain;
	generations = 0;
	lastDeathFitness = 0;
	currentObstacleNum = 0;

	redBrush = CreateSolidBrush(RGB(255, 0, 0));
	blueBrush = CreateSolidBrush(RGB(0, 0, 255));

	redPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	bluePen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));

	oldPen = NULL;

	bestFitnessRecordVal = 0;


	for (int i = 0; i < CParams::iNumBirds; i++) {
		birdList.push_back(bird());
		aliveList.push_back(i);
	}

	for (int i = 0; i < CParams::iNumObstacles; i++) {
		obstacleList.push_back(obstacle(RandFloat()*(CParams::WindowHeight-CParams::iObstacleLength),CParams::iObstacleLength));
	}

	initializeObstacles();

	weightsInNN = birdList[0].GetNumberOfWeights();

	genAlg_p = new CGenAlg(CParams::iNumBirds, CParams::dMutationRate, CParams::dCrossoverRate, weightsInNN);

	chromoPopulation = genAlg_p->GetChromos();

	for (int i = 0; i < CParams::iNumBirds; i++) {

		birdList[i].PutWeights(chromoPopulation[i].vecWeights);
	}


}

flappyController::~flappyController()
{
	if (genAlg_p) {
		delete genAlg_p;
	}



}

bool flappyController::initializeObstacles() {

	currentObstacleNum = 0;

	int obstacleSpacing = CParams::WindowWidth / CParams::iNumObstacles;

	for (int i = 0; i < CParams::iNumObstacles; i++) {

		obstacleList[i].randomize();
		obstacleList[i].xpos = CParams::WindowWidth + obstacleSpacing*i;
		obstacleList[i].id = currentObstacleNum;
		currentObstacleNum++;

	}

	return true;

}

bool flappyController::updateObstacles()
{

	for (int i = 0; i < CParams::iNumObstacles; i++) {

		obstacleList[i].xpos--;

		if (obstacleList[i].xpos <= 0) {
			
			obstacleList[i].randomize();
			obstacleList[i].xpos = CParams::WindowWidth;
			obstacleList[i].id = currentObstacleNum;
			currentObstacleNum++;

		}

	}

	return true;
}




bool flappyController::updateBirdsAlive() {

	if (aliveList.size() > 0) {
		birdsAlive = true;
	}
	else {
		birdsAlive = false; 
	}

	return birdsAlive;

}

bool flappyController::removeBirdFromAliveList(vector<int>::iterator &aliveIterator) {

	numAlive--;
	lastDeathFitness = birdList[(*aliveIterator)].fitness;
	aliveIterator = aliveList.erase(aliveIterator);

	return true;


}


// Necessary outward facing functions
void flappyController::Render(HDC surface)
{

	if (aliveList.size() > 0 && m_showAIBrain) {
		visualizer_NN.render(birdList[aliveList[0]], surface);
	}

	string s = "Generation:          " + itos(generations);
	TextOut(surface, CParams::WindowWidth-200, 5, s.c_str(), s.size());
	//do not render if running at accelerated speed
	if (!m_bFastRender) {

		SelectObject(surface, redBrush);

		for (int i = 0; i < aliveList.size(); i++) {


			Rectangle(surface, birdList[aliveList[i]].xPosition, birdList[aliveList[i]].yPosition, birdList[aliveList[i]].xPosition+CParams::birdSize, birdList[aliveList[i]].yPosition+CParams::birdSize);
		}

		//Render Obstacle
		for (int i = 0; i < CParams::iNumObstacles; i++) {

			s = itos(obstacleList[i].id);
			TextOut(surface, obstacleList[i].xpos + 10, CParams::topLimit, s.c_str(), s.size());

			

			Rectangle(surface, obstacleList[i].xpos, 0, obstacleList[i].xpos + 10, obstacleList[i].holeTopy);
			Rectangle(surface, obstacleList[i].xpos, obstacleList[i].holeBottomy, obstacleList[i].xpos + 10, CParams::WindowHeight);
		}

		Rectangle(surface, 0, 0, CParams::WindowWidth, CParams::borderWidth);
		Rectangle(surface, 0, CParams::bottomLimit, CParams::WindowWidth, CParams::WindowHeight);

	}
	else {


		if (!m_showAIBrain) {
			plotStats(surface);
		}

	}



}

void flappyController::plotStats(HDC surface) {

	string s = "Last Generation Best Fitness:       " + ftos(genAlg_p->BestFitness());
	TextOut(surface, 5, 20, s.c_str(), s.size());

	s = "Last Generation Average Fitness: " + ftos(genAlg_p->AverageFitness());
	TextOut(surface, 5, 40, s.c_str(), s.size());

	s = "Record Fitness: " + ftos(bestFitnessRecordVal);
	TextOut(surface, 5, 60, s.c_str(), s.size());

	s = "Birds Alive: " + ftos(aliveList.size());
	TextOut(surface, 5, 80, s.c_str(), s.size());

	//render the graph
	float HSlice = (float)CParams::WindowWidth / (generations + 1);
	float VSlice = (float)CParams::bottomLimit / ((bestFitnessRecordVal + 1) * 2);

	//plot the graph for the best fitness
	float x = 0;

	oldPen = (HPEN)SelectObject(surface, redPen);
	



	MoveToEx(surface, 0, CParams::WindowWidth, NULL);

	for (int i = 0; i<bestFitnessLog.size(); ++i)
	{
		LineTo(surface, x, CParams::bottomLimit - VSlice*bestFitnessLog[i]);

		x += HSlice;
	}

	//plot the graph for the average fitness
	x = 0;

	SelectObject(surface, bluePen);

	MoveToEx(surface, 0, CParams::bottomLimit, NULL);

	for (int i = 0; i<avgFitnessLog.size(); ++i)
	{
		LineTo(surface, (int)x, (int)(CParams::bottomLimit - VSlice*avgFitnessLog[i]));

		x += HSlice;
	}

	//replace the old pen
	SelectObject(surface, oldPen);

	

}

bool flappyController::Update()
{
	if (updateBirdsAlive()) {

		updateObstacles();

		vector<int>::iterator aliveIt;

		for (aliveIt = aliveList.begin(); aliveIt != aliveList.end(); ) {

			if (!(birdList[(*aliveIt)].update(obstacleList))) {
				MessageBox(winHandle, "Wrong # inputs!", "Error", MB_OK);
				return false;
			}

			if (!birdList[(*aliveIt)].alive) {

				if (!removeBirdFromAliveList(aliveIt)) {
					MessageBox(winHandle, "Error removing bird", "Error", MB_OK);
					return false;
				}

			}
			else {
		
				aliveIt++;

			}

		}


	}
	// New Generation
	else {

		newGeneration();





	}

	return true;



}

void flappyController::resetSim() {

	for (int i = 0; i < CParams::iNumBirds; i++) {
		birdList[i].reset();
	}

	initializeObstacles();
	aliveList.clear();

	for (int i = 0; i < CParams::iNumBirds; i++) {
		aliveList.push_back(i);
	}


}

void flappyController::trainSet()
{

	initializeObstacles();

	resetSim();

	obstacleList[0].holeTopy = 100;

	int generationAmount = 10;

	for (int trainNum = 0; trainNum < generationAmount; trainNum++) {

		for (int i = 0; i < CParams::iNumBirds; i++) {

			birdList[i].fitness = 100;
		}

		for (int i = 0; i < CParams::iNumBirds; i++) {

			birdList[i].yPosition = 140;
			birdList[i].ySpeed = 0;

			birdList[i].update(obstacleList);
			birdList[i].fitness -= 0.01;

			if (birdList[i].yThrust <= 0.5) {
				birdList[i].fitness = 0.1;
			}
		}

		/*for (int i = 0; i < CParams::iNumBirds; i++) {

			birdList[i].yPosition = 300;
			birdList[i].ySpeed = 1.5;

			birdList[i].update(obstacleList);

			if (birdList[i].yThrust > 0.5) {
				birdList[i].fitness = 0.1;
			}

		}*/


		/*obstacleList[0].holeTopy = 300;

		for (int i = 0; i < CParams::iNumBirds; i++) {

			birdList[i].yPosition = 100; 
			birdList[i].ySpeed = 0; 

			birdList[i].update(obstacleList);

			if (birdList[i].yThrust > 0.5) {
				birdList[i].fitness = 0;
			}

		}*/

		double fitnessSum = 0; 
		for (int i = 0; i < CParams::iNumBirds; i++) {

			fitnessSum += birdList[i].fitness;
		}
		if (fitnessSum <= 100) {
			//MessageBox(NULL, "All 0!", "Error", 0);
			fitnessSum = 10;
		}

		newGeneration();

	}


	for (int trainNum = 0; trainNum < generationAmount; trainNum++) {

		for (int i = 0; i < CParams::iNumBirds; i++) {

			birdList[i].fitness = 100;
		}

		obstacleList[0].holeTopy = 300;

		for (int i = 0; i < CParams::iNumBirds; i++) {

			birdList[i].yPosition = 100;
			birdList[i].ySpeed = 0;

			birdList[i].update(obstacleList);

			if (birdList[i].yThrust > 0.5) {
				birdList[i].fitness = 0.01;
			}

		}

		double fitnessSum = 0;
		for (int i = 0; i < CParams::iNumBirds; i++) {

			fitnessSum += birdList[i].fitness;
		}
		if (fitnessSum <= 100) {
			//MessageBox(NULL, "All 0!", "Error", 0);
			fitnessSum = 10;
		}



		newGeneration();

	}




}

void flappyController::newGeneration()
{

	generations++;

	for (int i = 0; i < CParams::iNumBirds; i++) {

		chromoPopulation[i].dFitness = birdList[i].fitness;
		birdList[i].fitness = 0;

	}

	bestFitnessLog.push_back(genAlg_p->BestFitness());
	avgFitnessLog.push_back(genAlg_p->AverageFitness());

	if (genAlg_p->BestFitness() > bestFitnessRecordVal) {

		bestFitnessRecordVal = genAlg_p->BestFitness();

	}

	chromoPopulation = genAlg_p->Epoch(chromoPopulation);


	for (int i = 0; i < CParams::iNumBirds; i++) {

		birdList[i].PutWeights(chromoPopulation[i].vecWeights);
	}

	resetSim();

}

