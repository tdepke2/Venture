#ifndef _COMBATSTATS_H
#define _COMBATSTATS_H

#include <string>
#include <vector>

using namespace std;

class CombatStats {
    public:
    CombatStats();
    CombatStats(const CombatStats& combatStats);
    void init(int attack = 0, int defense = 0, int health = 0, float critChance = 0.0, float dmgReduction = 100.0);
    void init(const vector<string>& data, int& index);
    string toString() const;
    
    private:
    int _attack, _defense, _health;
    float _critChance, _dmgReduction;
};

#endif