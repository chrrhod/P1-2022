/* her kommer programmer for LiDAR måling*/

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cmath>
#include <bitset>
#include <fstream>
#include <string>
#include "lds_driver.h"
#include <thread>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/SVD>

using namespace std;

#define PI 3.14159265
float xy_values [2][362];
float LiDAR_values [360];
float pos_ar[3];
float y_n[2][360];
float p_n[360];
float p_nl[360];
float xyr_val[2];
std::string y_nt[361];
//transformation matrix
Eigen::VectorXf x_0(2);
Eigen::VectorXf y_0(2);


void xy(){
    float rad;
    //cout<<"xy/xyval: "<<xy_values[0][360]<<" , "<<xy_values[1][360]<<std::endl;
    for (int i=0;i<360;i++){
        rad = (PI*(i+90))/180;
        xy_values[0][i] = cos(rad) * (LiDAR_values[i]+3) + xy_values[0][360];
        xy_values[1][i] = sin(rad) * (LiDAR_values[i]+3) + xy_values[1][360];
        //cout<<"LiDAR: "<<LiDAR_values[i]<<std::endl;
        //cout<<"pos: "<<xy_values[0][360]<<" , "<<xy_values[1][360]<<std::endl;
        //cout<<"xy :"<<xy_values[0][i]<<" , "<<xy_values[1][i]<<std::endl;
        if (LiDAR_values[i] < 12){
            p_n[i] = 0.0;
        }    
        else if (LiDAR_values[i] > 50){
            if (LiDAR_values[i] > 150){
                p_n[i] = 0;
            }
            else{
                p_n[i] = 0.95;
            }
        }
        else{
            p_n[i] = 0.875;
        }
    }
}

void xyz_print(int i){
    stringstream ss;
    ss << i;
    ofstream xyzmap("/home/pi/Navigating-Autonomously-with-a-Zumo-Implementation/RaspberyPi/testaa/testaa"+ss.str()+".csv");
    xyzmap<<"x,y,z"<<'\n';
    for (int i=0;i<360;i++){
                xyzmap<<xy_values[0][i]<<","<<xy_values[1][i]<<",0"<<'\n';
    }
    xyzmap.close();
}
void yn_print(int x){
    stringstream ss;
    ss << x;
    ofstream xyzmap("/home/pi/Navigating-Autonomously-with-a-Zumo-Implementation/RaspberyPi/testaa/yn"+ss.str()+".csv");
    xyzmap<<"x,y,z"<<'\n';
    for (int i=0;i<360;i++){
                xyzmap<<y_n[0][i]<<","<<y_n[1][i]<<",0"<<'\n';
    }
    xyzmap.close();
}

void pos_read(){
    int a;
    ifstream pos("position.txt");
    std::string text;
    std::string pos_x;
    std::string pos_y;
    int i = 0;
    while (getline (pos, text)){
        pos_ar[i] = std::stof(text);
        i = i+1;
        text = "";
        
    }
    pos.close();
}


void closest_neighbor(){
    int i = 0;
    float x;
    float y;
    float temp_dis;
    float dis;
    float length;
    std::string xs;
    std::string ys;
    std::string text;
    std::string delimiter = ",";
    for (int i=0;i<360;i++){
        std::ifstream map("map.csv");
        dis = 200000.0;
        while (getline (map, text)){
            if (text != "x,y"){
                xs = text.substr(0, text.find(delimiter)); 
                ys = text.substr(text.find(delimiter)+1,text.length());
                x = stof(xs);
                y = stof(ys);
                length = sqrtf(pow (x-xy_values[0][i],2) + pow(y-xy_values[1][i],2));
                /*
                */
                if (length < dis){
                    dis = length;
                    y_n[0][i]=(int)x;
                    y_n[1][i]=(int)y;
                }
            }
            else{
            }
            text = "";
        }
        //cout<<"yn"<<i<<"= "<<y_n[0][i]<<" , "<<y_n[1][i]<<std::endl;
        map.close();
    }
}



