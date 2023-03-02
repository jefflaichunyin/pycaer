#! /usr/bin/python3
import cv2 as cv
import numpy as np
from pycaer.aedat import Aedat
import sys

pkt_cnt = 0
frame_delay = 10
f = Aedat(sys.argv[1])
cv.namedWindow('frame', cv.WINDOW_NORMAL)
cv.resizeWindow('frame', 346*3, 260*3)
frame = np.full((260,346,3), 255, 'uint8')
# for _ in range(135):
#     events = f.read(frame)

# setup initial window location
x, y, w, h = 168, 125, 168, 125
track_window = (168, 125, 168, 125)
term_crit = ( cv.TERM_CRITERIA_EPS | cv.TERM_CRITERIA_COUNT, 10, 1 )
previous_center = np.array([0,0])
track = np.full((260,346,3), 0, 'uint8')
while True:
    # events are formated in the following way: [x, y, self.t, p]
    frame = np.full((260,346,3), 0, 'uint8')
    events = f.read(frame)
    pkt_cnt += 1
    dst = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
    dst = cv.GaussianBlur(dst,(9,9),0)
    ret, dst = cv.threshold(dst,32,255,cv.THRESH_TOZERO)
    ret, track_window = cv.CamShift(dst, track_window, term_crit)
    pts = cv.boxPoints(ret)
    pts = np.int0(pts)
    center = np.int0(ret[0])
    # print("center", center)
    annotated = cv.polylines(frame,[pts],True, 255,2)
    cv.putText(annotated, f"{ret[2]:3.2f}", pts[0], cv.FONT_HERSHEY_SIMPLEX, 0.5, (0,200,0), 2, cv.LINE_AA)

    if not np.all(previous_center == 0) and not np.all(center == 0):
        cv.line(track, previous_center, center, (0,255,0), 2)
    previous_center = center
    annotated = cv.add(annotated, track)

    cv.imshow('frame', annotated)
    key = cv.waitKey(frame_delay) & 0xFF
    if key == ord('q'):
        print("pkt cnt = ", pkt_cnt)
        exit(0)
    elif key == ord('w'):
        frame_delay  = max(1, frame_delay - 10)
        print("frame delay = ", frame_delay)
    elif key == ord('s'):
        frame_delay  = min(1000, frame_delay + 10)
        print("frame delay = ", frame_delay)
    elif key == ord('d'):
        for _ in range(10):
            events = f.read(frame)
            pkt_cnt += 1
        print(pkt_cnt)
    if len(events) == 0:
    	break

cv.putText(frame, "Press Q to exit", (50,50), cv.FONT_HERSHEY_SIMPLEX, 1, (0,200,0), 2, cv.LINE_AA)
cv.imshow('frame', frame)
while cv.waitKey(10) & 0xFF != ord('q'):
    pass
