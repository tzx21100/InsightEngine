#include "Pch.h"
#include "Category.h"
#include "Engine/Core/CoreEngine.h"

namespace IS
{
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
            }
        }
    }
} // end namespace IS
