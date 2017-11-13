import numpy as np
from numpy import linalg as LA

A = np.array([[12,-51],[6,167],[-4,24]])

def house(A):
	m, n = np.shape(A)
	W = np.zeros((m,n))
	R = A
	# householder procedure
	for k in range(n):
		#creat x, e, alpha
		x = A[k:m,k]
		e = np.zeros(len(x))
		e[0] = 1
		alpha = np.sign(x[0]) * LA.norm(x,2)
		#creat u and v
		u = x + alpha * e
		v = u / LA.norm(u,2)
		#creat R
		A[k:m, k:n] = A[k:m, k:n] - 2 * np.outer(v,np.dot(v.transpose(),A[k:m, k:n]))
		W[k:m,k]=v
	return W, R

def formQ(W):
	m, n = np.shape(A)
	Q = np.identity(m)
	for i in range(m):
		#x = Q[:,i]
		for k in range(n-1, -1, -1):
			#x[k:m] = x[k:m] - 2 * np.outer(W[:,k],np.dot(W[:,k].transpose(),x[k:m]))
			Q[k:m,i] = Q[k:m,i]-2*np.dot(np.outer(W[k:m,k],W[k:m,k]),Q[k:m,i])
			#Q[k:m,i] = x[k:m]
	return Q

def valid(Q,R):
	return np.dot(Q,R)
