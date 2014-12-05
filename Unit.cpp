#include "Unit.h"
#include "Utility.h"
using std::shared_ptr;
using std::weak_ptr;

bool Unit::is_offspring_of(shared_ptr<Unit> other) 
{
	shared_ptr<Unit> current = parent.lock();
	while (current) 
	{
		if (current == other)
			return true;
		current = current->parent.lock();
	}
	return false;
}

void Unit::set_parent(shared_ptr<Unit> parent_) 
{
	if (!parent.expired())
		parent.lock()->remove_component(shared_from_this());
	parent = parent_;
}

void Unit::add_component(std::shared_ptr<Unit>)
{
	throw (Error("I'm not a group!"));
}

void Unit::remove_component(std::shared_ptr<Unit>)
{
	throw (Error("I'm not a group!"));
}