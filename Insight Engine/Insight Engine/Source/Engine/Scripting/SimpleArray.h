/*!
* \file SimpleArray.h
* \author  Tan Zheng Xun, t.zhengxun@digipen.edu
* \par Course: CSD2451
* \date  08-03-2024
* \brief
* This header file declares the SimpleArray in order to pass specific vector types
* from c++ into c#. This is a buffer class meant solely for the purpose of doing the
* tasks.
*
* \copyright
* All content (C) 2024 DigiPen Institute of Technology Singapore.
* All rights reserved.
* Reproduction or disclosure of this file or its contents without the prior written
* consent of DigiPen Institute of Technology is prohibited.
*____________________________________________________________________________*/

/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_EDITOR_SCENE_SIMPLEARRAY_H
#define GAM200_INSIGHT_ENGINE_EDITOR_SCENE_SIMPLEARRAY_H

/*                                                                   includes
----------------------------------------------------------------------------- */

#include<vector>
#include "../Core/CoreEngine.h"

namespace IS {

    enum class DataType : int {
        Int,
        Float,
        Double
    };


    struct SimpleArray {
        void* data;  // Generic pointer to data
        int length;  // Number of elements in the array
        DataType type;  // Type of data in the array
    };

    class Slist {
    public:
        static  std::vector<SimpleArray> mSimpleArrays;
    };




    SimpleArray CreateIntArray(int length);

    SimpleArray CreateFloatArray(int length);

    SimpleArray CreateDoubleArray(int length);
    void DeleteArray(SimpleArray array);




    template <typename T>
    SimpleArray CreateSimpleArrayFromVector(const std::vector<T>& vector);

    template <>
    SimpleArray CreateSimpleArrayFromVector<int>(const std::vector<int>& vector);;
    template <>
    SimpleArray CreateSimpleArrayFromVector<float>(const std::vector<float>& vector);;

    template <>
    SimpleArray CreateSimpleArrayFromVector<double>(const std::vector<double>& vector);

    template <>
    SimpleArray CreateSimpleArrayFromVector<Entity>(const std::vector<Entity>& vector);
    void ClearSimpleArray();

}

#endif