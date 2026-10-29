from sklearn.tree import DecisionTreeClassifier
from sklearn.datasets import load_iris
import time
import statistics as st
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