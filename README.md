# GSIH Hackathon Solutions 

This repository contains my C++ solutions for the GSIH Hackathon. The entire suite of solutions was developed over an intensive **12-hour sprint**, focusing heavily on algorithmic efficiency, custom parsing, and Low-Level Design (LLD) principles.

**Author:** Harshit Singh  
**Institution:** National Institute of Technology Calicut (NITC)  
**Language:** C++17

## 🚀 Overview

The hackathon featured three complex algorithmic and systems-design challenges. Rather than relying on external libraries, these solutions implement core logic—like JSON AST parsing and NP-hard state simulations—entirely from scratch using standard C++.

### 1. JSON → TypeScript Type Generator
**Directory:** `/json-to-typescript`

A deterministic type generator that parses compact JSON arrays and outputs highly formatted, collision-free TypeScript interfaces. 
* **Key Features:** Custom built-in JSON parser (no external dependencies), depth-first ASCII-ordered type merging, and dynamic abstract syntax tree (AST) traversal.
* **Challenges Solved:** Handling mixed arrays, nested object type collisions, and strict deterministic lexicographical formatting.

### 2. Multi-Agent Drone Routing in a Temporal Urban Grid
**Directory:** `/drone-delivery`

A temporal-spatial routing engine for a fleet of autonomous delivery drones navigating a 2D city grid. 
* **Key Features:** Simulates energy constraints, payload capacities, and dynamic No-Fly Zones (NFZs) that activate/deactivate at specific time windows.
* **Challenges Solved:** Avoiding active NFZ intersections using continuous space-time checks, optimizing multi-package delivery paths to maximize raw scores, and managing charging station queues.

### 3. Group Trip Planner
**Directory:** `/group-trip-planner`

A deterministic optimization engine for planning multi-day group itineraries under strict budget, energy, and time constraints.
* **Key Features:** Bitmasking and lexicographical sorting to evaluate the NP-hard subset selection of daily activities.
* **Challenges Solved:** Real-time replanning based on dynamic chronological events (e.g., weather blocks, sudden fatigue, budget cuts, or users dropping out) while maintaining fair constraints bottlenecked by the most restricted group member.

## 🛠️ Build & Run Instructions

All solutions are contained in single C++ files for easy compilation. A standard C++17 compiler (like GCC) is required.

**Compile:**
```bash
g++ -O3 -std=c++17 solution_file.cpp -o solution
