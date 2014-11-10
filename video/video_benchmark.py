import ctypes
import numpy as np
import cv2
import sys

def clock():
    return cv2.getTickCount() / cv2.getTickFrequency()

def draw_str(dst, (x, y), s):
    cv2.putText(dst, s, (x+1, y+1), cv2.FONT_HERSHEY_PLAIN, 2.5, (0, 0, 0), thickness = 3)
    cv2.putText(dst, s, (x, y), cv2.FONT_HERSHEY_PLAIN, 2.5, (0, 0, 255), thickness = 3)

# load reference implementation as a shared library
libharris = ctypes.cdll.LoadLibrary("./harris.so")
harris = libharris.harris_ref

# Load the input video file  
fn = sys.argv[1]
cap = cv2.VideoCapture(fn)

show = int(sys.argv[2])

frames = 0
cv_mode = True
harris_mode = False

while(cap.isOpened()):
    # Read one frame
    ret, frame = cap.read()

    rows = frame.shape[0]
    cols = frame.shape[1]

    # Convert to gray scale
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    gray = np.float32(gray)

    frameStart = clock()

    if harris_mode:
        if cv_mode:
            # The OpenCV routine for corner detection. Corner
            # map computed by OpenCV should match yours.
            res = cv2.cornerHarris(gray, 3, 3, 0.04)
        else:    
            res = np.zeros((rows, cols), np.float32)
            # The number of cols and rows is reduced since the 
            # routine only computes the corner map for the
            # window (2, 2) -- (cols-2, rows-2)
            harris(ctypes.c_int(cols-2), ctypes.c_int(rows-2), \
            ctypes.c_void_p(gray.ctypes.data), ctypes.c_void_p(res.ctypes.data))
        res = cv2.cvtColor(res, cv2.COLOR_GRAY2RGB)     
    else:
        res = frame

    frameEnd = clock()
    # Draw the frame rate indicator
    cv2.rectangle(res, (0, 0), (750, 150), (255, 255, 255), thickness=cv2.cv.CV_FILLED)
    draw_str(res, (40, 40), "frame interval :  %.1f ms" % (frameEnd*1000 - frameStart*1000))
    if cv_mode and harris_mode:
        draw_str(res, (40, 80), "Pipeline     :  " + str("OpenCV"))
    elif harris_mode:
        draw_str(res, (40, 80), "Pipeline     :  " + str("Reference"))
    if harris_mode:    
        draw_str(res, (40, 120), "Benchmark   :  " + str("Harris Corner"))
    cv2.imshow('Video', res)

    if show == 1:
        cv2.imshow('Video', res)
    else:
        harris_mode = True
        if frames > 50:
            # Run first 50 frames in OpenCV mode and the rest 50 in reference mode
            cv_mode = False
        if frames > 100:
            # Stop after 100 frames
            break
        print "frame interval :  %.1f ms" % (frameEnd*1000 - frameStart*1000)

    ch = 0xFF & cv2.waitKey(1)
    if ch == ord('q'):
        break
    if ch == ord(' '):
        cv_mode = not cv_mode
    if ch == ord('h'):
        harris_mode = not harris_mode
    frames += 1

cap.release()
cv2.destroyAllWindows()
