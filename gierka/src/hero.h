#ifndef HERO_H
#define HERO_H

#include "entity.h"
#include "idamageable.h"
#include <godot_cpp/classes/area2d.hpp>
#include "inventory.h"
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/dictionary.hpp>

namespace godot {

    // 3 dziedziczenie 
    class Hero : public Entity {
        GDCLASS(Hero, Entity)

    private:
        float gravity;
        float jump_velocity;
        float speed;
        int current_jumps;
        int max_jumps;
        float attack_cooldown;
        Inventory<godot::String> backpack;
    protected:
        static void _bind_methods();

    public:
        Hero();
        ~Hero();

        void _physics_process(double delta) override;

        void take_damage(int amount) override;
        void die() override;
        void attack();
        void collect_item(godot::String item_name, int amount);
        godot::Dictionary get_inventory();
        void use_item(godot::String item_name);
        void clear_inventory();
        void add_to_inventory(godot::String item_name, int amount);
    };

}

#endif