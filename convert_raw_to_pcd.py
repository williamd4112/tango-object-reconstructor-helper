import sys
import struct

f = open("pointcloud.dat", "rb")

raw_data = []

try:
    byte = f.read(4)
    while byte != "":
        # Do stuff with byte.
		byte = f.read(4)
		data = struct.unpack('>f', byte)
		print data
finally:
    f.close()
