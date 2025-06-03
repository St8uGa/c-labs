#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

class Weapon {
private:
    string name;
    int damage;
    int criticalChance;
public:
    Weapon(string name, int damage, int criticalChance)
        : name(name), damage(damage), criticalChance(criticalChance) {
    }

    string getName() const { return name; }
    int getDamage() const { return damage; }
    int getCriticalChance() const { return criticalChance; }

    int attack() const {
        int roll = rand() % 100;
        if (roll < criticalChance) {
            cout << "Critical hit! ";
            return damage * 2;
        }
        return damage;
    }
};

class Character {
private:
    string name;
    int health;
    int maxHealth;
    Weapon* weapon;
public:
    Character(string name, int health, Weapon* weapon)
        : name(name), health(health), maxHealth(health), weapon(weapon) {
    }

    string getName() const { return name; }
    int getHealth() const { return health; }
    Weapon* getWeapon() const { return weapon; }

    bool isAlive() const { return health > 0; }

    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) health = 0;
    }

    void heal(int amount) {
        health += amount;
        if (health > maxHealth) health = maxHealth;
    }

    void attack(Character& target) {
        if (!isAlive() || !target.isAlive()) return;

        int damage = weapon->attack();
        cout << name << " attacks " << target.getName() << " with "
            << weapon->getName() << " and deals " << damage << " damage!" << endl;

        target.takeDamage(damage);

        if (!target.isAlive()) {
            cout << target.getName() << " has been defeated!" << endl;
        }
    }

    void showStatus() const {
        cout << name << ": Health " << health << "/" << maxHealth
            << ", Weapon: " << weapon->getName() << endl;
    }
};

class Battle {
private:
    Character& player;
    Character& enemy;
public:
    Battle(Character& player, Character& enemy) : player(player), enemy(enemy) {}

    void start() {
        cout << "Battle between " << player.getName() << " and " << enemy.getName() << " begins!\n";

        while (player.isAlive() && enemy.isAlive()) {
            player.attack(enemy);
            if (!enemy.isAlive()) break;

            enemy.attack(player);
            if (!player.isAlive()) break;

            player.showStatus();
            enemy.showStatus();
            cout << "-----------------------\n";
        }

        if (player.isAlive()) {
            cout << player.getName() << " wins!" << endl;
        }
        else {
            cout << enemy.getName() << " wins!" << endl;
        }
    }
};

int main() {
    srand(time(0));

    Weapon sword("Sword", 15, 20);
    Weapon axe("Axe", 20, 10);
    Weapon dagger("Dagger", 10, 30);

    Character stray("Stray", 100, &sword);
    Character roshan("Roshan", 80, &axe);

    stray.showStatus();
    roshan.showStatus();
    cout << "-----------------------\n";

    Battle battle(stray, roshan);
    battle.start();

    return 0;
}
