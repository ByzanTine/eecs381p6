#include "Agent_factory.h"

#include "Peasant.h"
#include "Soldier.h"
#include "Archer.h"
#include "Utility.h"

using std::shared_ptr;
using std::make_shared;

shared_ptr<Agent> create_agent(const std::string& name, const std::string& type, Point location)
{
	if (type == "Peasant")
		return make_shared<Peasant>(name, location);
	else if (type == "Soldier")
		return make_shared<Soldier>(name, location);
	else if (type == "Archer")
		return make_shared<Archer>(name, location);
	else
		throw Error("Trying to create agent of unknown type!");
}