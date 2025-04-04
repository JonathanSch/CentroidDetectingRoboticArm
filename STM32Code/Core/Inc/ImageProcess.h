#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H

#include "main.h"
#include "config.h"

// Function declarations
void calculateCenter(uint8_t *buffer, int IMAGE_ROWS, int IMAGE_COLS, int THRESHOLD);

void calculateDxDy();

double getTheta(int x, int y);
double getPhi();
double getR(int x, int y, int z);

float getDistanceUltrasonic();
float getCameraTheta();

#endif

