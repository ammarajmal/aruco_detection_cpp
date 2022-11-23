#!/usr/bin/env python

# *********************
# webcam video stream
# *********************
import cv2
import time

# 0 - Dell Webcam


path = '/home/ammar/sitl_agcam/aruco_detection_cpp/filename.avi'
cap = cv2.VideoCapture(path)

while cap.isOpened():
    start = time.time()
    ret, frame = cap.read()
    if ret == True:
        cv2.imshow('Frame', frame)
        end = time.time()
        seconds = end - start
        fps = 1/seconds
        start = end
        print(fps)
        if cv2.waitKey(25) & 0xFF == ord('q'):  break
    else:   break
cap.release()
cv2.destroyAllWindows()