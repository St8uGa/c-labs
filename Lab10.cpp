#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <cctype>
#include <map>
#include <set>

using namespace std;

// Базовый класс пользователя
class User {
protected:
    string name;
    int id;
    int accessLevel;

public:
    User(string name, int id, int accessLevel)
        : name(move(name)), id(id), accessLevel(accessLevel) {
        if (this->name.empty()) throw invalid_argument("Имя не может быть пустым");
        if (id < 0) throw invalid_argument("ID не может быть отрицательным");
        if (accessLevel < 0) throw invalid_argument("Уровень доступа не может быть отрицательным");
    }

    virtual ~User() = default;

    string getName() const { return name; }
    int getId() const { return id; }
    int getAccessLevel() const { return accessLevel; }

    void setName(const string& newName) {
        if (newName.empty()) throw invalid_argument("Имя не может быть пустым");
        name = newName;
    }

    void setAccessLevel(int level) {
        if (level < 0) throw invalid_argument("Уровень доступа не может быть отрицательным");
        accessLevel = level;
    }

    virtual void displayInfo() const {
        cout << "ID: " << id
            << "\nИмя: " << name
            << "\nУровень доступа: " << accessLevel << "\n";
    }
};

// Производные классы пользователей
class Student : public User {
    string group;

public:
    Student(string name, int id, int accessLevel, string group)
        : User(move(name), id, accessLevel), group(move(group)) {
    }

    void displayInfo() const override {
        cout << "СТУДЕНТ\n";
        User::displayInfo();
        cout << "Группа: " << group << "\n\n";
    }
};

class Teacher : public User {
    string department;

public:
    Teacher(string name, int id, int accessLevel, string department)
        : User(move(name), id, accessLevel), department(move(department)) {
    }

    void displayInfo() const override {
        cout << "ПРЕПОДАВАТЕЛЬ\n";
        User::displayInfo();
        cout << "Кафедра: " << department << "\n\n";
    }
};

class Administrator : public User {
    string position;

public:
    Administrator(string name, int id, int accessLevel, string position)
        : User(move(name), id, accessLevel), position(move(position)) {
    }

    void displayInfo() const override {
        cout << "АДМИНИСТРАТОР\n";
        User::displayInfo();
        cout << "Должность: " << position << "\n\n";
    }
};

// Класс ресурса с иерархией
class Resource {
    string name;
    int requiredAccessLevel;
    shared_ptr<Resource> parent;

public:
    Resource(string name, int requiredLevel, shared_ptr<Resource> parent = nullptr)
        : name(move(name)), requiredAccessLevel(requiredLevel), parent(parent) {
        if (this->name.empty()) throw invalid_argument("Название ресурса не может быть пустым");
        if (requiredLevel < 0) throw invalid_argument("Уровень доступа не может быть отрицательным");
    }

    string getName() const { return name; }
    int getRequiredAccessLevel() const { return requiredAccessLevel; }
    shared_ptr<Resource> getParent() const { return parent; }

    bool checkAccess(const User& user) const {
        if (user.getAccessLevel() >= requiredAccessLevel) {
            return true;
        }

        // Проверяем доступ к родительским ресурсам
        auto current = parent;
        while (current) {
            if (user.getAccessLevel() >= current->getRequiredAccessLevel()) {
                return true;
            }
            current = current->getParent();
        }

        return false;
    }
};

// Класс роли
class Role {
    string name;
    map<string, int> permissions; // Ресурс -> требуемый уровень доступа

public:
    Role(string name) : name(move(name)) {}

    void addPermission(const string& resourceName, int requiredLevel) {
        permissions[resourceName] = requiredLevel;
    }

    bool hasPermission(const string& resourceName, int userAccessLevel) const {
        auto it = permissions.find(resourceName);
        if (it != permissions.end()) {
            return userAccessLevel >= it->second;
        }
        return false;
    }

    string getName() const { return name; }
};

// Система контроля доступа
class AccessControlSystem {
    vector<unique_ptr<User>> users;
    vector<shared_ptr<Resource>> resources;
    map<string, shared_ptr<Role>> roles;

public:
    void addUser(unique_ptr<User> user) {
        users.push_back(move(user));
    }

    shared_ptr<Resource> addResource(const string& name, int requiredLevel,
        const string& parentName = "") {
        shared_ptr<Resource> parent;
        if (!parentName.empty()) {
            parent = getResource(parentName);
        }

        auto resource = make_shared<Resource>(name, requiredLevel, parent);
        resources.push_back(resource);
        return resource;
    }

    shared_ptr<Role> addRole(const string& name) {
        auto role = make_shared<Role>(name);
        roles[name] = role;
        return role;
    }

    void assignRoleToUser(int userId, const string& roleName) {
        auto user = getUser(userId);
        auto role = getRole(roleName);
        // В реальной системе здесь бы сохраняли связь пользователь-роль
    }

    bool checkAccess(int userId, const string& resourceName) const {
        auto user = getUser(userId);
        auto resource = getResource(resourceName);
        return resource->checkAccess(*user);
    }

    void displayAllUsers() const {
        for (const auto& user : users) {
            user->displayInfo();
        }
    }

    User* getUser(int id) const {
        auto it = find_if(users.begin(), users.end(),
            [id](const auto& user) { return user->getId() == id; });
        return it != users.end() ? it->get() : nullptr;
    }

    shared_ptr<Resource> getResource(const string& name) const {
        auto it = find_if(resources.begin(), resources.end(),
            [&name](const auto& res) { return res->getName() == name; });
        return it != resources.end() ? *it : nullptr;
    }

    shared_ptr<Role> getRole(const string& name) const {
        auto it = roles.find(name);
        return it != roles.end() ? it->second : nullptr;
    }
};

int main() {
    setlocale(LC_CTYPE, "rus");
    AccessControlSystem system;

    try {
        // Создание пользователей
        system.addUser(make_unique<Student>("Андрей Мороз", 1, 1, "T-189"));
        system.addUser(make_unique<Teacher>("Александр Иванов", 2, 3, "Физика"));
        system.addUser(make_unique<Administrator>("Валерия Андреевна", 3, 5, "Директор"));

        // Создание иерархии ресурсов
        auto building = system.addResource("Здание", 1);
        auto lectureHall = system.addResource("Лекционная аудитория", 1, "Здание");
        auto lab = system.addResource("Лаборатория", 3, "Здание");
        auto serverRoom = system.addResource("Серверная", 5, "Лаборатория");

        // Создание ролей
        auto studentRole = system.addRole("Студент");
        studentRole->addPermission("Лекционная аудитория", 1);

        auto teacherRole = system.addRole("Преподаватель");
        teacherRole->addPermission("Лаборатория", 3);

        auto adminRole = system.addRole("Администратор");
        adminRole->addPermission("Серверная", 5);

        // Проверка доступа
        cout << "Доступ студента в серверную: "
            << (system.checkAccess(1, "Серверная") ? "Да" : "Нет") << "\n";
        cout << "Доступ преподавателя в лабораторию: "
            << (system.checkAccess(2, "Лаборатория") ? "Да" : "Нет") << "\n";
        cout << "Доступ администратора в серверную: "
            << (system.checkAccess(3, "Серверная") ? "Да" : "Нет") << "\n";

        // Вывод информации о пользователях
        system.displayAllUsers();

    }
    catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << "\n";
    }

    return 0;
}