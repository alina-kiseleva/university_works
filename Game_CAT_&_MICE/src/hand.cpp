#include "../include/hand.hpp"
#include "../include/directDamageSpell.hpp"
#include "../include/areaDamageSpell.hpp"
#include "../include/trapSpell.hpp"

Hand::Hand(int size):size(size), fullness(0){
    addSpellCard();
}

int Hand::getFullness() const{
    return fullness;
}

void Hand::setFullness(int value){
    fullness = value;
}

int Hand::getSize() const{
    return size;
}

void Hand::setSize(int value){
    size = value;
}

spellType Hand::getNameSpellCard(int coord) const{
    if (coord < fullness && coord >= 0){
        return spellCards[coord]->name();
    }
    return spellType::NONE;
}

bool Hand::hasSpell(int index){
    if (index < fullness && index >= 0){
        return (spellCards[index] != nullptr);
    }
    return 0;
}

bool Hand::addSpellCard(){
    if (fullness < size){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(0, 2);
        int chance = dist(gen);

        ISpell* current_spell;
        
        if (chance == 0){
            current_spell = new DirectDamageSpell();
        } else if (chance == 1){
            current_spell = new AreaDamageSpell();
        } else {
            current_spell = new TrapSpell();
        }

        spellCards.push_back(current_spell);
        ++fullness;
        return true;
    }
    return false;
}

bool Hand::cast(int spellIndex, Field& field, Player& player, Enemy& enemy, std::pair<int, int> target){
    if (spellIndex < fullness && spellIndex >= 0){
        bool isCasted = spellCards[spellIndex]->use(field, player, enemy, target);
        if (isCasted){
            spellCards.erase(spellCards.begin() + spellIndex);
            --fullness;
            return 1;
        }
    }
    return 0;
}

Hand::~Hand(){
    while (spellCards.size()){
        spellCards.pop_back();
    }
}