#include "NNVisualizer.h"


//-----------------
//Created by Craig Loewen on February 02 2016
//-----------------

NNVisualizer::NNVisualizer()
{

	windowTopX = 200;
	windowTopY = CParams::borderWidth + 20;
	windowWidth = CParams::WindowWidth - 50 - windowTopY;
	windowHeight = CParams::bottomLimit - 10 - windowTopY;
	inputStringList[0] = "Dist.Obst.Top:";
	inputStringList[1] = "Dist.Obst.Bot:";
	inputStringList[2] = "Flap Time:";
	inputStringList[3] = "Dist to Top:";
	inputStringList[4] = "Dist to Bottom:";
	inputStringList[5] = "Vertical Speed:";

	inputColorWeightList[0] = 100; 
	inputColorWeightList[1] = 100;
	inputColorWeightList[2] = 100;
	inputColorWeightList[3] = 100;
	inputColorWeightList[4] = 100;
	inputColorWeightList[5] = 0.5; 


}

double NNVisualizer::map(double inval, double inStart, double inEnd, double newStart, double newEnd)
{
	double storeVal = (inval - inStart) * (newEnd - newStart) / (inEnd - inStart) + newStart;
	if (storeVal > newEnd) {
		storeVal = newEnd;
	}
	if (storeVal < newStart) {
		storeVal = newStart;
	}
	return storeVal;
}

void NNVisualizer::render(bird &inBird, HDC surface) {

	CNeuralNet* inBrain = &(inBird.brain);

	SelectObject(surface, GetStockObject(BLACK_PEN));
	SelectObject(surface, GetStockObject(DC_BRUSH));

	SetDCBrushColor(surface, RGB(100, 200, 0));

	vector<double> nodeXList;
	vector<double> nodeYList;

	double xPos, yPos;

	double horizSlice = windowWidth / (inBrain->m_NumHiddenLayers + 2);

	double vertSlice;

	//Display Inputs
	vertSlice = windowHeight / inBrain->m_NumInputs / 2;
	
	for (int i = 0; i < inBrain->m_NumInputs; i++) {
		xPos = windowTopX+35;

		yPos = vertSlice * 2 * i + vertSlice;

		nodeXList.push_back(xPos+10);
		nodeYList.push_back(yPos + 4);

		if (inBird.inputs.size() > 0) {
			if (inBird.inputs[i] >= 0) {
				SetDCBrushColor(surface, RGB(255, 255 - map(inBird.inputs[i], 0, inputColorWeightList[i], 0, 255), 255));
			}
			else {
				SetDCBrushColor(surface, RGB(255 - map(inBird.inputs[i], 0, 0 - inputColorWeightList[i], 0, 255), 255, 255));
			}
			
			string s = itos(inBird.inputs[i]);
			if (i == 5) {
				s = itos(inBird.inputs[i] * 100);
			}
			TextOut(surface, xPos - 35, yPos - 4, s.c_str(), s.size());
		}

		Rectangle(surface, xPos, yPos, xPos + 10, yPos + 10);

		TextOut(surface, xPos-135, yPos-4, inputStringList[i].c_str(), inputStringList[i].size());


	}

	//Display Hidden Layers

	for (int layerIterator = 0; layerIterator < inBrain->m_vecLayers.size(); layerIterator++) {

		vertSlice = windowHeight / inBrain->m_vecLayers[layerIterator].m_NumNeurons / 2;

		for (int i = 0; i < inBrain->m_vecLayers[layerIterator].m_NumNeurons; i++) {
			xPos = windowTopX + (layerIterator+1)*horizSlice;

			yPos = vertSlice * 2 * i + vertSlice;

			nodeXList.push_back(xPos+10);
			nodeYList.push_back(yPos+4);


			double colorVal = inBrain->m_vecLayers[layerIterator].m_vecNeurons[i].storedVal;
			SetDCBrushColor(surface, RGB(255, 255 - map(colorVal, 0, 1, 0, 255), 255));

			

			//If OutputBlock
			if (layerIterator == (inBrain->m_vecLayers.size() - 1) && i == (inBrain->m_vecLayers[layerIterator].m_NumNeurons - 1)) {
					SetDCBrushColor(surface, RGB(255, 255 - map(colorVal, 0.4, 0.6, 0, 255), 255 - map(colorVal, 0.4, 0.6, 0, 255)));
			}
			Rectangle(surface, xPos, yPos, xPos + 10, yPos + 10);

			HPEN oldPen;
			HPEN usePen = CreatePen(0, 0, RGB(0, 0, 0));
			oldPen = (HPEN)SelectObject(surface, usePen);

			DeleteObject(usePen);

			for (int weightIt = 0; weightIt < inBrain->m_vecLayers[layerIterator].m_vecNeurons[i].m_NumInputs-1; weightIt++) {
				double neuronWeightValue = inBrain->m_vecLayers[layerIterator].m_vecNeurons[i].m_vecWeight[weightIt];
				if (neuronWeightValue >= 0) {
					usePen = CreatePen(0, 2.5*neuronWeightValue, RGB(255, 255 - 255 * neuronWeightValue, 255));
				} else {
					neuronWeightValue = 0 - neuronWeightValue;
					usePen = CreatePen(0, 2.5*neuronWeightValue, RGB(255 - 255 * neuronWeightValue, 255, 255));
				}
				SelectObject(surface, usePen);
				
				int selectDrawVal = weightIt;

				if (layerIterator > 0) {

					selectDrawVal += (layerIterator-1)*CParams::iNeuronsPerHiddenLayer + CParams::iNumInputs;

				}

				


				MoveToEx(surface, xPos, yPos+4, NULL);
				LineTo(surface, nodeXList[selectDrawVal], nodeYList[selectDrawVal]);
				DeleteObject(usePen);
			}

			SelectObject(surface, oldPen);
			DeleteObject(oldPen);

		}
	}






	

}

