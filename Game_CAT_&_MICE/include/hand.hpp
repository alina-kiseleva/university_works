#ifndef HAND_H
#define HAND_H

#include <vector>
#include <random>
#include "iSpell.hpp"

class Hand{
    int size;
    std::vector<ISpell*> spellCards;

public:
    Hand(int size);

    int getCurrentSize() const;
    void setSize(int value);
    int getSize() const;
    
    spellType getNameSpellCard(int coord) const;
    bool hasSpell(int index);
    bool addRandomSpellCard(int koef);
    void addConcreteSpellCard(spellType type, int koef);
    void deleteRandomSpellCard(int quantity);
    bool cast(int spellIndex, Field& field, Player& player, Enemy& enemy, std::pair<int, int> target);
    void clearSpells();

    ~Hand();
};

#endif