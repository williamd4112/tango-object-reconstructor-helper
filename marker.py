import cv2
import numpy as np
import sys

def draw_circle(event,x,y,flags,param):
    global mouseX,mouseY
    if event == cv2.EVENT_LBUTTONDBLCLK:
        cv2.circle(img,(x,y),100,(255,0,0),-1)
        mouseX,mouseY = x,y

img = cv2.imread(sys.argv[1], 3)

while(1):
    cv2.imshow('image',img)
    cv2.rectangle(img, (439, 71), (805,601), (0, 255, 0))
    k = cv2.waitKey(20) & 0xFF
    if k == 27:
        break
    elif k == ord('a'):
        print mouseX,mouseY
