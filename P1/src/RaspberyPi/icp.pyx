#Import things
from cpython cimport array
import array
import numpy as np
import pandas as pd
from scipy.spatial import distance as dist

def run():
    #Sample size from lidar measurements
    cdef int sampleSizeLidar = 70
    #Sample size from map
    cdef int sampleSizeMap = 160
    #Open point clouds
    lidar = pd.read_csv('lidarMeasurement.csv')
    lidarOffset = lidar
    map = pd.read_csv('map.csv')

    #Init offsets
    cdef float xOffset = 0
    cdef float yOffset = 0

    #Check if actual array size is smaller than the sample size
    cdef int lidarPoints
    if lidar.y.size < sampleSizeLidar:
        #Set samples to actual size
        lidarPoints = lidar.y.size
    else: 
        #set samples to sample size
        lidarPoints = sampleSizeLidar
    #Make an array containing random sected indecies from the lidar point cloud
    cdef long[:] lidarMatch = np.random.choice(lidar.y.size, lidarPoints, False)

    #Set sample size like above
    cdef int mapPoints
    if map.y.size < sampleSizeMap:
        mapPoints = map.y.size
    else: 
        mapPoints = sampleSizeMap
    #Make an array containing random sected indecies from the lidar point cloud
    cdef long[:] mapMatch = np.random.choice(map.y.size, mapPoints, False)

    #define distance correction array
    cdef int[:] closest = np.zeros(lidarPoints, dtype=np.int32)
    #define distance array to hold distances
    cdef float[:] distances = np.zeros(lidarPoints, dtype=np.float32)
    #declare variables
    cdef float yOffsetCurr
    cdef float xOffsetCurr
    cdef float meanDist
    cdef float currentDist
    #iterate trough this
    for a in range(3):
        #Go trough all indecies in the lidar samples
        for i in range(lidarPoints):
            #For each lidar sample go trough the map samples
            for j in range(mapPoints):
                #Calculate the distance from sample to sample
                currentDist = dist.cityblock([lidarOffset.x[lidarMatch[i]],lidarOffset.y[lidarMatch[i]]],\
                    [map.x[mapMatch[j]],map.y[mapMatch[j]]])
                #If current distance is less than stored distance or there is no stored distance
                if currentDist < distances[i] or closest[i] == 0:
                    #Set distance correction index to be the closest so far on the map
                    closest[i] = mapMatch[j]
                    #Set the distance at i position
                    distances[i] = currentDist

        #Reset current offset
        xOffsetCurr = 0.0
        yOffsetCurr = 0.0
        #Reset mean distance 
        meanDist = 0.0
        #Go trough all indices in lidar samples
        for i in range(lidarPoints):
            #Set the offset to be avaraged between all point offsets
            xOffsetCurr += float((map.x[closest[i]] - lidarOffset.x[lidarMatch[i]]) / lidarMatch.size)
            yOffsetCurr += float((map.y[closest[i]] - lidarOffset.y[lidarMatch[i]]) / lidarMatch.size)
            #Set mean distance to be avarage distance between sampled points
            meanDist += distances[i] / lidarMatch.size

        #Go trough all lidar readings
        for i in range(lidarPoints):
            #Set lidar reading to be reading + ofset
            lidarOffset.at[lidarMatch[i],"x"] = int(lidarOffset.x[lidarMatch[i]] + xOffsetCurr)
            lidarOffset.at[lidarMatch[i],"y"] = int(lidarOffset.y[lidarMatch[i]] + xOffsetCurr)

        #Add current offset to total offset
        xOffset += xOffsetCurr
        yOffset += yOffsetCurr

        #Print status
        print(f"X Offset: {xOffset}")
        print(f"Y Offset: {yOffset}")
        print(f"Mean distance: {meanDist}")

    #Set offset array
    cdef int[:] offset = np.zeros(3, dtype=np.int32)
    f = open("position.txt", "r")
    offset[0] = int(f.readline()) + int(xOffset)
    offset[1] = int(f.readline()) + int(yOffset)
    offset[2] = int(f.readline())
    f.close()

    p = open("position.txt", "w")
    p.write(f"{offset[0]}\n{offset[1]}\n{offset[2]}\n")
    p.close()
