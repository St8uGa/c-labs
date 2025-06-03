#include <iostream>
#include <string>

// Базовый класс для всех игровых сущностей
class GameEntity {
protected:
    std::string name;
    int health;
    int level;

public:
    GameEntity(const std::string& entityName, int hp, int lvl)
        : name(entityName), health(hp), level(lvl) {
    }

    virtual void displayStats() const {
        std::cout << "Name: " << name
            << "\nLevel: " << level
            << "\nHealth: " << health << std::endl;
    }

    virtual void specialAction() const {
        std::cout << name << " performs a generic action!" << std::endl;
    }

    virtual ~GameEntity() {}
};

// Класс игрока
class Player : public GameEntity {
private:
    int experience;
    std::string characterClass;

public:
    Player(const std::string& pName, int hp, int lvl, int exp, const std::string& cls)
        : GameEntity(pName, hp, lvl), experience(exp), characterClass(cls) {
    }

    void displayStats() const override {
        GameEntity::displayStats();
        std::cout << "Class: " << characterClass
            << "\nExperience: " << experience << "/100" << std::endl;
    }

    void specialAction() const override {
        std::cout << name << " uses " << characterClass << " technique!" << std::endl;
    }

    void levelUp() {
        level++;
        health += 20;
        experience = 0;
        std::cout << name << " leveled up to " << level << "!" << std::endl;
    }
};

// Класс врага
class Enemy : public GameEntity {
protected:
    std::string type;
    int damage;

public:
    Enemy(const std::string& eName, int hp, int lvl, const std::string& enemyType, int dmg)
        : GameEntity(eName, hp, lvl), type(enemyType), damage(dmg) {
    }

    void displayStats() const override {
        GameEntity::displayStats();
        std::cout << "Type: " << type
            << "\nDamage: " << damage << std::endl;
    }

    void specialAction() const override {
        std::cout << name << " unleashes " << type << " assault!" << std::endl;
    }
};

// Босс
class Boss : public Enemy {
private:
    std::string specialAbility;

public:
    Boss(const std::string& bName, int hp, int lvl, const std::string& bossType,
        int dmg, const std::string& ability)
        : Enemy(bName, hp, lvl, bossType, dmg), specialAbility(ability) {
    }

    void displayStats() const override {
        Enemy::displayStats();
        std::cout << "Special Ability: " << specialAbility << std::endl;
    }

    void specialAction() const override {
        std::cout << name << " executes devastating " << specialAbility << "!" << std::endl;
    }
};

int main() {
    // Создание объектов с новыми именами
    Player wanderer("Stray", 120, 3, 45, "Blade Dancer");
    Enemy creature("Vovapin", 65, 2, "Cursed Beast", 12);
    Boss bladeMaster("Malenia", 350, 15, "Blade of Miquella", 30, "Waterfowl Dance");

    // Демонстрация полиморфизма
    std::cout << "=== Wanderer's Info ===" << std::endl;
    wanderer.displayStats();
    wanderer.specialAction();

    std::cout << "\n=== Creature's Info ===" << std::endl;
    creature.displayStats();
    creature.specialAction();

    std::cout << "\n=== Blade Master's Info ===" << std::endl;
    bladeMaster.displayStats();
    bladeMaster.specialAction();

    // Игрок получает опыт и уровень
    std::cout << "\n=== Progression ===" << std::endl;
    wanderer.levelUp();
    wanderer.displayStats();

    return 0;
}