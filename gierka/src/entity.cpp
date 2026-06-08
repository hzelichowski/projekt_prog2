#include "entity.h"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void Entity::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_hp"), &Entity::get_hp);
    ClassDB::bind_method(D_METHOD("set_hp", "value"), &Entity::set_hp);
    ClassDB::bind_method(D_METHOD("take_damage", "amount"), &Entity::take_damage);
}

Entity::Entity() {
    max_hp = 100;
    hp = max_hp;
}

Entity::Entity(int starting_hp) {
    max_hp = starting_hp;
    hp = max_hp;
}

Entity::~Entity() {}

int Entity::get_hp() const {
    return hp;
}

void Entity::set_hp(int value) {
    hp = value;
    if (hp > max_hp) hp = max_hp;
    if (hp <= 0) {
        hp = 0;
        die(); 
    }
}
void Entity::take_damage(int amount) {
    set_hp(hp - amount);
}