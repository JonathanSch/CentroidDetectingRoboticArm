#include "ImageProcess.h"
#include <math.h>
#include <stdio.h>



extern int BALL_CURRENT_X;
extern int BALL_CURRENT_Y;
extern int BALL_NEW_X;
extern int BALL_NEW_Y;
extern int Dx;
extern int Dy;

//this function takes in an array of pixels from the camera and finds the center of the ball. It will place
//the current coordinates of the center of the ball into global variables BALL_CURRENT_X and BALL_CURRENT_Y
//the coordinates themselves are in terms of PIXELS. It will let you know which PIXEL is the center.
void calculateCenter(uint8_t* buffer, int IMAGE_ROWS, int IMAGE_COLS, int THRESHOLD)
{
    int count = 0;
    float ballX = 0;
    float ballY = 0;
    //Find centroid

        // Iterate through the 1D array and calculate centroid
    for (int i = 0; i < IMAGE_ROWS * IMAGE_COLS; i++) {
        int row = i / IMAGE_COLS;  // Calculate row index
        int col = i % IMAGE_COLS;  // Calculate column index

        if (buffer[i] < THRESHOLD) {  // Check if pixel meets the threshold
            ballX += col;  // Accumulate x-coordinate
            ballY += row;  // Accumulate y-coordinate
            count++;        // Increment pixel count

        }
    }

    //ball is not detected in frame, set the X and Y coords back to the middle
    if(count == 0)
    {

        BALL_NEW_X = 0;
        BALL_NEW_Y = 0;
    }
    else
    {
        BALL_NEW_X = ballX / count;
        BALL_NEW_Y = ballY / count;
    }
    //	char msg[100];

    //	sprintf(msg, "Old CurrX%d\n\r", BALL_CURRENT_X);
    //	print_msg(msg);

}

//this function checks the global variables BALLCURRENT XY and BALLNEW XY and finds the difference between them. It will update the global variables Dx and DY
//accordingly, which will be sent as parameters to the motor code, telling the motors how fast to move.
void calculateDxDy()
{
//	char msg[100];

//	  sprintf(msg, "Old CurrX%d\n\r", BALL_CURRENT_X);
//	  print_msg(msg);
      Dx = BALL_NEW_X - BALL_CURRENT_X;
      Dy = BALL_NEW_Y - BALL_CURRENT_Y;
    //update the current coordinates with the new coordinates
    BALL_CURRENT_X = BALL_NEW_X;
    BALL_CURRENT_Y = BALL_NEW_Y;

//	  sprintf(msg, "New CurrX%d, Dx: %d\n\r", BALL_CURRENT_X,Dx);
//	  print_msg(msg);
}

double getTheta(int x, int y)
{
    double angle = atan2(y, x);
    return angle;
}

double getPhi(int x, int y, int z)
{
    double R = getR(x, y, z);
    double doublez = (double)z;
    double phi = acos(doublez / R);
    return phi;
}

double getR(int x, int y, int z)
{
    //cast xyz to doubles
    double R = sqrt(x * x + y * y + z * z);
    return R;
}
  //Now We calcultate the current postion of the ball.

//   float dist = getDistanceUltrasonic(); //Wait for this function to be done
//   float cameraTheta = getCameraTheta(); //Has to return in radians

//   float cameraPhi = getCameraPhi(); // Return in radians

//   float focalLength = IMG_COLS/(2.0* tan(65.0 * M_PI / 360.0)); // change the 65

//   float dx = (IMG_COLS/2) - ballX;
//   dx = (dx * dist) / focalLength;

//   float dy = (IMG_ROWS/2) - ballY;
//   dy = (dy * dist) / focalLength;

//   float thetaPrime = atan(dx/dist);
//   float theta = thetaPrime + cameraTheta;

//   float phiPrime = atan(dy/dist);
//   float phi = cameraPhi + phiPrime;

//  sprintf(msg, "R:%d,Theta:%d,Phi:%d\n", dist, theta, phi);
