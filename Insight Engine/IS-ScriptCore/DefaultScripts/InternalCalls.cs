
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
        internal extern static void NativeLog(string name, float param);

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
        internal extern static void SetSpriteAlphaEntity(float alpha, int entity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetSpriteImageEntity(SimpleImage image,int entity);

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
        internal extern static void CreateAnimationFromSpriteEntity(int row, int columns, float animation_time,int entity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AttachCamera(float x_offset,float y_offset);

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
        internal extern static int CreateEntitySprite(string str);        
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int CreateEntityVFX(string name, SimpleImage image);

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
        internal extern static void ColliderComponentAdd(int entity,float scaleX,float scaleY);        
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ColliderComponentRemove(int entity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int GetCurrentEntityID();        
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int GetCollidingEntity(int entity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool GetCollidingEntityCheck(int entity, int entityToCheckAgainst);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float GetCollidedObjectAngle(int entity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static SimpleVector2D GetMousePosition();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static SimpleVector2D RigidBodyGetVelocity();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBodySetForce(float x, float y);                
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBodySetBodyTypeEntity(short body_type, int entity);    
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static short RigidBodyGetBodyTypeEntity(int entity);        
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBodySetVelocityEntity(float x, float y,int entity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void DrawLineBetweenPoints(float x1, float y1 ,float x2, float y2, (float, float, float) color);        
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void DrawCircle(float pos_x, float pos_y ,float scale_x, float scale_y, (float, float, float) color);
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void DrawDarkCircle(float pos_x, float pos_y, float scale_x, float scale_y);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void DrawImageAt(SimpleVector2D pos, float rotation, SimpleVector2D scale, SimpleImage image, float alpha, int layer = 1);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void LoadScene(string file_name);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SwitchScene(int scene_id);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Exit();        
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool CollidingObjectIsStatic(int entity);        
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool CollidingObjectIsSpikes(int entity);        
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool CollidingObjectTypeIsGhost(int entity);        
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool CollidingObjectTypeIsIgnore(int entity);
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]        
        internal extern static bool CollidingObjectTypeIsWall(int entity);
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void GameSpawnParticle(float x, float y, string particle_name);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ResetSpriteAnimationFrameEntity(int entity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int GetCurrentAnimationEntity(int entity);//returns the index
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void GameSpawnParticleExtra(float x, float y, float direction, float size, float size_scale, float alpha, float alpha_growth, float lifespan, float speed, string particle_name);        

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void GameSpawnParticleExtraImage(float x, float y, float direction, float sizeX, float sizeY, float size_scale, float alpha, float alpha_growth, float lifespan, float speed, string particle_name, string image_name); 
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int CreateEntityButton(string name, SimpleImage image, string filename);
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int CreateEntityUI(string name, SimpleImage image);
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int GetEntityButtonState(int entity);
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void GamePause(bool pause);
    }
}
