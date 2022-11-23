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
        bool found = findChessboardCorners(*iter, Size(9,6), pointBuf, CALIB_CB_ADAPTIVE_THRESH);
        // bool found = findChessboardCorners(*iter, Size(9,6), pointBuf, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE); // for fast speed
        
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

void cameraCalibration(vector<Mat> calibrationImages, Size boardSize, float squareEdgeLength, Mat& cameraMatrix, Mat& distanceCoefficients){
    vector<vector<Point2f>> checkerboardImageSpacePoints;
    getChessboardCorners(calibrationImages, checkerboardImageSpacePoints, false);
    vector<vector<Point3f>> worldSpaceCornerPoints(1);
    createKnownBoardPosition(boardSize, squareEdgeLength, worldSpaceCornerPoints[0]);
    worldSpaceCornerPoints.resize(checkerboardImageSpacePoints.size(), worldSpaceCornerPoints[0]);
    vector<Mat> rVectors, tVectors;
    distanceCoefficients = Mat::zeros(8, 1, CV_64F);
    calibrateCamera(worldSpaceCornerPoints, checkerboardImageSpacePoints, boardSize, cameraMatrix, distanceCoefficients, rVectors, tVectors);
}

// int cameraCalibrationWebcam(){
//     Mat frame;
//     Mat drawToFrame;
//     Mat cameraMatrix = Mat::eye(3, 3, CV_64F);
//     Mat distanceCoefficients;
//     vector<Mat> savedImages;
//     vector<vector<Point2f>> markerCorners, rejectedCandidates;
//     VideoCapture vid(2);
//     if (!vid.isOpened()){
//         return 0;

//     }
//     int framesPerSecond = 30;
//     namedWindow("webcam", WINDOW_AUTOSIZE);
//     while (true){
//         if (!vid.read(frame)){
//             break;
//         }
//         vector<Vec2f> foundPoints;
//         bool found = false;

//         found = findChessboardCorners(frame, chessboardDimensions, foundPoints, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE | CALIB_CB_FAST_CHECK);
//         frame.copyTo(drawToFrame);
//         drawChessboardCorners(drawToFrame, chessboardDimensions, foundPoints, found);
//         if (found){
//             imshow("webcam", drawToFrame);
//         }
//         else{
//             imshow("webcam", frame);
//         }
//         char character = waitKey(1000/framesPerSecond);
//             // 1. Save an image(Space Key) -- if we see a valid checkerboard image
//             // 2. Start Camera clibration (Enter Key) -- if we wanted to start camera calibration so long as we have enough images like 10 or 15
//             // 3. Exit(Escape Key)
//         switch (character) {
//             case ' ' :{
//                 //saving image
//                 if (found){
//                     Mat temp;
//                     frame.copyTo(temp);
//                     savedImages.push_back(temp);
//                 }
//                 break;

//             }
//             case 13:{
//                 // start calibration
//                 if (savedImages.size() > 15) {
//                     cameraCalibration(savedImages, chessboardDimensions, calibrationSquareDimension, cameraMatrix, distanceCoefficients);

//                 }
//                 break;
//             }
            
//             case 27:{
//                 //exit 
//                 return 0;
//                 break;
//             }
            
//         }


//     }

// }

bool saveCameraCalibration(string name, Mat cameraMatrix, Mat distanceCoefficients){
    ofstream outstream(name);
    if (outstream){
        uint16_t rows = cameraMatrix.rows;
        uint16_t columns = cameraMatrix.cols;

        for (int r = 0; r < rows; r++){
            for(int c = 0; c< columns ; c++){
                double value = cameraMatrix.at<double>(r,c);
                outstream << value << endl;

            }
        }
        rows = distanceCoefficients.rows;
        columns = distanceCoefficients.cols;
        for (int r = 0; r < rows; r++){
            for(int c = 0; c< columns ; c++){
                double value = distanceCoefficients.at<double>(r,c);
                outstream << value << endl;

            }
        }
        outstream.close();
        return true;
    }
    return false;
}

int mainCalib()
{
    // cameraCalibrationWebcam();

    // createArucoMarkers();
    // markerGenerator(0);
    // markerDetector(2);


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
            // 1. Save an image(Space Key) -- if we see a valid checkerboard image
            // 2. Start Camera clibration (Enter Key) -- if we wanted to start camera calibration so long as we have enough images like 10 or 15
            // 3. Exit(Escape Key)
        switch (character) {
            case ' ' :{
                //saving image
                if (found){
                    Mat temp;
                    frame.copyTo(temp);
                    savedImages.push_back(temp);
                }
                break;

            }
            case 13:{
                // start calibration
                if (savedImages.size() > 15) {
                    cameraCalibration(savedImages, chessboardDimensions, calibrationSquareDimension, cameraMatrix, distanceCoefficients);
                    saveCameraCalibration("my_camera_calibration", cameraMatrix, distanceCoefficients);

                }
                break;
            }
            
            case 27:{
                //exit 
                return 0;
                break;
            }
            
        }


    }


    return 0;
}

int main(int, char**)
{
    mainCalib();
    return 0;
}