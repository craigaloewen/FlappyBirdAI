#ifndef FLAPPYCON_H
#define FLAPPYCON_H


//-----------------
//Created by Craig Loewen on February 02 2016
//-----------------

#include "CNeuralNet.h"
#include "CGenAlg.h"
#include "Bird.h"
#include "NNVisualizer.h"



class flappyController {

private:

	HWND winHandle;

	HBRUSH redBrush;
	HBRUSH blueBrush;

	HPEN redPen;
	HPEN bluePen;
	HPEN oldPen;

	NNVisualizer visualizer_NN;

	double bestFitnessRecordVal;

	int generations;

	vector<double> bestFitnessLog;
	vector<double> avgFitnessLog;

	vector<int> aliveList;

	bool birdsAlive;

	int numAlive;

	bool updateBirdsAlive();

	bool initializeObstacles();

	bool updateObstacles();

	bool removeBirdFromAliveList(vector<int>::iterator &aliveIterator);



public:




	bool m_bFastRender;
	bool m_showAIBrain;

	vector<bird> birdList;
	vector<obstacle> obstacleList;
	

	int weightsInNN;

	int currentObstacleNum;

	CGenAlg* genAlg_p;
	vector<SGenome> chromoPopulation;

	double lastDeathFitness;


	flappyController(HWND hwndMain);

	~flappyController();


	void resetSim();

	void trainSet();

	void newGeneration();

	//Necessary outward facing functions
	void		Render(HDC surface);

	bool		Update();

	void plotStats(HDC surface);

	//accessor methods
	bool		FastRender()const { return m_bFastRender; }
	void		FastRender(bool arg) { m_bFastRender = arg; }
	void		FastRenderToggle() { m_bFastRender = !m_bFastRender; }

	void showAIBrainToggle() { m_showAIBrain = !m_showAIBrain; }



};


#endif
