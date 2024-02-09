/*!
 * \file Layering.h
 * \author Koh Yan Khang, yankhang.k@digipen.edu
 * \par Course: CSD2451
 * \date 25-11-2023
 * \brief
 * This source file defines the Layering class, and its comparator.
 *
 * The Layering class is used to group entities into layers, and sort them by their layer value.
 * The comparator is used to sort the entities in the layer by their layer value.
 * 
 * \copyright
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

#ifndef GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_LAYERING_H
#define GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_LAYERING_H

#include "Sprite.h"

namespace IS {
    // Layering class
    class Layering {
    public:
        struct GfxLayerComparator {
            /*!
             * \brief Compares two Entity objects.
             *
             * Determines the ordering of two Entity objects based on a specific
             * comparison criterion, such as their IDs, names, or any other relevant
             * attributes. The specific comparison logic should be defined within
             * this method, ensuring that it provides a strict weak ordering.
             *
             * \param lhs The left-hand side Entity object in the comparison.
             * \param rhs The right-hand side Entity object in the comparison.
             * \return True if lhs should come before rhs according to the defined
             *         criterion; otherwise, false.
             */
            bool operator()(Sprite::instanceData const& a, Sprite::instanceData const& b) const {
                return a.layer < b.layer;
            }
        };

        /*!
         * \brief Adds an entity to the layer's entity set.
         *
         * This function inserts a new entity into the layer's set of entities. If the
         * entity already exists in the set, this operation will not duplicate it, as
         * sets do not store duplicate elements.
         *
         * \param entity The entity to add to the layer.
         */
        void AddEntity(Entity entity) {
            mLayerEntities.insert(entity);
        }

        /*!
         * \brief Removes an entity from the layer's entity set.
         *
         * Searches for the given entity in the layer's set of entities and removes it
         * if found. If the entity is not found, the function does nothing.
         *
         * \param entity The entity to be removed from the layer.
         */
        void RemoveEntity(Entity entity) {
            auto val = std::find(mLayerEntities.begin(), mLayerEntities.end(), entity);
            if (val == mLayerEntities.end()) {
                return;
            }
            mLayerEntities.erase(val);
        }

        bool mLayerActive=true;
        std::string mName = "";
        std::set<Entity> mLayerEntities;

        /*!
         * \brief Serializes the layer's state to JSON format.
         *
         * This function exports the layer's active status and name into a JSON object.
         * Other attributes, such as the entities within the layer, are not serialized
         * by this function.
         *
         * \return A Json::Value object containing the layer's active status and name.
         */
        Json::Value Serialize() {
            Json::Value data;
            data["Active"] = mLayerActive;
            data["Name"] = mName;
            return data;
        }

        /*!
         * \brief Deserializes the layer's state from a JSON object.
         *
         * This function imports the layer's active status and name from a given JSON
         * object. It expects the JSON object to have "Active" and "Name" fields.
         *
         * \param data The JSON object containing the layer's serialized state.
         */
        void Deserialize(Json::Value data) {
            mLayerActive = data["Active"].asBool();
            mName = data["Name"].asString();
        }

    };
}// end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_LAYERING_H