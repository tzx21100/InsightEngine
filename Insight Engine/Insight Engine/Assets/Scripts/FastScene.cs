using System.Runtime.CompilerServices;
namespace IS
{
    class FastScene
    {
        static public string splashScreenVideo = "Assets/Videos/Splashscreen.mov";
        static private float timer;
        static public void Init(){

            timer = 0.2f;
            InternalCalls.SetLightsToggle(false);
            InternalCalls.loadVideo(splashScreenVideo, 1.0f, 1.0f, 0.0f, 0.0f, true);

        }

        static public void Update(){
            float dt = InternalCalls.GetDeltaTime();
            timer -= dt;
            if (timer <= 0.0f)
            {
                InternalCalls.SetLightsToggle(true);
                InternalCalls.unloadVideos();
                int player = InternalCalls.CreateEntityPrefab("Player");
                InternalCalls.TransformSetPositionEntity(0, 0, player);
                InternalCalls.DestroyEntity(InternalCalls.GetCurrentEntityID());
            }
        }
        
        static public void vidUpdate()
        {
            float dt = InternalCalls.GetDeltaTime();
            timer -= dt;
            if (timer <= 0.0f)
            {
                InternalCalls.SetLightsToggle(true);
                InternalCalls.unloadVideos();
            }

        }


        static public void CleanUp(){

        }

    }
}
