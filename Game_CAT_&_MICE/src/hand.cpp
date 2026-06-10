#include "../include/hand.hpp"
#include "../include/directDamageSpell.hpp"
#include "../include/areaDamageSpell.hpp"
#include "../include/trapSpell.hpp"

Hand::Hand(int size):size(size){
    addRandomSpellCard(1);
}

int Hand::getCurrentSize() const{
    return spellCards.size();
}

int Hand::getSize() const{
    return size;
}

void Hand::setSize(int value){
    size = value;
}

spellType Hand::getNameSpellCard(int coord) const{
    if (coord < spellCards.size() && coord >= 0){
        return spellCards[coord]->name();
    }
    return spellType::NONE;
}

bool Hand::hasSpell(int index){
    if (index < spellCards.size() && index >= 0){
        return (spellCards[index] != nullptr);
    }
    return 0;
}

bool Hand::addRandomSpellCard(int koef){
    if (spellCards.size() < size){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(0, 2);
        int chance = dist(gen);

        ISpell* current_spell;
        
        if (chance == 0){
            current_spell = new DirectDamageSpell(koef);
        } else if (chance == 1){
            current_spell = new AreaDamageSpell(koef);
        } else {
            current_spell = new TrapSpell(koef);
        }
        spellCards.push_back(current_spell);
        return true;
    }
    return false;
}

void Hand::addConcreteSpellCard(spellType type, int koef){
    ISpell* current_spell;

    if (type == spellType::DIRECT_DAMAGE_SPELL){
        current_spell = new DirectDamageSpell(koef);
    } else if (type == spellType::AREA_DAMAGE_SPELL){
        current_spell = new AreaDamageSpell(koef);
    } else if (type == spellType::TRAP_SPELL){
        current_spell = new TrapSpell(koef);
    }

    spellCards.push_back(current_spell);
}

bool Hand::cast(int spellIndex, Field& field, Player& player, Enemy& enemy, std::pair<int, int> target){
    if (spellIndex < spellCards.size() && spellIndex >= 0){
        bool isCasted = spellCards[spellIndex]->use(field, player, enemy, target, 0);
        if (isCasted){
            spellCards.erase(spellCards.begin() + spellIndex);
            return 1;
        }
    }
    return 0;
}

void Hand::deleteRandomSpellCard(int quantity){
    if (spellCards.empty()) return;

    std::random_device rd;
    std::mt19937 gen(rd());

    for (int i = 0; i < quantity && !spellCards.empty(); ++i){
        std::uniform_int_distribution<int> dist(0, spellCards.size() - 1);
        int cardIndex = dist(gen);
        spellCards.erase(spellCards.begin() + cardIndex);
    }
}

void Hand::clearSpells(){
    while (spellCards.size()){
        spellCards.pop_back();
    }
}

Hand::~Hand(){
    clearSpells();
}