#include "hero.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/variant/utility_functions.hpp> 
#include "enemy.h"
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/area2d.hpp>
#include <godot_cpp/classes/animated_sprite2d.hpp>


using namespace godot;

void Hero::_bind_methods() {
    ClassDB::bind_method(D_METHOD("collect_item", "item_name", "amount"), &Hero::collect_item);
    ClassDB::bind_method(D_METHOD("get_inventory"), &Hero::get_inventory);
    ClassDB::bind_method(D_METHOD("use_item", "item_name"), &Hero::use_item);
    ClassDB::bind_method(D_METHOD("clear_inventory"), &Hero::clear_inventory);
    ClassDB::bind_method(D_METHOD("add_to_inventory", "item_name", "amount"), &Hero::add_to_inventory);
    ClassDB::bind_method(D_METHOD("get_hp"), &Hero::get_hp);
}

//2 przeciazanie konstruktorow
Hero::Hero() : Entity(150) {
    gravity = 980.0f;
    jump_velocity = -500.0f;
    speed = 300.0f;
    max_jumps = 2;     
    current_jumps = 0; 
    attack_cooldown = 0.0f;

    backpack.add_item("Miecz", 1);
}

Hero::~Hero() {}

void Hero::take_damage(int amount) {
    if (get_hp() <= 0) {
        return;
    }

    set_hp(get_hp() - amount);
    UtilityFunctions::print("HP ", get_hp());

    AnimatedSprite2D* anim = get_node<AnimatedSprite2D>("AnimatedSprite2D");

    if (get_hp() <= 0) {
        die();
    }
    else {
        if (anim) {
            anim->play("hurt");
        }
    }
}

void Hero::die() {
    UtilityFunctions::print("umarles");

    AnimatedSprite2D* anim = get_node<AnimatedSprite2D>("AnimatedSprite2D");
    if (anim) {
        anim->play("death");
    }
    set_physics_process(false);
}

void Hero::_physics_process(double delta) {
    if (Engine::get_singleton()->is_editor_hint()) return;

    Vector2 velocity = get_velocity();
    Input* input = Input::get_singleton();

    if (attack_cooldown > 0.0f) {
        attack_cooldown -= delta;
    }

    if (is_on_floor()) {
        current_jumps = 0;
    }
    else {
        velocity.y += gravity * delta;
    }

    if (input->is_action_just_pressed("ui_up")) {
        if (current_jumps < max_jumps) {
            velocity.y = jump_velocity;
            current_jumps++;
        }
    }

    if (input->is_action_just_pressed("ui_down") && is_on_floor()) {
        Vector2 pos = get_position();
        pos.y += 5.0f;
        set_position(pos);
    }

    float direction = input->get_axis("ui_left", "ui_right");
    AnimatedSprite2D* sprite = get_node<AnimatedSprite2D>("AnimatedSprite2D");

    if (direction != 0.0f) {
        velocity.x = direction * speed;
        if (sprite) {
            sprite->set_flip_h(direction < 0);
        }
    }
    else {
        velocity.x = 0;
    }
    set_velocity(velocity);
    move_and_slide();

    if (input->is_action_just_pressed("ui_accept") && is_on_floor() && attack_cooldown <= 0.0f) {
        attack();
    }

    //animacje
    if (sprite) {
        String current_anim = sprite->get_animation();

        bool is_busy = (current_anim == "attack" || current_anim == "dash_attack" || current_anim == "hurt");

        if (!is_busy || !sprite->is_playing()) {

            if (!is_on_floor()) {
                if (velocity.y < 0) {
                    sprite->play("jump");
                }
                else {
                    sprite->play("fall");
                }
            }
            else {
                if (velocity.x != 0) {
                    sprite->play("run");
                }
                else {
                    sprite->play("idle");
                }
            }
        }
    }
}
void Hero::attack() {
    AnimatedSprite2D* sprite = get_node<AnimatedSprite2D>("AnimatedSprite2D");
    if (sprite) {
        if (get_velocity().x != 0.0f) {
            sprite->play("dash_attack");
            attack_cooldown = 1.0f;
        }
        else {
            sprite->play("attack");
            attack_cooldown = 1.2f;
        }
    }

    //szukanie wezla swordraea i zadaje obrazenia
    Area2D* sword = get_node<Area2D>("SwordArea");
    if (!sword) return;

    TypedArray<Node2D> bodies = sword->get_overlapping_bodies();

    for (int i = 0; i < bodies.size(); i++) {
        Node2D* body = Object::cast_to<Node2D>(bodies[i]);
        if (body && body->has_method("take_damage")) {
            UtilityFunctions::print("Ciach! Trafiono wroga!");
            body->call("take_damage", 25);
        }
    }
}
void Hero::collect_item(String item_name, int amount) {
    if (backpack.add_item(item_name, amount)) {
        UtilityFunctions::print("Podniesiono: ", item_name, " (Ilosc: ", amount, ")");
        UtilityFunctions::print("Zajete sloty w plecaku: ", backpack.get_size(), "/10");
    }
    else {
        UtilityFunctions::print("Nie mozna podniesc ", item_name, " - plecak jest pelny!");
    }
}
Dictionary Hero::get_inventory() {
    Dictionary result;
    //tlumaczenie wektora cpp na godota
    for (const auto& slot : backpack.get_slots()) {
        result[slot.item] = slot.quantity;
    }
    return result;
}

void Hero::use_item(String item_name) {
    if (backpack.remove_item(item_name, 1)) {
        if (item_name == "Magiczny Napoj") {
            set_hp(get_hp() + 50);
            if (get_hp() > 150) set_hp(150);
            UtilityFunctions::print("dodano 50hp");
        }
    }
}
void Hero::clear_inventory() {
    backpack.clear();
    backpack.add_item("Miecz", 1); 
}

void Hero::add_to_inventory(String item_name, int amount) {
    backpack.add_item(item_name, amount);
}