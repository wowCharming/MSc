import numpy as np

#A = np.array([[2, 1, 1, 0], [4, 3, 3, 1], [8, 7, 9, 5], [6, 7, 9, 8]])
#A = np.array([[0, 2, -3], [2, 2, 1], [2, 4, 4]])
#A = np.array([[1, -4, 4, 7], [0, 2, -1, 0], [2, 1, 1, 4], [2, -3, 2, -5]])


def gausselim(A):
    m = len(A)
    U = A.copy().astype(float)  # must be float
    L = np.eye(m)
    P = np.eye(m)
    Q = np.eye(m)
    for k in range(m):
        pivot = abs(U[k, k])
        maxindex_i = k
        maxindex_j = k
        for i in range(k, m):  # pivoting
            for j in range(k, m):
                if abs(U[i, j]) > pivot:
                    # U[[k, i]] = U[[i, k]]
                    pivot = abs(U[i, j])
                    maxindex_i = i
                    maxindex_j = j
        Q[:, [k, maxindex_j]] = Q[:, [maxindex_j, k]]
        P[[k, maxindex_i]] = P[[maxindex_i, k]]

        # interchange U, NEED exchange each element of one row/column
        for i in range(m):
            swap = U[maxindex_i, i]
            U[maxindex_i, i] = U[k, i]
            U[k, i] = swap
        for j in range(m):
            swap1 = U[j, maxindex_j]
            U[j, maxindex_j] = U[j, k]
            U[j, k] = swap1

        # interchange L
        for i in range(k):
            swap0 = L[maxindex_i, i]
            L[maxindex_i, i] = L[k, i]
            L[k, i] = swap0
        for j in range(k):
            swap2 = L[j, maxindex_j]
            L[j, maxindex_j] = L[j, k]
            L[j, k] = swap2

        # elimination
        for j in range(k + 1, m):
            L[j, k] = float(U[j, k]) / U[k, k]
            for q in range(k, m):
                U[j, q] -= L[j, k] * U[k, q]

    return P, Q, L, U


# print(gausselim(A))
#print(A)
#print(np.dot(gausselim(A)[0],np.dot(A,gausselim(A)[1])))
#print(np.dot(gausselim(A)[2], gausselim(A)[3]))