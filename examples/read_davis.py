from pycaer import davis
import numpy as np
import cv2


davis.open()

while True:
    frame = np.full((260,346,3), 0, 'uint8')
    events = davis.read(frame)
    print(events)
    cv2.imshow('frame', frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

davis.close()