/*!
 * \file Prefab.h
 * \author Tan Zheng Xun, t.zhengxun@digipen.edu
 * \par Course: CSD2401
 * \date 27-09-2023
 * \brief
 * This header file declares class Prefab
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

 /*                                                                   guard
 ----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_SOURCE_PREFAB_H_
#define GAM200_INSIGHT_ENGINE_SOURCE_PREFAB_H_

 /*                                                                   includes
 ----------------------------------------------------------------------------- */
#include <string>
#include "Engine/ECS/Entities.h"

namespace IS {


    /**
   * \class Prefab
   * \brief A base class for prefabs
   *
   * This class contains the necessary information to create an entity with set components
   *
   */
    class Prefab {
    public:
        /**
         * \brief Constructs the prefab
         */
        Prefab(){};

        /**
         * \brief Constructs the prefab
         */
        Prefab(Signature signature, std::string name) { mSignature = signature; mName = name; }

        //Our prefab is just the signature of the entity
        Signature mSignature;
        //This is for the developers to change the name of the prefab
        std::string mName;
    };

} // namespace IS

#endif