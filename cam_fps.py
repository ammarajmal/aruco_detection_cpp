#!/usr/bin/env python

# *********************
# webcam video stream
# *********************
import cv2
import time

# 0 - Dell Webcam

cap = cv2.VideoCapture(0)
start = time.time()
while cap.isOpened():
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