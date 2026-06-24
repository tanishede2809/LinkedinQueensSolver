# LinkedIn Queens Solver (C++)

## Overview

This project is a C++ solver for the LinkedIn Queens puzzle.

The objective of the puzzle is to place exactly one queen in each colored region such that:

* Every color contains exactly one queen.
* No two queens share the same row.
* No two queens share the same column.
* No two queens are adjacent, including diagonally adjacent cells.

The solver uses a backtracking-based approach along with optimization techniques inspired by how humans typically solve the puzzle manually.

---

## Motivation

I regularly solve the LinkedIn Queens puzzle and became interested in automating the process.

Instead of directly searching for an existing solution online, I wanted to understand how a computer would approach the puzzle from scratch.

The development process began by analyzing my own solving strategy and gradually translating that reasoning into code.

---

## Initial Thought Process

When solving the puzzle manually, I noticed that I keep track of three important things:

1. Which color each cell belongs to.
2. Which colors have already received a queen.
3. Which rows and columns are already occupied.

My first idea was to represent every cell using three states:

* Free
* Occupied
* Blocked

However, this approach quickly became complicated when backtracking was introduced. Undoing blocked states correctly after removing a queen would require significant bookkeeping.

This led to a simpler observation:

Instead of storing blocked cells explicitly, we can check whether a move is legal whenever we attempt to place a queen.

This became the foundation of the final solution.

---

## Data Structures Used

### Color Board

The puzzle board is stored as:

```cpp
vector<vector<char>> colorBoard;
```

Each cell contains a color symbol entered by the user.

Example:

g g g p p
g g g p p
y o o o o
y o b b b
y o b b b

---

### Color-to-Cells Mapping

To efficiently find all cells belonging to a color:

```cpp
map<char, vector<pair<int,int>>> colorCells;
```

Example:

g → [(0,0), (0,1), (0,2), ...]

p → [(0,3), (0,4), ...]

This allows the solver to immediately access every candidate position for a given color.

---

### Row Tracking

```cpp
vector<bool> rowUsed;
```

Keeps track of occupied rows.

---

### Column Tracking

```cpp
vector<bool> colUsed;
```

Keeps track of occupied columns.

---

### Queen Positions

```cpp
vector<pair<int,int>> queens;
```

Stores the coordinates of all queens currently placed on the board.

---

## Core Algorithm

The final solution uses Recursive Backtracking.

The idea is:

1. Select a color.
2. Try placing a queen in each cell of that color.
3. If the placement is legal:

   * Mark row and column as occupied.
   * Store the queen position.
4. Recursively solve the remaining colors.
5. If a dead end is reached:

   * Remove the queen.
   * Restore the previous state.
   * Try another position.

This process continues until either:

* A complete solution is found, or
* Every possibility has been exhausted.

---

## Legality Check

A queen placement is considered legal if:

### Row Constraint

The row does not already contain another queen.

### Column Constraint

The column does not already contain another queen.

### Adjacency Constraint

No queen exists in any immediately adjacent cell.

This includes:

* Horizontal neighbors
* Vertical neighbors
* Diagonal neighbors

The check is performed using:

```cpp
abs(r - qr) <= 1 &&
abs(c - qc) <= 1
```

for every previously placed queen.

---

## Optimization 1: Most Constrained Color First (MRV)

While solving puzzles manually, I noticed that I naturally begin with the smallest color regions because they offer the fewest choices.

For example:

Yellow → 3 cells

Pink → 4 cells

Green → 6 cells

Instead of solving colors in arbitrary order, colors are sorted by the number of available cells.

This is a well-known Constraint Satisfaction Problem heuristic called:

**MRV (Minimum Remaining Values)**

Benefits:

* Fewer recursive calls
* Earlier detection of impossible states
* Reduced backtracking

---

## Optimization 2: Forward Checking

This optimization originated from an observation made during manual solving:

"If placing a queen completely blocks another color, there is no point continuing."

After every queen placement, the solver checks every remaining color.

If a color no longer has any legal cell available:

* The branch is immediately abandoned.
* Backtracking occurs instantly.

Benefits:

* Detects failures earlier.
* Prevents exploring hopeless branches.
* Significantly reduces search time.

---

## Time Complexity

Without optimizations, the search space grows exponentially.

In the worst case:

O(k₁ × k₂ × k₃ × ... × kₙ)

where kᵢ is the number of cells belonging to color i.

The MRV and Forward Checking optimizations dramatically reduce the practical search space, making the solver much faster on larger boards.

---

## Example Output

Solution found:

(0,1)

(1,3)

(2,0)

(3,4)

(4,2)

Each coordinate represents a queen position on the board.

---

## What I Learned

This project helped me understand:

* Recursive Backtracking
* Constraint Satisfaction Problems (CSPs)
* Maps and Vectors in C++
* State management during recursion
* Optimization through MRV
* Forward Checking
* Translating human problem-solving strategies into algorithms

More importantly, it demonstrated how a manually discovered solving strategy can be formalized into an efficient program.

---

## Future Improvements

Potential enhancements include:

* GUI-based puzzle input
* Automatic puzzle validation
* Support for arbitrary color names instead of single-character symbols
* Visualization of the solving process
* Performance statistics and benchmarking
* Support for importing puzzles from screenshots

---



Developed as a learning project to explore recursive backtracking, constraint satisfaction techniques, and algorithmic problem solving through the LinkedIn Queens puzzle.
