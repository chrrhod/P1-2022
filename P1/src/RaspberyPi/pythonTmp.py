#!/usr/bin/env python
# -*- coding: utf-8 -*-
# lsusb to check device name
#dmesg | grep "tty" to find port name

import serial,time

answer = 'o'
first = 1
flag = True

def Serial_send(file):       
        if arduino.isOpen():
            print("connected!".format(arduino.port))
            try:
                filesplit = file.split(',')
                xKort = (filesplit[0])
                yKort = (filesplit[1])
                cmd=("x" +(xKort)+"y"+(yKort)+"b")
                time.sleep(1.5)
                arduino.write(cmd.encode())
                print(cmd.encode())
                #time.sleep(0.1) #wait for arduino to answer
            except KeyboardInterrupt:
                print("KeyboardInterrupt has been caught.")    


with serial.Serial("/dev/ttyACM0", 9600, timeout=1) as arduino:
    time.sleep(0.1) #wait for serial to open
    cmd="c"
    time.sleep(1.5)
    arduino.write(cmd.encode())
    print(cmd.encode())
    while (True):
        first = 1
        with open("instructions.txt")as f:
            array = f.readlines()
            f.close()
            lines = len(array)  
            værdi = array[0]
            if værdi != "t\n":
                for i in range(lines):
                    answer = 'f'
                    while(flag):
                        cmd="r"
                        time.sleep(1.5)
                        arduino.write(cmd.encode())
                        print(cmd.encode())
                        answer=arduino.readline()
                        arduino.flushInput()
                        if answer == b'o':
                            flag = False
                    with open("instructions.txt")as f:
                        array = f.readlines()
                        f.close()  
                        værdi = array[i]
                        print(værdi)
                        if (answer == b'o') and (værdi != "s"): 
                            with open("instructions.txt")as f:
                                vektor = f.readlines()[i].split(';')[0]
                                print(vektor)
                                f.close() 
                                Serial_send(vektor)
                                flag = True
                        else:     
                            with open("instructions.txt", 'w')as f:         
                                f.write('t')
                                f.close()
                                flag = False
