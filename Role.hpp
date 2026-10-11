#ifndef ROLE_HPP
#define ROLE_HPP

#include <string>
#include <iostream>

namespace final {

class Fighter;
class Protector;
class Caster;

/**
 * Base class for all roles. It cannot be instantiated directly.
 * Only the three derived classes are friends and may access its private members.
 */
class Base {
    friend class Fighter;
    friend class Protector;
    friend class Caster;

protected:
    std::string name;
    int health;
    int attack;
    bool alive;
    bool defensive;
    // total number of alive objects of any derived type
    static int aliveNumber;

    // helper to output kill message (accessible to derived classes)
    void outputKill(const std::string& role) const {
        std::cout << role << " " << name << " is killed\n";
    }

public:
    // constructor used by derived classes
    Base(const std::string& n, int h, int a)
        : name(n), health(h), attack(a), alive(true), defensive(false) {
        ++aliveNumber;
    }

    // virtual destructor ensures proper cleanup and alive count update
    virtual ~Base() {
        if (alive) {
            --aliveNumber;
        }
    }

    // static accessor for alive count
    static int getAliveNumber() { return aliveNumber; }

    // pure virtual interface required by the problem statement
    virtual void launch_attack(Base* target) = 0;
    virtual void defend(bool flag) = 0;

    // role name for output – each derived class implements this
    virtual std::string roleName() const = 0;

    // simple state accessors (already provided in the original skeleton)
    bool isAlive() { return alive; }
    bool isDefensive() { return defensive; }
};

// initialise static member
int Base::aliveNumber = 0;

/**
 * Fighter implementation
 */
class Fighter : public Base {
public:
    Fighter(const std::string& n, int h, int a) : Base(n, h, a) {}
    std::string roleName() const override { return "Fighter"; }
    void launch_attack(Base* target) override;
    void defend(bool flag) override;
};

/**
 * Protector implementation
 */
class Protector : public Base {
public:
    Protector(const std::string& n, int h, int a) : Base(n, h, a) {}
    std::string roleName() const override { return "Protector"; }
    void launch_attack(Base* target) override;
    void defend(bool flag) override;
};

/**
 * Caster implementation
 */
class Caster : public Base {
public:
    Caster(const std::string& n, int h, int a) : Base(n, h, a) {}
    std::string roleName() const override { return "Caster"; }
    void launch_attack(Base* target) override;
    void defend(bool flag) override;
};

// ---------- Implementation of member functions ----------

// Fighter implementations
inline void final::Fighter::launch_attack(Base* target) {
    int factor = (dynamic_cast<final::Caster*>(target) != nullptr) ? 2 : 1;
    if (!target->defensive) {
        target->health -= this->attack * factor;
    } else {
        int dmg = this->attack * factor - target->attack;
        if (dmg > 0) target->health -= dmg;
    }
    // recoil (Fighter is not a Caster, so suffers full recoil)
    this->health -= target->attack;

    // target death first
    if (target->alive && target->health <= 0) {
        target->alive = false;
        --aliveNumber;
        target->outputKill(target->roleName());
    }
    // attacker death after
    if (this->alive && this->health <= 0) {
        this->alive = false;
        --aliveNumber;
        this->outputKill(this->roleName());
    }
}

inline void final::Fighter::defend(bool flag) {
    defensive = flag;
    if (flag) {
        health += 3;
        attack += 2;
    }
}

// Protector implementations
inline void final::Protector::launch_attack(Base* target) {
    int factor = (dynamic_cast<final::Fighter*>(target) != nullptr) ? 2 : 1;
    if (!target->defensive) {
        target->health -= this->attack * factor;
    } else {
        int dmg = this->attack * factor - target->attack;
        if (dmg > 0) target->health -= dmg;
    }
    // recoil (Protector is not a Caster)
    this->health -= target->attack;

    if (target->alive && target->health <= 0) {
        target->alive = false;
        --aliveNumber;
        target->outputKill(target->roleName());
    }
    if (this->alive && this->health <= 0) {
        this->alive = false;
        --aliveNumber;
        this->outputKill(this->roleName());
    }
}

inline void final::Protector::defend(bool flag) {
    defensive = flag;
    if (flag) {
        health += 7;
        attack += 1;
    }
}

// Caster implementations
inline void final::Caster::launch_attack(Base* target) {
    int factor = (dynamic_cast<final::Protector*>(target) != nullptr) ? 2 : 1;
    if (!target->defensive) {
        target->health -= this->attack * factor;
    } else {
        int dmg = this->attack * factor - target->attack;
        if (dmg > 0) target->health -= dmg;
    }
    // Caster receives no recoil
    if (target->alive && target->health <= 0) {
        target->alive = false;
        --aliveNumber;
        target->outputKill(target->roleName());
    }
    // attacker (Caster) does not suffer recoil, so no further death handling needed here
}

inline void final::Caster::defend(bool flag) {
    defensive = flag;
    if (flag) {
        health += 5;
    }
}

} // namespace final

#endif // ROLE_HPP
