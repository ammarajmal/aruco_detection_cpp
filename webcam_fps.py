#!/usr/bin/env python

# *********************
# webcam video stream
# *********************
import cv2
import time


wcam, hcam = 640, 480
cap = cv2.VideoCapture(2)


start_time = time.time()

while True:
    success, img = cap.read()
    end_time = time.time()
    fps = 1/(end_time - start_time)
    start_time = end_time
    
       
    fps = str(int(fps))
    print(fps)
    
    font = cv2.FONT_HERSHEY_SIMPLEX
    # putting the FPS count on the frame
    cv2.putText(img, fps, (7, 70), font, 3, (100, 255, 0), 3, cv2.LINE_AA)
 
 
    # press 'Q' if you want to exit
    cv2.imshow('Webcam', img)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()