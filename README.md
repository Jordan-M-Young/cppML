# cppML
Machine Learning Models Implemented in C++


# Models 

## Kmeans

| Implementation | Dataset | N Clusters | Iterations | Total Time (ms) | Avg Iteration Time (ms) | StDev | Notes  |
| -------- | -------- | -------- | -------- | -------- | -------- | -------- | -------- |
| kmeans-basic | Iris (n=150) | 3 | 1000 | 2833.49  | 2.83349 | 1.14609 | No optimization, just a working algo |
| kmeans-cleanup | Iris (n=150) | 3 | 1000 | 1105.49  | 1.10549 | 0.184732 | Functions accept vector refs now |
| preallocate-vector | Iris (n=150) | 3 | 1000 | 960.514  | 0.960514 | 0.138862 | Reserve vector sizes  |
| try-stack-allocation | Iris (n=150) | 3 | 1000 | 382.588  | 0.382588 | 0.0168277 | swap vectors for presized arrays via templates; user unfriendly  |



## Linear Regression

| Implementation | Dataset | N Features | Iterations | Total Time (ms) | Avg Iteration Time (ms) | StDev | Notes  |
| -------- | -------- | -------- | -------- | -------- | -------- | -------- | -------- |
| vanilla-LR | MPG (n=393) | 7 | 1000 | 39086.4  | 2.83349 | 39.0864 | No optimizations; uses cramer-inverse for inverting matrices |
