<!-- ABOUT THE PROJECT -->
## About The Project

This is my Solver for Sudoku using hyper-heuristic search combined with human-intuitive constraint propagation. 
The general approach is to iteratively mutate a randomly populated Sudoku board.
 ![Screenshot](docs/img/two_boards.png)
At each iteration, the Selection Policy will choose a 
Low Level Heuristic based on some specified strategy and apply it to produce a change. The Move Acceptance policy then evaluates
the objective change from the perturbation and decides whether to accept or reject this change based on some specified strategy.

In addition to local search, a constraint programming procedure is implemented that clears part of the board and attempts
to fill empty cells implicitly. This procedure is run periodically and is used to help guide local search methods as well
as help escape from local optima solutions.
 ![Screenshot](docs/img/hybrid_algorithm.png)

The goal is to produce a highly scalable framework for simple production of generalised optimisation techniques. In particular, the implementation is aimed to support extension of specific algorithmic components: 
* Move Acceptance
* Move Selection
* Low Level Heuristics.

This solver has shown to be more efficient than state of the art methods from the literature for complex instances. 
 ![Screenshot](docs/img/solver_comparison.png)



<!-- Input -->
## Input

The program works by taking a single puzzle as input and produces output to four files that log different parts of the algorithm.
Additionally, there are four Acceptance strategies, and five Selection policies implemented.
The solver uses Simple Random-Simulated Annealing configuration by default. A different policy pair can be specified using optional parameters:

| Acceptor Method                                        | Input Parameter                                          |
|--------------------------------------------------------|----------------------------------------------------------|
| Only Improve (OI)                                      | -oi, --only-improve                                      |
| Improve or Equal (IOE)                                 | -ie, -ioe, --improve-or-equal                            |
| Simulated Annealing (SA)                               | -sa, --simulated-annealing                               |
| Adaptive Iteration Limited Threshold Accepting (AILTA) | -ailta, --adaptive-iteration-limited-threshold-accepting |

| Selection Method                 | Input Parameter                    |
|----------------------------------|------------------------------------|
| Simple Random (SR)               | -sr, --simple-random               |
| Random Descent (RD)              | -rd, --random-descent              |
| Random Permutation (RP)          | -rp, --random-permutation          |
| Random Permutation Descent (RPD) | -rpd, --random-permutation-descent |
| Reinforcement Learning(RL)       | -rl, --reinforcement-learning      |

An example run of the program:
```./cmake-build-release sudoku_oop.exe data/puzzle1.txt ./solution.txt ./log.txt ./acc.txt ./sel.txt -sa, -sr```

### Prerequisites

To easier parse parameters, an external library https://nlohmann.github.io/json/ was used to parse JSON.
No other external libraries were used for this implementation.
  

