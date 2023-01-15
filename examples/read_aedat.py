from pycaer.aedat import Aedat
import sys
import numpy as np
import cv2

aedat_file = sys.argv[1]
aedat = Aedat(aedat_file)

cv2.namedWindow('frame', cv2.WINDOW_NORMAL)
while True:
    frame = np.full((260,346,3), 0, 'uint8')
    events = aedat.read(frame)
    print(events)
    if events.shape[0] == 0:
        print("End of file")
        break
    # print(events)
    cv2.imshow('frame', frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break