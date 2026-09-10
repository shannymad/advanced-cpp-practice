#pragma once

#include <memory>
#include <vector>
#include <string>
#include <map>


using std::string;


std::vector<std::unique_ptr<string>> Duplicate(const std::vector<std::shared_ptr<string>>& items) {
    std::vector<std::unique_ptr<std::string>> out;
    for (auto& item: items) {
        std::unique_ptr<std::string> unique_ptr = std::make_unique<std::string>((*item));
        out.push_back(std::move(unique_ptr));
    }

    return out;
}

std::vector<std::shared_ptr<string>> DeDuplicate(const std::vector<std::unique_ptr<string>>& items) {
    std::vector<std::shared_ptr<std::string>> out;
    std::map<std::string, std::shared_ptr<std::string>> seen;

    for (auto& item: items) {
            const std::string &value = *item;
            auto [it, inserted] = seen.try_emplace(value, std::make_shared<std::string> (value));
            out.push_back(it->second);
        }

    return out;

}
