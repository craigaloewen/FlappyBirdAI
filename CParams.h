#ifndef CPARAMS_H
#define CPARAMS_H

//This file was adapted from one provided by AI-Junkie.com
//Adapted by: Craig Loewen on February 02 2016

//------------------------------------------------------------------------
//
//	Name: CParams.h
//
//  Author: Mat Buckland 2002
//
//  Desc: Class to hold all the parameters used in this project. The values
//        are loaded in from an ini file when an instance of the class is
//        created.
//       
//
//------------------------------------------------------------------------
#include <fstream>
#include <windows.h>

using namespace std;	



class CParams
{

public:

  //------------------------------------general parameters
  static double dPi;
  static double dHalfPi;
  static double dTwoPi;

  static int    WindowWidth;
  static int    WindowHeight;

  static int    iFramesPerSecond;

  
  //-------------------------------------used for the neural network
  static int    iNumInputs;
  static int    iNumHidden;
  static int    iNeuronsPerHiddenLayer;
  static int    iNumOutputs;

  //for tweeking the sigmoid function
  static double dActivationResponse;
  //bias value
  static double dBias;

  //--------------------------------------used to define the sweepers

  //limits how fast the sweepers can turn
  static double dMaxTurnRate;

  static double dMaxSpeed;

  //for controlling the size
  static int   iBirdScale;


  //--------------------------------------controller parameters
  static int    iNumBirds;

  static int    iNumObstacles;

  //number of time steps we allow for each generation to live
  static int    iNumTicks;

  //scaling factor for mines
  static double dMineScale;

  //---------------------------------------GA parameters
  static double dCrossoverRate;
  static double dMutationRate;

  //the maximum amount the ga may mutate each weight by
  static double dMaxPerturbation;

  //used for elitism
  static int    iNumElite;
  static int    iNumCopiesElite;

  static double gravity;
  static int iObstacleLength;
  static int borderWidth;
  static int birdSize;
  static int windowConstant;

  static int topLimit;
  static int bottomLimit;
  static int randomObstacleWidth;
  
  static int flapRate;
  static double flapForce; 

  //ctor
  CParams()
  {
    if(!LoadInParameters("params.ini"))
    {
      MessageBox(NULL, "Cannot find ini file!", "Error", 0);
    }

  }

  bool LoadInParameters(char* szFileName);
};






#endif