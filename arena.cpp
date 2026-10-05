#include <iostream>
#include <memory>
#include <string>

class Weapon {

private:
    std::string name;
    int damage;

public:

    Weapon(const std::string& n, int d) : name(n), damage(d) {

        std::cout << "Weapon - " << name << " created - " << damage << std::endl;
    }

    ~Weapon() {

        std::cout << "Weapon - " << name << " destroyed" << std::endl;
    }

    int get_damage() const {

        return damage;
    }

    std::string get_name() const {

        return name;
    }

};

class Buff {

private:

    std::string name;
    int duration;

public:

    Buff(const std::string& n, int d) : name(n), duration(d) {

        std::cout << "Buff - " << name << " created - duration: " << duration << std::endl;
    }

    ~Buff() {

        std::cout << "Buff - " << name << " expired and destroyed" << std::endl;
    }

    std::string get_name() const {

        return name;
    }

    int get_duration() const {

        return duration;
    }
};

class Character {

private:

    std::string name;
    int hp;

    std::unique_ptr<Weapon> weapon;
    std::shared_ptr<Buff> active_buff;
    std::weak_ptr<Character> target;

public:

    Character(const std::string& n, int h) : name(n), hp(h) {

        std::cout << "Character - " << name << " spawned with " << hp << " HP" << std::endl;
    }

    ~Character() {

        std::cout << "Character - " << name << " died from arena" << std::endl;
    }

    void equip_weapon(std::unique_ptr<Weapon> w) {

        weapon = std::move(w);
    }

    void apply_buff(std::shared_ptr<Buff> b) {

        active_buff = b;
    }

    void set_target(std::weak_ptr<Character> t) {

        target = t;
    }

    void attack() {

        std::shared_ptr<Character> target_sp = target.lock();

        if (target_sp) {

            int damage = 5;
            std::string weapon_name = "fists";

            if (weapon) {

                damage = weapon->get_damage();
                weapon_name = weapon->get_name();
            }

            std::cout << name << " attack " << target_sp->get_name() << " with " << weapon_name << " for " << damage << " dmg!" << std::endl;

        } else {

            std::cout << name << " has no valid target!" << std::endl;
        }
    }

    std::string get_name() const {

        return name;
    }

};

int main() {

    std::cout << "=== ARENA INITIALIZATION ===" << std::endl;

    std::shared_ptr<Character> hero(new Character("Lakaka", 100));
    std::shared_ptr<Character> monster(new Character("Burger", 50));

    std::unique_ptr<Weapon> sword(new Weapon("sword", 35));
    hero->equip_weapon(std::move(sword));

    std::shared_ptr<Buff> shield(new Buff("shield", 5));
    hero->apply_buff(shield);

    hero->set_target(monster);

    std::cout << "\n=== COMBAT BEGINS ===" << std::endl;
    hero->attack();

    std::cout << "\n=== MONSTER DIES (RAM Cleanup) ===" << std::endl;
    monster.reset();

    std::cout << "\n=== ATTACK AFTER TARGET DEATH ===" << std::endl;
    hero->attack();


    return 0;
}
