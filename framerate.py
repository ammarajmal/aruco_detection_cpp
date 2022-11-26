import cv2
import numpy as np
import time
backSub = cv2.createBackgroundSubtractorKNN()


video = cv2.VideoCapture(0)

fps = video.get(cv2.CAP_PROP_FPS)
print('Frames per second camera: {0}'.format(fps))

num_frames = 1
print('Capture {} frames'.format(num_frames))
while True:
    start = time.time()
    ret, frame = video.read()
    
    if frame is None:
        print("no frame")
        break
    fgMask = backSub.apply(frame)
        
    sum = 0
    N = 100
    for i in range(0, N):
        for j in range(0, N):
            sum += 1
    kernel = np.ones((5,5), np.uint8)
        
    # fgMask = cv2.erode(fgMask, kernel, iteration=1)
    # fgMask = cv2.dilate(fgMask, kernel, iteration=1)
        
    end = time.time()
    seconds = end - start

    fps = num_frames/seconds
    cv2.putText(fgMask, "FPS: "+ str(round(fps)), (50, 50), cv2.FONT_HERSHEY_SIMPLEX, 1.5, (255,255,255))

    keyboard = cv2.waitKey(30)
    if keyboard == 'q' or keyboard == 27:
        break
video.release()    
    