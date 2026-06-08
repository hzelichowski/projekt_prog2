#include "trap.h"
#include "hero.h"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void Trap::_bind_methods() {
    ClassDB::bind_method(D_METHOD("_on_body_entered", "body"), &Trap::_on_body_entered);
}

Trap::Trap() {}

Trap::~Trap() {}

void Trap::_on_body_entered(Node2D* body) {
    Hero* hero = Object::cast_to<Hero>(body);

    if (hero != nullptr) {
        hero->take_damage(50);
    }
}