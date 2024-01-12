#include "Pch.h"
#include "Category.h"
#include "Engine/Core/CoreEngine.h"

namespace IS
{
    const std::string CategorySystem::ADD_NEW_CATEGORY = "Add New Category...";

    void CategorySystem::Initialize()
    {
        mCategories.insert(ADD_NEW_CATEGORY);
    }

    void CategorySystem::Update(float)
    {
        for (Entity entity : mEntities)
        {
            auto& engine = InsightEngine::Instance();
            Category const& category = engine.GetComponent<Category>(entity);
            if (category.mCategory.size() && mCategories.find(category.mCategory) == mCategories.end())
            {
                mCategories.insert(category.mCategory);

                // Ensure add new category is always the last
                mCategories.erase(ADD_NEW_CATEGORY);
                mCategories.insert(ADD_NEW_CATEGORY);
            }
        }
    }
} // end namespace IS
