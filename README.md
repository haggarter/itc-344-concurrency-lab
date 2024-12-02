# Concurrency Lab

## Objective
Develop a basic parallel wait-and-echo web server that demonstrates handling multiple client connections concurrently. The focus will be on exploring two concurrency models: using threads and using processes. Students will also analyze potential race conditions, particularly in the threaded implementation.

## Scenario Overview
EchoCo, a company running a simple echo server for testing communication protocols, has requested your help to upgrade their server to handle multiple client connections simultaneously. Since real-world web servers rely heavily on parallelization, this lab simulates a practical scenario where you must decide how to handle client connections efficiently.

The socket setup for accepting client connections is pre-built and provided to you. Your task is to implement the logic for managing these connections, either by:

- Forking a new process for each client.
- Assigning each client connection to a thread in a thread pool.
- Learning Objectives
- Understand and implement concurrency using threads and processes.
- Identify and address race conditions in a multi-threaded context.
- Compare performance and resource usage between threading and forking models.
