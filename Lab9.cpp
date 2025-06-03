#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <ctime>
#include <limits>

using namespace std;

// Базовый класс для всех существ
class Entity {
protected:
    string name;
    int health;
    int maxHealth;
    int attack;
    int defense;
    int agility; // Шанс уклониться от атаки

public:
    Entity(string n, int h, int a, int d, int agi)
        : name(n), health(h), maxHealth(h), attack(a), defense(d), agility(agi) {
    }

    virtual ~Entity() = default;

    bool isAlive() const { return health > 0; }

    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) health = 0;
    }

    void heal(int amount) {
        health += amount;
        if (health > maxHealth) health = maxHealth;
    }

    virtual void attackTarget(Entity& target) {
        // Проверка на уклонение
        if (rand() % 100 < target.agility) {
            cout << target.name << " dodged the attack!\n";
            return;
        }

        int damage = attack - target.defense;
        if (damage < 1) damage = 1; // Минимальный урон

        target.takeDamage(damage);
        cout << name << " hits " << target.name << " for " << damage << " damage!\n";
    }

    void displayStatus() const {
        cout << name << " [HP: " << health << "/" << maxHealth
            << " | ATK: " << attack
            << " | DEF: " << defense
            << " | AGI: " << agility << "]\n";
    }

    string getName() const { return name; }
    int getHealth() const { return health; }
};

// Класс игрока
class Player : public Entity {
private:
    int experience;
    int level;
    vector<string> inventory;

public:
    Player(string n) : Entity(n, 100, 15, 10, 20), experience(0), level(1) {
        inventory.push_back("Health Potion");
        inventory.push_back("Health Potion");
    }

    void levelUp() {
        level++;
        maxHealth += 20;
        health = maxHealth;
        attack += 5;
        defense += 3;
        agility += 2;
        cout << "Level up! Now you're level " << level << "!\n";
    }

    void gainExperience(int exp) {
        experience += exp;
        if (experience >= level * 50) {
            experience -= level * 50;
            levelUp();
        }
    }

    void usePotion() {
        for (auto it = inventory.begin(); it != inventory.end(); ++it) {
            if (*it == "Health Potion") {
                heal(30);
                inventory.erase(it);
                cout << "Used Health Potion. Healed 30 HP.\n";
                return;
            }
        }
        cout << "No Health Potions left!\n";
    }

    void addToInventory(string item) {
        inventory.push_back(item);
    }

    void showInventory() {
        cout << "Inventory: ";
        for (const auto& item : inventory) {
            cout << item << ", ";
        }
        cout << "\n";
    }
};

// Классы монстров
class Stray : public Entity {
public:
    Stray() : Entity("Stray", 60, 12, 5, 15) {}

    void attackTarget(Entity& target) override {
        // 20% шанс на критический удар
        if (rand() % 100 < 20) {
            int critDamage = (attack * 2) - target.getHealth();
            cout << "Stray lands a critical hit!\n";
            target.takeDamage(critDamage);
        }
        else {
            Entity::attackTarget(target);
        }
    }
};

class Kobold : public Entity {
public:
    Kobold() : Entity("Kobold", 45, 18, 3, 25) {}

    void attackTarget(Entity& target) override {
        // Кобольды атакуют дважды с 30% вероятностью
        Entity::attackTarget(target);
        if (rand() % 100 < 30) {
            cout << "Kobold attacks again!\n";
            Entity::attackTarget(target);
        }
    }
};

// Функция для очистки ввода
void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Главная игровая функция
void gameLoop() {
    srand(time(0));
    string playerName;
    cout << "Enter your character's name: ";
    getline(cin, playerName);

    Player player(playerName);
    vector<unique_ptr<Entity>> enemies;
    enemies.push_back(make_unique<Stray>());
    enemies.push_back(make_unique<Kobold>());
    enemies.push_back(make_unique<Stray>());

    cout << "\n=== Welcome to Simple RPG ===\n";
    cout << "You'll face " << enemies.size() << " enemies on your journey!\n\n";

    for (auto& enemy : enemies) {
        while (player.isAlive() && enemy->isAlive()) {
            cout << "\n--------------------------------\n";
            player.displayStatus();
            enemy->displayStatus();
            cout << "\n1. Attack\n2. Use Health Potion\n3. Show Inventory\n> ";

            int choice;
            cin >> choice;
            clearInput();

            switch (choice) {
            case 1:
                player.attackTarget(*enemy);
                break;
            case 2:
                player.usePotion();
                break;
            case 3:
                player.showInventory();
                continue;
            default:
                cout << "Invalid choice!\n";
                continue;
            }

            if (enemy->isAlive()) {
                enemy->attackTarget(player);
            }
            else {
                int expReward = enemy->getName() == "Stray" ? 35 : 25;
                cout << "You defeated " << enemy->getName() << " and gained "
                    << expReward << " experience!\n";
                player.gainExperience(expReward);

                // 50% шанс получить зелье за победу
                if (rand() % 100 < 50) {
                    player.addToInventory("Health Potion");
                    cout << "Found a Health Potion!\n";
                }
            }
        }

        if (!player.isAlive()) {
            cout << "Game Over! You were defeated by " << enemy->getName() << ".\n";
            return;
        }
    }

    cout << "\nCongratulations! You defeated all enemies!\n";
    cout << "Final stats:\n";
    player.displayStatus();
}

int main() {
    gameLoop();
    cout << "\nThanks for playing!\n";
    return 0;
}