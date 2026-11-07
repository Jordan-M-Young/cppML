from sklearn.neighbors import NearestNeighbors
import time
import statistics as st
import numpy as np

# Create the generator
rng = np.random.default_rng()

# Generate the 10000x10 array [0.0, 1.0)
arr = rng.random((10000, 10))

sample = rng.random((1, 10))


nn = NearestNeighbors(n_neighbors=10)
nn.fit(arr)
times = []
for i in range(1000):
    t1 = time.time()
    indices = nn.kneighbors(sample, return_distance=False)
    t2 = time.time()
    times.append((t2-t1)*1000)

print("Total Time (ms):  ",sum(times))
print("Mean Time (ms):  ",st.mean(times))







