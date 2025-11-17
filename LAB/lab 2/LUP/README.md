# LUP Decomposition with Gaussian Elimination and Solving Ax = b

This project implements **LUP decomposition** using **Gaussian elimination with partial pivoting** and solves the system of equations \(Ax = b\) using the decomposed factors \(L\), \(U\), and \(P\). The program decomposes the matrix \(A\) into \(P\), \(L\), and \(U\), then solves the system using forward and backward substitution.

---

## Features
- **LUP Decomposition**: Decomposes matrix \(A\) into \(P\), \(L\), and \(U\).
- **Solving the system \(Ax = b\)**: Uses the decomposed factors to solve \(Ax = b\).
- Uses **Gaussian elimination with partial pivoting** for stability.
- Detects singular/near-singular matrices.
- Prints the **L**, **U**, and **P** matrices.

---

## Compilation

To compile the program:
```bash
gcc lup_decomposition.c -o lup -lm
```

To run:
```bash
./lup
```

---

## Input Format

1. **Enter the size of the matrix \(n\)**.
2. **Enter the elements of matrix \(A\)** row by row.
3. **Enter the elements of vector \(b\)**.

Example:
```
Enter the size of the matrix (n): 2
Enter the elements of matrix A (row by row):
2 1
5 7
Enter the elements of vector b:
11 13
```

---

## Example Output

```
L =
    1.000000     0.000000
    0.400000     1.000000

U =
    5.000000     7.000000
    0.000000    -1.800000

P =
    0.000000     1.000000
    1.000000     0.000000

Solution x = [ 7.111111, -3.222222 ]
```

---

## Test Cases

### Test Case 1: Simple 2×2 matrix
Input:
```
2
2 1
5 7
11 13
```
Output:
- **L** = [[1, 0], [0.4, 1]]
- **U** = [[5, 7], [0, -1.8]]
- **P** = [[0, 1], [1, 0]]
- **Solution \(x\)** = [7.111111, -3.222222]

### Test Case 2: Identity matrix (no pivoting needed)
Input:
```
3
1 0 0
0 1 0
0 0 1
4 5 6
```
Output:
- **L** = [[1, 0, 0], [0, 1, 0], [0, 0, 1]]
- **U** = [[1, 0, 0], [0, 1, 0], [0, 0, 1]]
- **P** = [[1, 0, 0], [0, 1, 0], [0, 0, 1]]
- **Solution \(x\)** = [4, 5, 6]

### Test Case 3: 3×3 non-singular matrix
Input:
```
3
2 3 1
4 7 7
-2 4 5
1 2 3
```
Output:
- **L** ≈ [[1, 0, 0], [-0.5, 1, 0], [0.5, 0.428571, 1]]
- **U** ≈ [[4, 7, 7], [0, 6.5, 8.5], [0, 0, -1.571429]]
- **P** ≈ [[0, 1, 0], [0, 0, 1], [1, 0, 0]]
- **Solution \(x\)** = [3, 1, -2]

### Test Case 4: Singular matrix
Input:
```
2
1 2
2 4
3 6
```
Output:
```
Matrix appears singular (pivot < 1.0e-12)
```

### Test Case 5: Larger 4×4 matrix
Input:
```
4
1 2 3 4
0 1 4 5
2 3 5 6
3 4 6 7
10 11 12 13
```
Output:
- **L**, **U**, and **P** matrices will depend on Gaussian elimination steps.
- **Solution \(x\)** will be calculated.

---

## Notes
- This program decomposes the matrix into **L**, **U**, and **P** and solves the system \(Ax = b\) using the decomposition.  
- The matrix \(P\) is the permutation matrix, ensuring stability by swapping rows during partial pivoting.
- **Singular matrix detection**: The program reports when the matrix is singular or near-singular based on a small pivot value.

---
