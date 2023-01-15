from pycaer import davis
import numpy as np
import cv2
from timeit import timeit
import time
davis.open()

frame = np.full((260,346,3), 0, 'uint8')
print("Test 1 - Read as fast as possible")
prev_ts = 0
for i in range(100):
    events = davis.read(frame)
    print("difference in timestamp: ", events[0][2] - prev_ts)
    prev_ts = events[0][2]
print("Test 2 - Read after 0.05s delay")
prev_ts = 0
for j in range(2):
    for i in range(50):
        events = davis.read(frame)
        print("difference in timestamp: ", events[0][2] - prev_ts)
        prev_ts = events[0][2]
    time.sleep(0.05)
    print("delay 0.05")

print("Test 3 - Generate frame seperately")
print("reading 100 packets")
start_time = time.time()
for i in range(100):
    frame = np.full((260,346,3), 0, 'uint8')
    events = davis.read(None)
    y = events[:, 1]
    x = events[:, 0]
    frame[y, x] = [255, 255, 255]
print("Time elapsed: ", time.time() - start_time)

print("Test 4 - Using frame from pycaer")
print("reading 100 packets")
frame = np.full((260,346,3), 0, 'uint8')
start_time = time.time()
for i in range(100):
    events = davis.read(frame)
print("Time elapsed: ", time.time() - start_time)
davis.close()