using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;

namespace IS
{
    class PendulumBoulder
    {
        class Pendulum
        {
            private const float PENDULUM_LENGTH = 1000f; // length of the pendulum (in pixels)
            private const float GRAVITY = 980f;
            private float mAngle;
            private float mAngularVelocity;
            private float mPivotX;
            private float mPivotY;
            private int mStringEntity;
            private float mOffsetY;
            private float mOriginalHeight;

            public void Init()
            {
                mStringEntity = InternalCalls.CreateEntityPrefab("BreakableVine");

                // String
                mOriginalHeight = InternalCalls.GetTransformScaling().y;
                float string_height = InternalCalls.GetTransformScalingEntity(mStringEntity).y;
                mOffsetY = (mOriginalHeight / 2.5f + string_height) / 2f; // offset for whitespace in texture
                InternalCalls.TransformSetPositionEntity(InternalCalls.GetTransformPosition().x,
                    InternalCalls.GetTransformPosition().y + mOffsetY, mStringEntity);

                mAngle = (float)(Math.PI / 4); // initial angle (45 degrees in radians)
                mAngularVelocity = 0.0f; // initial angular velocity (in radians/s)
                mPivotX = InternalCalls.GetTransformPositionEntity(mStringEntity).x;
                mPivotY = InternalCalls.GetTransformPositionEntity(mStringEntity).y - string_height / 2f;
            }

            public void Update()
            {
                float stringCurrentHeight = InternalCalls.GetTransformPositionEntity(mStringEntity).y;

                if (stringCurrentHeight < mOriginalHeight)
                {
                    Console.WriteLine("old height: " + mOriginalHeight, "new height: " + stringCurrentHeight);
                }
                else
                {
                    // Update pendulum angle
                    float deltaTime = InternalCalls.GetDeltaTime();
                    float angularAcceleration = -(GRAVITY / PENDULUM_LENGTH) * (float)Math.Sin(mAngle);
                    mAngularVelocity += angularAcceleration * deltaTime;
                    mAngle += mAngularVelocity * deltaTime;

                    // Update pendulum bob
                    float bobX = mPivotX + PENDULUM_LENGTH * (float)Math.Sin(mAngle);
                    float bobY = mPivotY - PENDULUM_LENGTH * (float)Math.Cos(mAngle);
                    InternalCalls.TransformSetPositionEntity(bobX, bobY, mStringEntity);

                    // Offset string angle in degrees
                    float stringAngleDegrees = mAngle * (180 / (float)Math.PI); // Convert from radians to degrees
                    InternalCalls.TransformSetRotationEntity(stringAngleDegrees, 0f, mStringEntity);
                    InternalCalls.TransformSetPosition(bobX, bobY - mOffsetY);
                    Console.WriteLine("String Angle: " + stringAngleDegrees);
                    OffsetBoulder();
                }
            }

            private void OffsetBoulder()
            {
                float stringPosX = InternalCalls.GetTransformPositionEntity(mStringEntity).x;
                float stringPosY = InternalCalls.GetTransformPositionEntity(mStringEntity).y;

                float rotationAngle = InternalCalls.GetTransformRotationEntity(mStringEntity);
                float angleRadians = rotationAngle * (CustomMath.PI / 180.0f);

                Vector2D relativePosition = new Vector2D(0, mOffsetY);

                // Apply rotation to the relative position
                // Rotation matrix in 2D: [cos(theta), -sin(theta); sin(theta), cos(theta)]
                Vector2D rotatedRelativePosition = new Vector2D(
                    (float)(relativePosition.x * CustomMath.Cos(angleRadians) + relativePosition.y * CustomMath.Sin(angleRadians)),
                    (float)(relativePosition.x * CustomMath.Sin(angleRadians) - relativePosition.y * CustomMath.Cos(angleRadians))
                    );


                // Calculate the absolute position for the pendulum
                Vector2D checkerPosition = new Vector2D(
                    stringPosX + rotatedRelativePosition.x,
                    stringPosY + rotatedRelativePosition.y
                );

                // Set the pendulum's position
                InternalCalls.TransformSetPosition(checkerPosition.x, checkerPosition.y);
                InternalCalls.TransformSetRotation(rotationAngle, 0);
            }
        }

        static private Dictionary<int, Pendulum> mPendulums = new Dictionary<int, Pendulum>();

        static public void Init()
        {
            GetActivePendulum().Init();
        }

        static public void Update()
        {
            GetActivePendulum().Update();
        }

        static public void CleanUp()
        {

        }

        static private Pendulum GetActivePendulum()
        {
            int pendulum = InternalCalls.GetCurrentEntityID();
            if (!mPendulums.ContainsKey(pendulum))
                mPendulums.Add(pendulum, new Pendulum());

            return mPendulums[pendulum];
        }
    }
}
