import math
from i2clibraries import i2c_hmc5883l
#Init hmc5883l 
hmc5883l = i2c_hmc5883l.i2c_hmc5883l(1)
hmc5883l.setContinuousMode()
#Calibration values
hard_iron_bias_x =  36.65907715075241
hard_iron_bias_y =  -16.399444667093103
hard_iron_bias_z =  -167.8957569077871


soft_iron_bias_xx =  0.0009839589932664592
soft_iron_bias_xy =  -4.0839140438046866e-05
soft_iron_bias_xz =  4.046422546036914e-05


soft_iron_bias_yx =  -4.083914043804681e-05
soft_iron_bias_yy =  0.0009489139414647667
soft_iron_bias_yz =  2.090277772765527e-05


soft_iron_bias_zx =  4.0464225460369264e-05
soft_iron_bias_zy =  2.090277772765516e-05
soft_iron_bias_zz =  0.0011271856754772967

x = 0.0
y = 0.0
z = 0.0

#Sampled measurement
for i in range(50):
  (tx, ty, tz) = hmc5883l.getAxes()
  x += tx/50
  y += ty/50
  z += tz/50
#Soft iron calibration
x = x - hard_iron_bias_x
y = y - hard_iron_bias_y
z = z - hard_iron_bias_z
#Hard iron calibration
x = soft_iron_bias_xx * x + soft_iron_bias_yx * y + soft_iron_bias_zx * z
y = soft_iron_bias_xy * x + soft_iron_bias_yy * y + soft_iron_bias_zy * z
z = soft_iron_bias_xz * x + soft_iron_bias_yz * y + soft_iron_bias_zz * z
#heading
heading = math.atan2(x, y) * 180 / math.pi
print(f"Raw heading: {heading}")

f = open("startHeading.txt", "r")
startHeading = int(f.readline())
print(f"Start heading: {startHeading}")
f.close() 

heading = heading - startHeading
if heading > 180:
  heading -= 360
if heading < -180:
  heading += 360
if heading < -180:
  heading += 360

heading = int(heading)
print(f"Heading is: {heading}")

f = open("heading.txt", "w")
f.write(str(int(heading)))
f.close()
