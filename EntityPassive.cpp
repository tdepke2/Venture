#include "EntityPassive.h"
#include "Object.h"

using namespace std;

EntityPassive::EntityPassive() {
    init();
}

EntityPassive::EntityPassive(const EntityPassive& entityPassive) : Entity(entityPassive), CombatStats(entityPassive) {
    timer = entityPassive.timer;
}

void EntityPassive::init(int timer) {
    this->timer = timer;
}

void EntityPassive::init(const vector<string>& data, int& index) {
    Entity::init(data, index);
    CombatStats::init(data, index);
    init(stoi(data[index]));
    index += 1;
}

string EntityPassive::toString() const {
    return Entity::toString() + "," + CombatStats::toString() + "," + to_string(timer);
}

EntityPassive* EntityPassive::makeCopy() const {
    return new EntityPassive(*this);
}

void EntityPassive::update(Window& win, const PairXYi& playerPos) {
    PairXYi lastPos = getPosition();
    if (timer == 0) {
        move(win, 1, 0);
        if (lastPos == getPosition()) {
            timer = 1;
            move(win, -1, 0);
        }
    } else if (timer == 1) {
        move(win, -1, 0);
        if (lastPos == getPosition()) {
            timer = 0;
            move(win, 1, 0);
        }
    }
    
    _updateLight(win, getPosition());
}