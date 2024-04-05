using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace IS
{

    class MainMenuBackgroundLight
    {
        // Constants
        const float LIGHT_TIMER_INIT = 2f;
        static float MIN_LIGHT_SIZE = InternalCalls.GetWindowWidth() * 0.025f;
        static float MAX_LIGHT_SIZE = InternalCalls.GetWindowWidth() * 0.75f;
        static float LIGHT_XOFFSET = .7f;
        static float LIGHT_YOFFSET = .22f;

        // Light Entities
        static private int mBackgroundLantern;

        // Parameters
        static private float mBackgroundLanternTimer;
        static private bool mIncreasing = true;

        static public void Init()
        {
            mBackgroundLantern = InternalCalls.CreateEntityPrefab("Player Lantern Light");
            mBackgroundLanternTimer = LIGHT_TIMER_INIT;
        }

        static public void Update()
        {
            // Offset light position
            int width = InternalCalls.GetWindowWidth();
            int height = InternalCalls.GetWindowHeight();
            InternalCalls.TransformSetPositionEntity(LIGHT_XOFFSET * (width / 2f), LIGHT_YOFFSET * (height / 2f), mBackgroundLantern);

            // Toggle fullscreen mode
            PauseButtonScript.ToggleFullscreenMode();

            // Update light timer logic
            mBackgroundLanternTimer -= InternalCalls.GetDeltaTime();
            if (mBackgroundLanternTimer <= 0)
            {
                mBackgroundLanternTimer = LIGHT_TIMER_INIT;
                mIncreasing = !mIncreasing;
            }

            // Calculate lerped values for light size and intensity
            float t = 1f - (mBackgroundLanternTimer / LIGHT_TIMER_INIT);
            float lerpedSize = mIncreasing
                ? Vector2D.lerp(MIN_LIGHT_SIZE, MAX_LIGHT_SIZE, t)
                : Vector2D.lerp(MAX_LIGHT_SIZE, MIN_LIGHT_SIZE, t);

            // Set light size based on lerped value
            InternalCalls.SetLightSizeEntity(mBackgroundLantern, lerpedSize);
        }

        static public void CleanUp()
        {
            // Cleanup logic if needed
        }
    }
}
