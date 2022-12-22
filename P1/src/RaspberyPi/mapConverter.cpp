#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>


void convertMap(){
    //read settings from file
    std::ifstream settings("settings.txt");
    std::string input;
    int setting;
    int s = 0;
    int mS, omS, mR, oR, gX, gY;
    std::cout << "Settings for conversion: \n";
    while(getline(settings, input)){
        setting = stoi(input);
        switch(s) {
            case 0:
                mS = setting;
                std::cout << "  Input map size: " << setting << '\n';
                break;
            case 1:
                omS = setting;
                std::cout << "  Output map size: " << setting << '\n';
                break;
            case 2:
                mR = setting;
                std::cout << "  Map resolution (1 unit = resolution input map): " << setting << '\n';
                break;
            case 3:
                oR = setting;
                std::cout << "  Obstacle inflation radius: " << setting << '\n';
                break;
            case 4:
                gX = setting / 2;
                std::cout << "  Goal x: " << setting << '\n';
                break;
            case 5:
                gY = setting / 2;
                std::cout << "  Goal Y: " << setting << '\n';
                break;
            default:
                std::cout << "Error setting variables for converter :(" << '\n';
                break;
            }
        s++;
    }

    const int mapSize = mS;
    const int outMapSize = omS;
    const int mapRes = mR;
    const int obstacleRadius = oR;
    //converted grid
    //int cGrid[outMapSize][outMapSize];
    std::vector<std::vector<int>> cGrid(outMapSize, std::vector<int> (outMapSize));
    //input grid
    //bool iGrid[mapSize][mapSize];
    std::vector<std::vector<int>> iGrid(mapSize, std::vector<int> (mapSize));

    //Import grid from file, and save as IGrid
    // Create and open a text file
    std::ifstream inFile("bitmap.txt");

    //import iGrid from file
    //needs to be bool to avoid taking up too much space
    bool fileOut;
    for (int i = 0; i < mapSize; i++)
    {
        for (int j = 0; j < mapSize; j++)
        {
            //conversion to int
            if ((int)inFile.get() - 48 == 1)
            {
                fileOut = 1;
            }else {
                fileOut = 0;
            }
            iGrid.at(i).at(j) = fileOut;
        }
    }
    // Close the file
    inFile.close();

    //scale iGrid to cGrid
    for (int i = 0; i < outMapSize; i++)
    {
        for (int j = 0; j < outMapSize; j++)
        {
            //we are now iterating trough the scaled grid
            for (int y = 0; y < mapRes; y++)
            {
                for (int x = 0; x < mapRes; x++)
                {
                    //if there is an obstacle on the big grid, we set the scaled grid on in this position
                    if (iGrid.at(i*mapRes+y).at(j*mapRes+x) == 1)
                    {
                        cGrid.at(i).at(j) = 1;
                        break;
                    }
                    if (cGrid.at(i).at(j) == 1)
                    {
                        break;
                    }
                    
                }
                
            }
        }
        
    }

    //Scale alle obstacles by obstacleRadius
    for (int i = 0; i < outMapSize; i++)
    {
        for (int j = 0; j < outMapSize; j++)
        {
            //if there is an obstacle
            if (cGrid.at(i).at(j) == 1){
                //scaleGrid stores a grid to get the bits sourrunding the obstacle
                std::vector<int> scaleGrid(obstacleRadius*2+1);
                //start on negative
                scaleGrid.at(0) = -obstacleRadius;
                //add positions op to obstacleRadius
                for (int k = 1; k <= (obstacleRadius*2); k++)
                {   
                    scaleGrid.at(k) = scaleGrid.at(k-1) + 1;
                }
                //now we have an array going from -obstacleRadius to obstacleRadius
                //time to add the scaled obstacles to cGrid
                for (int k = 0; k <= obstacleRadius*2; k++)
                {
                    for (int l = 0; l <= obstacleRadius*2; l++)
                    {
                        if (i+scaleGrid.at(k) >= 0 && i+scaleGrid.at(k) < outMapSize)
                        {
                            if (j+scaleGrid.at(l) >= 0 && j+scaleGrid.at(l) < outMapSize)
                            {
                                //sets the obstacle to 2, so we wont go over it again.
                                if (cGrid.at(i+scaleGrid.at(k)).at(j+scaleGrid.at(l)) == 0)
                                {
                                    cGrid.at(i+scaleGrid.at(k)).at(j+scaleGrid.at(l)) = 2;
                                }
                                //we now have a square around the original obstacle 
                                //with the square marked by 2 and not 1. 
                            }
                            
                        }
                    }
                }
                //set to 3 to defferentiate in output
                cGrid.at(i).at(j) = 3;
                
            }
        }
    }

    //set the 2's back to obstacles
    for (int i = 0; i < outMapSize; i++)
    {
        for (int j = 0; j < outMapSize; j++)
        {
            if (cGrid.at(i).at(j) == 2)
            {
                cGrid.at(i).at(j) = 1;
            }
            
        }
    }

    //print map to terminal
    std::cout << "\nFinished bitmap:\n \n";
    for (int i = outMapSize-1; i >= 0; i--) {
        for (int j = 0; j < outMapSize; j++) {
            if(cGrid.at(i).at(j)==0){
                //print space
                std::cout << "\x1b[1;30m" << "◌ ";
            }else if(cGrid.at(i).at(j)==1){
                //print a inflation
                std::cout << "\x1b[1;30m" << "✖ ";
            }else{
                //print obstacle
                std::cout << "\x1b[1;31m" << "✖ ";
            }
        }
        //reset color, and new line
        std::cout << "\x1b[1;0m" << '\n';
    }

    //print map to file
    std::ofstream outFile("sBitmap.txt");
    for (int i = 0; i < outMapSize; i++) {
        for (int j = 0; j < outMapSize; j++) {
            //if output is more than 0 it should be 1
            if (cGrid.at(i).at(j) > 0) outFile << 1;
            else outFile << 0;
        }
        outFile << '\n';
    }
    outFile.close();


    //print converted start coordinates to file
    //read settings from file
    std::ifstream position("position.txt");
    int coordinates[10]; //hold numbers from file
    s = 0;
    while(getline(position, input)){
        coordinates[s] = stoi(input);
        s++;
    }
    position.close();
    /*
    //read angle
    std::ifstream angle("heading.txt");
    int angleString[10]; //hold numbers from file
    s = 0;
    while(getline(angle, input)){
        std::cout << input << '\n';
        angleString[s] = stoi(input);
        s++;
    }
    angle.close();*/

    //print position to file
    std::ofstream points("points.txt");
    std::cout << "  Points" << '\n';
    points << coordinates[0]/mapRes  << '\n';
    std::cout << "  Position x: " << coordinates[0]/mapRes << '\n';
    points << coordinates[1]/mapRes  << '\n';
    std::cout << "  Position y: " << coordinates[1]/mapRes << '\n';
    points << gX << '\n';
    std::cout << "  Goal x: " << gX << '\n';
    points << gY << '\n';
    std::cout << "  Goal y: " << gY << '\n';
    points << 0 << '\n';
    std::cout << "  Position angle: " << 0 << '\n';
    points.close();

    std::cout << "Conversion done" << '\n';

    //write instructions to file
    std::ofstream instructDone("instructions.txt");
    float distance = sqrtf(pow((coordinates[0]/mapRes)-gX,2)+pow((coordinates[1]/mapRes)-gY,2));
    if (distance < 3.5){
        std::cout << "Done! Reached goal.\n";
        instructDone << "d";
    }
    instructDone.close();    
}


int main(){
    convertMap();
    return 0;
}