void algebra(int it){
    int i;
    float x_00 = 0;
    float x_01 = 0;
    float y_00 = 0;
    float y_01 = 0;
    float sump_n = 0;
    x_0<<0,0;
    y_0<<0,0;
    
    for (i=0;i<360;i++){
        if (it==0){
        }
        x_00 = x_00 + xy_values[0][i]*p_n[i];
        x_01 = x_01 + xy_values[1][i]*p_n[i];
        
        y_00 = y_00 + y_n[0][i]*p_n[i];
        y_01 = y_01 + y_n[1][i]*p_n[i];
        
        sump_n += p_n[i];
        
    }
    x_0 << x_00/sump_n, x_01/sump_n;
    y_0 << y_00/sump_n, y_01/sump_n;
    
    cout<<"x_0 "<<x_0<<std::endl;
    cout<<"y_0 "<<y_0<<std::endl;
    cout<<"sump_n "<<sump_n<<std::endl;
    
    
if (it==0){
}
    

Eigen::VectorXf x_temp(2);
Eigen::VectorXf y_temp(2);
Eigen::VectorXf x_(2);
Eigen::VectorXf y_sub(2);
Eigen::Matrix2f H;
H << 0,0,
    0,0;


float sumx_x = 0;
float sumx_y = 0;
float sumy_x = 0;
float sumy_y = 0;

for (i=0;i<360;i++){//ændres til 360
    x_temp << xy_values[0][i], xy_values[1][i];
    y_temp << y_n[0][i], y_n[1][i];
    H = H + ((y_temp-y_0)*(x_temp-x_0).transpose()*p_n[i]);  
}
cout<<"H "<<H<<std::endl;


Eigen::JacobiSVD<Eigen::MatrixXf> svd(H.transpose(), Eigen::ComputeThinV | Eigen::ComputeThinU );

Eigen::MatrixXf R;
Eigen::MatrixXf t;
Eigen::MatrixXf U;
Eigen::MatrixXf V;
U = svd.matrixU();
V = svd.matrixV();



if (U.determinant()*V.determinant() < 0){
    for (i=0;i<2;++i)
        V(i, 1) *= -1;
        cout<<"s<0"<<std::endl;
}


R = V*(U.transpose());
cout<<"Det(R)= "<<R.determinant();
cout<<"R: "<<R<<std::endl;

t = y_0-R*x_0;
cout<<"t "<<t<<std::endl;
Eigen::VectorXf xn_trans (2);
Eigen::VectorXf temp2 (2);


for (i=0;i<362;i++){
    temp2<< xy_values[0][i],xy_values[1][i];
 
    xn_trans = t+(R*temp2);
    xy_values[0][i] = xn_trans(0);
    xy_values[1][i] = xn_trans(1);
}

}



