import time
from i2clibraries import i2c_hmc5883l
hmc5883l = i2c_hmc5883l.i2c_hmc5883l(1)
hmc5883l.setContinuousMode()
# To scaled axes
f = open("mag_out.txt", "w")
while True:
	j = input()
	if j == "0":
		print("Closing file...")
		break
	for i in range(1000):
		(x, y, z) = hmc5883l.getAxes()
		print(f"Coordinates: x {x}, y {y}, z {z}")
		f.write(f"{x},{y},{z}\n")
		time.sleep(0.01)
f.close()
