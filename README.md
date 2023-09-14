# Vertex Cover

## Table of Content:
- [Project Overview](#project-overview)
- [Implementation Details](#implementation-details)

## Project Overview

The Vertex Cover project is a simple implementation of the vertex cover problem. It comprises two main components: a Python program for input processing and a C++ program for finding the vertex cover using a SAT solver. These two programs communicate via pipes to solve the vertex cover problem efficiently.

## Implementation Details

### Python Input Processing

The Python program takes user input describing street information, represented as keywords such as "add street," "remove street," or "modify street," followed by 2D coordinates. This input is validated using regular expressions (regexes) to ensure correctness. The validated data is then stored in a graph data structure.

### C++ SAT Solver

The C++ program plays a crucial role in finding the vertex cover for the input graph. It employs a SAT (Boolean satisfiability problem) solver to determine the minimum vertex cover. This optimization task is essential for various applications, including network design and optimization.

The communication between the Python and C++ programs is facilitated via pipes, allowing for efficient data transfer and processing.

*Note: Vertex cover problems have applications in various fields, including network design, resource allocation, and optimization.*