namespace lds
{
LFCDLaser::LFCDLaser(const std::string& port, uint32_t baud_rate, boost::asio::io_service& io)
  : port_(port), baud_rate_(baud_rate), shutting_down_(false), serial_(io, port_)
{
  serial_.set_option(boost::asio::serial_port_base::baud_rate(baud_rate_));

  // Below command is not required after firmware upgrade (2017.10)
  boost::asio::write(serial_, boost::asio::buffer("b", 1));  // start motor
}

LFCDLaser::~LFCDLaser()
{
  boost::asio::write(serial_, boost::asio::buffer("e", 1));  // stop motor
}

void LFCDLaser::poll()
{
  uint8_t temp_char;
  uint8_t start_count = 0;
  bool got_scan = false;
  boost::array<uint8_t, 2520> raw_bytes;
  uint8_t good_sets = 0;
  uint32_t motor_speed = 0;
  rpms=0;
  int index;
for (int flag=0;flag<4;flag++){
    got_scan = false;
  while (!shutting_down_ && !got_scan)
  {
    // Wait until first data sync of frame: 0xFA, 0xA0
    boost::asio::read(serial_, boost::asio::buffer(&raw_bytes[start_count],1));

    if(start_count == 0)
    {
      if(raw_bytes[start_count] == 0xFA)
      {
        start_count = 1;
      }
    }
    else if(start_count == 1)
    {
      if(raw_bytes[start_count] == 0xA0)
      {
        start_count = 0;

        // Now that entire start sequence has been found, read in the rest of the message
        got_scan = true;

        boost::asio::read(serial_,boost::asio::buffer(&raw_bytes[2], 2518));

        // scan->angle_min = 0.0;
        // scan->angle_max = 2.0*M_PI;
        // scan->angle_increment = (2.0*M_PI/360.0);
        // scan->range_min = 0.12;
        // scan->range_max = 3.5;
        // scan->ranges.resize(360);
        // scan->intensities.resize(360);

        //read data in sets of 6
        for(uint16_t i = 0; i < raw_bytes.size(); i=i+42)
        {
          if(raw_bytes[i] == 0xFA && raw_bytes[i+1] == (0xA0 + i / 42)) //&& CRC check
          {
            good_sets++;
            motor_speed += (raw_bytes[i+3] << 8) + raw_bytes[i+2]; //accumulate count for avg. time increment
            rpms=(raw_bytes[i+3]<<8|raw_bytes[i+2])/10;

            for(uint16_t j = i+4; j < i+40; j=j+6)
            {
              index = 6*(i/42) + (j-4-i)/6;

              // Four bytes per reading
              uint8_t byte0 = raw_bytes[j];
              uint8_t byte1 = raw_bytes[j+1];
              uint8_t byte2 = raw_bytes[j+2];
              uint8_t byte3 = raw_bytes[j+3];

              // Remaining bits are the range in mm
              uint16_t intensity = (byte1 << 8) + byte0;

              // Last two bytes represent the uncertanty or intensity, might also be pixel area of target...
              // uint16_t intensity = (byte3 << 8) + byte2;
              uint16_t range = (byte3 << 8) + byte2;

              // scan->ranges[359-index] = range / 1000.0;
              // scan->intensities[359-index] = intensity;
              //printf ("grader: %d ",359-index);
              //printf ("længde:  %f \n", range /1000.0);
              LiDAR_values[359-index] = range/10;
              //cout<<"LiDAR: "<<range;
            }
          }
        }
        
        // scan->time_increment = motor_speed/good_sets/1e8;
      }
      else
      {
        start_count = 0;
      }
    }
  }
  cout<<"Lidar: "<<flag<<std::endl;
}
}
}


void update_map(std::string s){
    std::ofstream upmap("bitmap.txt");
    upmap<<s;
    upmap.close();
    
}


void pos_angle(){
    float posx = xy_values[0][360];
    float posy = xy_values[1][360];
    cout<<"final pos: "<<posx<<" , "<<posy<<std::endl;
    std::ofstream pos("position.txt");
    pos<<posx<<'\n';
    pos<<posy<<'\n';
    pos<<pos_ar[2];
}

void generate_map(){
    //convert bitmap to xyz file
    std::ofstream mapxyz("mapv2.csv");
    mapxyz << "x,y\n";
    for (int i = 0; i < 360; i++){
        if (LiDAR_values[i] < 2000){
            mapxyz << xy_values[0][i] << ',' << xy_values[1][i] << "\n";
      }
    }
  mapxyz.close();
}



int main(){
    std::string port;
    int baud_rate;
    uint16_t rpms;
    port = "/dev/ttyUSB0";
    baud_rate = 230400;
    boost::asio::io_service io;
    int x_map = 150;
    int map_size = x_map*x_map;
    int pos = ((map_size)/2)+x_map/2;
    std::string s(150*150,'0');
    int i = 0;
    float pos_x;
    float pos_y;
    bool run = true;
    

  try
  {
    lds::LFCDLaser laser(port, baud_rate, io);
    //måling til pre map, slettes nå vi har premap
    std::ifstream ogmap("ogmap.txt");
    ogmap >> s;
    ogmap.close();
    pos_read();
    pos_x = pos_ar[0];
    pos_y = pos_ar[1];
    xy_values[0][360] = pos_x;
    xy_values[1][360] = pos_y;
    xy_values[0][361] = pos_x;
    xy_values[1][361] = pos_y+1;
    cout<<"starting pos: "<<pos_x<<" , "<<pos_y<<std::endl;
    
    while (run)
    {   
        laser.poll();
        xy();
        xyz_print(0);
        for (int i=0;i<20;i++){
            cout<<"iterate number: "<<i<<std::endl;
            closest_neighbor();
            algebra(i);
            xyz_print(i+1);
            yn_print(i);
        }
        update_map(s);
        pos_angle();
        
        run = false;
    }
    laser.close();

    return 0;
  }
  catch (boost::system::system_error ex)
  {
    printf("An exception was thrown: %s", ex.what());
    cout<<"LiDAR Fejl"<<std::endl;
    return -1;
  }

}
