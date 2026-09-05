#include "dungeon.h"
#include <vector>
#include <string>
#include <iostream>
#include <set>

Room* FindFinalRoom(Room* starting_room) {
    std::set<std::string> keys_found;
    std::vector<Room*> rooms_visited;
    std::vector<Room*> to_go;

    to_go.push_back(starting_room);
    rooms_visited.push_back(starting_room);

    while (!to_go.empty()) {
        Room* current = to_go.back();
        to_go.pop_back();

        if (current->IsFinal()) {
            return current;
        }

        bool found_new_key = false;
        for (size_t i = 0; i < current->NumKeys(); ++i) {
            std::string key = current->GetKey(i);
            if (keys_found.count(key) == 0) {

            keys_found.insert(key);

            found_new_key = true;
            }
        }

        if (found_new_key) {
            for (Room* visited : rooms_visited) {
                to_go.push_back(visited);
            }
        }

        for (size_t i = 0; i < current->NumDoors(); ++i) {
            Door* door = current->GetDoor(i);
            
            if (!door->IsOpen()) {
                for (const std::string& key : keys_found) {
                    if (door->TryOpen(key)) {
                        break;
                    }
                }
            }

            if (door->IsOpen()) {
                Room* next = door->GoThrough();
                
                bool already_visited = false;
                for (Room* visited : rooms_visited) {
                    if (visited == next) {
                        already_visited = true;
                        break;
                    }
                }

                if (!already_visited) {
                    rooms_visited.push_back(next);
                    to_go.push_back(next);
                }
            }
        }
    }
    return nullptr;
}