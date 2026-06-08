#ifndef INVENTORY_H
#define INVENTORY_H
#include <vector>
#include <string>

//10 typy generyczne
template <typename T>
class Inventory {
public:
    //4 klasa zagniezdzona
    class Slot {
    public:
        T item;
        int quantity;

        Slot(T i, int q) : item(i), quantity(q) {}
    };

private:
    // 11 kolekcje
    std::vector<Slot> slots;
    int capacity;

public:
    Inventory(int cap = 10) : capacity(cap) {}

    bool add_item(T item, int amount) {
        // czy przedmiot jest w plecaku
        for (auto& slot : slots) {
            if (slot.item == item) {
                slot.quantity += amount;
                return true;
            }
        }

        if (slots.size() < capacity) {
            slots.push_back(Slot(item, amount));
            return true;
        }

        return false;
    }

    int get_size() const { return slots.size(); }

    // zwraca dostep do przedmiotow
    const std::vector<Slot>& get_slots() const {
        return slots;
    }
    void clear() {
        slots.clear();
    }
    bool remove_item(T item, int amount) {
        for (auto it = slots.begin(); it != slots.end(); ++it) {
            if (it->item == item) {
                if (it->quantity >= amount) {
                    it->quantity -= amount;
                    if (it->quantity == 0) {
                        slots.erase(it);
                    }
                    return true;
                }
                return false;
            }
        }
        return false;
    }
};

#endif // INVENTORY_H