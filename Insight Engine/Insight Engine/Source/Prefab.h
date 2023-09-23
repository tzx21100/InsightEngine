#pragma once
#include <string>
#include "Entities.h"
namespace IS {

    class Prefab {
    public:
        //Creating a prefab
        Prefab(){};
        Prefab(Signature signature, std::string name) { mSignature = signature; mName = name; }

        //Our prefab is just the signature of the entity
        Signature mSignature;
        //This is for the developers to change the name of the prefab
        std::string mName;
    };

} // namespace IS
