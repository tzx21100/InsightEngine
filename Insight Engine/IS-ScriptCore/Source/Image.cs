using System;
using System.Runtime.InteropServices;

[StructLayout(LayoutKind.Sequential)]
public struct SimpleImage
{
    public IntPtr mFileName; // This will point to a null-terminated string in unmanaged memory.
    public int width;
    public int height;
    public int channels;
    public ulong size; // Assuming size_t is 64 bits.
    public int texture_id;
    public ulong mTextureData;
    public int texture_index;
}