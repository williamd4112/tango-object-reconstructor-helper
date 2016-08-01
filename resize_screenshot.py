import cv2
import numpy as np
import sys

img = cv2.imread(sys.argv[1]) 
res = cv2.resize(img, (1280,720))

cv2.imwrite(sys.argv[2], res)
