import cv2
import numpy as np
import sys

img = cv2.imread(sys.argv[1]) 
res = cv2.resize(img, (int(sys.argv[2]),int(sys.argv[3])))

cv2.imwrite(sys.argv[1], res)
