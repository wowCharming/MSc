import numpy as np
from numpy import linalg as LA
import scipy.optimize as opt


class LinearRegression(object):

    def __init__(self, lossFunction='l2', lossFunctionParam=0.001, classification=False):
        self.__initialized = False  # set to true when done
        # when huber, lossFunctionParam is the parameter of the Huber function
        self.__lossFunctionParam = lossFunctionParam
        # params: a,b, arbitrary starting value
        self.params = np.array([0.05, 0.05])
        self.__lossFunction = lossFunction


    def fit(self, X, y):
        self.__entries = len(y)
        # no meaning for one point regression
        if self.__entries < 2:
            return None
        m = len(X)
        b = np.ones(m)
        self.new_X = np.column_stack((X, b))  # add 1 as new column to X (affine)

        if self.__lossFunction == 'huber':
            # key part to get self.params of line
            optimum = opt.minimize(lambda params: self.huber_objfunc(self.new_X, y, params, self.__lossFunctionParam), self.params)
            # optimum = opt.minimize(lambda params: self.huber_objfunc(self.new_X, y, params, self.__lossFunctionParam),
            #                    self.params, method='CG',
            #                    jac=lambda params: self.huber_objfunc_derivative(self.new_X, y, params, self.__lossFunctionParam))

            self.params = optimum.x  # get the opt params

        elif self.__lossFunction == 'l2':
            # r = AX - b <=> r = X * params - y
            # ||AX - b||2 = (y - params * X)^2
            # x = (ATA)^−1*ATb
            self.params = np.dot(LA.pinv(self.new_X.T.dot(self.new_X)), self.new_X.T.dot(y))  # LA.pinv(): pseudoinverse

        else:
            print("no other method possible \n")
        self.__initialized = True  # able to paint

    def huber_objfunc(self, X, y, params, a):
        '''
                :param a: Huber parameter a = self.__lossFunctionParam
                :param params: (a, b)
                r is vector: (yi - axi - b) = (y - params * X)
                :return: sum of H(yi - axi - b)
        '''
        r = y - (X.dot(params))  # residual
        temp = self.huber(r, a)
        return np.sum(temp)

    # return the gradient of the objective fcn. when used in scipy.optimize.minimize as jac
    def huber_objfunc_derivative(self, X, y, params, a):
        r = y - (X.dot(params))
        temp = self.huber_derivative(r, a)
        return X.T.dot(-temp)

    def huber(self, r, a):
        '''
                :param a: Huber parameter a = self.__lossFunctionParam
                :param r: vector of (yi - axi - b) = (y - params * X)
                :return: Huber fcn. result
        '''
        ans = np.empty(self.__entries)  # create an array of random values, self.__entries = len(y)
        # np.putmask(): Changes elements of an array based on conditional and input values
        np.putmask(ans, np.absolute(r) <= a, r * r)
        np.putmask(ans, np.absolute(r) > a, a * (2 * np.absolute(r) - a))
        return ans

    # return: derivation of Huber fcn. result
    def huber_derivative(self, r, a):
        ans = np.empty(self.__entries)  # create an array of random values
        np.putmask(ans, np.absolute(r) <= a, 2 * r)
        np.putmask(ans, np.absolute(r) > a, np.sign(r) * 2 * a)
        return ans

    def paint(self, qp, featurespace):
        if self.__initialized:
            x_min, y_min, x_max, y_max = featurespace.coordinateSystem.getLimits()
            y1 = self.params[0] * x_min + self.params[1]
            x1, y1 = featurespace.coordinateSystem.world2screen(x_min, y1)
            y2 = self.params[0] * x_max + self.params[1]
            x2, y2 = featurespace.coordinateSystem.world2screen(x_max, y2)
            qp.drawLine(x1, y1, x2, y2)

    def predict(self, X):
        return None
