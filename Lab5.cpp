#include <iostream>
#include <vector>
#include <string>

template <typename T>
class Queue {
private:
    std::vector<T> elements;  
    size_t frontIndex = 0;    

public:
    void enqueue(const T& item) {
        elements.push_back(item);
        std::cout << "Enqueued: " << item << std::endl;
    }

    T dequeue() {
        if (isEmpty()) {
            std::cerr << "Error: Queue is empty - cannot dequeue" << std::endl;
            return T(); 
        }

        T item = elements[frontIndex];
        frontIndex++;

        if (frontIndex > elements.size() / 2) {
            elements.erase(elements.begin(), elements.begin() + frontIndex);
            frontIndex = 0;
            std::cout << "Queue memory optimized" << std::endl;
        }

        return item;
    }

    T peek() const {
        if (isEmpty()) {
            std::cerr << "Error: Queue is empty - cannot peek" << std::endl;
            return T(); 
        }
        return elements[frontIndex];
    }

    bool isEmpty() const {
        return frontIndex >= elements.size();
    }

    size_t size() const {
        return elements.size() - frontIndex;
    }

    void clear() {
        elements.clear();
        frontIndex = 0;
        std::cout << "Queue cleared" << std::endl;
    }
};

int main() {
    Queue<std::string> messageQueue;

    messageQueue.enqueue("First message");
    messageQueue.enqueue("Second message");
    messageQueue.enqueue("Third message");

    std::cout << "\nQueue size: " << messageQueue.size() << std::endl;
    std::cout << "Front element: " << messageQueue.peek() << "\n" << std::endl;

    while (!messageQueue.isEmpty()) {
        std::cout << "Dequeued: " << messageQueue.dequeue() << std::endl;
    }

    std::cout << "\nQueue is now empty: " << std::boolalpha << messageQueue.isEmpty() << std::endl;

    std::cout << "\nTrying to dequeue from empty queue:" << std::endl;
    std::string result = messageQueue.dequeue();
    if (result.empty()) {
        std::cout << "Got default value from empty queue" << std::endl;
    }

    Queue<int> intQueue;
    intQueue.enqueue(100);
    intQueue.enqueue(200);
    std::cout << "\nInteger queue front: " << intQueue.peek() << std::endl;

    return 0;
}