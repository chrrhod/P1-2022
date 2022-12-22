#!/usr/bin/env python
# -*- coding: utf-8 -*-
# lsusb to check device name
#dmesg | grep "tty" to find port name

import serial,time

answer = 'o'
flag = True

def Serial_send(file):       
        if arduino.isOpen():
            print("connected!".format(arduino.port))
            try:
                filesplit = file.split(',')
                xKort = (filesplit[0])
                yKort = (filesplit[1])
                cmd=("x" +(xKort)+"y"+(yKort)+"b")
                time.sleep(0.1)
                arduino.write(cmd.encode())
                print(cmd.encode())
                #time.sleep(0.1) #wait for arduino to answer
            except KeyboardInterrupt:
                print("KeyboardInterrupt has been caught.")    

def Serial_send_first(file):       
        if arduino.isOpen():
            print("connected!".format(arduino.port))
            try:
                cmd=("m" + file)
                time.sleep(0.1)
                arduino.write(cmd.encode())
                print(cmd.encode())
                #time.sleep(0.1) #wait for arduino to answer
            except KeyboardInterrupt:
                print("KeyboardInterrupt has been caught.") 

def feedback():
    while(True):
        cmd="r"
        time.sleep(0.1)
        arduino.write(cmd.encode())
        print(cmd.encode())
        answer=arduino.readline()
        arduino.flushInput()
        if answer == b'o':
            break
    return(answer)    



with serial.Serial("/dev/ttyACM0", 115200, timeout=1) as arduino:
    time.sleep(0.2) #wait for serial to open
    cmd="c"
    time.sleep(0.2)
    arduino.write(cmd.encode())
    print(cmd.encode())
    while (True):
        time.sleep(0.2)
        with open("heading.txt")as f:
            grad = f.readlines()[0]
            f.close()
            if grad != "t":
                feedback()
                with open("heading.txt")as f:
                    vektor = f.readlines()[0].split(';')[0]
                    f.close()
                    print("Turning:" + "" + vektor)
                    Serial_send_first(vektor)
                    time.sleep(0.1)
                    with open("heading.txt", 'w')as f:         
                        f.write('t')
                        f.close()
        time.sleep(0.2)
        with open("instructions.txt")as f:
            array = f.readlines()
            f.close()
            lines = len(array)
            værdi = array[0]
            if værdi != "t":
                for i in range(lines-1):
                    feedback()
                    if (i == lines-2): 
                        cmd="z"
                        time.sleep(0.1)
                        arduino.write(cmd.encode())
                        print(cmd.encode()) 
                        
                    with open("instructions.txt")as f:
                        vektor = f.readlines()[i].split(';')[0]
                        f.close() 
                        Serial_send(vektor)
                        flag = True
                
                feedback()
                with open("instructions.txt", 'w')as f:         
                    f.write('t')
                    f.close()
                    print("Instructions Done!")
                    flag = False

