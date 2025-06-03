#include <iostream>
#include <string>

// Базовый класс для всех персонажей
class Character {
protected:
    std::string name;
    int health;
    int attackPower;

public:
    Character(const std::string& name, int health, int attackPower)
        : name(name), health(health), attackPower(attackPower) {
    }

    // Метод для атаки 
    virtual void attack(Character& target) {
        std::cout << name << " attack " << target.getName() << " get " << attackPower << " damage\n";
        target.takeDamage(attackPower);
    }

    // Метод для получения урона
    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) health = 0;
        std::cout << name << " get " << damage << " damage. health: " << health << "\n";

        if (health == 0) {
            std::cout << name << " defeated!\n";
        }
    }

    // Геттер для имени
    std::string getName() const {
        return name;
    }

    // Геттер для здоровья
    int getHealth() const {
        return health;
    }

    // Метод для специальной способности
    virtual void useSpecialAbility() {
        std::cout << name << " use base ability!\n";
    }

    // Деструктор
    virtual ~Character() {
        std::cout << name << " leave game.\n";
    }
};

// Класс для персонажа Stray
class Stray : public Character {
private:
    int stealthLevel;

public:
    Stray() : Character("Stray", 100, 15), stealthLevel(50) {}

    // Переопределение метода атаки с учетом stealth
    void attack(Character& target) override {
        int totalDamage = attackPower + (stealthLevel / 10);
        std::cout << name << " attack " << target.getName()
            << ", get " << totalDamage << " damage\n";
        target.takeDamage(totalDamage);
    }

    // Специальная способность Stray
    void useSpecialAbility() override {
        stealthLevel += 20;
        std::cout << name << " activate special ability Stealth! Lvele of stealth increased " << stealthLevel << "\n";
    }
};

// Класс для персонажа Vovapain
class Vovapain : public Character {
private:
    int rageLevel;

public:
    Vovapain() : Character("Vovapain", 150, 20), rageLevel(0) {}

    // Переопределение метода атаки с учетом ярости
    void attack(Character& target) override {
        int totalDamage = attackPower + (rageLevel / 5);
        std::cout << name << " attack " << target.getName()
            << " get " << totalDamage << " damage\n";
        target.takeDamage(totalDamage);
        rageLevel += 10;
    }

    // Специальная способность Vovapain
    void useSpecialAbility() override {
        health += 30;
        rageLevel = 100;
        std::cout << name << " heatlh increased, override get maxed\n";
    }
};

int main() {
    // Создаем персонажей
    Stray stray;
    Vovapain vovapain;

    std::cout << "=== start battle ===\n";
    std::cout << stray.getName() << " vs " << vovapain.getName() << "\n\n";

    // Демонстрация боя
    stray.attack(vovapain);
    vovapain.attack(stray);

    std::cout << "\n=== using ability ===\n";
    stray.useSpecialAbility();
    vovapain.useSpecialAbility();

    std::cout << "\n=== continuation of the battle ===\n";
    stray.attack(vovapain);
    vovapain.attack(stray);
    stray.attack(vovapain);

    std::cout << "\n=== finalе ===\n";
    std::cout << stray.getName() << ": health " << stray.getHealth() << "\n";
    std::cout << vovapain.getName() << ": heatlth " << vovapain.getHealth() << "\n";

    return 0;
}

