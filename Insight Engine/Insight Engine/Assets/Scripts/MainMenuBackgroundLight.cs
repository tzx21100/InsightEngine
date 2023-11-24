using System;
using System.Runtime.CompilerServices;

namespace IS
{
    class MainMenuBackgroundLight
    {
        // Constants
        const int MAX_LIGHTS = 10;
        const float LIGHT_TIMER_INIT = 2f;
        const float MIN_LIGHT_SIZE = 200f;
        const float MAX_LIGHT_SIZE = 5000f;
        const float MIN_LIGHT_INTENSITY = 0.5f;
        const float MAX_LIGHT_INTENSITY = 1f;
        const float SPIRAL_RADIUS = 10f; // Adjust the radius of the spiral
        const float SPIRAL_ANGLE_OFFSET = 0.1f; // Adjust the angle offset for the spiral
        const float DNA_TWIST = 0.1f; // Adjust the twist for the DNA effect

        // Light Entities
        static private int background_lantern;
        static private int[] lightTrail = new int[MAX_LIGHTS];

        // Parameters
        static private float light_timer;
        static private bool is_increasing = true;

        static public void Init()
        {
            background_lantern = InternalCalls.CreateEntityPrefab("Player Lantern Light");
            light_timer = LIGHT_TIMER_INIT;

            // Initialize the lights in the trail
            for (int i = 0; i < MAX_LIGHTS; i++)
            {
                lightTrail[i] = InternalCalls.CreateEntityPrefab("Default Light");
                InternalCalls.SetLightSizeEntity(lightTrail[i], 50f);
            }
        }

        static public void Update()
        {
            light_timer -= InternalCalls.GetDeltaTime();
            if (light_timer <= 0)
            {
                light_timer = LIGHT_TIMER_INIT;
                is_increasing = !is_increasing;
            }

            float t = 1f - (light_timer / LIGHT_TIMER_INIT);
            float lerped_size = is_increasing
                ? Vector2D.lerp(MIN_LIGHT_SIZE, MAX_LIGHT_SIZE, t)
                : Vector2D.lerp(MAX_LIGHT_SIZE, MIN_LIGHT_SIZE, t);
            float lerped_intensity = is_increasing
                ? Vector2D.lerp(MIN_LIGHT_INTENSITY, MAX_LIGHT_INTENSITY, t)
                : Vector2D.lerp(MAX_LIGHT_INTENSITY, MIN_LIGHT_INTENSITY, t);

            InternalCalls.SetLightSizeEntity(background_lantern, lerped_size);

            // Update the positions and intensities of lights in the DNA spiral trail
            for (int i = 0; i < MAX_LIGHTS; i++)
            {
                float angle = i * SPIRAL_ANGLE_OFFSET;
                float radius = SPIRAL_RADIUS * i / MAX_LIGHTS;
                float twist = CustomMath.Sin(angle * DNA_TWIST);

                // Calculate the position in a spiral pattern
                float xPos = radius * CustomMath.Cos(angle);
                float yPos = radius * CustomMath.Sin(angle) * twist;

                // Set the position based on the current mouse position
                InternalCalls.TransformSetPositionEntity(InternalCalls.GetMousePosition().x + xPos,
                                                         InternalCalls.GetMousePosition().y + yPos, lightTrail[i]);

                float trailIntensity = Vector2D.lerp(MIN_LIGHT_INTENSITY, MAX_LIGHT_INTENSITY, i / (float)MAX_LIGHTS);
                InternalCalls.SetLightIntensityEntity(lightTrail[i], trailIntensity);
            }
        }

        static public void CleanUp()
        {
            // Cleanup logic if needed
        }
    }
}
