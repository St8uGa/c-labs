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

// ������� ����� ������������
class User {
protected:
    string name;
    int id;
    int accessLevel;

public:
    User(string name, int id, int accessLevel)
        : name(move(name)), id(id), accessLevel(accessLevel) {
        if (this->name.empty()) throw invalid_argument("��� �� ����� ���� ������");
        if (id < 0) throw invalid_argument("ID �� ����� ���� �������������");
        if (accessLevel < 0) throw invalid_argument("������� ������� �� ����� ���� �������������");
    }

    virtual ~User() = default;

    string getName() const { return name; }
    int getId() const { return id; }
    int getAccessLevel() const { return accessLevel; }

    void setName(const string& newName) {
        if (newName.empty()) throw invalid_argument("��� �� ����� ���� ������");
        name = newName;
    }

    void setAccessLevel(int level) {
        if (level < 0) throw invalid_argument("������� ������� �� ����� ���� �������������");
        accessLevel = level;
    }

    virtual void displayInfo() const {
        cout << "ID: " << id
            << "\n���: " << name
            << "\n������� �������: " << accessLevel << "\n";
    }
};

// ����������� ������ �������������
class Student : public User {
    string group;

public:
    Student(string name, int id, int accessLevel, string group)
        : User(move(name), id, accessLevel), group(move(group)) {
    }

    void displayInfo() const override {
        cout << "�������\n";
        User::displayInfo();
        cout << "������: " << group << "\n\n";
    }
};

class Teacher : public User {
    string department;

public:
    Teacher(string name, int id, int accessLevel, string department)
        : User(move(name), id, accessLevel), department(move(department)) {
    }

    void displayInfo() const override {
        cout << "�������������\n";
        User::displayInfo();
        cout << "�������: " << department << "\n\n";
    }
};

class Administrator : public User {
    string position;

public:
    Administrator(string name, int id, int accessLevel, string position)
        : User(move(name), id, accessLevel), position(move(position)) {
    }

    void displayInfo() const override {
        cout << "�������������\n";
        User::displayInfo();
        cout << "���������: " << position << "\n\n";
    }
};

// ����� ������� � ���������
class Resource {
    string name;
    int requiredAccessLevel;
    shared_ptr<Resource> parent;

public:
    Resource(string name, int requiredLevel, shared_ptr<Resource> parent = nullptr)
        : name(move(name)), requiredAccessLevel(requiredLevel), parent(parent) {
        if (this->name.empty()) throw invalid_argument("�������� ������� �� ����� ���� ������");
        if (requiredLevel < 0) throw invalid_argument("������� ������� �� ����� ���� �������������");
    }

    string getName() const { return name; }
    int getRequiredAccessLevel() const { return requiredAccessLevel; }
    shared_ptr<Resource> getParent() const { return parent; }

    bool checkAccess(const User& user) const {
        if (user.getAccessLevel() >= requiredAccessLevel) {
            return true;
        }

        // ��������� ������ � ������������ ��������
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

// ����� ����
class Role {
    string name;
    map<string, int> permissions; // ������ -> ��������� ������� �������

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

// ������� �������� �������
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
        // � �������� ������� ����� �� ��������� ����� ������������-����
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
        // �������� �������������
        system.addUser(make_unique<Student>("������ �����", 1, 1, "T-189"));
        system.addUser(make_unique<Teacher>("��������� ������", 2, 3, "������"));
        system.addUser(make_unique<Administrator>("������� ���������", 3, 5, "��������"));

        // �������� �������� ��������
        auto building = system.addResource("������", 1);
        auto lectureHall = system.addResource("���������� ���������", 1, "������");
        auto lab = system.addResource("�����������", 3, "������");
        auto serverRoom = system.addResource("���������", 5, "�����������");

        // �������� �����
        auto studentRole = system.addRole("�������");
        studentRole->addPermission("���������� ���������", 1);

        auto teacherRole = system.addRole("�������������");
        teacherRole->addPermission("�����������", 3);

        auto adminRole = system.addRole("�������������");
        adminRole->addPermission("���������", 5);

        // �������� �������
        cout << "������ �������� � ���������: "
            << (system.checkAccess(1, "���������") ? "��" : "���") << "\n";
        cout << "������ ������������� � �����������: "
            << (system.checkAccess(2, "�����������") ? "��" : "���") << "\n";
        cout << "������ �������������� � ���������: "
            << (system.checkAccess(3, "���������") ? "��" : "���") << "\n";

        // ����� ���������� � �������������
        system.displayAllUsers();

    }
    catch (const exception& e) {
        cerr << "������: " << e.what() << "\n";
    }

    return 0;
}