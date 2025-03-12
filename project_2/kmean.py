import string
import numpy as np
from numpy import random as rand

rand.seed(5)
data = rand.randint(0, 100,100)
print(data)

k: int = 2

# initialize centroid
centers = rand.randint(0, 100, 2)
print(centers)

group0 = []
group1 = []

# Define a stable condition
stable = False
threshhold = .001 # Represent the percentage of max centroid change

# Step 2: Start K-mean repeating process
interations = 0
while not stable and interations < 100:
  interations += 1
  group0 = []
  group1 = []

  # Traverse each number and calculate its distance to each centroid
  # Pick the smaller group as group assignment
  for i in range(data.shape[0]):
    value0 =  abs(data[i] - centers[0])
    value1 =  abs(data[i] - centers[1])
    
    if value0 < value1:
      group0.append(data[i])
    else:
      group1.append(data[i])

  # Re-compute the new centroid
  centers0_avg = sum(group0) / len(group0)
  centers1_avg = sum(group1) / len(group1)

  print(np.array(group0))
  print(np.array(group1))

  # Check if the centers get stable
  if abs(centers[0] - centers0_avg) < threshhold and abs(centers[1] - centers1_avg):
    stable = True
  
  centers[0] = centers0_avg
  centers[1] = centers1_avg