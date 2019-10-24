#include <iostream>
#include <fstream>
#include <cmath>
#include <chrono>
#include <sstream>
#include <cstdlib>
#include <map>
#include <vector>
#include <random>

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

    // create 2D array to hold the calculation results
    int** imageArray = new int*[imageSize];
    for(int i = 0; i < imageSize; i++)
        imageArray[i] = new int[imageSize];

    // create vectors of possible X and Y coordinates for the points
    vector<int> pointsX;
    vector<int> pointsY;

    // load the vectors with all possible X and Y values
    for(int i=0; i<imageSize; i++) {
        pointsX.push_back(i);
        pointsY.push_back(i);
    }

    // shuffle the coordinate vectors
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle(pointsX.begin(), pointsX.end(), default_random_engine(seed));
    shuffle(pointsY.begin(), pointsY.end(), default_random_engine(seed));

    // fill the points with random coordinates
    int** points = new int*[numPoints];
    for(int i = 0; i < numPoints; i++)
        points[i] = new int[2] {pointsX[i], pointsY[i]};

    // create maps to hold the RGB values of each point
    map<int, int> pointRed;
    map<int, int> pointGreen;
    map<int, int> pointBlue;

    // set the final entry to white to color the points against the cells
    pointRed[numPoints] = 0;
    pointGreen[numPoints] = 0;
    pointBlue[numPoints] = 0;

    // populate maps with random colors
    for (int i=0; i<numPoints; i++) {
        pointRed[i] = rand() % 256;
        pointGreen[i] = rand() % 256;
        pointBlue[i] = rand() % 256;
    }

    // loop through each cell, compare to each point and assign cell to closest point
    for (int i=0; i<imageSize; i++) {
        for (int j=0; j<imageSize; j++) {
            double minDistance = DBL_MAX;
            int minPoint = -1;
            for (int k=0; k<numPoints; k++) {
                double distance = sqrt(pow(j - points[k][1], 2) + pow(i - points[k][0], 2));
                if (distance < minDistance) {
                    minDistance = distance;
                    minPoint = k;
                }
            }
            imageArray[i][j] = minPoint;
        }
    }

    // loop through points and set color to white (numPoints)
    for (int i=0; i<numPoints; i++) {
        imageArray[points[i][1]][points[i][0]] = numPoints;
    }

    // TODO write points to file by pulling colors from maps
    ofstream output("C:/Users/jeffp/CLionProjects/HPC3/output.ppm", std::ios_base::binary);

    // store the image header in our output file
    output << "P6" << "\n";
    output << imageSize << " " << imageSize << "\r\n";
    output << 255 << "\r\n";

    // store the updated array in our output file
    for(int row = 0; row < imageSize; ++row) {
        for(int col = 0; col < imageSize; ++col) {
            output << pointRed[imageArray[row][col]] << " " << pointGreen[imageArray[row][col]] << " " << pointBlue[imageArray[row][col]] << " ";
        }
        output << "\r\n";
    }




    // TODO delete arrays from memory
    return 0;
}