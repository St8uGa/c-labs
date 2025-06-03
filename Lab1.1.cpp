#include <iostream>
#include <string>

class Character {
private:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    Character(std::string n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {
    }

    std::string getName() { return name; }

    void displayStats() {
        std::cout << "\n=== " << name << " ===\n"
            << "Health: " << health << "\n"
            << "Attack: " << attack << "\n"
            << "Defense: " << defense << "\n";
    }

    void attackTarget(Character& target) {
        int damage = attack - target.defense;
        if (damage < 1) damage = 1;

        std::cout << name << " attacks " << target.name
            << " for " << damage << " damage!\n";
        target.takeDamage(damage);
    }

    void takeDamage(int amount) {
        health -= amount;
        if (health < 0) health = 0;
        std::cout << name << " now has " << health << " health.\n";

        if (health == 0) {
            std::cout << name << " has been defeated!\n";
        }
    }

    void heal(int amount) {
        health += amount;
        std::cout << name << " healed for " << amount
            << " HP! Now has " << health << " health.\n";
    }

    bool isAlive() { return health > 0; }
};

void battle(Character& player, Character& enemy) {
    std::cout << "\n=== BATTLE START ===\n";
    std::cout << player.getName() << " vs " << enemy.getName() << "\n";

    while (player.isAlive() && enemy.isAlive()) {
        // ’од игрока

        std::cout << "\n1. Attack\n2. Heal\nChoose action: ";
        int choice;
        std::cin >> choice;

        if (choice == 1) {
            player.attackTarget(enemy);
        }
        else if (choice == 2) {
            player.heal(15);
        }
        else {
            std::cout << "Invalid choice! Skipping turn.\n";
        }

        // ’од врага при условии, что он все ещЄ жив
        if (enemy.isAlive()) {
            enemy.attackTarget(player);
        }
    }

    std::cout << "\n=== BATTLE OVER ===\n";
    if (player.isAlive()) {
        std::cout << player.getName() << " won!\n";
    }
    else {
        std::cout << enemy.getName() << " won!\n";
    }
}

void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main() {
    Character stray("Stray", 100, 15, 5);
    Character kobald("Kobald", 60, 10, 3);

    stray.displayStats();
    kobald.displayStats();

    battle(stray, kobald);

    return 0;
}