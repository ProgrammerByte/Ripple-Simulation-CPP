//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
#include "maths.h"
#include "Graphics2D.h"

#include <vector>
#include <iostream>

using namespace std;

Graphics2D engine = Graphics2D(720, 720, "Ripple Simulation", false);
const int tileCount = 50;

const int halfWaveLength = 5;
const double waveSpeed = (double)halfWaveLength / PI;

vector<vector<int>> epicenter;
vector<double> durations;
vector<int> tempVec;

bool justPressed = false;

//for now the coordinates system goes from (-aspectRatio, -1) to (aspectRatio, 1)

void input() {
	if (engine.mouseButtonPress(GLFW_MOUSE_BUTTON_1) == true) {
		if (justPressed == false) {
			double xPos, yPos;
			int xCoord, yCoord;
			engine.getMousePos(&xPos, &yPos);
			if (xPos >= -1 && xPos < 1 && yPos >= -1 && yPos < 1) {
				xCoord = ((double)(xPos + 1) / 2) * tileCount;
				yCoord = ((double)(yPos + 1) / 2) * tileCount;
				tempVec = { xCoord, yCoord };
				epicenter.push_back(tempVec);
				durations.push_back(0);
			}
			justPressed = true;
		}
	}
	else {
		justPressed = false;
	}
}



int main() {
	double tileColour[tileCount][tileCount]; //between -1 and 1
	for (int i = 0; i < tileCount; i++) {
		for (int x = 0; x < tileCount; x++) {
			tileColour[i][x] = 0;
		}
	}

	const int phaseDifferenceFactor = 4;

	double tileWidth = (double)(2 * engine.getAspectRatio()) / tileCount;
	double halfTileWidth = (double)tileWidth / 2;
	double currentDist, currentColour, changeY, changeX, yPos, xPos, negXPos, negYPos;

	

	while (engine.shouldClose() == false) {
		for (int a = epicenter.size() - 1; a >= 0; a--) { //performs all calculations for tiles
			double durExp = exp(-(double)durations[a] / 20);
			if (durExp >= 0.01) {
				for (int x = 0; x < tileCount; x++) {
					for (int y = 0; y < tileCount; y++) {
						changeX = epicenter[a][0] - x;
						changeY = epicenter[a][1] - y;
						currentDist = root(changeX * changeX + changeY * changeY, 2, 100);
						//CHECK IF WAVE HAS TRAVELLED FAR ENOUGH FOR THE TILE TO BE CONSIDERED HERE
						if (durations[a] * waveSpeed >= currentDist) {
							tileColour[x][y] += 0.9 * durExp * exp(-(double)currentDist / 10) * sin(((double)(PI * currentDist) / halfWaveLength) - durations[a]); //1 full cycle covers 10 units in 2PI time -> 5 units in PI time -> 5 / PI units every 1 duration
						}
					}
				}
				durations[a] += 0.15;
			}
			else {
				durations.erase(durations.begin() + a);
				epicenter.erase(epicenter.begin() + a);
			}
		}


		for (int i = 0; i < tileCount; i++) { //renders tiles
			for (int x = 0; x < tileCount; x++) {
				currentColour = (double)(tileColour[i][x] + 1) / 2;
				engine.setFillColour(0, currentColour, currentColour, 1);
				engine.rect((i * tileWidth) - 1, (x * tileWidth) - 1, tileWidth, tileWidth);
				tileColour[i][x] = 0;
			}
		}

		input();
		engine.clear();
	}
}