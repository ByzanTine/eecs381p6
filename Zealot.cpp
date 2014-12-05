#include "Zealot.h"

#include <iostream>
#include <cassert>
#include "Utility.h"

using std::cout;
using std::endl;
using std::shared_ptr;
using std::dynamic_pointer_cast;

const int init_attack_strength = 2;
const double init_attack_range = 2.0;

Zealot::Zealot(const std::string& name_, Point location_):
	Warrior(name_, location_, init_attack_strength, init_attack_range)
{}

// update implements Soldier behavior
void Zealot::update()
{
	Agent::update();
	if (!is_alive())
		return;
	if (!is_attacking())
		return;
	// In attack state now, target should have decent weak_ptr
	shared_ptr<Agent> shared_target = get_target_ptr();

	if (!shared_target || !shared_target->is_alive())
	{
		cout << get_name() << ": Target is dead" << endl;
		// find another target to attack
		go_peace();
		return;
	}
	Point target_location = shared_target->get_location();
	if (cartesian_distance(get_location(), target_location) > get_attack_range())
	{
		cout << get_name() << ": Target is now out of range, chase it down!" << endl;
		// chase it down
		move_to(target_location);
		// go_peace();
		return;
	}
	cout << get_name() << ": Clang!" << endl;
	shared_target->take_hit(get_attack_strength(), dynamic_pointer_cast<Agent>(shared_from_this()));
	if (!shared_target->is_alive())
	{
		cout << get_name() << ": I triumph!" << endl;
		// Find another target to attack
		go_peace();	
	}
}

// output information about the current state
void Zealot::describe() const 
{
	cout << "Zealot ";
	Warrior::describe();
	
}
