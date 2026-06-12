#include <iostream>
#include <stdexcept>
using namespace std;

// ============================================================
//  Linked List - Singly linked list with a rich set of
//  operations: insertion, deletion, search, reversal, cycle
//  detection, and a doubly linked list variant for comparison
// ============================================================

struct Node {
    int data;
    Node* next;
    Node(int val) : data(val), next(nullptr) {}
};

class LinkedList {
private:
    Node* head;
    int length;

public:
    LinkedList() : head(nullptr), length(0) {}

    // Insert at the front of the list - O(1)
    void push(int val) {
        Node* newNode = new Node(val);
        newNode->next = head;
        head = newNode;
        length++;
    }

    // Insert at the end of the list - O(n)
    void append(int val) {
        Node* newNode = new Node(val);
        if (!head) {
            head = newNode;
        } else {
            Node* curr = head;
            while (curr->next) curr = curr->next;
            curr->next = newNode;
        }
        length++;
    }

    // Insert at a specific 0-based index
    void insertAt(int index, int val) {
        if (index < 0 || index > length)
            throw out_of_range("Index out of range");
        if (index == 0) { push(val); return; }

        Node* newNode = new Node(val);
        Node* curr = head;
        for (int i = 0; i < index - 1; i++) curr = curr->next;
        newNode->next = curr->next;
        curr->next = newNode;
        length++;
    }

    // Remove the first node with the given value; returns true if removed
    bool remove(int val) {
        if (!head) return false;
        if (head->data == val) {
            Node* toDelete = head;
            head = head->next;
            delete toDelete;
            length--;
            return true;
        }
        Node* curr = head;
        while (curr->next && curr->next->data != val) curr = curr->next;
        if (!curr->next) return false;
        Node* toDelete = curr->next;
        curr->next = toDelete->next;
        delete toDelete;
        length--;
        return true;
    }

    // Remove node at a specific index
    void removeAt(int index) {
        if (index < 0 || index >= length)
            throw out_of_range("Index out of range");
        Node* toDelete;
        if (index == 0) {
            toDelete = head;
            head = head->next;
        } else {
            Node* curr = head;
            for (int i = 0; i < index - 1; i++) curr = curr->next;
            toDelete = curr->next;
            curr->next = toDelete->next;
        }
        delete toDelete;
        length--;
    }

    // Search for a value, returning its index or -1 if not found
    int find(int val) const {
        Node* curr = head;
        int index = 0;
        while (curr) {
            if (curr->data == val) return index;
            curr = curr->next;
            index++;
        }
        return -1;
    }

    // Get the value at a specific index
    int get(int index) const {
        if (index < 0 || index >= length)
            throw out_of_range("Index out of range");
        Node* curr = head;
        for (int i = 0; i < index; i++) curr = curr->next;
        return curr->data;
    }

    int size() const { return length; }
    bool isEmpty() const { return length == 0; }

    // Reverse the list in place by relinking pointers
    void reverse() {
        Node* prev = nullptr;
        Node* curr = head;
        while (curr) {
            Node* next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }
        head = prev;
    }

    // Return the middle element using the slow/fast pointer technique
    int findMiddle() const {
        if (!head) throw runtime_error("List is empty");
        Node* slow = head;
        Node* fast = head;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        return slow->data;
    }

    // Detect whether the list has a cycle (Floyd's algorithm)
    bool hasCycle() const {
        Node* slow = head;
        Node* fast = head;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
            if (slow == fast) return true;
        }
        return false;
    }

    // Remove duplicate values, keeping the first occurrence of each
    void removeDuplicates() {
        if (!head) return;
        Node* curr = head;
        while (curr) {
            Node* runner = curr;
            while (runner->next) {
                if (runner->next->data == curr->data) {
                    Node* toDelete = runner->next;
                    runner->next = toDelete->next;
                    delete toDelete;
                    length--;
                } else {
                    runner = runner->next;
                }
            }
            curr = curr->next;
        }
    }

    // Merge another sorted list into this sorted list, producing a sorted result
    static LinkedList mergeSorted(const LinkedList& a, const LinkedList& b) {
        LinkedList result;
        Node* pa = a.head;
        Node* pb = b.head;
        // Build a new list by repeatedly appending the smaller front value
        while (pa || pb) {
            if (pa && (!pb || pa->data <= pb->data)) {
                result.append(pa->data);
                pa = pa->next;
            } else {
                result.append(pb->data);
                pb = pb->next;
            }
        }
        return result;
    }

    void print() const {
        Node* curr = head;
        while (curr != nullptr) {
            cout << curr->data << " -> ";
            curr = curr->next;
        }
        cout << "NULL" << endl;
    }

    ~LinkedList() {
        Node* curr = head;
        while (curr) {
            Node* next = curr->next;
            delete curr;
            curr = next;
        }
    }
};

