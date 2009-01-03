#!/usr/bin/env python

import os
import socket
import time
from struct import *
from scipy import *

""" open socket to info port on dataserver """
def info_socket():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect(("ao-server.mmto.arizona.edu", 7500))
    return sock

""" open socket on command port on dataserver """
def cmd_socket():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect(("ao-server.mmto.arizona.edu", 7501))
    return sock

""" open socket on data port on dataserver """
def data_socket():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect(("ao-server.mmto.arizona.edu", 7502))
    return sock

""" set camera bias for given quadrant to given value """
def set_bias(quad, value):
    s = cmd_socket()
    s.send("cam_bias_set\n")
    s.send(str(quad) + "\n")
    s.send(str(value) + "\n")
    s.close()

""" get dictionary of all info published by dataserver """
def get_info():
    info = {}
    s = info_socket()
    s.send("all\n")
    """ this is weak, but it appears to be necessary for it to work reliably """
    time.sleep(0.5)
    results = s.recv(4096)
    s.close()
    for i in results.split('\n'):
        data = i.split(' ')
        if len(data) > 1:
            info[data[0]] = data[1]

    return info

""" get single WFSC frame.  need to provide socket since opening a socket per frame 
    is very slow """
def get_frame(s):
    dup = True

    """ loop here to make sure we always return unique frames """
    while dup == True:
        s.send("Single\n")
        """ get number of bytes in frame (unsigned long) """
        packet = s.recv(4)
        nbytes = long(unpack('!l', packet)[0])

        """ get frame number (unsigned long) """
        packet = s.recv(4)
        num = long(unpack('!L', packet)[0])
        
        """ if nbytes is neg, then frame is duplicate """
        if nbytes < 0:
            nbytes = abs(nbytes)
            dup = True
        else:
            dup = False

        """ now use the nbytes we read to get the actual data (unsigned shorts) """
        packet = s.recv(nbytes)
        fmt = "!" + str(nbytes/2) + "H"
        frame = unpack(fmt, packet)

    size = int(sqrt(nbytes/2))

    """ take the frame data and reshape appropriately it into a numpy array object """
    image = reshape(frame, (size,size))
    return image

""" average num WFSC frames """
def ave_frames(num):
    s = data_socket()
    n = num
    ave = get_frame(s);
    n = n - 1
    while n > 0:
        ave = ave + get_frame(s)
        n = n - 1

    ave = ave / num
    s.close()
    return ave
