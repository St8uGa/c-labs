#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <string>
#include <stdexcept>

// Базовый класс для всех игровых сущностей
class Entity {
public:
    virtual ~Entity() = default;

    virtual std::string getType() const = 0;
    virtual std::string getName() const = 0;
    virtual int getHealth() const = 0;
    virtual int getLevel() const = 0;

    virtual void display() const {
        std::cout << getType() << ": " << getName()
            << " (HP: " << getHealth()
            << ", LVL: " << getLevel() << ")\n";
    }

    virtual std::string serialize() const {
        return getType() + "," + getName() + ","
            + std::to_string(getHealth()) + ","
            + std::to_string(getLevel());
    }

    virtual void deserialize(const std::vector<std::string>& parts) {
        if (parts.size() != 4) {
            throw std::runtime_error("Invalid data format for Entity");
        }
    }
};

// Класс игрока
class Player : public Entity {
public:
    Player(const std::string& name = "Unknown", int health = 100, int level = 1)
        : name_(name), health_(health), level_(level) {
    }

    std::string getType() const override { return "Player"; }
    std::string getName() const override { return name_; }
    int getHealth() const override { return health_; }
    int getLevel() const override { return level_; }

    void deserialize(const std::vector<std::string>& parts) override {
        if (parts.size() != 4 || parts[0] != "Player") {
            throw std::runtime_error("Invalid data format for Player");
        }
        name_ = parts[1];
        health_ = std::stoi(parts[2]);
        level_ = std::stoi(parts[3]);
    }

private:
    std::string name_;
    int health_;
    int level_;
};

// Класс врага
class Enemy : public Entity {
public:
    Enemy(const std::string& name = "Enemy", int health = 50, int level = 1,
        const std::string& type = "Goblin")
        : name_(name), health_(health), level_(level), enemyType_(type) {
    }

    std::string getType() const override { return "Enemy:" + enemyType_; }
    std::string getName() const override { return name_; }
    int getHealth() const override { return health_; }
    int getLevel() const override { return level_; }

    std::string serialize() const override {
        return Entity::serialize() + "," + enemyType_;
    }

    void deserialize(const std::vector<std::string>& parts) override {
        if (parts.size() != 5 || parts[0] != "Enemy") {
            throw std::runtime_error("Invalid data format for Enemy");
        }
        name_ = parts[1];
        health_ = std::stoi(parts[2]);
        level_ = std::stoi(parts[3]);
        enemyType_ = parts[4];
    }

private:
    std::string name_;
    int health_;
    int level_;
    std::string enemyType_;
};

// Менеджер игровых сущностей
class EntityManager {
public:
    void addEntity(std::unique_ptr<Entity> entity) {
        entities_.push_back(std::move(entity));
    }

    void displayAll() const {
        std::cout << "=== Game Entities ===\n";
        for (const auto& entity : entities_) {
            entity->display();
        }
        std::cout << "=====================\n";
    }

    void saveToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file) {
            throw std::runtime_error("Cannot open file for writing: " + filename);
        }

        for (const auto& entity : entities_) {
            file << entity->serialize() << "\n";
        }

        std::cout << "Saved " << entities_.size() << " entities to " << filename << "\n";
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            throw std::runtime_error("Cannot open file for reading: " + filename);
        }

        entities_.clear();

        std::string line;
        while (std::getline(file, line)) {
            std::vector<std::string> parts;
            size_t start = 0, end;

            while ((end = line.find(',', start)) != std::string::npos) {
                parts.push_back(line.substr(start, end - start));
                start = end + 1;
            }
            parts.push_back(line.substr(start));

            std::unique_ptr<Entity> entity;
            if (parts[0] == "Player") {
                entity = std::make_unique<Player>();
            }
            else if (parts[0] == "Enemy") {
                entity = std::make_unique<Enemy>();
            }
            else {
                std::cerr << "Unknown entity type: " << parts[0] << "\n";
                continue;
            }

            try {
                entity->deserialize(parts);
                entities_.push_back(std::move(entity));
            }
            catch (const std::exception& e) {
                std::cerr << "Error loading entity: " << e.what() << "\n";
            }
        }

        std::cout << "Loaded " << entities_.size() << " entities from " << filename << "\n";
    }

private:
    std::vector<std::unique_ptr<Entity>> entities_;
};

int main() {
    EntityManager manager;

    // Измененные имена персонажей
    manager.addEntity(std::make_unique<Player>("Stray", 150, 5));
    manager.addEntity(std::make_unique<Player>("Yuta", 80, 7));
    manager.addEntity(std::make_unique<Enemy>("Orc", 120, 3, "Orc"));
    manager.addEntity(std::make_unique<Enemy>("Gransax", 300, 10, "Dragon"));

    manager.displayAll();

    try {
        manager.saveToFile("game_save.txt");
    }
    catch (const std::exception& e) {
        std::cerr << "Save error: " << e.what() << "\n";
    }

    EntityManager loadedManager;
    try {
        loadedManager.loadFromFile("game_save.txt");
        loadedManager.displayAll();
    }
    catch (const std::exception& e) {
        std::cerr << "Load error: " << e.what() << "\n";
    }

    return 0;
}