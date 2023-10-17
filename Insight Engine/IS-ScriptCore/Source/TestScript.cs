using System.Runtime.CompilerServices;

namespace IS
{
    public class Test
    {
        static public void Init()
        {
            // Initialization code
            Console.WriteLine("ctor!");
            InternalCalls.NativeLog("Entity Initialized", 0);
        }

        static public void Update()
        {
            // Update code
            if (InternalCalls.MouseHeld(1) == true)
            {
                InternalCalls.NativeLog("Calling Right Mouse Pressed From C#", 1);
            }
        }
    }
}