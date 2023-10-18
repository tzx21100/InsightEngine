using System.Numerics;
using System;
using System.Runtime.CompilerServices;




namespace IS
{
    public class PlayerScript
    {

        public static int BoolToInt(bool boolValue)
        {
            return boolValue ? 1 : 0;
        }

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

            //movement
            int hori_movement = BoolToInt(InternalCalls.KeyHeld((int)KeyCodes.D)) - BoolToInt(InternalCalls.KeyHeld((int)KeyCodes.A));
            int verti_movement = BoolToInt(InternalCalls.KeyHeld((int)KeyCodes.W)) - BoolToInt(InternalCalls.KeyHeld((int)KeyCodes.S));
            uint entity = InternalCalls.GetCurrentEntity("Player");
            Vector2D vect2 = new Vector2D(10, 0);
            InternalCalls.NativeLog("PlayerID", (int)entity);
            InternalCalls.RigidBodyAddForce(hori_movement*10f, verti_movement*10f, entity);
        }


    }



}

