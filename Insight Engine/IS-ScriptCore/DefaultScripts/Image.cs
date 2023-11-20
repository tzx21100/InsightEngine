using System;
using System.Runtime.InteropServices;

[StructLayout(LayoutKind.Sequential)]
public struct SimpleImage : IDisposable
{
    public IntPtr mFileName;
    public int width;
    public int height;
    public int channels;
    public ulong size;
    public int texture_id;
    public ulong mTextureData;
    public int texture_index;

    public void Dispose()
    {
        // Free the memory allocated for mFileName, if applicable.
        if (mFileName != IntPtr.Zero)
        {
            Marshal.FreeHGlobal(mFileName);
            mFileName = IntPtr.Zero;
        }

    }
}