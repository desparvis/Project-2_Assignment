# Project 2 Assignment Linux Programming

## Author  
**Credo Desparvis Gutabarwa**

---

## Overview

This repository contains four system-level and multi-threaded C programming tasks. The project demonstrates concepts such as process creation, inter-process communication, file I/O, performance analysis, and multi-threading using POSIX threads.

Each question is organized in its own directory.

---


---

## Question 1: Process Pipeline & System Calls

### 🔹 Description  
Implements a pipeline equivalent to:
ps aux | grep root


using:
- `fork()` for process creation  
- `pipe()` for inter-process communication  
- `execvp()` for command execution  

The parent process captures the output into a file and displays part of it.

### 🔹 Features  
- Two child processes  
- Output redirection using `dup2()`  
- File handling  
- System call tracing using `strace`  

---

## Question 2: File Copy Performance Comparison

### 🔹 Description  
Two versions of a large file copy utility:

- **Low-level:** `read`, `write`  
- **High-level:** `fread`, `fwrite`  

Each version copies a file (100MB+) and measures performance.

### 🔹 Features  
- Execution time measurement (`clock()` and `time`)  
- System call analysis using `strace -c`  

### 🔹 Findings  
- Both versions show similar performance  
- ~51,000 system calls (`read`/`write`)  
- Program is **I/O-bound** (disk speed dominates)

---

## Question 3: Multi-threaded Prime Counter

### 🔹 Description  
Counts prime numbers between **1 and 200,000** using **16 threads**.

### 🔹 Features  
- Even workload distribution  
- Efficient prime checking (√n optimization)  
- Mutex (`pthread_mutex_t`) for synchronization  

### 🔹 Output  
The synchronized total number of prime numbers between 1 and 200000 is 17984


---

## 🧩 Question 4: Multi-threaded Keyword Search

### 🔹 Description  
Searches for a keyword across multiple text files using multi-threading.

### 🔹 Features  
- Each thread processes one file  
- Counts keyword occurrences  
- Writes results to a shared output file  
- Mutex ensures safe concurrent writes  

### 🔹 Execution Format  
./search keyword output.txt file1.txt file2.txt ... <number_of_threads>


### 🔹 Example 
./search hello output.txt file1.txt file2.txt file3.txt 4


---

## Performance Analysis (Q4)

- Best performance observed at **4 threads**
- Increasing threads improves speed initially  
- Too many threads → less improvement due to extra work and coordination  

### 🔹 Results

| Threads | Time (real) |
|--------|------------|
| 2      | 0.021s     |
| 4      | 0.013s     |
| 8      | 0.018s     |
| 12     | 0.015s     |

---

## Compilation & Execution

### 🔹 Compile
gcc q1.c -o q1
gcc q2v1.c -o low
gcc q2v2.c -o high
gcc q3.c -o q3 -lm
gcc q4.c -o search -lpthread


---

## Key Concepts

- Process creation (`fork`, `exec`)  
- Inter-process communication (`pipe`)  
- File I/O (low-level vs high-level)  
- System call tracing (`strace`)  
- Multi-threading (`pthread`)  
- Synchronization (`mutex`)  
- Performance analysis  

---

## Conclusion

This project demonstrates practical use of operating system concepts, including process management, threading, and performance analysis. The results show how system-level factors like disk I/O and thread overhead affect real-world performance.
