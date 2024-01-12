#ifndef GAM200_INSIGHT_ENGINE_SOURCE_SYSTEMS_CATEGORY_H
#define GAM200_INSIGHT_ENGINE_SOURCE_SYSTEMS_CATEGORY_H

#include "../../ECS/Component.h"
#include "../../ECS/System.h"

namespace IS
{
    class Category : public IComponent
    {
    public:
        Json::Value Serialize() override
        {
            Json::Value value;
            value["Category"] = mCategory;
            return value;
        }

        void Deserialize(Json::Value value) override
        {
            mCategory = value["Category"].asString();
        }

        static std::string GetType()
        {
            return "Category";
        }

        std::string mCategory;
    };

    class Caregory : public ParentSystem
    {
    public:
        static std::unordered_map<std::string, std::string> mCategories;

    };

} // end namespace IS

#endif //!GAM200_INSIGHT_ENGINE_SOURCE_SYSTEMS_CATEGORY_H
