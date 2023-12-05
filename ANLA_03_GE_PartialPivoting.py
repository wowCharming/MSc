import numpy as np
from numpy import linalg as LA

A = np.array([[2, 1, 1, 0], [4, 3, 3, 1], [8, 7, 9, 5], [6, 7, 9, 8]])


def gausselim(A):
    m = len(A)
    U = A.copy().astype(float) # must be float
    L = np.eye(m)
    P = np.eye(m)
    for k in range(m):
        pivot = abs(U[k, k])
        maxindex = k
        for i in range(k+1, m):  # pivoting
            if abs(U[i, k]) > pivot:
                # U[[k, i]] = U[[i, k]]
                pivot = abs(U[i, k])
                maxindex = i
                P[[k, i]] = P[[i, k]]

        # interchange U
        for i in range(k, m):
            swap = U[maxindex, i]
            U[maxindex, i] = U[k, i]
            U[k, i] = swap

        # interchange L
        for i in range(k):
            swap0 = L[maxindex, i]
            L[maxindex, i] = L[k, i]
            L[k, i] = swap0

        for j in range(k+1, m):
            L[j, k] = float(U[j, k]) / U[k, k]
            for q in range(k, m):
                U[j, q] -=L[j, k] * U[k, q]



    return P,  L, U # P, Q, L, U


print(gausselim(A))
