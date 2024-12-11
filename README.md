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

### Threads

A thread is the smallest unit of execution within a process. While a process operates in its own isolated memory space, threads within the same process share memory and other resources like file descriptors. This shared environment makes threads lightweight and efficient, allowing for faster communication and reduced overhead compared to processes.

**How Threads Work** 
Threads run concurrently within a process, meaning they can execute tasks in parallel on multi-core processors or simulate parallelism on a single core through time-sharing. For example, a web browser might use threads to render a page, load images, and handle user input simultaneously, all within a single process. Using the same diagram as before, it would look something like this: 

![thread_in_process](https://github.com/user-attachments/assets/78b44cf4-e918-4387-8f2f-835d47987ad2)

**Why Threads are Beneficial**

- Efficiency: Since threads share the process's memory space, the overhead of creating and managing them is much lower than that of processes.
- Parallelism: Threads enable tasks to run simultaneously, making better use of multi-core processors and improving performance for multi-tasking.
- Communication: Because threads share memory, they can exchange information quickly and easily without needing complex Inter-Process Communication (IPC) mechanisms.

**Challenges with Threads**
While threads offer many advantages, they also introduce complexities, especially when multiple threads access shared resources:

 **Race Conditions:**
 A race condition occurs when two or more threads access shared data simultaneously, and the final outcome depends on the timing of their execution. For instance, if two threads increment the same counter variable without proper synchronization, they might overwrite each other's updates, leading to incorrect results.


**Synchronization and the Role of Semaphores:**
To prevent race conditions, synchronization mechanisms like semaphores are used. A semaphore acts as a signaling mechanism to control access to shared resources. For example, a semaphore can allow only one thread to modify a shared variable at a time, ensuring consistency. 

However, improper use of semaphores can lead to:

- Deadlocks: When two or more threads are waiting on each other to release resources, resulting in a standstill.
- Starvation: When a thread is perpetually delayed because higher-priority threads monopolize access to the semaphore.

Threads provide powerful tools for improving performance and resource utilization, but they require careful management to avoid these pitfalls. Let’s explore how these challenges are addressed in practice and why threads are a critical component of modern computing.

## Additional Information
Below are some sites that discuss more of the topics if you have any questions:  
[Processes vs. Threads](https://www.geeksforgeeks.org/difference-between-process-and-thread/#)  
[How semaphores work](https://www.geeksforgeeks.org/semaphores-in-process-synchronization/)

## Grade
- [ ] Successfully implement processes
- [ ] Successfully implement thread
- [ ] Identify the benefits and costs of each option
- [ ] Resolve race-case scenarios with threads
