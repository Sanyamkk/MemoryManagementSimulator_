# Memory Management Simulator  

A C++ project that simulates memory allocation and deallocation using **FIFO** and **LRU** replacement policies, with automatic **defragmentation** and memory map visualization.  

---

## Features
- Memory allocation & deallocation for processes  
- FIFO & LRU page replacement  
- Defragmentation to reduce fragmentation  
- Visualization of memory map (`P1 | P2 | Free | Free`)  

---

## Data Structures Used
- **Vector** → to represent the memory map  
- **Map / Unordered Map** → to keep track of allocated blocks and process IDs  
- **Queue** → for FIFO replacement policy  
- **Hash Map** → for storing last used times (LRU policy)  

---

## Algorithms Used
- **FIFO (First-In-First-Out)** for memory replacement  
- **LRU (Least Recently Used)** for memory replacement  
- **Defragmentation Algorithm** → shifting processes to eliminate free memory gaps  

---

## 🛠Tech Stack
- **Language:** C++  
- **Paradigm:** Object-Oriented Programming (OOP)  
- **Build Tool:** g++ (GNU Compiler)  
- **Version Control:** Git & GitHub  

---

## How to Run
```bash
# Clone the repo
git clone https://github.com/your-username/memory-management-simulator.git
cd memory-management-simulator

# Compile
g++ src/main.cpp src/MemoryManager.cpp -I include -o memory_manager

# Run
./memory_manager
