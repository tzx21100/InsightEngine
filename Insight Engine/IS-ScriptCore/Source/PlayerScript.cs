using System.Numerics;
using System;
using System.Runtime.CompilerServices;




namespace IS
{
    public class PlayerScript
    {
        //private entity
        private static uint entity;
        public static int BoolToInt(bool boolValue)
        {
            return boolValue ? 1 : 0;
        }

        static public void Init()
        {
            // Initialization code
            Console.WriteLine("ctor!");
            entity = InternalCalls.GetCurrentEntity("Player");
            InternalCalls.NativeLog("Entity Initialized", (int)entity);
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
            
            Vector2D vect2 = new Vector2D(10, 0);
            InternalCalls.RigidBodyAddForce(hori_movement*10f, verti_movement*10f, entity);
        }


    }



}

