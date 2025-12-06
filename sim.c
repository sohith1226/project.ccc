/* queue_array.c
   Circular Queue Simulation (visual)
   - Shows internal front/rear indices, buffer contents, wrap-around
   Compile: gcc queue_array.c -o queue_array
   Run: ./queue_array
*/

#include <stdio.h>
#include <stdlib.h>

#define Q_MAX 20

typedef struct {
    int data[Q_MAX];
    int front;  // index of first element (valid if count>0)
    int rear;   // index where next element will be inserted
    int count;  // number of elements in queue
} CircularQueue;

/* initialize queue */
void q_init(CircularQueue *q) {
    q->front = 0;
    q->rear = 0;
    q->count = 0;
}

/* check empty/full */
int q_is_empty(const CircularQueue *q) { return q->count == 0; }
int q_is_full(const CircularQueue *q) { return q->count == Q_MAX; }

/* enqueue (returns 1 on success, 0 on overflow) */
int q_enqueue(CircularQueue *q, int val) {
    if (q_is_full(q)) return 0;
    q->data[q->rear] = val;
    q->rear = (q->rear + 1) % Q_MAX;
    q->count++;
    return 1;
}

/* dequeue (returns 1 on success, 0 on underflow) */
int q_dequeue(CircularQueue *q, int *out) {
    if (q_is_empty(q)) return 0;
    *out = q->data[q->front];
    q->front = (q->front + 1) % Q_MAX;
    q->count--;
    return 1;
}

/* peek */
int q_peek(const CircularQueue *q, int *out) {
    if (q_is_empty(q)) return 0;
    *out = q->data[q->front];
    return 1;
}

/* display internal state */
void q_display(const CircularQueue *q) {
    printf("\n--- Circular Queue Internal State ---\n");
    printf("capacity = %d, count = %d\n", Q_MAX, q->count);
    printf("front index = %d, rear index = %d\n", q->front, q->rear);
    printf("buffer (index:value):\n");
    for (int i = 0; i < Q_MAX; ++i) {
        if (q->count > 0) {
            int occupied = 0;
            // an index i is occupied if it lies within the queue contents
            if (q->front < q->rear) {
                if (i >= q->front && i < q->rear) occupied = 1;
            } else if (q->front > q->rear) {
                if (i >= q->front || i < q->rear) occupied = 1;
            } else { // front == rear
                if (q->count == Q_MAX) occupied = 1; // full
            }
            if (occupied) {
                printf(" [%2d] : %d", i, q->data[i]);
                if (i == q->front && i == ((q->rear + Q_MAX - 1) % Q_MAX)) printf("  <-- only element (front==rearprev)");
                else if (i == q->front) printf("  <-- front");
                else if (i == (q->rear + Q_MAX - 1) % Q_MAX) printf("  <-- last in queue (rear-1)");
                printf("\n");
            } else {
                printf(" [%2d] : --\n", i);
            }
        } else {
            printf(" [%2d] : --\n", i);
        }
    }
    printf("-------------------------------------\n");
}

/* automatic demo that shows wrap-around */
void auto_demo() {
    CircularQueue q;
    q_init(&q);
    printf("\nAuto Demo: enqueue 1..6, dequeue 3, enqueue 7..10 (shows wrap-around)\n");
    for (int i = 1; i <= 6; ++i) {
        q_enqueue(&q, i);
        printf("\nEnqueued %d\n", i);
        q_display(&q);
    }
    for (int i = 0; i < 3; ++i) {
        int out;
        q_dequeue(&q, &out);
        printf("\nDequeued %d\n", out);
        q_display(&q);
    }
    for (int i = 7; i <= 10; ++i) {
        if (!q_enqueue(&q, i)) {
            printf("\nAttempt to enqueue %d -> overflow\n", i);
        } else {
            printf("\nEnqueued %d\n", i);
        }
        q_display(&q);
    }
    printf("\nAuto demo finished.\n");
}

/* interactive menu */
void interactive() {
    CircularQueue q;
    q_init(&q);
    int choice;
    while (1) {
        printf("\n--- Circular Queue Menu ---\n");
        printf("1. Enqueue (push to rear)\n");
        printf("2. Dequeue (pop from front)\n");
        printf("3. Peek (front element)\n");
        printf("4. Display internal state\n");
        printf("5. Auto Demo (wrap-around)\n");
        printf("6. Exit\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) { while (getchar() != '\n'); continue; }
        while (getchar() != '\n');
        if (choice == 1) {
            int v;
            printf("Enter value to enqueue: ");
            if (scanf("%d", &v) != 1) { while (getchar() != '\n'); printf("Invalid value.\n"); continue; }
            while (getchar() != '\n');
            if (!q_enqueue(&q, v)) {
                printf("Enqueue failed: queue overflow (capacity %d)\n", Q_MAX);
            } else {
                printf("Enqueued %d\n", v);
                q_display(&q);
            }
        } else if (choice == 2) {
            int out;
            if (!q_dequeue(&q, &out)) {
                printf("Dequeue failed: queue underflow (empty)\n");
            } else {
                printf("Dequeued %d\n", out);
                q_display(&q);
            }
        } else if (choice == 3) {
            int out;
            if (!q_peek(&q, &out)) printf("Peek failed: queue empty\n");
            else printf("Front element = %d\n", out);
        } else if (choice == 4) {
            q_display(&q);
        } else if (choice == 5) {
            auto_demo();
        } else if (choice == 6) {
            printf("Exiting.\n");
            break;
        } else {
            printf("Invalid choice.\n");
        }
    }
}

int main() {
    printf("Circular Queue Simulation\n");
    interactive();
    return 0;
}