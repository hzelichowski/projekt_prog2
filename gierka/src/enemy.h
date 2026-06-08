#ifndef ENEMY_H
#define ENEMY_H

#include "entity.h"
#include "idamageable.h"

namespace godot {

    class Enemy : public Entity {
        GDCLASS(Enemy, Entity)

    private:
        float speed;
        int direction; // 1 prawo -1 lewo
        float attack_cooldown;
        float start_x;
        float patrol_distance;
        float idle_timer;
        bool is_start_pos_saved;
        bool is_dead;
    protected:
        static void _bind_methods();

    public:
        Enemy();
        ~Enemy();

        //implementacja interfejsu
        void take_damage(int amount) override;
        void die() override;

        void _physics_process(double delta) override;
    };

}

#endif