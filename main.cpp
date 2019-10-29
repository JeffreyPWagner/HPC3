#include <iostream>
#include <fstream>
#include <cmath>
#include <chrono>
#include <cstdlib>
#include <map>
#include <vector>
#include <random>
#include <algorithm>
#include <cfloat>
#include <sys/time.h>

using namespace std;
int main (int argc, char *argv[]) {

    // check to make sure we have correct number of arguments
    if (argc != 3) {
        cerr<< "invalid number of arguments" << "\n";
        exit (-1);
    }

    // get the image size and number of points from the arguments
    int imageSize = atoi(argv[1]);
    int numPoints = atoi(argv[2]);

    // create array to hold the calculation results
    int* imageArray = new int[imageSize * imageSize];

    // create vectors of possible X and Y coordinates for the points
    vector<int> pointsX;
    vector<int> pointsY;

    // load the vectors with all possible X and Y values
    for(int i=0; i<imageSize; i++) {
        pointsX.push_back(i);
        pointsY.push_back(i);
    }

    // shuffle the coordinate vectors
    unsigned seedX = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle(pointsX.begin(), pointsX.end(), default_random_engine(seedX));
    unsigned seedY = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle(pointsY.begin(), pointsY.end(), default_random_engine(seedY));

    // create an array to hold the points and fill it with random coordinates
    int* points = new int[numPoints + numPoints];
    for(int i = 0; i < numPoints; i++) {
        points[i] = pointsX[i];
        points[i + numPoints] = pointsY[i];
    }

    // create maps to hold the RGB values of each point
    map<int, int> pointRed;
    map<int, int> pointGreen;
    map<int, int> pointBlue;

    // set the final entry to white to color the points against the cells
    pointRed[numPoints] = 255;
    pointGreen[numPoints] = 255;
    pointBlue[numPoints] = 255;

    // populate maps with random colors
    for (int i=0; i<numPoints; i++) {
        pointRed[i] = rand() % 256;
        pointGreen[i] = rand() % 256;
        pointBlue[i] = rand() % 256;
    }

    // start timing for the calculation
    struct timeval start, end;
    gettimeofday(&start, NULL);

    // loop through each cell, compare them to each point and assign the cell's value to closest point
    for (int x=0; x<imageSize * imageSize; x++) {
        double minDistance = DBL_MAX;
        int minPoint = -1;
        for (int k=0; k<numPoints; k++) {
            double distance = sqrt(pow((double) (x % imageSize - points[k + numPoints]), 2.0) + pow((double) (x / imageSize - points[k]), 2.0));
            if (distance < minDistance) {
                minDistance = distance;
                minPoint = k;
            }
        }
        imageArray[x] = minPoint;
    }

    // end timing and print processing time
    gettimeofday(&end, NULL);
    long seconds = (end.tv_sec - start.tv_sec);
    long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
    printf("Processing time elpased is %zu seconds or %zu micros\n", seconds, micros);

    // loop through the points and set color to white so they are visible against the colored cells
    for (int i=0; i<numPoints; i++) {
        imageArray[points[i + numPoints] + imageSize * points[i]] = numPoints;
    }

    // store the image header in our output file
    ofstream output("output.ppm", ios_base::binary);
    output << "P3" << "\n";
    output << imageSize << " " << imageSize << "\r\n";
    output << "255" << "\r\n";

    // store the image in our output file
    for(int i = 0; i < imageSize * imageSize; i++) {
        output << pointRed[imageArray[i]] << " " << pointGreen[imageArray[i]] << " " << pointBlue[imageArray[i]] << " ";
        output << "\r\n";
    }
    output.close();

    // delete arrays from memory
    delete[] imageArray;
    delete[] points;

    return 0;
}