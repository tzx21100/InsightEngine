
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
        internal extern static bool ControllerKeyPressed(int keycode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool ControllerKeyHeld(int keycode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool ControllerKeyReleased(int keycode);        
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool ControllerConnected();

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool MousePressed(int mouseButton);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool MouseHeld(int mouseButton);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool MouseReleased(int mouseButton);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static double GetMouseScrollXOffset();

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static double GetMouseScrollYOffset();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBodyAddForce(float x, float y);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBodyAddForceEntity(float x, float y, int entity);

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
        internal extern static float GetFixedDeltaTime();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetSpriteImage(SimpleImage image);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetSpriteAlphaEntity(float alpha, int entity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetSpriteAlpha(float alpha);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float GetSpriteAlphaEntity(int entity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetSpriteImageEntity(SimpleImage image,int entity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static SimpleImage GetSpriteImage(string filename);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void EmplaceImageToGraphics(SimpleImage image);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetSpriteAnimationIndex(int num);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetStartAnimationEntity(int entity, int start_idx);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetAnimationAlpha(float val);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void FreeSpriteImage(SimpleImage image);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CreateAnimationFromSprite(int row, int columns, float animation_time);        
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CreateAnimationFromSpriteEntity(int row, int columns, float animation_time,int entity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AttachCamera(float x_offset,float y_offset);        
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static SimpleVector2D GetCameraPos();
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float CameraGetZoom();
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float GetCameraAspectRatio();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AudioPlaySound(string name, bool loop,float volume);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AudioPlaySoundBGM(string name, bool loop, float volume);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AudioPlaySoundSFX(string name, bool loop, float volume);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AudioPlayMusic(string name ,float volume);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AudioPlayMusicSFX(string name, float volume);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AudioPlayMusicBGM(string name, float volume);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AudioEmitterEnableEntity(bool enable, int entity);        
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AudioSetMaster(float volume);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AudioSetBGM(float volume);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AudioSetSFX(float volume);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AudioMuteMaster(bool mute);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AudioMuteBGM(bool mute);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AudioMuteSFX(bool mute);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool AudioIsMasterMute();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool AudioIsBGMMute();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool AudioIsSFXMute();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ResetAnimations();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ResetAnimationFrames(int entity, int start_index, int end_index);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int GetButtonState();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ButtonComponentAdd(int entity, float idle_alpha, float hover_alpha, float click_alpha, float idle_scale, float hover_scale);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ButtonComponentRemove(int entity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool ButtonComponentExists(int entity);

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

        /*[MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static Array GetCollidingEntityList(int entity);*/

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool GetCollidingEntityCheck(int entity, int entityToCheckAgainst);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float GetCollidedObjectAngle(int entity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static SimpleVector2D GetMousePosition();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static SimpleVector2D RigidBodyGetVelocity();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float RigidBodyGetVelocityX();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float RigidBodyGetVelocityY();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBodySetForce(float x, float y);               
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBodySetForceX(float x);                
        
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
        internal extern static void DrawSquare(float pos_x, float pos_y, float scale_x, float scale_y, float colx, float coly, float colz, float cola, int layer);


        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void DrawDarkCircle(float pos_x, float pos_y, float scale_x, float scale_y);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void DrawImageAt(SimpleVector2D pos, float rotation, SimpleVector2D scale, SimpleImage image, float alpha, int layer = 1);        
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void DrawImageExtraAt(int rowIndex, int columnIndex, int totalRows, int totalCols, SimpleVector2D  pos, float rotation, SimpleVector2D  scale, SimpleImage texture, float alpha, int layer=1);

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
        internal extern static bool CheckMouseIntersectEntity(int entity);
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]        
        internal extern static bool CollidingObjectTypeIsWall(int entity);
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void GameSpawnParticle(float x, float y, string particle_name);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void GameSpawnParticleFrames(float x, float y, int col, int row, string particle_name);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ResetSpriteAnimationFrameEntity(int entity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int GetCurrentAnimationEntity(int entity);//returns the index
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void GameSpawnParticleExtra(float x, float y, float direction, float size, float size_scale, float alpha, float alpha_growth, float lifespan, float speed, string particle_name);                    
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void GameSpawnParticleExtraLayer(float x, float y, float direction, float size, float size_scale, float alpha, float alpha_growth, float lifespan, float speed, string particle_name, (float, float, float) color,int layer);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void GameSpawnParticleExtraLayerFrames(float x, float y, float direction, float size, float size_scale, float alpha, float alpha_growth, float lifespan, float speed, string particle_name, (float, float, float) color, int layer ,int col, int row);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void GameSpawnParticleExtraFrames(float x, float y, float direction, float size, float size_scale, float alpha, float alpha_growth, float lifespan, float speed, string particle_name,int col, int rows);
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void GameSpawnParticleExtraFramesRotate(float x, float y, float direction, float size, float size_scale, float alpha, float alpha_growth, float lifespan, float speed, string particle_name, int col, int rows,float rotate);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void GameSpawnParticleExtraFramesFull(float x, float y, float direction, float size, float size_scale, float alpha, float alpha_growth, float lifespan, float speed, string particle_name, int col, int row, float rotate, int particle_effect, float particle_effect_timer, float rotation_speed, int layer, (float, float, float) color);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void GameSpawnParticleExtraImage(float x, float y, float direction, float sizeX, float sizeY, float size_scale, float alpha, float alpha_growth, float lifespan, float speed, string particle_name, string image_name); 
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int CreateEntityButton(string name, SimpleImage image, string filename, string text);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int CreateEntityButtonNoText(string name, SimpleImage image, string filename);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int CreateEntityUI(string name, SimpleImage image);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int CreateEntityUIScript(string name, SimpleImage image, string filename);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int GetEntityButtonState(int entity);
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void GamePause(bool pause);
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int GetTitleBarHeight();
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool IsFullscreen();
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ToggleFullscreen();
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ShowCursor();
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void HideCursor();
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int ButtonRenderText(int entity, float x, float y, float size, (float, float, float) color);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int RenderText(string text, float x, float y, float size, (float, float, float) color);        
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int RenderTextFont(string text, string font, float x, float y, float size, (float, float, float, float) color);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int RenderLeftAlignTextFont(string text, string font, float x, float y, float size, (float, float, float, float) color);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int loadVideo(string file_path, float widthScalar, float heightScalar, float xPosScalar, float yPosScalar, bool loop);
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int pauseVideo(int index);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int resumeVideo(int index);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int restartVideo(int index);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int unloadVideos();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int SetButtonSizeScale(int entity, float scale);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int SetButtonSize(int entity, SimpleVector2D size);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int SetButtonHoverScale(int entity, float scale); 
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int SetButtonIdleAlpha(int entity, float alpha);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int GetButtonHoverScale(int entity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int GetButtonIdleScale(int entity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int GetWindowWidth();
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int GetWindowHeight();
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetWindowSize(int width, int height);
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int GetTargetFPS();
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetTargetFPS(int fps);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int GetMonitorWidth();
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int GetMonitorHeight();
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool IsWindowFocused();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AttachLightComponentToEntity(int entity, float colorX, float colorY, float colorZ, float Intensity, float size);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AttachLightComponentToEntityWithOffset(int entity, float colorX, float colorY, float colorZ, float Intensity, float size, SimpleVector2D offset);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetLightComponentToEntity(int entity, float colorX,float colorY,float colorZ, float Intensity, float size);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetLightIntensityEntity(int entity, float Intensity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float GetLightIntensityEntity(int entity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetLightSizeEntity(int entity, float size);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float GetLightSizeEntity(int entity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetLightToggleEntity(int entity, bool toggle);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetLightHueEntity(int entity, float r, float g, float b);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static (float, float, float) GetLightHueEntity(int entity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int CreateEntityPrefab(string prefab_name);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void FollowMouseCursorEntity(int entity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float GetRandomFloat();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool IsLightRenderingEntity(int entity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float MathSin(float val);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float MathCos(float val);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float MathAtan(float val);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float MathAtan2(float x,float y);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float MathSqrt(float val);        
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void GlitchEnable(bool set);
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetLightsToggle(bool toogle);        
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int CloneEntity(int entity);
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AudioStopAllSounds();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool CompareCategory(string str);
            
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool CompareEntityCategory(int entity_id, string str);        
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool CheckEntityCategory(int entity_id, string category_to_check_its_in);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool OnCollisionEnter();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool OnEntityCollisionEnter(int entity_id);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float GetGravityScale();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetGravityScale(float scale);
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetEntityGravityScale(float scale, int entity_id);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetStatic();
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetStaticEntity(int entity_id);
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void UpdateCategory(int entity, string category);        
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SendCondition(bool condition_return_bool);        
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ChangeLightType(int light_type );

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ChangeShaderIntensity(float intensity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetLayerVisible(int layer, bool visible );        
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int GetTopLayer();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int GetCollidingEnemyEntity(int entity);
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RemoveColliderComponentEntity(int entity);
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void DrawNonEnityAnimation(float deltaTime, SimpleVector2D pos, float rotation, SimpleVector2D scale, SimpleImage texture, float alpha, int layer );

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void FadeOutAudio(float fade_time);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetCircleColliderOffsetX(float x);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetAnimationEntityPlaying(int entity, bool toggle);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void DeleteSimpleArray(SimpleArray array);        
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static SimpleArray GetCollidingEntityArray(int entity);        
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBodySetMassEntity(float mass, int entity);        
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBodyGetMassEntity(int entity);        
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float GetLeftTriggerY();        
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float GetLeftTriggerX();


    }
}
