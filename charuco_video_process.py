import glob
import numpy as np
from numpy import genfromtxt
import cv2
import os
import datetime


cap = cv2.VideoCapture('video/sample.mp4')
camera_matrix = genfromtxt('config/mtx.csv', delimiter=',')
dist_coeffs = genfromtxt('config/dist.csv', delimiter=',')
aruco_dict = cv2.aruco.Dictionary_get(cv2.aruco.DICT_5X5_100)
arucoParams = cv2.aruco.DetectorParameters_create()
markerLength = 25
squareLength = 30
while cap.isOpened():
    start = datetime.datetime.now()
    ret,img = cap.read()

    if ret == True:
        try:
            h, w = img.shape[:2]

            newcameramtx, roi = cv2.getOptimalNewCameraMatrix(
                camera_matrix, dist_coeffs, (w, h), 1, (w, h))
            dst = cv2.undistort(img, camera_matrix,
                                dist_coeffs, None, newcameramtx)

            corners, ids, rejectedImgPoints = cv2.aruco.detectMarkers(dst, dictionary=aruco_dict,
                                                                      parameters=arucoParams)  # First, detect markers
            diamondCorners, diamondIds = cv2.aruco.detectCharucoDiamond(dst, corners, ids, squareLength / markerLength,
                                                                        diamondCorners=None, diamondIds=None,
                                                                        cameraMatrix=camera_matrix,
                                                                        distCoeffs=dist_coeffs)
            if len(diamondIds[0][0])>=4:
                rvecs, tvecs, _objPoints = cv2.aruco.estimatePoseSingleMarkers(diamondCorners, markerLength, camera_matrix,
                                                                           dist_coeffs, rvecs=None, tvecs=None, _objPoints=None)

                rotMatrix = cv2.Rodrigues(src=rvecs[0])[0]
                trsMatrix = tvecs[0].T

                disp_3D = np.matmul(np.linalg.inv(rotMatrix), trsMatrix)

                x, y, z = disp_3D[0], disp_3D[1], disp_3D[2]

                time = datetime.datetime.utcnow()
                time = time.hour * 60 * 60 + time.minute * 60 + time.second + time.microsecond / (
                            10 ** 6)


                data_3d = [time, x, y, z]
                rawdata = [rotMatrix,trsMatrix]
                # print('rawdata: ',rawdata)
        except Exception:
            print(Exception)
            pass
        end = datetime.datetime.now()

        t=end-start
        fps = 1/t.total_seconds()
        print(fps)