#include "Structure_factory.h"

#include "Town_Hall.h"
#include "Farm.h"
#include "Utility.h"

using std::shared_ptr;
using std::make_shared;
using std::string;
shared_ptr<Structure> create_structure(const string& name, const string& type, Point location)
{
	if (type == "Farm")
		return make_shared<Farm>(name, location);
	else if (type == "Town_Hall")
		return make_shared<Town_Hall>(name, location);
	else
		throw Error("Trying to create structure of unknown type!");
}