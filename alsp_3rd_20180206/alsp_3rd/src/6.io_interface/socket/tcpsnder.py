#!/usr/bin/env python
# -*- coding: utf-8 -*-
# UDP sender
import socket
import sys
import string
import time

# ---------------------------[ End of Variable definition ]-------------------------------
# DESC : 사용법을 출력한다.
# ARGV : none
# RET  : none
def usages():
    print sys.argv[0], "<host> <port>"
    sys.exit()
# ---------------------------[ End of Function Definition ]-------------------------------

if len(sys.argv) != 3:
    usages()

HOST = sys.argv[1]
PORT = int(sys.argv[2])
print "Destination Address :", HOST, PORT
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))
idx=0
while 1:
    idx += 1
    sbuf = 'Time:' + str(time.time())
    slength = s.send("1234567890abcde"*1024)
    print "[Send] " + str(slength) + " bytes "
    if idx%10 == 0 :
        time.sleep(1)

    time.sleep(0.1)
s.close()
