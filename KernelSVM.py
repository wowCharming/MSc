import cvxopt
import numpy as np
from numpy import linalg as LA


class KernelSVM(object):

    def __init__(self, C=1.0, gamma=0.5):
        '''
                :param C: para
                :param gamma: width of the Gaussian Kernel window
                :return: None
                Receive parameters
        '''
        self.C = C  # restricted para for Lagrange dual problem
        self.gamma = gamma  # width of the Gaussian Kernel window
        self.alpha0 = 0  # bias parameter
        self.N = 0  # len of training data
        self.lambd = 0  # x_value of Lagrange dual problem

    def fit(self, X, y):
        '''
                :param X: 2-D feature vectors X.
                :param y: their respective class labels y.
                :return: None
                Receive training parameters then estimate lambd of the Lagrange dual problem & bias parameter alpha0
        '''
        N = len(X)
        self.N = N
        self.X = X
        # make label y to be -1&1 (original:0&6)
        max = np.amax(y)
        y = (y - max / 2) / (max / 2)
        self.y = y

        KernelMat = np.zeros((N, N))  # kernel function evaluations between each pair of training samples
        for i in range(N):
            for j in range(N):
                KernelMat[i, j] = self.GaussianRBFKernelMatrix(X[i, :], X[j, :])

        # optimize the Lagrange dual problem to get lambd
        P = cvxopt.matrix(np.zeros((N, N)))
        for i in range(N):
            for j in range(N):
                P[i, j] = y[i] * y[j] * KernelMat[i, j]
        q = cvxopt.matrix(-1.0 * np.ones(N))

        G = np.concatenate((-np.eye(N), np.eye(N)), axis=0)  # axis: corresponding rows to splice
        G = cvxopt.matrix(G)
        h = np.concatenate((np.zeros(N), self.C * np.ones(N)), axis=0)
        h = cvxopt.matrix(h)

        A = cvxopt.matrix(y.reshape(1, -1))  # to be 1 row, "-1": auto column
        b = cvxopt.matrix(0.0)
        sol = cvxopt.solvers.qp(P, q, G, h, A, b)
        self.lambd = np.array(sol['x'])

        # estimate alpha0, decide (support vectors S with size Ns) first
        threshold = 1e-4
        idx = np.argwhere(self.lambd > threshold)  # Find the indices of array satisfied elements
        Ns = idx.shape[0]
        for n in range(Ns):
            temp = 0
            for m in range(Ns):
                temp = temp + self.lambd[m] * y[m] * KernelMat[n, m]
            self.alpha0 = self.alpha0 + (y[n] - temp)
        self.alpha0 = self.alpha0 / Ns

    # kernel matrix K using Gaussian radial basis function
    def GaussianRBFKernelMatrix(self, X1, X2):
        return np.exp(-0.5 * (LA.norm(X1 - X2)**2) / (self.gamma**2))

    def predict(self, X, mapping=True):
        '''
                :param X: feature vectors X
                :return: classification results for a set of feature vectors X
        '''
        result = np.zeros(0)
        numRuns = len(X)
        for i in range(numRuns):
            print(numRuns - i)
            D = 0
            for j in range(self.N):
                D += self.lambd[j] * self.y[j] * self.GaussianRBFKernelMatrix(X[i, :], self.X[j, :])
            D += self.alpha0

            if D >= 0:
                result = np.append(result, 1)
            else:
                result = np.append(result, 0)
        return result
