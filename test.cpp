#include <iostream>
#include <string>
#include "Role.hpp"

using final::Base;
using final::Fighter;
using final::Protector;
using final::Caster;

Base* baseptrArray[1005];

int main() {
    int n, h, att, round = 0, cnt = 0;
    std::string roleInput, nameInput;
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        std::cin >> roleInput >> nameInput >> h >> att;
        if (roleInput == "Fighter") {
            baseptrArray[i] = new Fighter(nameInput, h, att);
        } else if (roleInput == "Protector") {
            baseptrArray[i] = new Protector(nameInput, h, att);
        } else if (roleInput == "Caster") {
            baseptrArray[i] = new Caster(nameInput, h, att);
        } else {
            baseptrArray[i] = nullptr;
        }
    }
    while (Base::getAliveNumber() > 1) {
        ++round;
        // before round begin: decide defend
        for (int i = 0; i < n; ++i) {
            Base* &ptr = baseptrArray[i];
            if (ptr && ptr->isAlive()) {
                ++cnt;
                if (cnt == 7) {
                    ptr->defend(true);
                    cnt = 0;
                } else {
                    ptr->defend(false);
                }
            }
        }
        // round begin: attacks
        for (int i = 0; i < n; ++i) {
            Base* &ptr = baseptrArray[i];
            if (ptr && ptr->isAlive() && !ptr->isDefensive()) {
                for (int j = 0; j < n; ++j) {
                    if (j != i && baseptrArray[j] && baseptrArray[j]->isAlive()) {
                        ptr->launch_attack(baseptrArray[j]);
                        break;
                    }
                }
            }
        }
    }
    std::cout << round << '\n';
    for (int i = 0; i < n; ++i) {
        delete baseptrArray[i];
    }
    return 0;
}

