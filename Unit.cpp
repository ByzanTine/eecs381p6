#include "Unit.h"
using std::shared_ptr;

bool Unit::is_offspring_of(shared_ptr<Unit> other) {
	shared_ptr<Unit> current(parent);
	while (current) {
		if (current == other) {
			return true;
		}
		current = current->parent;
	}
	return false;
}

void Unit::check_unset_parent(shared_ptr<Unit> other) {
	if (parent == other) {
		parent.reset();
	}
}

void Unit::set_parent(shared_ptr<Unit> parent_) {
	if (parent) {
		parent->remove_component(shared_from_this());
	}
	parent = parent_;
}