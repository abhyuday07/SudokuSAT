# SudokuSAT
Assignment CS202
---------------------------------------
SUDOKU SOLVER AND GENERATOR
---------------------------------------
Copyright 2018 Abhyuday Pandey (170039) and Srinjay Kumar (170722) 
---------------------------------------

Code explanation and commands to run the program

Text files to be created :-
1. input.txt: This file stores the problem sudoku to be solved
2. sat.txt: Contains the clauses that are to be fed into the miniSAT.
3. ans.txt: Contains the satisfiable clauses that are output by the SAT solver.
4. o.txt: Contains the decoded sudoku.

Most importantly, we also need to store an unsolved sudoku in the file named input.txt.
---------------
Sudoku Solver
---------------
file name : sudoku.cpp

In this file we have the code to take the unsolved sudoku as the input and we will get solved sudoku as output.
In the input we would substitute "." with empty places in order to get the solved answer.
Steps to run the file

1.compile the c++ file.
2.Run the c++ file
3.Give the unsolved sudoku as input.
4.We would get the solved sudoko as the output.

Within the file, these things happen in the file:-
1.The C++ code outputs the SAT conditions in a file.
2.From the code, we then execute a command to the SAT to solve the SAT encoding and store the satisfiable encodings in a separate file.
3.The last part of the code then decodes the information and ouputs the solved sudoku.

Required Commands :-
1. g++ -std=c++11 sudoku.cpp
2. ./a.out

------------------
Sudoku Generator
------------------
file name : generator.cpp

Here we will take a solved sudoku as an input and output a minimal sudoku as a problem.
Components of code and their functions:-
1. invoke - generates a solved sudoku from an unsolved one.
2. isunique - tests the uniqueness of the unsolved sudoku generated.
3. main - generate an unsolved sudoku and check the minimality of the sudoku generated.

Commands to execute the sudoku generator :-
1. g++ -std=c++11 generator.cpp
2. ./a.out

Uses sudoku.cpp to generate a random sudoku and find its minimal solution and outputs in STDOUT.

The algorithm and the proof of its correctness can be found in given link :-
http://home.iitk.ac.in/~abhyuday/minimalsudoku.html






