/*!
 * \file Category.h
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2451
 * \date 09-02-2024
 * \brief
 * This header file declares the interface for Component Category and System CategorySystem.
 *
 * \copyright
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_SOURCE_SYSTEMS_CATEGORY_H
#define GAM200_INSIGHT_ENGINE_SOURCE_SYSTEMS_CATEGORY_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "../../ECS/Component.h"
#include "../../ECS/System.h"

namespace IS
{
    /*!
     * \brief The Category class is a component that stores the category of an entity.
     */
    class Category : public IComponent
    {
    public:

        /*!
         * Serialize the component to a JSON value.
         */
        Json::Value Serialize() override
        {
            Json::Value value;
            value["Category"] = mCategory;
            return value;
        }

        /*!
         * Deserialize the component from a JSON value.
         */
        void Deserialize(Json::Value value) override
        {
            mCategory = value["Category"].asString();
        }

        /*!
         * The type of the component.
         */
        static std::string GetType()
        {
            return "Category";
        }

        std::string mCategory; ///< The category of the entity.
    };

    /*!
     * \brief The CategorySystem class manages categories of entities.
     *
     * \brief CategorySystem is a subclass of ParentSystem and handles category-related functionality.
     */
    class CategorySystem : public ParentSystem
    {
    public:

        /*!
         * \brief Initializes the CategorySystem.
         */
        void Initialize() override;

        /*!
         * \brief Updates the CategorySystem.
         * \param deltaTime The time difference since the last update.
         */
        void Update(float) override;

        /*!
         * \brief Gets the name of the CategorySystem.
         */
        std::string GetName() override
        {
            return "CategorySystem";
        }

        /*!
         * \brief Gets the type of the CategorySystem.
         */
        static std::string GetType()
        {
            return "CategorySystem";
        }

        std::unordered_set<std::string> mCategories; //< The set of categories.

        static const std::string ADD_NEW_CATEGORY; //< The string for adding a new category.
    };

} // end namespace IS

#endif //!GAM200_INSIGHT_ENGINE_SOURCE_SYSTEMS_CATEGORY_H
