#include "register_types.h"
#include "hero.h" 
#include "entity.h"
#include "enemy.h"
#include "trap.h"
#include "save_manager.h"
#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

using namespace godot;

void initialize_dino_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }

    // KOLEJNOŚĆ JEST BARDZO WAŻNA!
    GDREGISTER_ABSTRACT_CLASS(Entity); 
    GDREGISTER_CLASS(Hero);   
    GDREGISTER_CLASS(Enemy);  
    GDREGISTER_CLASS(Trap);   
    GDREGISTER_CLASS(SaveManager);
}

void uninitialize_dino_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }
}

extern "C" {
GDExtensionBool GDE_EXPORT dino_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
    godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

    init_obj.register_initializer(initialize_dino_module);
    init_obj.register_terminator(uninitialize_dino_module);
    init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

    return init_obj.init();
}
}