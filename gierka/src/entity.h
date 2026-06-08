#ifndef ENTITY_H
#define ENTITY_H

#include <godot_cpp/classes/character_body2d.hpp>
#include "idamageable.h" 

namespace godot {

    //3 dziedziczenie po klasie bazowej
    class Entity : public CharacterBody2D, public IDamageable {
        GDCLASS(Entity, CharacterBody2D)

    protected:
        int hp;
        int max_hp;

        //1 metody statyczne
        static void _bind_methods();

    public:
        //2 Przeciążanie konstruktorów
        Entity();
        Entity(int starting_hp);

        //5 wirtualny destruktor metoda (virtual)
        virtual ~Entity();

        int get_hp() const;
        virtual void set_hp(int value);

        //6 klasy abstrakcyjne
        virtual void die() = 0;

        virtual void take_damage(int amount) override;
    };

}

#endif