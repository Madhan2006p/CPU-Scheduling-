Priority Splay Tree with Task Queues

This project implements a priority-based Red-Black tree where each node has two priority queues for managing tasks. The goal is to simulate a scheduling algorithm with a hierarchical task queue structure, where tasks are assigned to nodes based on priority, arrival time, and burst time.

Table of Contents
- Features
- Requirements
- Usage
- Code Overview
- Example Input and Output
- Compilation and Execution
- License

---

Features
- Red-Black Tree Implementation: Nodes are inserted and organized using Red-Black Tree properties.
- High and Low Priority Queues: Each node has two queues (high and low priority) with tasks sorted by burst time and arrival time.
- Task Processing: Tasks are processed by priority, first handling high-priority tasks and then low-priority tasks within each node.

Requirements
- C Compiler (e.g., GCC)
- Standard C Library

Usage
1. Tree Node Insertion: Enter priority keys to insert nodes into the tree.
2. Task Queue Insertion: Assign tasks with parameters such as process ID, arrival time, burst time, and priority (high or low) to nodes.
3. Task Processing: Processes tasks based on node priority and task priority queues.

Code Overview
- struct queueNode: Represents a task with data, arrival time, and burst time.
- struct queue: Manages a linked list queue for high or low priority tasks.
- struct node: Represents a node in the Red-Black Tree with its priority queues.
- Functions:
  - newQueueNode: Creates a new task for the queue.
  - priorityEnqueue: Adds a task to a priority queue, sorted by burst and arrival times.
  - dequeue: Removes the front task from a queue.
  - newNode: Initializes a Red-Black Tree node with priority queues.
  - leftRotate and rightRotate: Perform rotations for Red-Black Tree balancing.
  - fixViolation: Ensures Red-Black Tree properties after insertion.
  - insert: Inserts a node into the Red-Black Tree.
  - insertQueueInNode: Adds a task to the appropriate queue in a specified node.
  - inOrder: Displays nodes and their queues in in-order traversal.
  - processTasks: Processes all tasks in the Red-Black Tree nodes, prioritizing high-priority tasks first.

Example Input and Output
Example:
```
Enter the number of nodes in the tree: 2
Enter the priority key for node 1: 10
Enter the priority key for node 2: 20

Enter the number of tasks to insert: 3
Enter the node key, process ID, arrival time, burst time, and priority (H/L) for task 1: 10 1 0 5 H
Enter the node key, process ID, arrival time, burst time, and priority (H/L) for task 2: 20 2 1 3 L
Enter the node key, process ID, arrival time, burst time, and priority (H/L) for task 3: 10 3 2 2 H
```

Output:
```
In-Order Traversal and Task Details:
Node priority: 10
High-priority queue:
H: Process 3, Arrival Time: 2, Burst Time: 2
H: Process 1, Arrival Time: 0, Burst Time: 5
Low-priority queue:

Node priority: 20
High-priority queue:
Low-priority queue:
L: Process 2, Arrival Time: 1, Burst Time: 3

Processing Tasks:
Processing high-priority tasks for node with priority 10:
Processing task: 3
Processing task: 1
Processing low-priority tasks for node with priority 10:
Processing high-priority tasks for node with priority 20:
Processing low-priority tasks for node with priority 20:
Processing task: 2
```
