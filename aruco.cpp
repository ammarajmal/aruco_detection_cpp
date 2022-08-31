// OpenCv 4.2.0

#include "opencv2/opencv.hpp"
#include <opencv2/aruco.hpp>
using namespace cv;
using namespace std;
int markNum(1);

Mat markerGenerator(int markNum){
    Mat markerImage;
    Ptr<aruco::Dictionary> dictionary = aruco::getPredefinedDictionary(aruco::DICT_6X6_250);
    aruco::drawMarker(dictionary, markNum, 200, markerImage, 1);
    imwrite("marker0.png", markerImage);
    return markerImage;
}


void markderDetector(){
    VideoCapture inputVideo;
    inputVideo.open(4);
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

    markderDetector();

    return 0;
}
