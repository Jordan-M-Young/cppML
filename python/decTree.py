from sklearn.tree import DecisionTreeClassifier
from sklearn.neighbors import NearestNeighbors
from sklearn.datasets import load_iris
import time
import statistics as st
import numpy as np


data = load_iris()

times = []
for i in range(1000):
    t1 = time.time()
    clf = DecisionTreeClassifier(random_state=0, max_depth=3)
    clf.fit(data.data, data.target)
    clf.predict(data.data)
    t2 = time.time()
    times.append((t2-t1)*1000)

print("Total Time (ms):  ",sum(times))
print("Mean Time (ms):  ",st.mean(times))



X = np.array([[1, 1], [2, 1], [3, 2], [8, 8], [9, 8]])

nn = NearestNeighbors(n_neighbors=10)
nn.fit(X)





indices = nn.kneighbors([[2, 2]], return_distance=False)

