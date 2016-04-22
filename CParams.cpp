#include "CParams.h"

//This file was adapted from one provided by AI-Junkie.com

//because we will always be loading in the settings from an ini file
//we can just initialize everything to zero
double CParams::dPi                 = 3.14159265358979;
double CParams::dHalfPi             = dPi / 2;
double CParams::dTwoPi              = dPi * 2;
int CParams::WindowWidth            = 800;
int CParams::WindowHeight           = 400;
int CParams::iFramesPerSecond       = 120;
int CParams::iNumInputs             = 6;
int CParams::iNumHidden             = 1;
int CParams::iNeuronsPerHiddenLayer = 6;
int CParams::iNumOutputs            = 1;
double CParams::dActivationResponse = 1;
double CParams::dBias               = -1;
double CParams::dMaxTurnRate        = 0.3;
double CParams::dMaxSpeed           = 2;
int CParams::iBirdScale          = 5;
int CParams::iNumBirds           = 50;
int CParams::iNumObstacles              = 2;
int CParams::iNumTicks              = 2000;
double CParams::dMineScale          = 2;
double CParams::dCrossoverRate      = 0.7;//0.7
double CParams::dMutationRate       = 0.3;//0.3
double CParams::dMaxPerturbation    = 0.3;//0.3
int CParams::iNumElite              = 1;
int CParams::iNumCopiesElite        = 1;
double CParams::gravity				= 0.01;
int CParams::iObstacleLength		= 100;

int CParams::borderWidth = 5;
int CParams::birdSize = 10;
int CParams::windowConstant = 40;

int CParams::topLimit = borderWidth;
int CParams::bottomLimit = CParams::WindowHeight - CParams::borderWidth - CParams::windowConstant;
int CParams::randomObstacleWidth = CParams::bottomLimit - CParams::topLimit - CParams::iObstacleLength;

int CParams::flapRate = 50;
double CParams::flapForce = 1.2;


//this function loads in the parameters from a given file name. Returns
//false if there is a problem opening the file.
bool CParams::LoadInParameters(char* szFileName)
{
  
  return true;
}
 




  
  
