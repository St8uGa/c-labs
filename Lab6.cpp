#include <iostream>
#include <vector>
#include <stdexcept>

template <typename T>
class Queue {
private:
    std::vector<T> elements;
    size_t head = 0;
    size_t tail = 0;
    size_t count = 0;
    size_t capacity = 4

    void resize() {
        size_t new_capacity = capacity * 2;
        std::vector<T> new_elements(new_capacity);

        for (size_t i = 0; i < count; ++i) {
            new_elements[i] = elements[(head + i) % capacity];
        }

        elements = std::move(new_elements);
        head = 0;
        tail = count;
        capacity = new_capacity;

        std::cout << "Очередь увеличена до " << capacity << " элементов\n";
    }

public:
    Queue() : elements(capacity) {}

    void enqueue(const T& value) {
        if (count == capacity) {
            resize();
        }

        elements[tail] = value;
        tail = (tail + 1) % capacity;
        count++;
    }

    T dequeue() {
        if (isEmpty()) {
            throw std::runtime_error("Очередь пуста - невозможно извлечь элемент");
        }

        T value = elements[head];
        head = (head + 1) % capacity;
        count--;

        return value;
    }

    T peek() const {
        if (isEmpty()) {
            throw std::runtime_error("Очередь пуста - невозможно посмотреть элемент");
        }
        return elements[head];
    }

    bool isEmpty() const {
        return count == 0;
    }

    size_t size() const {
        return count;
    }

    void clear() {
        head = 0;
        tail = 0;
        count = 0;
    }
};

int main() {
    try {
        Queue<int> intQueue;

        for (int i = 1; i <= 10; ++i) {
            intQueue.enqueue(i);
            std::cout << "Добавлен: " << i << " (размер: " << intQueue.size() << ")\n";
        }

        std::cout << "\nИзвлечение элементов:\n";
        while (!intQueue.isEmpty()) {
            std::cout << "Извлечен: " << intQueue.dequeue() << " (осталось: " << intQueue.size() << ")\n";
        }

        std::cout << "\nПопытка извлечь из пустой очереди:\n";
        intQueue.dequeue();
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
    }

    return 0;
}