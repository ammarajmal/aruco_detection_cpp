// OpenCv 4.2.0
#include "opencv2/opencv.hpp"
#include <opencv2/aruco.hpp>
using namespace cv;
using namespace std;
int markNum(1);
int waitTime(50);

Mat markerGenerator(int markNum){
    Mat markerImage;
    Ptr<aruco::Dictionary> dictionary = aruco::getPredefinedDictionary(aruco::DICT_4X4_1000);
    aruco::drawMarker(dictionary, markNum, 200, markerImage, 1);
    imwrite("marker0.png", markerImage);
    return markerImage;
}



void markerDetector(){
    VideoCapture inputVideo;
    inputVideo.open(0);
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
void markerDetector_new(){
    // Mat inputImage;
    // vector<int> markerIds;
    // vector<vector<Point2f>> markerCorners, rejectedCandidates;
    // Ptr<aruco::DetectorParameters> parameters = aruco::DetectorParameters::create();
    // Ptr<aruco::Dictionary> dictionary = aruco::getPredefinedDictionary(aruco::DICT_4X4_1000);
    // aruco::detectMarkers(inputImage, dictionary, markerCorners, markerIds, parameters, rejectedCandidates);

    // Mat outputImage = inputImage.clone();
    // aruco::drawDetectedMarkers(outputImage, markerCorners, markerIds);


    VideoCapture inputVideo;
    inputVideo.open(0);
    Ptr<aruco::Dictionary> dictionary = aruco::getPredefinedDictionary(aruco::DICT_4X4_1000);
    while (inputVideo.grab()) {
        Mat image, imageCopy;
        inputVideo.retrieve(image);
        image.copyTo(imageCopy);
        vector<int> ids;
        vector<vector<cv::Point2f>> corners;
        aruco::detectMarkers(image, dictionary, corners, ids);
        // if at least one marker detected
        if (ids.size() > 0)
            aruco::drawDetectedMarkers(imageCopy, corners, ids);
        imshow("out", imageCopy);
        char key = (char) cv::waitKey(waitTime);
        if (key == 27)
            break;
    }
}

static bool readCameraParameters(string filename, Mat &camMatrix, Mat &distCoeffs) {
    FileStorage fs(filename, FileStorage::READ);
    if(!fs.isOpened())
        return false;
    fs["camera_matrix"] >> camMatrix;
    fs["distortion_coefficients"] >> distCoeffs;
    return true;
}


void poseEstimation (void) {
    cv::VideoCapture inputVideo;
    inputVideo.open(0);
    Mat cameraMatrix, distCoeffs;
    // You can read camera parameters from tutorial_camera_params.yml
    // readCameraParameters(filename, cameraMatrix, distCoeffs); // This function is located in detect_markers.cpp
    Ptr<aruco::Dictionary> dictionary = aruco::getPredefinedDictionary(aruco::DICT_4X4_1000);

    while (inputVideo.grab()) {
        cv::Mat image, imageCopy;
        inputVideo.retrieve(image);
        image.copyTo(imageCopy);
        std::vector<int> ids;
        std::vector<std::vector<cv::Point2f>> corners;
        cv::aruco::detectMarkers(image, dictionary, corners, ids);
        // if at least one marker detected
        if (ids.size() > 0) {
            cv::aruco::drawDetectedMarkers(imageCopy, corners, ids);
            std::vector<cv::Vec3d> rvecs, tvecs;
            cv::aruco::estimatePoseSingleMarkers(corners, 0.05, cameraMatrix, distCoeffs, rvecs, tvecs);
            // draw axis for each marker
            for(int i=0; i<ids.size(); i++)
                cv::drawFrameAxes(imageCopy, cameraMatrix, distCoeffs, rvecs[i], tvecs[i], 0.1);
        }
        cv::imshow("out", imageCopy);
        char key = (char) cv::waitKey(waitTime);
        if (key == 27)
            break;
    }

}


int main(int, char**)
{
    // markerGenerator(0);
    markerDetector();
    // markerDetector_new();

    return 0;
}
