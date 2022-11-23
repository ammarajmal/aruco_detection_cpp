#!/usr/bin/env python

# *********************
# webcam video stream
# *********************
import cv2
import datetime

# 0 - Dell Webcam


path = '/home/ammar/sitl_agcam/aruco_detection_cpp/filename.avi'
cap = cv2.VideoCapture(path)

while cap.isOpened():
    start = datetime.datetime.now()
    ret, frame = cap.read()
    if ret == True:
        cv2.imshow('Frame', frame)
        
        end = datetime.datetime.now()

        t=end-start
        fps = 1/t.total_seconds()
        print(fps)

        if cv2.waitKey(25) & 0xFF == ord('q'):  break
    else:   break
cap.release()
cv2.destroyAllWindows()