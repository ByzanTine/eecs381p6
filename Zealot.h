/*
A Zealot is an Agent that has attack all the way. It can be commanded
to start attacking another Agent and will continue the attack as long as 
it is alive and the target is alive and even when the target is out of range,
Zealot will chase it down. If attacked, the Zealot won't care, so it behaves 
the same as Peasant when take_hit.
A Zealot never stay calm.
*/
	

#ifndef ZEALOT_H
#define ZEALOT_H
#include <memory>
#include "Warrior.h"


// A soldier is a special warrior that do counter-attack
class Zealot: public Warrior
{
public:
	
	Zealot(const std::string& name_, Point location_);
	
	// update implements Soldier behavior
	void update() override;
		
	// output information about the current state
	void describe() const override;

};



#endif