#pragma once
#include <string>
#include "Entities.h"
namespace IS {

    class Prefab {
    public:
        void saveAsPrefab(std::string name, Signature signature) {
            prefabList.insert({name,signature});
        }

    private:
        std::unordered_map<std::string, Signature> prefabList;
    };

} // namespace IS