// A doubly linked list, included for comparison with the singly linked list above
struct DNode {
    int data;
    DNode* prev;
    DNode* next;
    DNode(int val) : data(val), prev(nullptr), next(nullptr) {}
};

class DoublyLinkedList {
private:
    DNode* head;
    DNode* tail;
    int length;

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), length(0) {}

    void pushFront(int val) {
        DNode* node = new DNode(val);
        node->next = head;
        if (head) head->prev = node;
        head = node;
        if (!tail) tail = head;
        length++;
    }

    void pushBack(int val) {
        DNode* node = new DNode(val);
        node->prev = tail;
        if (tail) tail->next = node;
        tail = node;
        if (!head) head = tail;
        length++;
    }

    void printForward() const {
        DNode* curr = head;
        cout << "Forward: ";
        while (curr) {
            cout << curr->data << " ";
            curr = curr->next;
        }
        cout << endl;
    }

    void printBackward() const {
        DNode* curr = tail;
        cout << "Backward: ";
        while (curr) {
            cout << curr->data << " ";
            curr = curr->prev;
        }
        cout << endl;
    }

    int size() const { return length; }

    ~DoublyLinkedList() {
        DNode* curr = head;
        while (curr) {
            DNode* next = curr->next;
            delete curr;
            curr = next;
        }
    }
};

int main() {
    cout << "=== Basic Push and Print ===" << endl;
    LinkedList list;
    list.push(3);
    list.push(2);
    list.push(1);
    list.print(); // 1 -> 2 -> 3 -> NULL

    cout << endl << "=== Append ===" << endl;
    list.append(4);
    list.append(5);
    list.print(); // 1 -> 2 -> 3 -> 4 -> 5 -> NULL
    cout << "Size: " << list.size() << endl;

    cout << endl << "=== Insert and Get ===" << endl;
    list.insertAt(2, 99);
    list.print();
    cout << "Element at index 2: " << list.get(2) << endl;

    cout << endl << "=== Find ===" << endl;
    cout << "Index of 99: " << list.find(99) << endl;
    cout << "Index of 1000 (not present): " << list.find(1000) << endl;

    cout << endl << "=== Remove ===" << endl;
    list.remove(99);
    list.print();
    list.removeAt(0);
    list.print();

    cout << endl << "=== Reverse ===" << endl;
    list.reverse();
    list.print();

    cout << endl << "=== Find Middle ===" << endl;
    cout << "Middle element: " << list.findMiddle() << endl;

    cout << endl << "=== Cycle Detection ===" << endl;
    cout << "Has cycle: " << (list.hasCycle() ? "yes" : "no") << endl;

    cout << endl << "=== Removing Duplicates ===" << endl;
    LinkedList dupList;
    for (int v : {1, 2, 2, 3, 3, 3, 4, 1}) dupList.push(v);
    dupList.reverse(); // restore insertion order for clarity
    cout << "Before: ";
    dupList.print();
    dupList.removeDuplicates();
    cout << "After:  ";
    dupList.print();
    cout << "Size after dedup: " << dupList.size() << endl;

    cout << endl << "=== Merging Two Sorted Lists ===" << endl;
    LinkedList a, b;
    for (int v : {5, 3, 1}) a.push(v); // results in 1 -> 3 -> 5
    for (int v : {6, 4, 2}) b.push(v); // results in 2 -> 4 -> 6
    cout << "List A: "; a.print();
    cout << "List B: "; b.print();
    LinkedList merged = LinkedList::mergeSorted(a, b);
    cout << "Merged: "; merged.print();

    cout << endl << "=== Exception Handling for Invalid Index ===" << endl;
    try {
        list.get(100);
    } catch (const out_of_range& e) {
        cout << "Caught exception: " << e.what() << endl;
    }

    cout << endl << "=== Doubly Linked List ===" << endl;
    DoublyLinkedList dll;
    dll.pushBack(1);
    dll.pushBack(2);
    dll.pushBack(3);
    dll.pushFront(0);
    dll.printForward();
    dll.printBackward();
    cout << "Size: " << dll.size() << endl;

    return 0;
}