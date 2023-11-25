using System.Runtime.CompilerServices;

namespace IS
{
    class ParticleEmitter
    {
        const int MAX_PARTICLES = 10;
        const float PARTICLE_BASE_SPEED = 50f;
        const float PARTICLE_SPEED_VARIATION = 100f;
        const float PARTICLE_LIFE_TIME = 5f;

        private int[] mParticles;
        private float[] mParticleLifetimes;
        private int mCurrentParticleIndex;

        public ParticleEmitter()
        {
            mParticles = new int[MAX_PARTICLES];
            mParticleLifetimes = new float[MAX_PARTICLES];
            mCurrentParticleIndex = 0;

            // Initialize particles
            for (int i = 0; i < MAX_PARTICLES; i++)
            {
                mParticles[i] = InternalCalls.CreateEntityPrefab("Default Light");
                mParticleLifetimes[i] = 0f;
            }
        }

        public void Emit(float x, float y, float lerpedIntensity)
        {
            // Activate and reposition an existing particle
            InternalCalls.SetLightToggleEntity(mParticles[mCurrentParticleIndex], true);
            InternalCalls.TransformSetPositionEntity(x, y, mParticles[mCurrentParticleIndex]);
            mParticleLifetimes[mCurrentParticleIndex] = 0f;

            // Increment the particle index for the next frame
            mCurrentParticleIndex = (mCurrentParticleIndex + 1) % MAX_PARTICLES;

            // Update the positions and intensities of the emitted particle
            for (int i = 0; i < MAX_PARTICLES; i++)
            {
                if (InternalCalls.IsLightRenderingEntity(mParticles[i]))
                {
                    float particleSpeed = PARTICLE_BASE_SPEED + GetRandomFloat(-PARTICLE_SPEED_VARIATION, PARTICLE_SPEED_VARIATION);

                    float xPos = InternalCalls.GetTransformPositionEntity(mParticles[i]).x;
                    float yPos = InternalCalls.GetTransformPositionEntity(mParticles[i]).y + particleSpeed * InternalCalls.GetDeltaTime();

                    // Calculate distance from the cursor
                    Vector2D mouse = new Vector2D(x, y);
                    float distanceToCursor = Vector2D.Distance(new Vector2D(xPos, yPos), mouse);

                    // Calculate intensity based on distance
                    float distanceIntensityFactor = 1f - (distanceToCursor / InternalCalls.GetWindowWidth()); // Adjust based on your screen width
                    float intensity = lerpedIntensity * distanceIntensityFactor;

                    // Calculate opacity reduction factor
                    float opacityReductionFactor = GetRandomFloat(0.3f, 0.8f); // Varying opacity for each particle

                    // Reset the particle position and life timer when it goes off-screen
                    if (mParticleLifetimes[i] > PARTICLE_LIFE_TIME)
                    {
                        InternalCalls.SetLightToggleEntity(mParticles[i], false); // Deactivate particle
                        mParticleLifetimes[i] = 0f;
                    }

                    InternalCalls.TransformSetPositionEntity(xPos, yPos, mParticles[i]);

                    // Reduce the opacity based on the particle's life time
                    float opacity = 1f - (mParticleLifetimes[i] / PARTICLE_LIFE_TIME) * opacityReductionFactor;
                    opacity = CustomMath.clamp(opacity, 0f, 0.6f);
                    InternalCalls.SetLightIntensityEntity(mParticles[i], intensity * opacity);

                    // Update the life timer
                    mParticleLifetimes[i] += InternalCalls.GetDeltaTime();
                }
            }
        }

        // Placeholder method for getting a random float within a range
        private float GetRandomFloat(float min, float max)
        {
            return min + InternalCalls.GetRandomFloat() * (max - min);
        }
    }

    class MainMenuBackgroundLight
    {
        // Constants
        const float LIGHT_TIMER_INIT = 2f;
        const float MIN_LIGHT_INTENSITY = 0.5f;
        const float MAX_LIGHT_INTENSITY = 1f;
        static float MIN_LIGHT_SIZE = InternalCalls.GetWindowWidth() * 0.05f;
        static float MAX_LIGHT_SIZE = InternalCalls.GetWindowWidth() * 2.5f;

        // Light Entities
        static private int mBackgroundLantern;
        static private ParticleEmitter? mParticleEmitter; // Nullable reference type

        // Parameters
        static private float mBackgroundLanternTimer;
        static private bool mIncreasing = true;

        static public void Init()
        {
            mBackgroundLantern = InternalCalls.CreateEntityPrefab("Player Lantern Light");
            mBackgroundLanternTimer = LIGHT_TIMER_INIT;
            mParticleEmitter = new ParticleEmitter();
        }

        static public void Update()
        {
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
            float lerpedIntensity = mIncreasing
                ? Vector2D.lerp(MIN_LIGHT_INTENSITY, MAX_LIGHT_INTENSITY, t)
                : Vector2D.lerp(MAX_LIGHT_INTENSITY, MIN_LIGHT_INTENSITY, t);

            // Set light size based on lerped value
            InternalCalls.SetLightSizeEntity(mBackgroundLantern, lerpedSize);

            // Emit particles every frame
            if (mParticleEmitter != null)
            {
                mParticleEmitter.Emit(InternalCalls.GetMousePosition().x, InternalCalls.GetMousePosition().y, lerpedIntensity);
            }
        }

        static public void CleanUp()
        {
            // Cleanup logic if needed
        }
    }
}
