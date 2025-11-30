#include "../include/field.hpp"

Field::Field(int length, int width):length(length), width(width){
    if (length >= 10 && length <= 25 && width >= 10 && width <= 25){
        int limit_obstacles = (length * width) * 0.08;
        int limit_freezers = (length * width) * 0.07;
        area.resize(length);

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(0, 100);

        for (int i = 0; i < length; ++i){
            area[i].resize(width);
            for (int j = 0; j < width; ++j){
                int chance = dist(gen);
                if (chance <= 8 && limit_obstacles && (i + j >= 2)){
                    area[i][j] = new Cell(typeOfCell::OBSTACLE);
                    --limit_obstacles;
                } else if (chance <= 15 && limit_freezers && (i + j > 2)){
                    area[i][j] = new Cell(typeOfCell::FREEZE);
                    --limit_freezers;
                } else {
                    area[i][j] = new Cell();
                }
            }
        }

        area[0][0]->setCharacter(character::PLAYER);
    } 
}

Field::Field(const Field& field):length(field.length), width(field.width){
    area.resize(length);
    for (int i = 0; i < length; ++i){
        area[i].resize(width);
        for (int j = 0; j < width; ++j){
            area[i][j] = new Cell(*field.area[i][j]);
        }
    }
}

Field::Field(Field&& field):area(std::move(field.area)), length(field.length), width(field.width){
    field.length = 0;
    field.width = 0;
}

Field& Field::operator = (const Field& field){
    Field temp(field);
    std::swap(area, temp.area);
    std::swap(length, temp.length);
    std::swap(width, temp.width);
    return *this;
}

Field& Field::operator = (Field&& field){
    if (this != &field){
        std::swap(area, field.area);
        std::swap(length, field.length);
        std::swap(width, field.width);
    }
    return *this;
}

int Field::getLength() const{
    return length;
}

int Field::getWidth() const{
    return width;
}

character Field::getCellCharacter(int x, int y) const{
    if (x >= 0 && x < length && y >= 0 && y < width){
        return area[x][y]->getCharacter();
    }
    return character::NOBODY;
}

void Field::setCellCharacter(int x, int y, character newCharacter){
    if (x >= 0 && x < length && y >= 0 && y < width){
        area[x][y]->setCharacter(newCharacter);
    }
}

typeOfCell Field::getCellType(int x, int y) const{
    if (x >= 0 && x < length && y >= 0 && y < width){
        return area[x][y]->getType();
    }
    return typeOfCell::OBSTACLE;
}

void Field::setCellType(int x, int y, typeOfCell newType){
    area[x][y]->setType(newType);
}

void Field::setCellDamage(int x, int y, int newDamage){
    area[x][y]->setDamage(newDamage);
}

int Field::getCellDamage(int x, int y) const{
    return area[x][y]->getDamage();
}

bool Field::canMoveTo(int x, int y){
    return (x < length && y < width && x >= 0 && y >= 0 
        && area[x][y]->getType() != typeOfCell::OBSTACLE 
        && area[x][y]->getCharacter() != character::ENEMY_TOWER
    );
}

void Field::restoreField(std::vector<int> cellTypes, std::vector<int> cellCharacters, std::vector<int> cellDamages){
    for (int i = 0; i < length; ++i){
        for (int j = 0; j < width; ++j){
            int index = i * width + j;
            area[i][j]->setType(static_cast<typeOfCell>(cellTypes[index]));
            character person = static_cast<character>(cellCharacters[index]);
            area[i][j]->setCharacter(static_cast<character>(cellCharacters[index]));
            area[i][j]->setDamage(cellDamages[index]);
        }
    }
}

Field::~Field(){
    for (int i = 0; i < length; ++i){
        for (int j = 0; j < width; ++j){
            delete area[i][j];
        }
    }
}