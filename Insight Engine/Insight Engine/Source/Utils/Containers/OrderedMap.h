/*!
 * \file OrderedMap.h
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 12-11-2023
 * \brief
 * This header file declares and defines class template OrderedMap which encapsulates
 * the functionality of an unordered map but retains insertion order for iterations.
 *
 * \copyright
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_UTILS_ORDERED_MAP_H
#define GAM200_INSIGHT_ENGINE_UTILS_ORDERED_MAP_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include <memory>
#include <utility>
#include <vector>
#include <unordered_map>

namespace IS {

    /*!
     * \brief A class representing an ordered map where values are stored based on a unique key.
     *
     * The key is used to uniquely identify each value, and the values are stored in the order of insertion.
     * This class supports insertion, emplacement, retrieval, and iteration of values based on their types.
     *
     * \tparam Key The type of the unique key.
     * \tparam Base The base type from which all derived types are derived.
     */
    template <typename Key, typename Base>
    class OrderedMap
    {
    public:
        using BaseSharedPtr = std::shared_ptr<Base>; ///< Shared pointer type for the base class.
        using BaseVector = std::vector<BaseSharedPtr>; ///< Vector type for storing shared pointers to the base class.
        using BaseIterator = typename BaseVector::iterator; ///< Iterator type for iterating over elements in the vector.
        using ConstBaseIterator = typename BaseVector::const_iterator; ///< Const iterator type for iterating over elements in the vector.

        /*!
         * \brief Inserts a value into the map using the specified key.
         *
         * If the key already exists, the insertion is ignored.
         *
         * \tparam Derived The derived type of the value being inserted.
         * \param key The unique key associated with the value.
         * \param value A shared pointer to the derived type value.
         */
        template <typename Derived>
        void Insert(const Key& key, const std::shared_ptr<Derived>& value)
        {
            // Check if the key already exists
            if (mKeyIndex.find(key) == mKeyIndex.end())
            {
                // If the key is not present, add the value to the vector and update the index
                mData.push_back(value);
                mKeyIndex[key] = mData.size() - 1;
            }
        }

        /*!
         * \brief Emplaces a value into the map using the specified key and constructor arguments.
         *
         * If the key already exists, the emplacement is ignored.
         *
         * \tparam Derived The derived type of the value being emplaced.
         * \tparam Args Variadic template parameter for constructor arguments.
         * \param key The unique key associated with the value.
         * \param args Constructor arguments for creating the derived type value.
         */
        template <typename Derived, typename... Args>
        void Emplace(const Key& key, Args&&... args)
        {
            // Check if the key already exists
            if (mKeyIndex.find(key) == mKeyIndex.end())
            {
                // If the key is not present, emplace the value in the vector and update the index
                mData.emplace_back(std::make_shared<Derived>(std::forward<Args>(args)...));
                mKeyIndex[key] = mData.size() - 1;
            }
        }

        /*!
         * \brief Retrieves a mutable pointer to the value associated with the specified key and type.
         *
         * \tparam Derived The derived type of the value being retrieved.
         * \param key The unique key associated with the value.
         * \return A shared pointer to the derived type value, or nullptr if the key is not found.
         */
        template <typename Derived>
        std::shared_ptr<Derived> Get(const Key& key)
        {
            // Check if the key exists
            auto it = mKeyIndex.find(key);
            if (it != mKeyIndex.end())
            {
                // Use dynamic_pointer_cast to cast to the derived type
                return std::dynamic_pointer_cast<Derived>(mData[it->second]);
            }
            // Return nullptr if the key is not found
            return nullptr;
        }

        /*!
         * \brief Retrieves a const pointer to the value associated with the specified key and type.
         *
         * \tparam Derived The derived type of the value being retrieved.
         * \param key The unique key associated with the value.
         * \return A shared pointer to the const derived type value, or nullptr if the key is not found.
         */
        template <typename Derived>
        std::shared_ptr<const Derived> Get(const Key& key) const
        {
            // Check if the key exists
            auto it = mKeyIndex.find(key);
            if (it != mKeyIndex.end())
            {
                // Use dynamic_pointer_cast to cast to the derived type
                return std::dynamic_pointer_cast<const Derived>(mData[it->second]);
            }
            // Return nullptr if the key is not found
            return nullptr;
        }

        /*!
         * \brief Returns an iterator pointing to the beginning of the vector.
         *
         * \return Iterator to the beginning.
         */
        BaseIterator begin() { return mData.begin(); }

        /*!
         * \brief Returns an iterator pointing to the end of the vector.
         *
         * \return Iterator to the end.
         */
        BaseIterator end() { return mData.end(); }

        /*!
         * \brief Returns a const iterator pointing to the beginning of the vector.
         *
         * \return Const iterator to the beginning.
         */
        ConstBaseIterator begin() const { return mData.begin(); }

        /*!
         * \brief Returns a const iterator pointing to the end of the vector.
         *
         * \return Const iterator to the end.
         */
        ConstBaseIterator end() const { return mData.end(); }

        /*!
         * \brief Returns a const iterator pointing to the beginning of the vector.
         *
         * \return Const iterator to the beginning.
         */
        ConstBaseIterator cbegin() const { return mData.cbegin(); }

        /*!
         * \brief Returns a const iterator pointing to the end of the vector.
         *
         * \return Const iterator to the end.
         */
        ConstBaseIterator cend() const { return mData.cbegin(); }

        /*!
         * \brief Clears all values from the map.
         */
        void Clear()
        {
            mData.clear();
            mKeyIndex.clear();
        }

    private:
        BaseVector mData; ///< Vector to store shared pointers to the base class values.
        std::unordered_map<Key, size_t> mKeyIndex; ///< Unordered map to store indices of keys in the vector.
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_UTILS_ORDERED_MAP_H
