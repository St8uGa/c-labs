#include <iostream>
#include <thread>
#include <mutex>
#include <random>
#include <chrono>

class Creature {
protected:
    std::string name;
    int health;
    int damage;
    std::mutex mtx;

public:
    Creature(const std::string& name, int health, int damage)
        : name(name), health(health), damage(damage) {
    }

    bool isAlive() const {
        return health > 0;
    }

    void takeDamage(int amount) {
        std::lock_guard<std::mutex> lock(mtx); // Защищаем доступ к health
        health -= amount;
        if (health < 0) health = 0;
    }

    int getHealth() const {
        return health;
    }

    std::string getName() const {
        return name;
    }

    virtual void attack(Creature& target) = 0;
};

class Hero : public Creature {
public:
    Hero() : Creature("Hero", 100, 15) {}

    void attack(Creature& target) override {
        if (!isAlive() || !target.isAlive()) return;

        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Задержка между атаками
        target.takeDamage(damage);
        std::cout << name << " hits " << target.getName() << " for " << damage << " damage. ";
        std::cout << target.getName() << " health: " << target.getHealth() << std::endl;
    }
};

class Monster : public Creature {
public:
    Monster() : Creature("Monster", 80, 10) {}

    void attack(Creature& target) override {
        if (!isAlive() || !target.isAlive()) return;

        std::this_thread::sleep_for(std::chrono::milliseconds(700)); // Монстр атакует реже
        target.takeDamage(damage);
        std::cout << name << " hits " << target.getName() << " for " << damage << " damage. ";
        std::cout << target.getName() << " health: " << target.getHealth() << std::endl;
    }
};

void fight(Creature& attacker, Creature& defender) {
    while (attacker.isAlive() && defender.isAlive()) {
        attacker.attack(defender);
    }
}

int main() {
    Hero hero;
    Monster monster;

    std::cout << "=== Battle Start ===" << std::endl;
    std::cout << hero.getName() << " (HP: " << hero.getHealth() << ") vs ";
    std::cout << monster.getName() << " (HP: " << monster.getHealth() << ")" << std::endl;

    // Запускаем бой в двух потоках
    std::thread heroThread(fight, std::ref(hero), std::ref(monster));
    std::thread monsterThread(fight, std::ref(monster), std::ref(hero));

    heroThread.join();
    monsterThread.join();

    // Определяем победителя
    if (hero.isAlive()) {
        std::cout << hero.getName() << " wins the battle!" << std::endl;
    }
    else {
        std::cout << monster.getName() << " wins the battle!" << std::endl;
    }

    return 0;
}