import matplotlib.pyplot as plt
import csv
import numpy as np
#x=[]
#y=[]
z= [0] * 1000
#with open('out_file.txt')as csvfile:

#    plots=csv.reader(csvfile,delimiter='\t')
#    for row in plots:
#        x.append(int(row[0]))
#        y.append(int(row[1]))

x,y= np.loadtxt('out_file.txt',delimiter=',',unpack=True)
x=range(1,1001)
#print(x)
#print(y)
#plt.plot(x,y,label='Loaded from out_file.txt')
for i in range(0,1000):
    #z[i]=sum(y[:i]) / float(i+1)
    #z[i]=min(y[:i+1])
    z[i]=max(y[:i+1]) 

print(z)
plt.plot (x,z,label='Loaded from out_file.txt')

plt.xlabel('Values from 1->1000')
plt.ylabel('Avg. of collisions')
plt.title('Graph depiction of collisions in twine_fun1')
plt.show()
