#include "CombatStats.h"

using namespace std;

CombatStats::CombatStats() {
    init();
}

CombatStats::CombatStats(const CombatStats& combatStats) {
    _attack = combatStats._attack;
    _defense = combatStats._defense;
    _health = combatStats._health;
    _critChance = combatStats._critChance;
    _dmgReduction = combatStats._dmgReduction;
}

void CombatStats::init(int attack, int defense, int health, float critChance, float dmgReduction) {
    _attack = attack;
    _defense = defense;
    _health = health;
    _critChance = critChance;
    _dmgReduction = dmgReduction;
}

void CombatStats::init(const vector<string>& data, int& index) {
    init(stoi(data[index]), stoi(data[index + 1]), stoi(data[index + 2]), stof(data[index + 3]), stof(data[index + 4]));
    index += 5;
}

string CombatStats::toString() const {
    return to_string(_attack) + "," + to_string(_defense) + "," + to_string(_health) + "," + to_string(_critChance) + "," + to_string(_dmgReduction);
}