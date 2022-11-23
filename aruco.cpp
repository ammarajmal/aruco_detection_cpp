// OpenCv 4.2.0
#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/aruco.hpp"

#include <sstream>
#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;
int markNum(1);
int waitTime(50);
const float calibrationSquareDimension = 0.02245f; // meters
const float arucoSquareDimention = 0.02f; // meters
const Size chessboardDimensions = Size(6,9);

void createKnownBoardPosition(Size boardSize, float squareEdgeLength, vector<Point3f>& corners){
    for (int i = 0; i < boardSize.height; i++) {
        for (int j = 0; j < boardSize.width; j ++) {
            corners.push_back(Point3f( j * squareEdgeLength, i * squareEdgeLength, 0.0f ));
        }
    }
}

void getChessboardCorners(vector<Mat> images, vector<vector<Point2f>>& allFoundCorners, bool showResults = false){
    for  (vector<Mat>::iterator iter = images.begin(); iter != images.end(); iter++){
        vector<Point2f> pointBuf;
        bool found = findChessboardCorners(*iter, Size(9,6), pointBuf, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE);
        
        if (found){
            allFoundCorners.push_back(pointBuf);
        }
        if (showResults){
            drawChessboardCorners(*iter, Size(9,6), pointBuf, found);
            imshow("Look for Corners", *iter);
            waitKey(0);
        }
    }
}

Mat markerGenerator(int markNum){
    Mat markerImage;
    Ptr<aruco::Dictionary> dictionary = aruco::getPredefinedDictionary(aruco::DICT_4X4_1000);
    aruco::drawMarker(dictionary, markNum, 200, markerImage, 1);
    imwrite("marker0.png", markerImage);
    return markerImage;
}

void createArucoMarkers(){
    Mat outputMarker;
    Ptr<aruco::Dictionary> markerDictionary = aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME::DICT_4X4_50);
    for (int i = 0; i<50; i++){
        aruco::drawMarker(markerDictionary, i, 500, outputMarker, 1);
        ostringstream convert;
        string imageName = "Marker_4x4_";
        convert << imageName << i << ".jpg";
        imwrite(convert.str(), outputMarker);
    }
}



void markerDetector(int camera){
    VideoCapture inputVideo;
    inputVideo.open(camera);
    Ptr<aruco::Dictionary> dictionary = aruco::getPredefinedDictionary(aruco::DICT_4X4_100);
    while (inputVideo.grab()) {
    Mat image, imageCopy;
    inputVideo.retrieve(image);
    image.copyTo(imageCopy);
    vector<int> ids;
    vector<vector<Point2f> > corners;
    aruco::detectMarkers(image, dictionary, corners, ids);
    
    // if at least one marker detected
    if (ids.size() > 0)
            aruco::drawDetectedMarkers(imageCopy, corners, ids);
    imshow("out", imageCopy);
    char key = (char) cv::waitKey(50);
    if (key == 'q')
        break;
    }
}


int main(int, char**)
{
    Mat frame;
    Mat drawToFrame;
    Mat cameraMatrix = Mat::eye(3, 3, CV_64F);
    Mat distanceCoefficients;
    vector<Mat> savedImages;
    vector<vector<Point2f>> markerCorners, rejectedCandidates;
    VideoCapture vid(2);
    if (!vid.isOpened()){
        return 0;

    }
    int framesPerSecond = 30;
    namedWindow("webcam", WINDOW_AUTOSIZE);
    while (true){
        if (!vid.read(frame)){
            break;
        }
        vector<Vec2f> foundPoints;
        bool found = false;

        found = findChessboardCorners(frame, chessboardDimensions, foundPoints, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE | CALIB_CB_FAST_CHECK);
        frame.copyTo(drawToFrame);
        drawChessboardCorners(drawToFrame, chessboardDimensions, foundPoints, found);
        if (found){
            imshow("webcam", drawToFrame);
        }
        else{
            imshow("webcam", frame);
        }
        char character = waitKey(1000/framesPerSecond);
    }
    // createArucoMarkers();
    // markerGenerator(0);
    // markerDetector(2);
    return 0;
}
