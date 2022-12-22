#!/bin/bash
path=$PWD
#input map size
inMapSize=150
#mapsize after conversion
mapSize=75
#map resulution
mapRes=2
#obstacle radius
obstRadius=4
#max length of the instruction for the zumo
saveMaxInstruct=20
maxInstruct=$saveMaxInstruct
#X coordinate of goal on converted map 
goalX=10 
#Y coordinate of goal on converted map 
goalY=25

echo "Saving settings..."
echo $inMapSize > settings.txt
echo $mapSize >> settings.txt
echo $mapRes >> settings.txt
echo $obstRadius >> settings.txt
echo $goalX >> settings.txt
echo $goalY >> settings.txt

#copy start position to position.txt
#cp startPosition.txt position.txt

#remember start heading
python3 i2clibraries/startHeading.py

#To get instruction on first run, with no regard for t in the file
echo t > instructions.txt

#Getting point cloud map
#./csvMap

#for deadzone escape
calibrate=1

echo "checking..."
while :
do
    if grep -q t "instructions.txt";
    then
        #If calibration should run
        if [ $calibrate == "1" ]; then
            echo "Reading heading..."
            python3 i2clibraries/heading.py
            sleep 2.7
            python3 i2clibraries/heading.py
            sleep 2.7
            python3 i2clibraries/heading.py
            sleep 2.7
            python3 i2clibraries/heading.py
            sleep 2.7
                
            echo "Running lidar..." 
            echo "Running ICP algorithm"
            ./lds_driver
            
            #echo "Running extra position ICP"
            #./lidarMeasure
            #python3 icprun.py
            #echo "Adding new obstacles"
            #./addLidarToMap
        fi
        
        echo "Running path planning algorithm..."

        echo "Converting bitmap..."
        #convert bitmap.txt to sBitmap.txt
        ./convert

        echo -e "\n"
        echo "Running A* Algorithm..."
        #run A* and get coordinates.txt
        mv sBitmap.txt ../path_planning/build
        cp points.txt ../path_planning/build
        cd ../path_planning/build
        ./a_star --stdin test-user <<< $mapSize
        cd $path
        if mv ../path_planning/build/coordinates.txt coordinates.txt;
        then
            rm ../path_planning/build/sBitmap.txt
            echo -e "\n"
            echo "Writing instructions to instructions.txt"
            #get instructions based on coordinates print in instructions.txt
            ./instruct --stdin test-user <<< $maxInstruct
            rm coordinates.txt
            cp position.txt prevPosition.txt
            #go back to loop
            echo -e "\n"
            calibrate=1
            maxInstruct=$saveMaxInstruct
            echo "Checking..."
        else
            echo "Path not found!"
            cp prevPosition.txt position.txt
            calibrate=0
            maxInstruct=3
            echo t > instructions.txt
        fi
    fi
    sleep 1
done
