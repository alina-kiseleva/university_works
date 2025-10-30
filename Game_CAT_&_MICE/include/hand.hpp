#ifndef HAND_H
#define HAND_H

#include <vector>
#include <random>
#include "iSpell.hpp"

class Hand{
    int size;
    int fullness;
    std::vector<ISpell*> spellCards;

public:
    Hand(int size);

    int getFullness() const;
    void setFullness(int value);
    int getSize() const;
    void setSize(int value);
    spellType getNameSpellCard(int coord) const;

    bool hasSpell(int index);
    bool addSpellCard();
    bool cast(int spellIndex, Field& field, Player& player, Enemy& enemy, std::pair<int, int> target);
    
    ~Hand();
};

#endif