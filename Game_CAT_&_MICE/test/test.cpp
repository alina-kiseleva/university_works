#include <gtest/gtest.h>
#include "../include/hand.hpp"
#include "../include/directDamageSpell.hpp"
#include "../include/areaDamageSpell.hpp"
#include "../include/trapSpell.hpp"
#include "../include/field.hpp"
#include "../include/player.hpp"
#include "../include/enemy.hpp"

TEST(DirectDamageSpellTest, SuccessfulAttackInRange) {
    DirectDamageSpell spell;
    Field field(10, 10);
    Player player;
    Enemy enemy;
    
    player.setCoordinates(2, 2);
    enemy.setCoordinates(4, 2);
    enemy.setHealth(5);
    field.setCellCharacter(4, 2, character::ENEMY);
    
    bool result = spell.use(field, player, enemy, {4, 2});
    EXPECT_TRUE(result);
    EXPECT_EQ(enemy.getHealth(), 3);
}

TEST(DirectDamageSpellTest, AttackOutOfRange) {
    DirectDamageSpell spell;
    Field field(10, 10);
    Player player;
    Enemy enemy;
    
    player.setCoordinates(0, 0);
    enemy.setCoordinates(4, 4);
    enemy.setHealth(5);
    field.setCellCharacter(4, 4, character::ENEMY);
    
    bool result = spell.use(field, player, enemy, {4, 4});
    EXPECT_FALSE(result);
    EXPECT_EQ(enemy.getHealth(), 5);
}

TEST(DirectDamageSpellTest, AttackEmptyCell) {
    DirectDamageSpell spell;
    Field field(10, 10);
    Player player;
    Enemy enemy;
    
    player.setCoordinates(2, 2);
    enemy.setHealth(5);
    
    bool result = spell.use(field, player, enemy, {3, 2});
    EXPECT_FALSE(result);
    EXPECT_EQ(enemy.getHealth(), 5);
}

TEST(DirectDamageSpellTest, TowerAttackInRange) {
    DirectDamageSpell spell;
    Player player;
    
    player.setCoordinates(2, 2);
    player.setHealth(10);
    std::pair<int, int> tower = {3, 2};
    
    bool result = spell.use(player, tower);
    EXPECT_TRUE(result);
    EXPECT_EQ(player.getHealth(), 9);
}

TEST(AreaDamageSpellTest, SuccessfulAreaAttack) {
    AreaDamageSpell spell;
    Field field(10, 10);
    Player player;
    Enemy enemy;
    
    player.setCoordinates(0, 0);
    enemy.setCoordinates(2, 2);
    enemy.setHealth(3);

    field.setCellCharacter(2, 3, character::ENEMY);
    
    bool result = spell.use(field, player, enemy, {2, 2});
    EXPECT_TRUE(result);
    EXPECT_EQ(enemy.getHealth(), 2);
}

TEST(AreaDamageSpellTest, AreaAttackOutOfRange) {
    AreaDamageSpell spell;
    Field field(10, 10);
    Player player;
    Enemy enemy;
    
    player.setCoordinates(0, 0);
    enemy.setCoordinates(6, 6);
    enemy.setHealth(3);
    field.setCellCharacter(6, 6, character::ENEMY);
    
    bool result = spell.use(field, player, enemy, {6, 6});
    EXPECT_FALSE(result);
    EXPECT_EQ(enemy.getHealth(), 3);
}

TEST(AreaDamageSpellTest, TowerAttackNotImplemented) {
    AreaDamageSpell spell;
    Player player;
    
    player.setHealth(10);
    std::pair<int, int> tower = {0, 0};
    
    bool result = spell.use(player, tower);
    EXPECT_FALSE(result);
    EXPECT_EQ(player.getHealth(), 10);
}

TEST(TrapSpellTest, SuccessfulTrapPlacement) {
    TrapSpell spell;
    Field field(10, 10);
    Player player;
    Enemy enemy;
    
    player.setCoordinates(2, 2);

    bool result = spell.use(field, player, enemy, {3, 3});
    EXPECT_TRUE(result);
    EXPECT_EQ(field.getCellCharacter(3, 3), character::TRAP);
}

TEST(TrapSpellTest, TrapPlacementOnOccupiedCell) {
    TrapSpell spell;
    Field field(10, 10);
    Player player;
    Enemy enemy;
    
    player.setCoordinates(2, 2);
    field.setCellCharacter(3, 3, character::ENEMY);
    
    bool result = spell.use(field, player, enemy, {3, 3});
    EXPECT_FALSE(result);
    EXPECT_EQ(field.getCellCharacter(3, 3), character::ENEMY);
}

TEST(TrapSpellTest, TrapPlacementOnObstacle) {
    TrapSpell spell;
    Field field(10, 10);
    Player player;
    Enemy enemy;
    
    player.setCoordinates(2, 2);
    
    bool result = spell.use(field, player, enemy, {0, 0});
    EXPECT_FALSE(result);
}

TEST(TrapSpellTest, TowerAttackNotImplementedForTrap) {
    TrapSpell spell;
    Player player;
    
    player.setHealth(10);
    std::pair<int, int> tower = {0, 0};
    
    bool result = spell.use(player, tower);
    EXPECT_FALSE(result);
    EXPECT_EQ(player.getHealth(), 10);
}

TEST(HandTest, AddSpellCard) {
    Hand hand(2);
    
    EXPECT_EQ(hand.getFullness(), 1);

    bool result = hand.addSpellCard();
    EXPECT_TRUE(result);
    EXPECT_EQ(hand.getFullness(), 2);
    EXPECT_TRUE(hand.hasSpell(0));
    EXPECT_TRUE(hand.hasSpell(1));
    
    result = hand.addSpellCard();
    EXPECT_FALSE(result);
    EXPECT_EQ(hand.getFullness(), 2);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}