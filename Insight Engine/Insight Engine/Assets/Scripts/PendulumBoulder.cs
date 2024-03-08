using System;
using System.Reflection.Metadata;
using System.Runtime.CompilerServices;

namespace IS
{
    class PendulumBoulder
    {
        static private float mLength;
        static private float mGravity;
        static private float mAngle;
        static private float mAngularVelocity;
        static private float mPivotX;
        static private float mPivotY;

        static public void Init()
        {
            mLength = 2000f;  // length of the pendulum (in pixels)
            mGravity = 980.0f; // acceleration due to gravity (in pixels/s^2), assuming 1 pixel = 1 cm
            mAngle = (float)(Math.PI / 4); // initial angle (45 degrees in radians)
            mAngularVelocity = 0.0f; // initial angular velocity (in radians/s)
            mPivotX = InternalCalls.GetTransformPosition().x;
            mPivotY = InternalCalls.GetTransformPosition().y;
        }

        static public void Update()
        {
            float deltaTime = InternalCalls.GetDeltaTime();
            float angularAcceleration = -(mGravity / mLength) * (float)Math.Sin(mAngle);
            mAngularVelocity += angularAcceleration * deltaTime;
            mAngle += mAngularVelocity * deltaTime;
            float bobX = mPivotX + mLength * (float)Math.Sin(mAngle);
            float bobY = mPivotY + mLength * (float)Math.Cos(mAngle);
            InternalCalls.TransformSetPosition(bobX, bobY);
            Console.WriteLine("Bob position: (" + bobX + ", " + bobY + ")");
        }

        static public void CleanUp()
        {

        }

    }
}
