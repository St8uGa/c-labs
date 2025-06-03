#include <iostream>
#include <string>
#include <algorithm>

class Person {
private:
    std::string name;
    int age;
    std::string email;
    std::string address;

public:
    Person() : name(""), age(0), email(""), address("") {}

    std::string getName() const {
        return name;
    }

    int getAge() const {
        return age;
    }

    std::string getEmail() const {
        return email;
    }

    std::string getAddress() const {
        return address;
    }

    void setName(const std::string& newName) {
        if (!newName.empty()) {
            bool isValid = std::all_of(newName.begin(), newName.end(), [](char c) {
                return std::isalpha(c) || std::isspace(c);
                });

            if (isValid) {
                name = newName;
            }
            else {
                std::cerr << "Error: Name can only contain letters and spaces!\n";
            }
        }
        else {
            std::cerr << "Error: Name cannot be empty!\n";
        }
    }

    void setAge(int newAge) {
        if (newAge >= 0 && newAge <= 120) {
            age = newAge;
        }
        else {
            std::cerr << "Error: Age must be between 0 and 120!\n";
        }
    }

    void setEmail(const std::string& newEmail) {
        if (newEmail.find('@') != std::string::npos && newEmail.find('.') != std::string::npos) {
            email = newEmail;
        }
        else {
            std::cerr << "Error: Invalid email format! Must contain '@' and '.'\n";
        }
    }

    void setAddress(const std::string& newAddress) {
        if (!newAddress.empty()) {
            address = newAddress;
        }
        else {
            std::cerr << "Error: Address cannot be empty!\n";
        }
    }

    void displayInfo() const {
        std::cout << "=== Person Information ===\n";
        std::cout << "Name: " << name << "\n";
        std::cout << "Age: " << age << "\n";
        std::cout << "Email: " << email << "\n";
        std::cout << "Address: " << address << "\n";
        std::cout << "=========================\n";
    }
};

int main() {
    Person person;

    person.setName("Yuta Okkotsu");
    person.setAge(18);
    person.setEmail("YutaOkko@example.com");
    person.setAddress("180 Shibuya Tokio");

    std::cout << "Getting data via getters:\n";
    std::cout << "Name: " << person.getName() << "\n";
    std::cout << "Age: " << person.getAge() << "\n";
    std::cout << "Email: " << person.getEmail() << "\n";
    std::cout << "Address: " << person.getAddress() << "\n\n";

    std::cout << "Trying to set invalid data:\n";
    person.setName("lordex123");
    person.setAge(150);
    person.setEmail("invalid-email");
    person.setAddress("");

    std::cout << "\nFinal person info:\n";
    person.displayInfo();

    return 0;
}
