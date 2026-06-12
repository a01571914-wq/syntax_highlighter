#include <iostream>
#include <stdexcept>
#include <queue>
#include <deque>
#include <climits>
using namespace std;

// ============================================================
//  Queue Implementations - Circular array queue, linked-list
//  queue, priority queue, and a deque-based double-ended queue
// ============================================================

// Circular array-based queue with a fixed capacity
template <typename T>
class ArrayQueue {
private:
    T* data;
    int front, rear, count, capacity;

public:
    explicit ArrayQueue(int cap) : front(0), rear(-1), count(0), capacity(cap) {
        data = new T[capacity];
    }

    void enqueue(const T& val) {
        if (count == capacity)
            throw overflow_error("Queue is full");
        rear = (rear + 1) % capacity;
        data[rear] = val;
        count++;
    }

    T dequeue() {
        if (count == 0)
            throw underflow_error("Queue is empty");
        T val = data[front];
        front = (front + 1) % capacity;
        count--;
        return val;
    }

    T peekFront() const {
        if (count == 0)
            throw underflow_error("Queue is empty");
        return data[front];
    }

    T peekBack() const {
        if (count == 0)
            throw underflow_error("Queue is empty");
        return data[rear];
    }

    bool isEmpty() const { return count == 0; }
    bool isFull() const { return count == capacity; }
    int size() const { return count; }

    ~ArrayQueue() { delete[] data; }
};

// Linked-list based queue with no fixed capacity
template <typename T>
class LinkedQueue {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& val) : data(val), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int count;

public:
    LinkedQueue() : head(nullptr), tail(nullptr), count(0) {}

    void enqueue(const T& val) {
        Node* node = new Node(val);
        if (tail) tail->next = node;
        tail = node;
        if (!head) head = tail;
        count++;
    }

    T dequeue() {
        if (!head)
            throw underflow_error("Queue is empty");
        Node* old = head;
        T val = old->data;
        head = head->next;
        if (!head) tail = nullptr;
        delete old;
        count--;
        return val;
    }

    T peekFront() const {
        if (!head)
            throw underflow_error("Queue is empty");
        return head->data;
    }

    bool isEmpty() const { return count == 0; }
    int size() const { return count; }

    ~LinkedQueue() {
        while (head) {
            Node* next = head->next;
            delete head;
            head = next;
        }
    }
};

// A simple min-priority queue implemented with a binary heap stored in a vector
class MinPriorityQueue {
private:
    vector<int> heap;

    void siftUp(int idx) {
        while (idx > 0) {
            int parent = (idx - 1) / 2;
            if (heap[parent] <= heap[idx]) break;
            swap(heap[parent], heap[idx]);
            idx = parent;
        }
    }

    void siftDown(int idx) {
        int n = (int)heap.size();
        while (true) {
            int left = 2 * idx + 1;
            int right = 2 * idx + 2;
            int smallest = idx;
            if (left < n && heap[left] < heap[smallest]) smallest = left;
            if (right < n && heap[right] < heap[smallest]) smallest = right;
            if (smallest == idx) break;
            swap(heap[idx], heap[smallest]);
            idx = smallest;
        }
    }

public:
    void push(int val) {
        heap.push_back(val);
        siftUp((int)heap.size() - 1);
    }

    int pop() {
        if (heap.empty())
            throw underflow_error("Priority queue is empty");
        int top = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        if (!heap.empty()) siftDown(0);
        return top;
    }

    int top() const {
        if (heap.empty())
            throw underflow_error("Priority queue is empty");
        return heap[0];
    }

    bool isEmpty() const { return heap.empty(); }
    size_t size() const { return heap.size(); }
};

// A double-ended queue (deque) implementation using a fixed-size circular buffer
template <typename T>
class CircularDeque {
private:
    T* data;
    int front, rear, count, capacity;

public:
    explicit CircularDeque(int cap) : front(0), rear(0), count(0), capacity(cap) {
        data = new T[capacity];
    }

    void pushFront(const T& val) {
        if (count == capacity) throw overflow_error("Deque is full");
        front = (front - 1 + capacity) % capacity;
        data[front] = val;
        count++;
    }

    void pushBack(const T& val) {
        if (count == capacity) throw overflow_error("Deque is full");
        data[rear] = val;
        rear = (rear + 1) % capacity;
        count++;
    }

