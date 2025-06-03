#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

class Inventory {
private:
    std::vector<std::unique_ptr<std::string>> items;
    size_t capacity;
    size_t count;

public:
    explicit Inventory(size_t cap = 10) : capacity(cap), count(0) {}

    void addItem(const std::string& item) {
        if (count >= capacity) {
            throw std::runtime_error("Inventory is full!");
        }
        items.push_back(std::make_unique<std::string>(item));
        count++;
    }

    void removeItem(size_t index) {
        if (index >= items.size()) {
            throw std::out_of_range("Invalid item index!");
        }
        items.erase(items.begin() + index);
        count--;
    }

    void useItem(size_t index) {
        if (index >= items.size()) {
            throw std::out_of_range("Invalid item index!");
        }
        std::cout << "Using item: " << *items[index] << std::endl;
        removeItem(index);
    }

    void display() const {
        std::cout << "Inventory (" << count << "/" << capacity << "):\n";
        if (items.empty()) {
            std::cout << "  (empty)\n";
        }
        else {
            for (size_t i = 0; i < items.size(); ++i) {
                std::cout << "  " << i + 1 << ". " << *items[i] << "\n";
            }
        }
    }

    bool isFull() const { return count >= capacity; }

    size_t getCount() const { return count; }

    size_t getCapacity() const { return capacity; }

    void setCapacity(size_t newCapacity) {
        if (newCapacity < count) {
            throw std::runtime_error("New capacity is less than current item count!");
        }
        capacity = newCapacity;
    }
};

int main() {
    try {
        Inventory inv(5); 

        inv.addItem("Sword");
        inv.addItem("Health Potion");
        inv.addItem("Shield");
        inv.addItem("Magic Ring");

        inv.display();

        inv.useItem(1); 

        inv.display();

        inv.addItem("Bow");
        inv.addItem("Arrow");

        if (inv.isFull()) {
            std::cout << "Inventory is now full!\n";
        }

        inv.display();

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}