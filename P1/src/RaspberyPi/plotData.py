import pandas as pd
import matplotlib.pyplot as plt

offset = [0, 0]
#lidar = pd.read_csv('lidarMeasurement.csv')

map = pd.read_csv('map.csv')
plt.scatter(map.x, map.y, label='Map')

test0 = pd.read_csv('testaa/testaa0.csv')
plt.scatter(test0.x, test0.y, label='0. Interation', marker="x")
#yn0 = pd.read_csv('testaa/yn0.csv')
#plt.scatter(yn0.x, yn0.y, label='0. Interation match', marker="+")
test1 = pd.read_csv('testaa/testaa1.csv')
plt.scatter(test1.x, test1.y, label='1. Interation', marker="x")
#yn1 = pd.read_csv('testaa/yn1.csv')
#plt.scatter(yn1.x, yn1.y, label='1. Interation match', marker="+")
#test2 = pd.read_csv('testaa/testaa2.csv')
#plt.scatter(test2.x, test2.y, label='2. Interation', marker="x")
#yn2 = pd.read_csv('testaa/yn2.csv')
#plt.scatter(yn2.x, yn2.y, label='2. Interation match', marker="+")
#test3 = pd.read_csv('testaa/testaa3.csv')
#plt.scatter(test3.x, test3.y, label='3. Interation', marker="x")
#yn3 = pd.read_csv('testaa/yn3.csv')
#plt.scatter(yn3.x, yn3.y, label='3. Interation match', marker="+")
#test4 = pd.read_csv('testaa/testaa4.csv')
#plt.scatter(test4.x, test4.y, label='4. Interation', marker="x")
#yn4 = pd.read_csv('testaa/yn4.csv')
#plt.scatter(yn4.x, yn4.y, label='4. Interation match', marker="+")
test5 = pd.read_csv('testaa/testaa5.csv')
plt.scatter(test5.x, test5.y, label='5. Interation', marker="x")
#yn5 = pd.read_csv('testaa/yn5.csv')
#plt.scatter(yn5.x, yn5.y, label='5. Interation match', marker="+")
"""
test6 = pd.read_csv('testaa/testaa6.csv')
plt.scatter(test6.x, test6.y, label='6. Interation')
test7 = pd.read_csv('testaa/testaa7.csv')
plt.scatter(test7.x, test7.y, label='7. Interation')
test8 = pd.read_csv('testaa/testaa8.csv')
plt.scatter(test8.x, test8.y, label='8. Interation')
test9 = pd.read_csv('testaa/testaa9.csv')
plt.scatter(test9.x, test9.y, label='9. Interation')
yn9 = pd.read_csv('testaa/yn9.csv')
plt.scatter(yn9.x, yn9.y, label='9. Interation match', marker="+")
"""
test10 = pd.read_csv('testaa/testaa10.csv')
plt.scatter(test10.x, test10.y, label='10. Interation', marker="x")
test15 = pd.read_csv('testaa/testaa15.csv')
plt.scatter(test15.x, test15.y, label='15. Interation', marker="x")
test20 = pd.read_csv('testaa/testaa20.csv')
plt.scatter(test20.x, test20.y, label='20. Interation', marker="x")

#plt
plt.title("Lidar Reading")
plt.xlabel('X (mm)')
plt.ylabel('Y (mm)')
plt.legend()
plt.show()
 
