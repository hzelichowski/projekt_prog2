#ifndef TRAP_H
#define TRAP_H

#include <godot_cpp/classes/area2d.hpp>
#include <godot_cpp/classes/node2d.hpp>

namespace godot {

    class Trap : public Area2D {
        GDCLASS(Trap, Area2D)

    protected:
        static void _bind_methods();

    public:
        Trap();
        ~Trap();

        void _on_body_entered(Node2D* body);
    };

} 

#endif 