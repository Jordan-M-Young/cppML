# cppML
Machine Learning Models Implemented in C++


# Models 


## Kmeans

| Implementation | Dataset | N Clusters | Iterations | Total Time (ms) | Avg Iteration Time (ms) | StDev | Notes  |
| -------- | -------- | -------- | -------- | -------- | -------- | -------- | -------- |
| kmeans-basic | Iris (n=150) | 3 | 1000 | 2833.49  | 2.83349 | 1.14609 | No optimization, just a working algo |
| kmeans-cleanup | Iris (n=150) | 3 | 1000 | 1105.49  | 1.10549 | 0.184732 | Functions accept vector refs now |
| preallocate-vector | Iris (n=150) | 3 | 1000 | 960.514  | 0.960514 | 0.138862 | Reserve vector sizes  |