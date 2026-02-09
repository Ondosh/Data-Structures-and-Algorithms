Abstract is based on this presentation: https://docs.google.com/presentation/d/1ofbDxk9cbT4ABdCmGFqftz3u166DV1KkrJGU_Z7E6Kg/edit?slide=id.p#slide=id.p


Abstract is based on this presentation: https://docs.google.com/presentation/d/1ofbDxk9cbT4ABdCmGFqftz3u166DV1KkrJGU_Z7E6Kg/edit?slide=id.p#slide=id.p


Algorithm is an sequence of commands, that can solve any problem. Sequence of commands can't be endless. Algorithm's commands can't have a double meaning. Algorithm in this formal language can be defined as a program.


Ways to write an algorithm:
* Natural language;
* Formal verbal;*
* Flowchart;
* Pseudocode;
* Structural diagrams;
* Programming languages.


Properties of algorithm: discreteness, clarity, determinism, finiteness, mass character.


Algorithm effeciency. Time and operational complexity.

Example:

F1(n) = $33n + 10$ 


F2(n) = $n^2$

Let's decribe effeciency as function F(n), which will show dependence of the number of operations on the size of the input data ($n$). In first function we see linear complexity, in second - quadratic. If someone asks, what algorithm is faster, usually we want to check it with any big enough $n$, that will show which function can faster process this data.

BigO Notation.

Describes how an algorithm's runtime or memory usage **scales** with input size (n). Focuses on **worst-case** performance.

How to compute it:
1. Find the operation count function F(n).   
2. Keep only the fastest-growing term (dominant term).
3. Drop all constants (coefficients and lower-order terms).

Example:  
`F(n) = 5n² + 100n + 3000` → `O(n²)`

- `n²` grows faster than `n` and constants
- Coefficient `5` is ignored

Complexity hierarchy (fastest to slowest): 
`O(1)` → `O(log n)` → `O(n)` → `O(n log n)` → `O(n²)` → `O(2ⁿ)` → `O(n!)`

Enables comparison of algorithms for large datasets, independent of hardware speed. For large n, the difference between complexity classes (e.g., `O(n)` vs `O(n²)`) matters far more than optimizing constants within the same class.