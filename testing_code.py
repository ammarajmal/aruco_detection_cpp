import cv2

def live_undistort(camera, camera_matrix, distortion_coefficients):
    """ Using a given calibration matrix, display the distorted, undistorted, and cropped frame"""
    scaled_camera_matrix, roi = cv2.getOptimalNewCameraMatrix(
        camera_matrix, distortion_coefficients, camera.size, 1, camera.size
    )
    while True:
        ret, frame = camera.cap.read()
        assert ret
        distorted_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        undistorted_frame = cv2.undistort(
            distorted_frame, camera_matrix, distortion_coefficients, None, scaled_camera_matrix,
        )
        roi_x, roi_y, roi_w, roi_h = roi
        cropped_frame = undistorted_frame[roi_y : roi_y + roi_h, roi_x : roi_x + roi_w]
        cv2.imshow("distorted %s" % (distorted_frame.shape,), distorted_frame)
        cv2.imshow("undistorted %s" % (undistorted_frame.shape,), undistorted_frame)
        cv2.imshow("cropped %s" % (cropped_frame.shape,), cropped_frame)
        cv2.waitKey(10) 