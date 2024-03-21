import numpy as np
import sympy as sp

# Graph definition
V = [1, 2, 3, 4, 5, 6]
E = [[1,2], [2,3], [3,4], [4,1], [3,5], [5,6], [3,6]]
T = []

# Find 3-cliques
for i in range(1,len(V)-1):
    for j in range(i+1,len(V)):
        for k in range(j+1, len(V)+1):
            if (([i,j] in E) or ([j,i] in E)) and (([j,k] in E) or ([k,j] in E)) and (([i,k] in E) or ([k,i] in E)):
                T.append([i,j,k])

# Initialize coboundary operators
A = np.zeros((len(E),len(V)))
B = np.zeros((len(T),len(E)))

# Populate A
for e in range(len(E)):
    A[e][E[e][0]-1] = -1
    A[e][E[e][1]-1] = 1

# Populate B
for t in range(len(T)):
    if [T[t][0],T[t][1]] in E:
        B[t][E.index([T[t][0],T[t][1]])] = 1
    else:
        B[t][E.index([T[t][1],T[t][0]])] = -1
    if [T[t][1],T[t][2]] in E:
        B[t][E.index([T[t][1],T[t][2]])] = 1
    else:
        B[t][E.index([T[t][2],T[t][1]])] = -1
    if [T[t][2],T[t][0]] in E:
        B[t][E.index([T[t][2],T[t][0]])] = 1
    else:
        B[t][E.index([T[t][0],T[t][2]])] = -1

# Form graph Laplacian
L = np.transpose(A)@A

# Form graph Hemholtzian
H = A@np.transpose(A)+ np.transpose(B)@B
#H = np.array([[2, 2, -1, 0, 1], [-1, -1, 2, -3, 1], [1, 1, -2, 0, -1], [0, 0, 1, 1, 1]])

# Put Hemholtzian into reduced row echelon form
rref_H = sp.Matrix(H).rref()

# Find pivots
pivots = list(rref_H[1])
nonPivots = list(set(rref_H[1])^set(range(np.shape(H)[1])))


# Convert back to nparray
rref_H = np.array(rref_H[0])

# Initialise basis for kernel
K = []
Betti = len(nonPivots)

for n in range(len(nonPivots)):
    v = np.zeros(np.shape(H)[1])
    v[nonPivots[n]] = 1
    j = 0
    for i in pivots:
        v[i] = -rref_H[j][nonPivots[n]]
        j = j + 1
    K.append(list(v))

print(f" The Kernel of H is spanned by {K}")
print(f" The Betti number is {Betti}")
#print(H@K[0])
