import math
import numpy
import operator


class kNearestNeighbor(object):
    def __init__(self, k):
        self.k = k

    def fit(self, X, y):
        # store references to the labeled training data
        '''
                :param X: 2-D feature vectors X.
                :param y: their respective class labels y.
                :return: None
                Receive training parameters
        '''
        self.__X = X
        self.__y = y
        self.__m = len(X)
        # self.__mMax = 1e8

    def predict(self, X):
        '''
                :param X: 测试集
                :return: 测试集的预测值
                预测一个测试集
        '''
        # |x-y|^2 = (x - y)^T (x - y) = - 2 * x^T y + x^T x + y^T y
        # runtime efficient as for-loops are avoided, but runs out of memory
        # pretty fast for large training and test sets;
        # process only m test samples at a time

        # m = int(self.__mMax / self.__m)
        # numRuns = math.ceil(len(X) / m)  # Round up??

        #Z = numpy.zeros(0)
        result = numpy.zeros(0)
        # for i in range(numRuns):  # from 0 to numRuns-1
        numRuns = len(X)
        for i in range(numRuns):
            Xs = X[i: (i + 1)]  # Segmentation??
            diffMat = numpy.tile(Xs, (self.__m, 1)) - self.__X
            D = numpy.square(diffMat).sum(axis=1)  # .square():Calculate the square of each element,
            # .sum(axis=1): Add each row vector
            # d2 = numpy.square(self.__X).sum(axis=1)
            # D = numpy.dot(Xs, self.__X.T)  # Matrix multiplication
            # D *= -2
            # D += d1.reshape(-1, 1)  # Let d1 become only one column
            # D += d2

            #ind = numpy.argsort(D, axis=1)[:, 0]  # .argsort(): Returns the index value of array value from small to
            # large, axis=1: Sort by row, [:, 0]: get first column
            # index
            ind = D.argsort()
            #Z = numpy.append(Z, self.__y[ind])  # .append(): append result is always an 1-D array.

            # 选择距离最小的k个点
            classCount = {}
            for j in range(self.k):
                a = ind[j]
                numOflabel = self.__y[a]
                classCount[numOflabel] = classCount.get(numOflabel, 0) + 1
                # 按classCount字典的第2个元素（即类别出现的次数）从大到小排序
                sortedClassCount = sorted(classCount.items(), key=operator.itemgetter(1), reverse=True)
            result = numpy.append(result, sortedClassCount[0][0])
        return result
