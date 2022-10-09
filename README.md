# SAT-"Solver"
This is a simple C++ implementation of a SAT-Solver based on generating every 
possible assignment until truth. There is also the possibilty to print 
the whole truth tables. The assignments are not generated recursively, but backed on
the conversion of integers to bits.

How to use:

Normal: Compile & launch with C++ > 11. After that you will be asked to enter your formula
which should be formated in this way:


| Literaly | Logic writing | In program |
|:--------:|:-------------:|:----------:|
|   AND    |    ∧     |     &      | 
|    OR    |    ∨     |   &#124;   | 
|   NOT    | ¬ |     -      |

Example: A & (- B | C & ( - D ) )
It is necessary as shown above to leave a space between brackets, literals and negations.

If the formula is solveable, the isSolveAble method will return true.

As API: Remove the main-method, then use isSolveAble(formula..) for solving your formula.
If you wish to see solving assignment, call after solving printCurrentAssignment(..).

It is also notable to mention that it is also possibile to print whole truthtables to stdout with printTruthTable(..).