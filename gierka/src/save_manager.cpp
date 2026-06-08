#include "save_manager.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <fstream>
#include <stdexcept> //9 obsluga wyjatkow

using namespace godot;

SaveManager* SaveManager::instance = nullptr;

void SaveManager::_bind_methods() {
    ClassDB::bind_method(D_METHOD("save_game", "json_data"), &SaveManager::save_game);
    ClassDB::bind_method(D_METHOD("load_game"), &SaveManager::load_game);
}

SaveManager::SaveManager() {
    if (instance == nullptr) instance = this;
}

SaveManager::~SaveManager() {
    if (instance == this) instance = nullptr;
}

SaveManager* SaveManager::get_instance() { return instance; }

//8 strumienie i pliki 
void SaveManager::save_game(String json_data) {
    std::ofstream file("save.txt");
    if (file.is_open()) {
        file << json_data.utf8().get_data();
        file.close();
        UtilityFunctions::print("Zapisano dane w formacie JSON!");
    }
}

String SaveManager::load_game() {
    try {
        std::ifstream file("save.txt");

        if (!file.is_open()) {
            throw std::runtime_error("brak save'a");
        }

        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();
        return String(content.c_str());

    }
    catch (const std::exception& e) {
        UtilityFunctions::print("wystapil wyjatek I/O: ", String(e.what()));
        return "";
    }
}