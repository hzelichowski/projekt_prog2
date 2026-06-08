#ifndef SAVE_MANAGER_H
#define SAVE_MANAGER_H

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <string>

namespace godot {

    //12 wzorzec projektowy
    class IDataRepository {
    public:
        virtual ~IDataRepository() = default;
        virtual void save_game(godot::String json_data) = 0;
        virtual godot::String load_game() = 0;
    };
    class SaveManager : public Node, public IDataRepository {
        GDCLASS(SaveManager, Node)
    private:
        static SaveManager* instance;

    protected:
        static void _bind_methods();

    public:
        SaveManager();
        virtual ~SaveManager();

        static SaveManager* get_instance();

        void save_game(godot::String json_data) override;
        godot::String load_game() override;
    };

}

#endif