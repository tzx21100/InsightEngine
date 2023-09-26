/* Start Header **************************************************************/
/*!
\file	Main.cs
\author Matthew

All content (C) 2023 DigiPen Institute of Technology Singapore. All rights reserved.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
*/
/* End Header ****************************************************************/

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
        internal extern static void NativeLog(string name, int param);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void NativeLogVector(ref Vector3 param);
    }
    public class Entity
    {
        public float FloatVar { get; set; }
        public Entity()
        {
            Console.WriteLine("ctor!");
            Log("matt", 1010);

            Vector3 pos = new Vector3(10,5.5f,2);
            Log(ref pos);
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
        private void Log(string str, int param)
        {
            InternalCalls.NativeLog(str, param);
        }
        private void Log(ref Vector3 param)
        {
            InternalCalls.NativeLogVector(ref param);
        }

    
    }
}