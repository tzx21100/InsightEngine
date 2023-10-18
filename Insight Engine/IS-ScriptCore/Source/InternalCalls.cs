
using System.Numerics;
using System.Runtime.CompilerServices;

namespace IS
{

    /// <summary>
    /// A static class used for declaring C++ functions to be called from C# scripts.
    /// </summary>
    public static class InternalCalls
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void NativeLog(string name, int param);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void NativeLogVector(ref Vector3 param);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool KeyPressed(int keycode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool KeyHeld(int keycode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool KeyReleased(int keycode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool MousePressed(int mouseButton);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool MouseHeld(int mouseButton);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool MouseReleased(int mouseButton);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static uint GetCurrentEntity(string name);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBodyAddForce(float x,float y,uint entity);

    }
}
