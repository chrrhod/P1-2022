#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int main(){
  //convert bitmap to xyz file
  std::ofstream mapxyz("mapv2.csv");
  mapxyz << "x,y\n";
  const int mapSize = 1500;
  //Import grid from file, and save as IGrid
  std::vector<std::vector<int>> iGrid(mapSize, std::vector<int> (mapSize));
  // Create and open a text file
  std::ifstream inFile("ogmap.txt");
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
  for (int i = 0; i < (int)iGrid.size(); i ++){
    for (int j = 0; j < (int)iGrid.at(i).size(); j++){
      if (iGrid.at(i).at(j) == 1){
        mapxyz << j << ',' << i << "\n";
      }
    }
  }
  mapxyz.close();
}
