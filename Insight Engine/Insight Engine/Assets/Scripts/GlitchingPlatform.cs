using System.Runtime.CompilerServices;
namespace IS
{

    class GlitchingPlatform
    {
        static Dictionary<int, GlitchedPlatforms> platforms = new Dictionary<int, GlitchedPlatforms>();

        static public void Init(){
            int entity_id = InternalCalls.GetCurrentEntityID();
            platforms[entity_id] = new GlitchedPlatforms();
        }

        static public void Update(){
            int entity_id = InternalCalls.GetCurrentEntityID();
            platforms[entity_id].update(); // Call Update on each GlitchingPlatform instance
           

        }



        static public void CleanUp(){
            InternalCalls.SetSpriteImageEntity(InternalCalls.GetSpriteImage("glitched_platform_vfx 2R6C.png"), InternalCalls.GetCurrentEntityID());
        }

    }


    class GlitchedPlatforms {

        private float TimeOutTimer = 4f;
        private float TimeOutTimerSet = 4f;
        private float RespawnTimerSet = 2f;
        private float RespawnTimer = 2f;
        public void update()
        {
            SimpleImage tra = InternalCalls.GetSpriteImage("transparent.png");
            SimpleImage tra2 = InternalCalls.GetSpriteImage("glitched_platform_vfx 2R6C.png");
            int entity_id = InternalCalls.GetCurrentEntityID();
            TimeOutTimer -= InternalCalls.GetDeltaTime();
            if (TimeOutTimer < 0)
            {
                InternalCalls.ColliderComponentRemove(entity_id);
                InternalCalls.SetSpriteImageEntity(tra,entity_id);
                RespawnTimer -= InternalCalls.GetDeltaTime();
            }

            if (RespawnTimer < 0)
            {
                InternalCalls.ColliderComponentAdd(entity_id, 0.6f, 0.5f);
                InternalCalls.SetSpriteImageEntity(tra2,entity_id);
                TimeOutTimer = TimeOutTimerSet;
                RespawnTimer = RespawnTimerSet;
            }

        }

    }


}
