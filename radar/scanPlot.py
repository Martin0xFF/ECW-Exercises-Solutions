import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import pandas as pd
import numpy as np



data = open('data.csv','r')
df = pd.read_csv(data)

print(df)


angle = df['angle']
pitch = df['pitch']
distance = df['dist']

n = len(angle)

xs = np.zeros(n)
ys = np.zeros(n)
zs = np.zeros(n)

for i in range(0,len(df)):
    xs[i] = float(distance.iloc[i])*np.sin(np.pi*float(pitch.iloc[i])/180.0)*np.cos(np.pi*float(angle.iloc[i])/180.0)
    ys[i] = float(distance.iloc[i])*np.sin(np.pi*float(pitch.iloc[i])/180.0)*np.sin(np.pi*float(angle.iloc[i])/180.0)
    zs[i] = float(distance.iloc[i])*np.cos(np.pi*float(pitch.iloc[i])/180.0)


fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

ax.scatter(xs,ys,zs, 'z',20)
plt.show()

