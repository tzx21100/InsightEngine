using System.Runtime.CompilerServices;

namespace IS
{
    public class Main
    {

        public float FloatVar { get; set; }
        public Main()
        {
            Console.WriteLine("ctor!");
            nativeLog("matt", 1010);
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

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void nativeLog(string name, int param);
    }
}