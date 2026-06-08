#include "enemy.h"
#include "hero.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/area2d.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/progress_bar.hpp>
#include <godot_cpp/classes/animated_sprite2d.hpp>

using namespace godot;

void Enemy::_bind_methods() {
    ClassDB::bind_method(D_METHOD("take_damage", "amount"), &Enemy::take_damage);
}

Enemy::Enemy() : Entity(50) {
    speed = 100.0f;
    direction = 1;
    attack_cooldown = 0.0f;

    patrol_distance = 150.0f;
    idle_timer = 2.0f;
    is_start_pos_saved = false;
    is_dead = false;
}

Enemy::~Enemy() {}

void Enemy::take_damage(int amount) {
    if (is_dead) return;

    set_hp(get_hp() - amount);
    UtilityFunctions::print("przeciwnik ", get_hp(), " HP.");

    ProgressBar* hp_bar = get_node<ProgressBar>("ProgressBar");
    if (hp_bar) {
        hp_bar->set_value(get_hp());
    }

    AnimatedSprite2D* anim = get_node<AnimatedSprite2D>("AnimatedSprite2D");

    if (get_hp() <= 0) {
        die();
    }
    else {
        if (anim) anim->play("hit");
    }
}

void Enemy::die() {
    is_dead = true;
    set_physics_process(false);

    AnimatedSprite2D* anim = get_node<AnimatedSprite2D>("AnimatedSprite2D");
    if (anim) anim->play("dead");

    ProgressBar* hp_bar = get_node<ProgressBar>("ProgressBar");
    if (hp_bar) hp_bar->hide();

    // ustawienie go jako martwego dla save'a
    Node* global = get_node<Node>("/root/Global");
    if (global) {
        Array destroyed = global->get("destroyed_objects");
        destroyed.append(get_name());
        global->set("destroyed_objects", destroyed);
    }
}

void Enemy::_physics_process(double delta) {
    if (Engine::get_singleton()->is_editor_hint()) return;

    Vector2 velocity = get_velocity();

    if (attack_cooldown > 0.0f) {
        attack_cooldown -= delta;
    }

    if (!is_on_floor()) {
        velocity.y += 980.0 * delta;
    }
    Vector2 pos = get_global_position();

    if (!is_start_pos_saved) {
        start_x = pos.x;
        is_start_pos_saved = true;
    }

    bool is_chasing = false;
    bool close_to_attack = false;

    Area2D* detection = get_node<Area2D>("DetectionArea");
    if (detection) {
        TypedArray<Node2D> bodies = detection->get_overlapping_bodies();
        if (bodies.size() > 0) {
            Node2D* hero = Object::cast_to<Node2D>(bodies[0]);
            if (hero) {
                float diff = hero->get_global_position().x - pos.x;

                if (diff > 0) direction = 1;
                else if (diff < 0) direction = -1;

                is_chasing = true;

                if (abs(diff) < 45.0f) {
                    close_to_attack = true;
                }
            }
        }
    }

    if (is_chasing) {
        idle_timer = 0.0f;

        if (close_to_attack) {
            velocity.x = 0.0f;
        }
        else {
            velocity.x = speed * direction * 1.5;
        }
    }
    else { //patrol
        if (idle_timer > 0.0f) {
            velocity.x = 0.0f;
            idle_timer -= delta;
        }
        else {
            velocity.x = speed * direction;
            if (abs(pos.x - start_x) > patrol_distance) {
                direction *= -1;
                start_x = pos.x;
                idle_timer = 2.0f;
            }
            if (is_on_wall()) {
                direction *= -1;
                start_x = pos.x;
                idle_timer = 2.0f;
            }
        }
    }

    set_velocity(velocity);
    move_and_slide();

    if (!is_chasing && is_on_wall()) {
        direction *= -1;
    }

    Area2D* attack_area = get_node<Area2D>("AttackArea");
    AnimatedSprite2D* anim = get_node<AnimatedSprite2D>("AnimatedSprite2D");

    if (attack_area && attack_cooldown <= 0.0f) {
        TypedArray<Node2D> bodies = attack_area->get_overlapping_bodies();

        for (int i = 0; i < bodies.size(); i++) {
            Hero* hero = Object::cast_to<Hero>(bodies[i]);
            if (hero) {
                hero->take_damage(25);
                attack_cooldown = 1.5f;

                if (anim) anim->play("attack");
                break;
            }
        }
    }

    //animacje
    if (anim) {
        String current_anim = anim->get_animation();

        bool is_busy = (current_anim == "attack" || current_anim == "hit");

        if (!is_busy || !anim->is_playing()) {
            if (velocity.x != 0) {
                anim->play("walk");
                anim->set_flip_h(velocity.x < 0);
            }
            else {
                anim->play("idle");
                anim->set_flip_h(direction < 0);
            }
        }
    }
}