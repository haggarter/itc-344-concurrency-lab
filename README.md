# Concurrency Lab

## Objective
Develop a basic parallel wait-and-echo web server that demonstrates handling multiple client connections concurrently. The focus will be on exploring two concurrency models: using threads and using processes. Students will also analyze potential race conditions, particularly in the threaded implementation.

## Scenario Overview
EchoCo, a company running a simple echo server for testing communication protocols, has requested your help to upgrade their server to handle multiple client connections simultaneously. Since real-world web servers rely heavily on parallelization, this lab simulates a practical scenario where you must decide how to handle client connections efficiently.

The socket setup for accepting client connections is pre-built and provided to you. Your task is to implement the logic for managing these connections, either by:

- Forking a new process for each client.
- Assigning each client connection to a thread in a thread pool.

## Learning Objectives
- Understand and implement concurrency using threads and processes.
- Identify and address race conditions in a multi-threaded context.
- Compare performance and resource usage between threading and forking models.


## Where to Start
- get server running
- Test the code to see what happens
- what should be changed?

## Processes vs. Threads

### Processes
A process is an active instance of a program running on a computer. Each process has its own dedicated memory space, separate from other processes, ensuring they don’t interfere with one another. The operating system is responsible for managing processes, allocating the necessary resources—such as memory, CPU time, and file handles—for each one. The following diagram illustrates a single process in action.

> ![asingle process](https://github.com/user-attachments/assets/41dfca9c-dd91-45a2-97e6-95392ae44f8d)
When only one process is running, the computer can focus all its resources on completing it efficiently. However, what happens when we need to run multiple instances of the same process or several different processes simultaneously? The diagram below shows how this scenario unfolds.

> ![multiple processes](https://github.com/user-attachments/assets/f8a9cbf0-3b39-415c-afec-d6853813999b)

This approach is not efficient and can overwhelm the CPU, causing processes to compete for resources and take longer to complete. To address these challenges, modern systems often rely on threads, which allow multiple tasks to run concurrently within a single process, reducing overhead and improving performance. Let’s explore how threads work and why they are a more efficient alternative in certain scenarios. 

## Additional Information
(include links to more resources)


## Grade
- [ ] Successfully implement processes
- [ ] Successfully implement thread
- [ ] Identify the benefits and costs of each option
- [ ] Resolve race-case scenarios with threads
