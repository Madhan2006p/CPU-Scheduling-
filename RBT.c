#include <stdio.h>
#include <stdlib.h>

struct queueNode {
    int data;
    int arrivalTime;  
    int burstTime;    
    struct queueNode* next;
};

struct queue {
    struct queueNode* front;
};

#define RED 1
#define BLACK 0

struct node {
    int key;                
    int color;              
    struct node *left, *right, *parent;
    struct queue* highPriorityQueue;  
    struct queue* lowPriorityQueue;   
};

struct queueNode* newQueueNode(int data, int arrivalTime, int burstTime) {
    struct queueNode* temp = (struct queueNode*)malloc(sizeof(struct queueNode));
    temp->data = data;
    temp->arrivalTime = arrivalTime;
    temp->burstTime = burstTime;
    temp->next = NULL;
    return temp;
}

struct queue* createQueue() {
    struct queue* q = (struct queue*)malloc(sizeof(struct queue));
    q->front = NULL;
    return q;
}

// Enqueue task into the priority queue, sorted by burstTime, then arrivalTime
void priorityEnqueue(struct queue* q, int data, int arrivalTime, int burstTime) {
    struct queueNode* temp = newQueueNode(data, arrivalTime, burstTime);

    if (q->front == NULL || (burstTime < q->front->burstTime || 
        (burstTime == q->front->burstTime && arrivalTime < q->front->arrivalTime))) {
        temp->next = q->front;
        q->front = temp;
    } else {
        struct queueNode* current = q->front;
        while (current->next != NULL && 
               (current->next->burstTime < burstTime || 
               (current->next->burstTime == burstTime && current->next->arrivalTime <= arrivalTime))) {
            current = current->next;
        }
        temp->next = current->next;
        current->next = temp;
    }
}

int dequeue(struct queue* q) {
    if (q->front == NULL)
        return -1;

    int data = q->front->data;
    struct queueNode* temp = q->front;
    q->front = q->front->next;

    free(temp);
    return data;
}

struct node* newNode(int key) {
    struct node* temp = (struct node*)malloc(sizeof(struct node));
    temp->key = key;
    temp->color = RED; 
    temp->left = temp->right = temp->parent = NULL;
    temp->highPriorityQueue = createQueue();
    temp->lowPriorityQueue = createQueue();
    return temp;
}

void leftRotate(struct node** root, struct node* x) {
    struct node* y = x->right;
    x->right = y->left;

    if (y->left != NULL)
        y->left->parent = x;

    y->parent = x->parent;

    if (x->parent == NULL)
        *root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;
    x->parent = y;
}

void rightRotate(struct node** root, struct node* y) {
    struct node* x = y->left;
    y->left = x->right;

    if (x->right != NULL)
        x->right->parent = y;

    x->parent = y->parent;

    if (y->parent == NULL)
        *root = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;

    x->right = y;
    y->parent = x;
}

void fixViolation(struct node** root, struct node* z) {
    struct node* parent = NULL;
    struct node* grandparent = NULL;

    while ((z != *root) && (z->color != BLACK) && (z->parent->color == RED)) {
        parent = z->parent;
        grandparent = z->parent->parent;

        if (parent == grandparent->left) {
            struct node* uncle = grandparent->right;

            if (uncle != NULL && uncle->color == RED) {
                grandparent->color = RED;
                parent->color = BLACK;
                uncle->color = BLACK;
                z = grandparent;
            } else {
                if (z == parent->right) {
                    leftRotate(root, parent);
                    z = parent;
                    parent = z->parent;
                }

                rightRotate(root, grandparent);
                int tempColor = parent->color;
                parent->color = grandparent->color;
                grandparent->color = tempColor;
                z = parent;
            }
        } else {  
            struct node* uncle = grandparent->left;

            if (uncle != NULL && uncle->color == RED) {
                grandparent->color = RED;
                parent->color = BLACK;
                uncle->color = BLACK;
                z = grandparent;
            } else {
                if (z == parent->left) {
                    rightRotate(root, parent);
                    z = parent;
                    parent = z->parent;
                }

                leftRotate(root, grandparent);
                int tempColor = parent->color;
                parent->color = grandparent->color;
                grandparent->color = tempColor;
                z = parent;
            }
        }
    }

    (*root)->color = BLACK;
}

