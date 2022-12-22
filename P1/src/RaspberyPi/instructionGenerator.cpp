
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
//for sin and cos
#include <cmath>

void getInstructions(){
    //multiply values by x to get mm for the zumo. 
    int multiplier = 2;
    //get max lenght of instructions from terminal input (comes from bash)
    char max[10];
    std::cin.getline (max,10);
    int maxDrive = std::strtol(max, nullptr, 10);

    //Get angle the zumo is at, to rotate it later
    /*std::ifstream pointFile("points.txt");
    std::string savePoints;
    int points[10]; //acutally 4 but more can be saved in the file
    int a = 0;
    //count lines in file (coordinates)
    while(getline(pointFile, savePoints)){
        points[a] = stoi(savePoints);
        a++;
    }*/
    /*//angle is on the 5th line in the document
    float angle = points[4];
    std::cout << "Correction angle: " << angle << '\n';
    //deg*pi/180=rad
    //2*acos(0)=pi
    angle = angle * 2 * acos(0) / 180;*/

    // Create and open text file
    std::ifstream inFile("coordinates.txt");
    std::string coord;
    int n = 0;
    //count lines in file (coordinates)
    while(getline(inFile, coord)){
        n++;
    }
    std::cout << "Total steps to target: " << n << '\n';
    int commands = n;
    bool done = false;
    if(commands < maxDrive){
        maxDrive = commands;
        done = true;
    }
    std::cout << "Max length of instructions: " << maxDrive << '\n';
    //reset file, so we can read from begining again
    inFile.clear();
    inFile.seekg(0, inFile.beg); 

    //int coords[commands][2];
    std::vector<std::vector<int>> coords(commands+3, std::vector<int> (2));
    n = 0;
    int a = 0;
    std::cout << "Input coordinates: \n  ";
    //read file line by line
    while(getline(inFile, coord)) 
    {
        for (int i = 0; i < (int)coord.length(); i++)
        {
            //if space detected
            if (coord[i] == ' ')
            {
                //if we have reached the end of the file break
                if (n >= commands){break;}
                //else set x coordinate
                coords.at(n).at(0) = a;
                a=0;
            }else {
                //if there is no space, add the digit to 10x the current value
                //(reading 1 & 5 would then be 1*10+5=15) The -48 is conversion from a charecter to an int
                a = a*10+(coord[i]-48);
            }
            
        }
        //if end of file break
        if (n >= commands){break;}
        //set y coordinate
        coords.at(n).at(1) = a;
        //print coordinates
        std::cout << coords.at(n).at(0) << "," << coords.at(n).at(1) << ' ';
        //reset a, and move up the index (n)
        a=0;
        n++;
        //if we have enough coordinates to generate instructions for zumo
        if(n == maxDrive){
            std::cout << "\n  Max instructions reached... " << n << '\n';
            std::cout << "Absolut coordinates for next time: " << (coords.at(n-1).at(1))*multiplier << ',' << (75-coords.at(n-1).at(0))*multiplier << '\n';
            //write next position to file
            std::ofstream position("position.txt");
            position << (coords.at(n-1).at(1))*multiplier << '\n';
            position << (75-coords.at(n-1).at(0))*multiplier << '\n';
            position << '0';
            position.close();
            
            std::cout << "  Finishing vectors: ";
            commands = n;
            coords.at(n+1).at(0) = coords.at(n).at(0);
            coords.at(n+1).at(1) = coords.at(n).at(1);
            std::cout << coords.at(n+1).at(0) << "," << coords.at(n+1).at(1) << ' ';
            coords.at(n+2).at(0) = coords.at(n).at(0);
            coords.at(n+2).at(1) = coords.at(n).at(1);
            std::cout << coords.at(n+2).at(0) << "," << coords.at(n+2).at(1) << ' ';
            break;
        }
    }

    //we now have an array of coordinates, we want an array of vector movements, 
    //less vector movements are better, so if we have same movement two times, 
    //be just multiply the vector by two. 
    //int vectors[commands][2];
    std::vector<std::vector<int>> vectors(commands, std::vector<int> (2));
    //store current vector
    int currentVec[2];
    //sote next vector
    int nextVec[2];
    //store command index
    n = 0;
    //store multiplication for vectors that are the same
    int t = 0;
    //store first vector with rotation
    //float firstVec[2] = {0,0};
    //if the vector is the first one
    //bool first = true;
    //for loop start
    for (int i = 0; i < commands; i++)
    {
        //calculate the step from current coordinates to the next coordinates
        currentVec[0] = coords.at(i+1).at(0) - coords.at(i).at(0);
        currentVec[1] = coords.at(i+1).at(1) - coords.at(i).at(1); 
        //calculate the step from next coordinates to the coordinates after the next ones     
        nextVec[0] = coords.at(i+2).at(0) - coords.at(i+1).at(0);
        nextVec[1] = coords.at(i+2).at(1) - coords.at(i+1).at(1);

        //if it is the first vector that should be written
        //we need to rotate it, so the zumo starts at the right angle compared to the map
        /*if (firstVec[0]==0 && firstVec[1]==0)
        {
            //rotate vector
            firstVec[0] = currentVec[0];//(float)currentVec[0]*cos(angle)-(float)currentVec[1]*sin(angle);
            firstVec[1] = currentVec[0];//(float)currentVec[0]*sin(angle)+(float)currentVec[1]*cos(angle);
            //if the component is clos to 0 set it to 0
            //firstVec[0] = (pow(firstVec[0], 2) < 0.0001) ? 0 : firstVec[0];
            //firstVec[1] = (pow(firstVec[1], 2) < 0.0001) ? 0 : firstVec[1];
            //print output
            std::cout << "\n  Start vector: " << currentVec[0] << ',' << currentVec[1] << '\n';
            //std::cout << "  Rotated start vector: " << firstVec[0] << ',' << firstVec[1];
        }*/

        if (currentVec[0]==nextVec[0] && currentVec[1]==nextVec[1])
        {
            //remember occurences
            t++;
        }else {
            if(i != commands-1){         
                //set vector
                vectors.at(n).at(0) = currentVec[0] * (t+1);
                vectors.at(n).at(1) = currentVec[1] * (t+1);
                n++;
                t = 0;
            }
        }

        if(i == commands-1){
            //Finishing vector        
            //set vector
            vectors.at(n).at(0) = currentVec[0] * (t+1);
            vectors.at(n).at(1) = currentVec[1] * (t+1);
            t = 0;
            break;
        }
    }

    //write instructions to file
    std::ofstream outFile("instructions.txt");
    std::cout << '\n' << "Vectors: " << n << '\n';
    for (int i = 0; i < n; i++)
    {
        //format is: x,y\n
        std::cout << vectors.at(i).at(1)*multiplier*10 << ',' << vectors.at(i).at(0)*-multiplier*10 << '\n';
        outFile << vectors.at(i).at(1)*multiplier*10 << ',' << vectors.at(i).at(0)*-multiplier*10 << '\n';
    }
    if (done)
    {
        outFile << "s";
    }else{
        outFile << "s";
    }
    outFile.close();
}


int main(){
    getInstructions();
    return 0;
}