    T popFront() {
        if (count == 0) throw underflow_error("Deque is empty");
        T val = data[front];
        front = (front + 1) % capacity;
        count--;
        return val;
    }

    T popBack() {
        if (count == 0) throw underflow_error("Deque is empty");
        rear = (rear - 1 + capacity) % capacity;
        count--;
        return data[rear];
    }

    bool isEmpty() const { return count == 0; }
    int size() const { return count; }

    ~CircularDeque() { delete[] data; }
};

// Demonstrates a classic queue application: sliding window maximum using a deque
vector<int> slidingWindowMax(const vector<int>& nums, int k) {
    deque<int> dq; // stores indices, values decreasing from front to back
    vector<int> result;

    for (int i = 0; i < (int)nums.size(); i++) {
        while (!dq.empty() && dq.front() <= i - k) dq.pop_front();
        while (!dq.empty() && nums[dq.back()] <= nums[i]) dq.pop_back();
        dq.push_back(i);
        if (i >= k - 1) result.push_back(nums[dq.front()]);
    }
    return result;
}

int main() {
    cout << "=== Array-Based Circular Queue ===" << endl;
    ArrayQueue<int> aq(5);
    aq.enqueue(10);
    aq.enqueue(20);
    aq.enqueue(30);
    cout << "Front: " << aq.peekFront() << ", Back: " << aq.peekBack() << endl;
    cout << "Dequeue: " << aq.dequeue() << endl;
    cout << "Dequeue: " << aq.dequeue() << endl;
    aq.enqueue(40);
    aq.enqueue(50);
    aq.enqueue(60);
    cout << "Size after wraparound: " << aq.size() << endl;

    try {
        ArrayQueue<int> small(2);
        small.enqueue(1);
        small.enqueue(2);
        small.enqueue(3); // should throw
    } catch (const overflow_error& e) {
        cout << "Caught: " << e.what() << endl;
    }

    cout << endl << "=== Linked-List Based Queue ===" << endl;
    LinkedQueue<string> lq;
    lq.enqueue("first");
    lq.enqueue("second");
    lq.enqueue("third");
    cout << "Front: " << lq.peekFront() << endl;
    cout << "Dequeue: " << lq.dequeue() << endl;
    cout << "Dequeue: " << lq.dequeue() << endl;
    cout << "Size: " << lq.size() << endl;

    try {
        LinkedQueue<int> emptyQ;
        emptyQ.dequeue();
    } catch (const underflow_error& e) {
        cout << "Caught: " << e.what() << endl;
    }

    cout << endl << "=== Min Priority Queue (Binary Heap) ===" << endl;
    MinPriorityQueue pq;
    for (int v : {5, 1, 8, 2, 9, 0, 3}) pq.push(v);
    cout << "Popping in priority order: ";
    while (!pq.isEmpty()) cout << pq.pop() << " ";
    cout << endl;

    cout << endl << "=== Circular Deque ===" << endl;
    CircularDeque<int> cd(6);
    cd.pushBack(2);
    cd.pushBack(3);
    cd.pushFront(1);
    cd.pushFront(0);
    cd.pushBack(4);
    cout << "Size: " << cd.size() << endl;
    cout << "popFront: " << cd.popFront() << endl;
    cout << "popBack: " << cd.popBack() << endl;
    cout << "Remaining size: " << cd.size() << endl;

    cout << endl << "=== Sliding Window Maximum ===" << endl;
    vector<int> nums = {1, 3, -1, -3, 5, 3, 6, 7};
    int k = 3;
    cout << "Array: ";
    for (int n : nums) cout << n << " ";
    cout << endl << "Window size: " << k << endl;
    vector<int> maxes = slidingWindowMax(nums, k);
    cout << "Max of each window: ";
    for (int m : maxes) cout << m << " ";
    cout << endl;

    cout << endl << "=== Standard Library queue and priority_queue ===" << endl;
    queue<int> stdQ;
    stdQ.push(1);
    stdQ.push(2);
    stdQ.push(3);
    cout << "std::queue front: " << stdQ.front() << ", back: " << stdQ.back() << endl;

    priority_queue<int> stdMaxHeap;
    for (int v : {3, 1, 4, 1, 5, 9, 2, 6}) stdMaxHeap.push(v);
    cout << "std::priority_queue (max-heap) pop order: ";
    while (!stdMaxHeap.empty()) {
        cout << stdMaxHeap.top() << " ";
        stdMaxHeap.pop();
    }
    cout << endl;

    return 0;
}