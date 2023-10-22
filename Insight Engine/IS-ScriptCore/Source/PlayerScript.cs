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
            InternalCalls.RigidBodyAddForce(hori_movement*1000f*InternalCalls.GetDeltaTime(), verti_movement*1000f * InternalCalls.GetDeltaTime());

            // scaling transform with movement
            Vector2D trans_pos = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformPosition());
            Vector2D trans_scaling = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformScaling());
            float trans_rotate = InternalCalls.GetTransformRotation();
            if (InternalCalls.KeyHeld((int)KeyCodes.A)) { if (trans_scaling.x > 0) { trans_scaling.x *= -1; } }
            if (InternalCalls.KeyHeld((int)KeyCodes.D)){ if (trans_scaling.x < 0) { trans_scaling.x *= -1; } }
            InternalCalls.TransformSetScale(trans_scaling.x, trans_scaling.y);

            int rotate = BoolToInt(InternalCalls.KeyHeld((int)KeyCodes.Q)) - BoolToInt(InternalCalls.KeyHeld((int)KeyCodes.E));
            trans_rotate += rotate * InternalCalls.GetRigidBodyAngularVelocity();
            trans_rotate = trans_rotate <0? 360: trans_rotate %= 360;
            InternalCalls.TransformSetRotation(trans_rotate, 10);


        }


    }



}

