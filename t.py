import numpy as np

class Node:
    def __init__(self, feature=None, threshold=None, left=None, right=None, *, value=None):
        self.feature = feature        # Index of the feature to split on
        self.threshold = threshold  # Threshold value for split
        self.left = left              # Left child node
        self.right = right            # Right child node
        self.value = value            # Classification label (only if leaf node)

    def is_leaf(self):
        return self.value is not None

class DecisionTreeFromScratch:
    def __init__(self, max_depth=10, min_samples_split=2):
        self.max_depth = max_depth
        self.min_samples_split = min_samples_split
        self.root = None

    def _entropy(self, y):
        hist = np.bincount(y)
        ps = hist / len(y)
        return -np.sum([p * np.log2(p) for p in ps if p > 0])

    def _best_split(self, X, y):
        best_gain = -1
        split_idx, split_thresh = None, None
        n_features = X.shape[1]
        
        for feat_idx in range(n_features):
            X_column = X[:, feat_idx]
            thresholds = np.unique(X_column)
            for threshold in thresholds:
                # Calculate Information Gain
                parent_entropy = self._entropy(y)
                
                left_idxs = np.argwhere(X_column <= threshold).flatten()
                right_idxs = np.argwhere(X_column > threshold).flatten()
                
                if len(left_idxs) == 0 or len(right_idxs) == 0:
                    continue
                    
                n = len(y)
                n_l, n_r = len(left_idxs), len(right_idxs)
                e_l, e_r = self._entropy(y[left_idxs]), self._entropy(y[right_idxs])
                child_entropy = (n_l / n) * e_l + (n_r / n) * e_r
                
                ig = parent_entropy - child_entropy
                
                if ig > best_gain:
                    best_gain = ig
                    split_idx = feat_idx
                    split_thresh = threshold
                    
        return split_idx, split_thresh

    def _grow_tree(self, X, y, depth=0):
        n_samples, n_features = X.shape
        n_labels = len(np.unique(y))

        # Stopping criteria
        if (depth >= self.max_depth or n_labels == 1 or n_samples < self.min_samples_split):
            most_common = np.bincount(y).argmax()
            return Node(value=most_common)

        # Find the best split
        feat_idx, threshold = self._best_split(X, y)
        
        # Create child splits
        left_idxs = np.argwhere(X[:, feat_idx] <= threshold).flatten()
        right_idxs = np.argwhere(X[:, feat_idx] > threshold).flatten()
        
        left = self._grow_tree(X[left_idxs, :], y[left_idxs], depth + 1)
        right = self._grow_tree(X[right_idxs, :], y[right_idxs], depth + 1)
        return Node(feature=feat_idx, threshold=threshold, left=left, right=right)

    def fit(self, X, y):
        self.root = self._grow_tree(X, y)

    def _traverse_tree(self, x, node):
        if node.is_leaf():
            return node.value
        
        if x[node.feature] <= node.threshold:
            return self._traverse_tree(x, node.left)
        return self._traverse_tree(x, node.right)

    def predict(self, X):
        return np.array([self._traverse_tree(x, self.root) for x in X])
import numpy as np

class Node:
    def __init__(self, feature=None, threshold=None, left=None, right=None, *, value=None):
        self.feature = feature        # Index of the feature to split on
        self.threshold = threshold  # Threshold value for split
        self.left = left              # Left child node
        self.right = right            # Right child node
        self.value = value            # Classification label (only if leaf node)

    def is_leaf(self):
        return self.value is not None

class DecisionTreeFromScratch:
    def __init__(self, max_depth=10, min_samples_split=2):
        self.max_depth = max_depth
        self.min_samples_split = min_samples_split
        self.root = None

    def _entropy(self, y):
        hist = np.bincount(y)
        ps = hist / len(y)
        return -np.sum([p * np.log2(p) for p in ps if p > 0])

    def _best_split(self, X, y):
        best_gain = -1
        split_idx, split_thresh = None, None
        n_features = X.shape[1]
        
        for feat_idx in range(n_features):
            X_column = X[:, feat_idx]
            thresholds = np.unique(X_column)
            for threshold in thresholds:
                # Calculate Information Gain
                parent_entropy = self._entropy(y)
                
                left_idxs = np.argwhere(X_column <= threshold).flatten()
                right_idxs = np.argwhere(X_column > threshold).flatten()
                
                if len(left_idxs) == 0 or len(right_idxs) == 0:
                    continue
                    
                n = len(y)
                n_l, n_r = len(left_idxs), len(right_idxs)
                e_l, e_r = self._entropy(y[left_idxs]), self._entropy(y[right_idxs])
                child_entropy = (n_l / n) * e_l + (n_r / n) * e_r
                
                ig = parent_entropy - child_entropy
                
                if ig > best_gain:
                    best_gain = ig
                    split_idx = feat_idx
                    split_thresh = threshold
                    
        return split_idx, split_thresh

    def _grow_tree(self, X, y, depth=0):
        n_samples, n_features = X.shape
        n_labels = len(np.unique(y))

        # Stopping criteria
        if (depth >= self.max_depth or n_labels == 1 or n_samples < self.min_samples_split):
            most_common = np.bincount(y).argmax()
            return Node(value=most_common)

        # Find the best split
        feat_idx, threshold = self._best_split(X, y)
        
        # Create child splits
        left_idxs = np.argwhere(X[:, feat_idx] <= threshold).flatten()
        right_idxs = np.argwhere(X[:, feat_idx] > threshold).flatten()
        
        left = self._grow_tree(X[left_idxs, :], y[left_idxs], depth + 1)
        right = self._grow_tree(X[right_idxs, :], y[right_idxs], depth + 1)
        return Node(feature=feat_idx, threshold=threshold, left=left, right=right)

    def fit(self, X, y):
        self.root = self._grow_tree(X, y)

    def _traverse_tree(self, x, node):
        if node.is_leaf():
            return node.value
        
        if x[node.feature] <= node.threshold:
            return self._traverse_tree(x, node.left)
        return self._traverse_tree(x, node.right)

    def predict(self, X):
        return np.array([self._traverse_tree(x, self.root) for x in X])
