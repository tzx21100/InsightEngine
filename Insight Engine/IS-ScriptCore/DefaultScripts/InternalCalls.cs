
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
        internal extern static void RigidBodyAddForce(float x, float y);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBodyAddForceEntity(float x, float y, uint entity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformSetRotation(float angle, float angle_speed);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformSetPosition(float x, float y);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformSetScale(float x, float y);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformSetRotationEntity(float angle, float angle_speed,int entity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformSetPositionEntity(float x, float y, int entity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformSetScaleEntity(float x, float y, int entity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float GetRigidBodyAngularVelocity();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static SimpleVector2D GetTransformPosition();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static SimpleVector2D GetTransformScaling();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float GetTransformRotation();        
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static SimpleVector2D GetTransformPositionEntity( int entity );

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static SimpleVector2D GetTransformScalingEntity(int entity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float GetTransformRotationEntity(int entity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float GetDeltaTime();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetSpriteImage(SimpleImage image);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static SimpleImage GetSpriteImage(string filename);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void EmplaceImageToGraphics(SimpleImage image);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetSpriteAnimationIndex(int num);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void FreeSpriteImage(SimpleImage image);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CreateAnimationFromSprite(int row, int columns, float animation_time);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AttachCamera();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AudioPlaySound(string name);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AudioPlayMusic(string name);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ResetAnimations();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int GetButtonState();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int CreateEntity(string str);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void DestroyEntity(int entity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool EntityCheckCollide(int entity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AddCollider(int entity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CameraSetZoom(float value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ColliderNone(int entity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int GetCurrentEntityID();
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float GetCollidedObjectAngle(int entity);
    }
}
