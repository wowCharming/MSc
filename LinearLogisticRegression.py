import numpy as np
from numpy import linalg as LA
import numpy.matlib

# parameterized logistic function
def sigmoid(theta, X):
    return 1.0 / (1 + np.exp(-np.dot(X, theta)))

class LinearLogisticRegression(object):

    def __init__(self, learningRate = 0.5, maxIterations = 100):
        '''
                :param learningRate: step length for Newton's method
                :param maxIterations: maximum number of iterations for learning the parameters
                :return: None
                Receive parameters
        '''
        self.learningRate = learningRate
        self.maxIterations = maxIterations


    def fit(self, X, y):
        '''
                :param X: 2-D feature vectors X to train
                :param y: their respective class labels y
                :return: None
                Receive training parameters then perform the parameters (theta) estimation for the decision function
        '''
        n = 6
        m = len(X)
        res = np.zeros((len(X),6))
        # expand X to affine decision function
        for i in range(len(X)):
            res[i, 0] = X[i, 0] * X[i, 0]
            res[i, 1] = X[i, 0] * X[i, 1]
            res[i, 2] = X[i, 1] * X[i, 1]
            res[i, 3] = X[i, 0]
            res[i, 4] = X[i, 1]
            res[i, 5] = 1
            if (y[i] > 0): y[i] = 1
        # newtonMethod
        # initialize theta
        theta = np.zeros((n,1))
        j = 0
        check = 10
        while j < self.maxIterations and check > 1e-5:
        #for i in range(self.maxIterations):
            g = sigmoid(theta, res)
            a = [x-y for x,y in zip(y,g)]
            grad = np.dot(res.T, a)  # gradient
            check = LA.norm(grad)  # l-2norm of gradient
            j+=1
            A = g * (1 - g) * np.eye(m)
            H = np.mat(res.T) * A * np.mat(res)  # Hessian matrix, H = X`AX
            theta += self.learningRate * np.dot(H.I, grad)
        self.theta = theta



    def gFunc(self, X, theta):
        return None


    def predict(self, X):
        '''
                :param X: feature vectors X
                :return: classification results for a set of feature vectors X
        '''
        result = np.zeros(0)
        Xi = np.zeros(6)
        # calculate the probability of Y = 1 of respective feature vector X
        for i in range(len(X)):
            Xi[0] = X[i, 0] * X[i, 0]
            Xi[1] = X[i, 0] * X[i, 1]
            Xi[2] = X[i, 1] * X[i, 1]
            Xi[3] = X[i, 0]
            Xi[4] = X[i, 1]
            Xi[5] = 1
            predictedY = np.array(sigmoid(self.theta, Xi)).flatten()
            if predictedY >= 0.5:
                result = numpy.append(result, 1)
            else:
                result = numpy.append(result, 0)

        return result

