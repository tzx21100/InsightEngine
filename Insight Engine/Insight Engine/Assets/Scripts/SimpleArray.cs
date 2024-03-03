using IS;
using System;
using System.Runtime.InteropServices;

public enum DataType
{
    Int,
    Float,
    Double
}

[StructLayout(LayoutKind.Sequential)]
public struct SimpleArray : IDisposable
{
    public IntPtr data; // Pointer to the data
    public int length;  // Number of elements
    public DataType type;  // Type of the data

    // Adjusted method to read values based on type
    public dynamic GetValue(int index)
    {
        if (index < 0 || index >= length)
        {
            throw new IndexOutOfRangeException();
        }

        switch (type)
        {
            case DataType.Int:
                return Marshal.ReadInt32(data, index * sizeof(int));
            case DataType.Float:
                float[] floatArray = new float[1];
                Marshal.Copy(IntPtr.Add(data, index * sizeof(float)), floatArray, 0, 1);
                return floatArray[0];
            case DataType.Double:
                double[] doubleArray = new double[1];
                Marshal.Copy(IntPtr.Add(data, index * sizeof(double)), doubleArray, 0, 1);
                return doubleArray[0];
            default:
                throw new InvalidOperationException("Unsupported data type.");
        }
    }

    // Adjusted method to set values based on type
    public void SetValue(int index, dynamic value)
    {
        if (index < 0 || index >= length)
        {
            throw new IndexOutOfRangeException();
        }

        switch (type)
        {
            case DataType.Int:
                Marshal.WriteInt32(data, index * sizeof(int), value);
                break;
            case DataType.Float:
                Marshal.Copy(new float[] { value }, 0, IntPtr.Add(data, index * sizeof(float)), 1);
                break;
            case DataType.Double:
                Marshal.Copy(new double[] { value }, 0, IntPtr.Add(data, index * sizeof(double)), 1);
                break;
            default:
                throw new InvalidOperationException("Unsupported data type.");
        }
    }

    public int FindIndex(dynamic value)
    {
        for (int index = 0; index < length; index++)
        {
            switch (type)
            {
                case DataType.Int:
                    if (Marshal.ReadInt32(data, index * sizeof(int)) == value)
                        return index;
                    break;
                case DataType.Float:
                    float[] floatArray = new float[1];
                    Marshal.Copy(IntPtr.Add(data, index * sizeof(float)), floatArray, 0, 1);
                    if (floatArray[0] == value)
                        return index;
                    break;
                case DataType.Double:
                    double[] doubleArray = new double[1];
                    Marshal.Copy(IntPtr.Add(data, index * sizeof(double)), doubleArray, 0, 1);
                    if (doubleArray[0] == value)
                        return index;
                    break;
            }
        }
        return -1; // Value not found
    }

    // Method to release unmanaged resources
    public void Dispose()
    {
        // Call the DeleteArray function from C++
        InternalCalls.DeleteSimpleArray(this);
    }

}