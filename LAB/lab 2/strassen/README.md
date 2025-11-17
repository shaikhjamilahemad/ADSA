
# Strassen's Matrix Multiplication in C


This project implements matrix multiplication using **Strassen’s algorithm** (Divide & Conquer).

It supports:
- ✅ Square and non-square matrices
- ✅ Automatic zero-padding to next power of 2
- ✅ Hybrid approach: switches to naive O(n^3) for small blocks (faster in practice)
- ✅ Dynamic memory allocation



## How to Compile and Run
# Compile (Windows / VS Code PowerShell)
gcc strassen_mul.c -o strassen_mul.exe

# Run (PowerShell)
.\strassen_mul.exe

# Run (Command Prompt)
strassen_mul.exe

## Input Format
1) Enter r1 c1  (rows, cols of A)
2) Enter r2 c2  (rows, cols of B)
3) Enter r1*c1 elements of A, row-wise
4) Enter r2*c2 elements of B, row-wise

## Code Flow / Explanation
1. Read dimensions r1, c1, r2, c2.
2. Compute max_dim = max(r1, c1, r2, c2) and set n = next power of 2 (>= max_dim).
3. Allocate A, B, C as n×n matrices (calloc → extra cells are 0) → this auto-pads:
   - handles non-square shapes
   - handles incompatible inner dimensions
4. Strassen(A, B, C, n):
   - Base case: if n <= threshold (e.g., 64), use naive multiply for speed.
   - Otherwise:
     a) Split A and B into quadrants: A11, A12, A21, A22 and B11, B12, B21, B22
     b) Compute 7 products (M1..M7) using Strassen formulas
     c) Combine them to build C11, C12, C21, C22
     d) Stitch the four quadrants back to C
5. Print only the meaningful top-left r1×c2 part of C (trim padded zeros).
6. Free all allocated memory.

## Zero-Padding Example
Example: A = 2×3, B = 2×2 → normally incompatible (c1=3 ≠ r2=2).
We compute n = next power of 2 of max(2,3,2,2) = 4.
Create 4×4 A and B with zeros in extra cells.
Multiply using Strassen → print top-left 2×2 of the result.

## Test Cases
### Test Case 1 — Square
A (2×2):
1 2
3 4

B (2×2):
5 6
7 8

Output (2×2):
19 22
43 50

### Test Case 2 — Rectangular (compatible, no extra pad beyond power-of-2)
A (2×3):
1 2 3
4 5 6

B (3×2):
7 8
9 10
11 12

Output (2×2):
58  64
139 154

### Test Case 3 — Incompatible (auto zero-padding)
A (2×3):
1 2 3
4 5 6

B (2×2):
7 8
9 10

Output (2×2):
25 28
73 82

### Test Case 4 — Identity Check
A (3×3):
1 2 3
4 5 6
7 8 9

B = I (3×3):
1 0 0
0 1 0
0 0 1

Output (3×3):
1 2 3
4 5 6
7 8 9

### Test Case 5 — Rectangular Output
A (3×2):
1 2
3 4
5 6

B (2×4):
7 8 9 10
11 12 13 14

Output (3×4):
29 32 35 38
65 72 79 86
101 112 123 134

## Features
- Strassen’s algorithm (7 recursive products + additions/subtractions)
- Auto-padding to next power of 2 for clean quadrant splits
- Fallback naive multiply for small n (tunable threshold)
- Works for square and non-square matrices (prints trimmed result)

## Author
**Shaikh Jamil Ahemad**

M.Tech CSE, IIIT Bhubaneswar
