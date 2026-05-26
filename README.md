# GSIH Hackathon 2026 - Algorithmic & Systems Design Solutions

![Language](https://img.shields.io/badge/Language-C++17-blue.svg)
![Build](https://img.shields.io/badge/Build-Passing-brightgreen.svg)
![Status](https://img.shields.io/badge/Status-Completed-success)

This repository contains my comprehensive C++ solutions for the GSIH Hackathon. The entire suite of solutions was conceptualized, designed, and developed over an intensive **12-hour sprint**. 

The challenges required a strong foundation in complex data structures, NP-hard state simulations, and custom parsing. Rather than relying on heavy external libraries, these solutions implement core logic entirely from scratch, showcasing a deep integration of **Low-Level Design (LLD)** principles and competitive programming efficiency.

---

## 🛠️ Tech Stack & Tools
* **Core Language:** C++17
* **Standard Library:** Heavily utilized `<bits/stdc++.h>`, specifically `std::vector`, `std::map`, `std::set`, `std::tuple`, and `std::string` for memory-efficient state management.
* **Architecture:** Custom Abstract Syntax Tree (AST) parsing, Event-Driven Simulation, and State-Space Search (Bitmasking).
* **Build System:** GCC (GNU Compiler Collection).

---

## 🚀 Project Deep Dives

### 1. JSON → TypeScript Type Generator
**Directory:** `/json-to-typescript`

A deterministic type generator that ingests compact JSON arrays and outputs highly formatted, collision-free TypeScript interface declarations.

**Technical Implementation:**
* **Custom JSON AST Parser:** Built a minimal, zero-dependency JSON parser from scratch. It uses a recursive descent approach to tokenize and parse strings, numbers, booleans, nested objects, and arrays into a custom `JsonVal` struct.
* **Deterministic Lexicographical Merging:** Implemented a depth-first, ASCII-ordered traversal algorithm to merge properties across thousands of JSON objects.
* **Collision-Free Naming Engine:** Designed a dynamic naming system that capitalizes parent keys and automatically appends numeric suffixes (e.g., `Address`, `Address2`) when structural collisions occur in the AST, tracking global state via `std::set`.
* **Union Type Resolution:** Engineered logic to detect mixed arrays (e.g., objects mixed with primitives) and compute ASCII-sorted union strings like `(boolean | null | number | string)[]`.

### 2. Multi-Agent Drone Routing in a Temporal Urban Grid
**Directory:** `/drone-delivery`

A spatial-temporal routing engine for managing a fleet of autonomous delivery drones navigating a 2D city grid while balancing battery life and strict package deadlines.

**Technical Implementation:**
* **Temporal-Spatial Collision Detection:** Implemented continuous line-segment checks to prevent drones from intersecting dynamic No-Fly Zones (NFZs) during their active time windows (`T_start` to `T_end`).
* **Energy & Payload State Management:** Built a highly tracked state machine for each drone, calculating Euclidean distances and applying the `distance * (1 + current_payload_weight)` energy degradation formula leg-by-leg.
* **Charging Queue Optimization:** Integrated logic to route drones to the nearest viable charging stations when battery capacity falls below the required threshold for a return trip, accounting for limited station slots.
* **Score Maximization Strategy:** Structured the routing to prioritize dropping heavier payloads first to minimize compounding energy costs, maximizing the custom evaluation metric.

### 3. Group Trip Planner
**Directory:** `/group-trip-planner`

A deterministic optimization engine for planning multi-day group itineraries under strict, dynamic constraints (budget, stamina, and time).

**Technical Implementation:**
* **Bitmasking for NP-Hard Subset Evaluation:** Evaluated all valid combinations of daily activities using bitmask generation (`1 << n`). Filtered out combinations that exceeded the limits of the most restricted group member (lowest budget, lowest energy).
* **Lexicographical Tie-Breaking:** Designed a custom `std::tuple` sorting mechanism to guarantee deterministic output. It prioritizes maximum group satisfaction, then lowest cost, and finally the lexicographical order of activity IDs.
* **Event-Driven Replanning Architecture:** Built a robust replay system to handle chronological real-world interruptions (`WEATHER`, `DROP`, `FATIGUE`, `BUDGET`). Whenever an event triggers, the system accurately recalculates the remaining itinerary by persisting the state of used activities and active users.

---

## 🧠 Design Philosophy

Coming from a background in Mechanical Engineering, my approach to software architecture focuses on robust, fail-safe LLD structures. I write code that is logically transparent, highly optimized, and maintainable.
* **Zero Abstraction Bloat:** Preferring explicit, readable loop structures and intuitive variable naming over overly abstracted "magic."
* **Memory Efficiency:** Extensive use of object references (`&`) and pointers to avoid deep copies during recursive state merging and simulation rollbacks.
* **Edge-Case Resilience:** The solutions are built to handle boundary conditions strictly specified by the problem statements (e.g., empty JSON arrays, null fields, overlapping NFZs, and immediate budget cuts).

---

## ⚙️ Build & Execute

All solutions are self-contained in standard C++ source files.

**Compilation:**
```bash
g++ -O3 -std=c++17 <solution_file.cpp> -o target_executable
