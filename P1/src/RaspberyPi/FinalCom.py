#!/usr/bin/env python
# -*- coding: utf-8 -*-
# lsusb to check device name
#dmesg | grep "tty" to find port name

import serial,time

answer = 'o'
flag = True
program = True
vektor = "10,10;"

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


def newPosition():
    with open("settings.txt")as f:
        data = f.readlines()
        f.close()
    
    data[4] = input("x: ") + "\n"
    data[5] = input("y: ") + "\n"
        
    with open('settings.txt', 'w') as f:
        f.writelines(data)
        f.close()

with serial.Serial("/dev/ttyACM0", 115200, timeout=1) as arduino:
    time.sleep(0.1) #wait for serial to open
    cmd="c"
    time.sleep(0.1)
    arduino.write(cmd.encode())
    print(cmd.encode())
    newPosition()
    with open("instructions.txt", 'w')as f:         
        f.write('t')
        f.close()
        print("Send t")
    while (program):
        time.sleep(0.2)
        with open("heading.txt")as f:
            valueList_h = f.readlines()
            f.close()
            if not valueList_h:
                print("valueList_h IS EMPTY! Checkpoint A")
            else:
                lineOne = valueList_h[0]
                if lineOne != "t":
                    with open("heading.txt")as h:
                        vektor = h.readlines()[0]
                        h.close()
                        print("Turning:" + "" + vektor)
                        Serial_send_first(vektor)
                        time.sleep(0.1)
                        with open("heading.txt", 'w')as f:         
                            f.write('t')
                            f.close()
        
        time.sleep(0.2)
        with open("instructions.txt")as f:
            valueList_v = f.readlines()
            f.close()
            if not valueList_v:
                print("valueList_v IS EMPTY! Checkpoint B")
            else:
                lines = len(valueList_v)
                lineOne = valueList_v[0]
                if lineOne != "t":
                    for i in range(lines):
                        feedback()
                        if (i == lines-2): 
                            cmd="z"
                            time.sleep(0.1)
                            arduino.write(cmd.encode())
                            print(cmd.encode()) 
                            
                  #      with open("instructions.txt")as f:
                        vektor = valueList_v[i]
              #          f.close()
                        if (vektor == "s"):
                            print("New vectors")
                        elif (vektor == "d"):
                            print("Position found")
                            newPosition()
                            with open("instructions.txt", 'w')as f:         
                                f.write('t')
                                f.close()
                        else:
                            Serial_send(vektor)
                            flag = True
                    
                    
                    feedback()
                    if (vektor != "d"):
                        with open("instructions.txt", 'w')as f:         
                            f.write('t')
                            f.close()
                            print("Instructions Done!")
                            flag = False
                            
                    
