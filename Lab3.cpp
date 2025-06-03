#include <iostream>
#include <string>

class Weapon {
private:
    std::string name;
    int damage;

public:
    Weapon(const std::string& n, int d) : name(n), damage(d) {}

    std::string getName() const { return name; }
    int getDamage() const { return damage; }

    Weapon operator+(const Weapon& other) const {
        return Weapon(name + " + " + other.name, damage + other.damage);
    }

    bool operator>(const Weapon& other) const {
        return damage > other.damage;
    }

    friend std::ostream& operator<<(std::ostream& os, const Weapon& w) {
        os << "Weapon: " << w.name << " (Damage: " << w.damage << ")";
        return os;
    }
};

class Character {
private:
    std::string name;
    int health;
    Weapon weapon;

public:
    Character(const std::string& n, int h, const Weapon& w)
        : name(n), health(h), weapon(w) {
    }

    std::string getName() const { return name; }
    int getHealth() const { return health; }
    Weapon getWeapon() const { return weapon; }

    bool isAlive() const { return health > 0; }

    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) health = 0;
    }

    void attack(Character& target) {
        if (!isAlive() || !target.isAlive()) return;

        int damage = weapon.getDamage();
        std::cout << name << " attacks " << target.getName()
            << " with " << weapon.getName() << " for " << damage << " damage!\n";
        target.takeDamage(damage);

        if (!target.isAlive()) {
            std::cout << target.getName() << " has been defeated!\n";
        }
    }

    bool operator==(const Character& other) const {
        return name == other.name && health == other.health;
    }

    friend std::ostream& operator<<(std::ostream& os, const Character& c) {
        os << "Character: " << c.name << " (HP: " << c.health
            << "), Weapon: " << c.weapon.getName();
        return os;
    }
};

int main() {
    Weapon sword("Sword", 15);
    Weapon axe("Axe", 20);
    Weapon magicStaff("Magic Staff", 25);

    Weapon superWeapon = sword + axe;
    std::cout << "Combined weapon: " << superWeapon << "\n";

    if (axe > sword) {
        std::cout << axe.getName() << " is stronger than " << sword.getName() << "!\n";
    }

    Character hero("Hero", 100, sword);
    Character enemy("Goblin", 50, axe);
    Character mage("Mage", 80, magicStaff);

    std::cout << "\nCharacters:\n";
    std::cout << hero << "\n";
    std::cout << enemy << "\n";
    std::cout << mage << "\n\n";

    if (hero == Character("Hero", 100, sword)) {
        std::cout << "This is our hero!\n\n";
    }

    std::cout << "Battle starts!\n";
    while (hero.isAlive() && enemy.isAlive()) {
        hero.attack(enemy);
        if (enemy.isAlive()) {
            enemy.attack(hero);
        }
        std::cout << "\n";
    }

    std::cout << "\nBattle result:\n";
    std::cout << hero << "\n";
    std::cout << enemy << "\n";

    return 0;
}
