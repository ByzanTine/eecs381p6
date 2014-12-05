#include "Unit.h"
using std::shared_ptr;
using std::weak_ptr;
using std::string;

bool Unit::is_offspring_of(shared_ptr<Unit> other) {
	shared_ptr<Unit> current = parent.lock();
	while (current) {
		if (current == other) {
			return true;
		}
		current = current->parent.lock();
	}
	return false;
}

void Unit::unset_parent() {
	parent.reset();
}

void Unit::set_parent(shared_ptr<Unit> parent_) {
	if (!parent.expired()) {
		parent.lock()->remove_component(shared_from_this());
	}
	parent = parent_;
}