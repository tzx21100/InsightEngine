#pragma once
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




    SimpleArray CreateIntArray(int length) {
        int* data = new int[length];
        return { static_cast<void*>(data), length, DataType::Int };
    }


    SimpleArray CreateFloatArray(int length) {
        float* data = new float[length];
        return { static_cast<void*>(data), length, DataType::Float };
    }

    SimpleArray CreateDoubleArray(int length) {
        double* data = new double[length];
        return { static_cast<void*>(data), length, DataType::Double };
    }

    void DeleteArray(SimpleArray array) {
        switch (array.type) {
        case DataType::Int:
            delete[] static_cast<int*>(array.data);
            break;
        case DataType::Float:
            delete[] static_cast<float*>(array.data);
            break;
        case DataType::Double:
            delete[] static_cast<double*>(array.data);
            break;
        }
    }




    template <typename T>
    SimpleArray CreateSimpleArrayFromVector(const std::vector<T>& vector) {
        // Static assert to cause a compile error intentionally if this generic version is instantiated,
        // indicating the type T is not supported.
        static_assert(sizeof(T) == 0, "CreateSimpleArrayFromVector is not implemented for this type.");
    }

    template <>
    SimpleArray CreateSimpleArrayFromVector<int>(const std::vector<int>& vector) {
        int* data = new int[vector.size()];
        for (size_t i = 0; i < vector.size(); ++i) {
            data[i] = vector[i];
        }
        return { static_cast<void*>(data), static_cast<int>(vector.size()), DataType::Int };
    }

    template <>
    SimpleArray CreateSimpleArrayFromVector<float>(const std::vector<float>& vector) {
        float* data = new float[vector.size()];
        for (size_t i = 0; i < vector.size(); ++i) {
            data[i] = vector[i];
        }
        return { static_cast<void*>(data), static_cast<int>(vector.size()), DataType::Float };
    }

    template <>
    SimpleArray CreateSimpleArrayFromVector<double>(const std::vector<double>& vector) {
        double* data = new double[vector.size()];
        for (size_t i = 0; i < vector.size(); ++i) {
            data[i] = vector[i];
        }
        return { static_cast<void*>(data), static_cast<int>(vector.size()), DataType::Double };
    }

    template <>
    SimpleArray CreateSimpleArrayFromVector<Entity>(const std::vector<Entity>& vector) {
        int* data = new int[vector.size()];
        for (size_t i = 0; i < vector.size(); ++i) {
            data[i] = static_cast<int>(vector[i]);
        }
        return { static_cast<void*>(data), static_cast<int>(vector.size()), DataType::Int };
    }



}