void insert(struct node** root, int key) {
    struct node* z = newNode(key);

    struct node* y = NULL;
    struct node* x = *root;

    while (x != NULL){
        y = x;
        if (key < x->key)
            x = x->left;
        else if (key > x->key)
            x = x->right;
        else
            return; 
    }

    z->parent = y;

    if (y == NULL)
        *root = z;
    else if (key < y->key)
        y->left = z;
    else
        y->right = z;

    fixViolation(root, z);
}

// Modified function to include priority as a parameter
void insertQueueInNode(struct node* root, int key, int data, int arrivalTime, int burstTime, char priority) {
    if (root == NULL)
        return;

    if (key == root->key) {
        if (priority == 'H') {
            priorityEnqueue(root->highPriorityQueue, data, arrivalTime, burstTime);
        } else {
            priorityEnqueue(root->lowPriorityQueue, data, arrivalTime, burstTime);
        }
    } else if (key < root->key) {
        insertQueueInNode(root->left, key, data, arrivalTime, burstTime, priority);
    } else {
        insertQueueInNode(root->right, key, data, arrivalTime, burstTime, priority);
    }
}

void printQueue(struct queue* q, char priorityLabel) {
    struct queueNode* temp = q->front;
    while (temp != NULL) {
        printf("%c: Process %d, Arrival Time: %d, Burst Time: %d\n", 
               priorityLabel, temp->data, temp->arrivalTime, temp->burstTime);
        temp = temp->next;
    }
}

void inOrder(struct node* root) {
    if (root != NULL) {
        inOrder(root->left);
        printf("Node priority: %d\n", root->key);
        printf("High-priority queue:\n");
        printQueue(root->highPriorityQueue, 'H');
        printf("Low-priority queue:\n");
        printQueue(root->lowPriorityQueue, 'L');
        printf("\n");
        inOrder(root->right);
    }
}

void processTasks(struct node* root) {
    if (root == NULL)
        return;

    processTasks(root->left);

    printf("Processing high-priority tasks for node with priority %d:\n", root->key);
    while (root->highPriorityQueue->front != NULL) {
        printf("Processing task: %d\n", root->highPriorityQueue->front->data);
        dequeue(root->highPriorityQueue);
    }

    printf("Processing low-priority tasks for node with priority %d:\n", root->key);
    while (root->lowPriorityQueue->front != NULL) {
        printf("Processing task: %d\n", root->lowPriorityQueue->front->data);
        dequeue(root->lowPriorityQueue);
    }

    processTasks(root->right);
}

int main() {
    struct node* root = NULL;
    int numNodes, key, queueData, numQueueElements, nodeKey, arrivalTime, burstTime;
    char priority;

    printf("Enter the number of nodes in the tree: ");
    scanf("%d", &numNodes);

    // Insert nodes into the tree
    for (int i = 0; i < numNodes; i++) {
        printf("Enter the priority key for node %d: ", i + 1);
        scanf("%d", &key);
        insert(&root, key);
    }

    printf("Enter the number of tasks to insert: ");
    scanf("%d", &numQueueElements);

    // Insert tasks into the queues of corresponding nodes
    for (int i = 0; i < numQueueElements; i++) {
        printf("Enter the node key, process ID, arrival time, burst time, and priority (H/L) for task %d: ", i + 1);
        scanf("%d %d %d %d %c", &nodeKey, &queueData, &arrivalTime, &burstTime, &priority);
        insertQueueInNode(root, nodeKey, queueData, arrivalTime, burstTime, priority);
    }

    // Print and process the tasks
    printf("\nIn-Order Traversal and Task Details:\n");
    inOrder(root);

    printf("\nProcessing Tasks:\n");
    processTasks(root);

    return 0;
}
