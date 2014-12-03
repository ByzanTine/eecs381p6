/*
A Soldier is an Agent that has attack and defense behaviors. It can be commanded
to start attacking another Agent and will continue the attack as long as 
it is alive and the target is alive and in range. If attacked, the Soldier will
start attacking its attacker.
*/
	

#ifndef SOLDIER_H
#define SOLDIER_H
#include <memory>
#include "Warrior.h"


// A soldier is a special warrior that do counter-attack
class Soldier: public Warrior
{
public:
	
	Soldier(const std::string& name_, Point location_);
	~Soldier();
	
	// update implements Soldier behavior
	void update() override;
	
	
	// Overrides Agent's take_hit to counterattack when attacked.
	void take_hit(int attack_strength, std::shared_ptr<Agent> attacker_ptr) override;
	
	// output information about the current state
	void describe() const override;

};



#endif