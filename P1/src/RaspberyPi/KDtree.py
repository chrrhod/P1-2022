# -*- coding: utf-8 -*-
"""
Created on Tue Dec 13 14:26:59 2022

@author: chrrh
"""





samples = [[0., 0.]]
from sklearn.neighbors import NearestNeighbors
import time


vektor = [0,0]

map = "t"
flag = True
o = 0
answer = [[0,0]]
with open("ogmap.txt") as m:
    map = m.readline()
    m.close()
    
for y in range(1500):
    for x in range(1500):
        if map[x+y*1500] == '1':
            samples.append([float(x),float(y)])
                               
neigh = NearestNeighbors(n_neighbors=1, algorithm="brute", p=1.0, n_jobs=-1)
neigh.fit(samples)
while True:
    with open("vektor.txt") as v:
        s = v.readline()[0]
        if s == "a":
            v.close()
            with open("vektor2.txt") as v:
                array = v.readlines()
                lines = len(array)
                v.close()
                for i in range(360):
                    with open("vektor2.txt") as v:
                        vektor = v.readlines()[i].split(';')
                        vektor[0] = float(vektor[0])
                        vektor[1] = float(vektor[1])
                        yn = (neigh.kneighbors([vektor], return_distance=False))
                        if i == 0:
                            answer[0]=samples[int(yn[0])]
                        else:
                            answer.append(samples[int(yn[0])])
                v.close()
                with open("vektor.txt","w") as v:
                    for i in range(360):
                        v.write(str(answer[i][0]) +";"+ str(answer[i][1]))
                        print(str(answer[i][0]) +";"+ str(answer[i][1]))
                        v.write('\n')
                v.close()
                with open("vektor.txt","a") as v:
                    v.write("t")
                    v.close()
                v.close()
        else:
            v.close()
            time.sleep(0.1)
            

        

    
        
    
a = input("slut")


