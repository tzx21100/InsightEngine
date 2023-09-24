using System.Runtime.CompilerServices;

namespace IS
{
    public struct Vector3
    {
        public float x, y, z;

        public Vector3(float X, float Y, float Z)
        {
            x = X;
            y = Y;
            z = Z;
        }
    }

    public static class InternalCalls
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void nativeLog(string name, int param);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void nativeLog_v(ref Vector3 param);
    }
    public class Entity
    {
        public float FloatVar { get; set; }
        public Entity()
        {
            Console.WriteLine("ctor!");
            log("matt", 1010);

            Vector3 pos = new Vector3(10,5.5f,2);
            log(ref pos);
        }

        public void PrintMessage()
        {
            Console.WriteLine("Hello World from C#!");
        }
        public void PrintInt(int value)
        {
            Console.WriteLine($"C# says: {value}");
        }
        public void PrintCustomMessage(string message)
        {
            Console.WriteLine($"C# says: {message}");
        }
        private void log(string str, int param)
        {
            InternalCalls.nativeLog(str, param);
        }
        private void log(ref Vector3 param)
        {
            InternalCalls.nativeLog_v(ref param);
        }

    
    }
}