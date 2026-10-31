# cppML
Machine Learning Models Implemented in C++


# Models 

## Clustering

### Kmeans

| Implementation | Dataset | N Clusters | Iterations | Total Time (ms) | Avg Iteration Time (ms) | StDev | Notes  | Compiler/Opt |
| -------- | -------- | -------- | -------- | -------- | -------- | -------- | -------- |  -------- |
| kmeans-basic | Iris (n=150) | 3 | 1000 | 2833.49  | 2.83349 | 1.14609 | No optimization, just a working algo | g++ -O0 |
| kmeans-cleanup | Iris (n=150) | 3 | 1000 | 1105.49  | 1.10549 | 0.184732 | Functions accept vector refs now | g++ -O0 |
| preallocate-vector | Iris (n=150) | 3 | 1000 | 960.514  | 0.960514 | 0.138862 | Reserve vector sizes  | g++ -O0 |
| try-stack-allocation | Iris (n=150) | 3 | 1000 | 382.588  | 0.382588 | 0.0168277 | swap vectors for presized arrays via templates; user unfriendly  | g++ -O0 |
| try-stack-allocation | Iris (n=150) | 3 | 1000 | 12.7214  | 0.0127214 | 4.79619e-05 |  same impl; optimized binary  | g++ -O2 |

## Regression

### Linear Regression

| Implementation | Dataset | N Features | Iterations | Total Time (ms) | Avg Iteration Time (ms) | StDev | Notes  | Compiler/Opt |
| -------- | -------- | -------- | -------- | -------- | -------- | -------- | -------- | -------- |
| vanilla-LR | MPG (n=393) | 7 | 1000 | 39086.4  | 39.0864 | 0.416398 | No optimizations; uses cramer-inverse for inverting matrices | g++ -O0 |
| reference-args | MPG (n=393) | 7 | 1000 | 23294.3  | 23.2943 | 0.813787 |Functions accept vector refs now | g++ -O0 |
| reference-args | MPG (n=393) | 7 | 1000 | 4281.71  | 4.28171 | 0.33003 |same impl; optimized binary | g++ -O2 |


## Classification

### Logistic Regression

| Implementation | Dataset | N Features | LRate | GD Iterations  |Iterations | Total Time (ms) | Avg Iteration Time (ms) | StDev | Notes  | Compiler/Opt |
| -------- | -------- | -------- | -------- | -------- | -------- | -------- | -------- | -------- | -------- | -------- |
| logistic-regression | wikipedia(n=20) | 1 | 0.001 | 10000 | 1000 | 136864  | 136.864 | 12.1644 | No optimizations |  g++ -O0 |
| ref-args | wikipedia(n=20) | 1 | 0.001 | 10000 | 1000 | 69424.4  | 69.4244 | 5.76012 | Reference Args | g++ -O0 |
| logreg-reserve | wikipedia(n=20) | 1 | 0.001 | 10000 | 1000 | 43001.7  | 43.0017 | 0.814364 | Reference Args | g++ -O0 |
| logreg-reserve | wikipedia(n=20) | 1 | 0.001 | 10000 | 1000 | 7275.39  | 7.27539 | 0.133346 | same impl; optimized binary | g++ -O2 |


### Decision Tree Classifier

| Implementation | Dataset | N Features | Iterations | Total Time (ms) | Avg Iteration Time (ms) | StDev | Notes  | Compiler/Opt |
| -------- | -------- | -------- | -------- | -------- | -------- | -------- | -------- | -------- |
| decTree | Iris (n=150) | 4 | 1000 | 7674.14  | 7.67414 | 1.11896 | No optimizations | g++ -O0 |
| decTree Refs | Iris (n=150) | 4 | 1000 | 4527.37  | 4.52737 | 0.0581833 | Ref args | g++ -O0 |
| decTreeReserve | Iris (n=150) | 4 | 1000 | 3550.82  | 3.55082 | 0.00171164 | Reserve vector sizes | g++ -O0 |



### KNearestNeighbors
| Implementation | Dataset | N Features | Iterations | Total Time (ms) | Avg Iteration Time (ms) | StDev | Notes  | Compiler/Opt |
| -------- | -------- | -------- | -------- | -------- | -------- | -------- | -------- | -------- |
| knn<sup>1</sup> | Random(10,000) | 10 | 1000 | 7658.64  | 7.65864 | 0.739412 | No optimizations | g++ -O0 |
| knn-refsreserve<sup>1</sup> | Random(10,000) | 10 | 1000 | 3452.65  | 3.45265 | 0.0180376 | Ref Args; Vector Reserves | g++ -O0 |
| knn-removepow<sup>1</sup> | Random(10,000) | 10 | 1000 | 1561.8  | 1.5618 | 0.00231417 | Replaced pow function | g++ -O0 |
| knn-array<sup>1</sup> | Random(10,000) | 10 | 1000 | 1331.49  | 1.33149 | 0.00511735 | Template sized arrays | g++ -O0 |
| knn-partial-sort<sup>1</sup> | Random(10,000) | 10 | 1000 | 414.443  | 0.414443 | 0.00217342 | sort smallest k elements only | g++ -O0 |
| knn-const-args<sup>1</sup> | Random(10,000) | 10 | 1000 | 404.047  | 0.404 | 0.00010546 | const args | g++ -O0 |
| knn-const-args<sup>1</sup> | Random(10,000) | 10 | 1000 | 36.809  | 0.036 | 1.23533e-05 | const args | g++ -O2 |
Brute Force Search<sup>1</sup>