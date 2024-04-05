using System;
using System.Runtime.CompilerServices;
namespace IS
{
    class CursorLight
    {
        private const float LIGHT_TIMER_INIT = 2f;
        public const float MIN_LIGHT_INTENSITY = 0.5f;
        public const float MAX_LIGHT_INTENSITY = 1f;

        static private ParticleEmitter mParticleEmitter;

        static private float lastMouseX;
        static private float lastMouseY;

        static private float mTimer;
        static private bool mIncreasing = true;

        static public void Init()
        {
            mParticleEmitter = new ParticleEmitter();
            lastMouseX = InternalCalls.GetMousePosition().x;
            lastMouseY = InternalCalls.GetMousePosition().y;
            mTimer = LIGHT_TIMER_INIT;
        }

        static public void Update()
        {
            mTimer -= InternalCalls.GetDeltaTime();
            if (mTimer <= 0)
            {
                mTimer = LIGHT_TIMER_INIT;
                mIncreasing = !mIncreasing;
            }

            float mouseX = InternalCalls.GetMousePosition().x;
            float mouseY = InternalCalls.GetMousePosition().y;
            float dx = mouseX - lastMouseX;
            float dy = mouseY - lastMouseY;
            float mouseMovement = dx * dx + dy * dy;
            lastMouseX = mouseX;
            lastMouseY = mouseY;
            ParticleEmitter.particleBaseSpeed = (mouseMovement < ParticleEmitter.MOUSE_MOVEMENT_THRESHOLD)
                ? ParticleEmitter.PARTICLE_FAST_SPEED : ParticleEmitter.PARTICLE_SLOW_SPEED;

            float t = 1f - (mTimer / LIGHT_TIMER_INIT);

            float lerpedIntensity = mIncreasing
                ? Vector2D.lerp(MIN_LIGHT_INTENSITY, MAX_LIGHT_INTENSITY, t)
                : Vector2D.lerp(MAX_LIGHT_INTENSITY, MIN_LIGHT_INTENSITY, t);

            // Emit particles every frame
            if (mParticleEmitter != null)
            {
                mParticleEmitter.Emit(InternalCalls.GetMousePosition().x, InternalCalls.GetMousePosition().y, lerpedIntensity);
            }
        }
        
        static public void CleanUp(){

        }
    }

    class ParticleEmitter
    {
        private const int MAX_PARTICLES = 10;
        public const float PARTICLE_FAST_SPEED = 150f;
        public const float PARTICLE_SLOW_SPEED = 10f;
        public const float MOUSE_MOVEMENT_THRESHOLD = 100f;
        static public float particleBaseSpeed = PARTICLE_FAST_SPEED;
        private const float PARTICLE_SPEED_VARIATION = 10f;
        private const float PARTICLE_LIFE_TIME = 5f;

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
            // InternalCalls.TransformSetPositionEntity(x, y, mParticles[mCurrentParticleIndex]);
            // Define the emission area size
            const int EMISSION_AREA_SIZE = 32;

            // Generate a random offset within the emission area
            Random random = new Random();
            int offsetX = random.Next(0, EMISSION_AREA_SIZE);
            int offsetY = random.Next(0, EMISSION_AREA_SIZE);

            // Add the offset to the x and y coordinates
            float newX = x + offsetX;
            float newY = y - offsetY;

            // Set the new position
            InternalCalls.TransformSetPositionEntity(newX, newY, mParticles[mCurrentParticleIndex]);
            mParticleLifetimes[mCurrentParticleIndex] = 0f;

            // Increment the particle index for the next frame
            mCurrentParticleIndex = (mCurrentParticleIndex + 1) % MAX_PARTICLES;

            // Update the positions and intensities of the emitted particle
            for (int i = 0; i < MAX_PARTICLES; i++)
            {
                if (InternalCalls.IsLightRenderingEntity(mParticles[i]))
                {
                    float particleSpeed = particleBaseSpeed + GetRandomFloat(-PARTICLE_SPEED_VARIATION, PARTICLE_SPEED_VARIATION);

                    float xPos = InternalCalls.GetTransformPositionEntity(mParticles[i]).x + particleSpeed * InternalCalls.GetDeltaTime();
                    float yPos = InternalCalls.GetTransformPositionEntity(mParticles[i]).y + particleSpeed * InternalCalls.GetDeltaTime();

                    // Calculate distance from the cursor
                    Vector2D mouse = new Vector2D(x, y);
                    float distanceToCursor = Vector2D.Distance(new Vector2D(xPos, yPos), mouse);

                    // Calculate intensity based on distance
                    float distanceIntensityFactor = 1f - (distanceToCursor / InternalCalls.GetWindowWidth()); // Adjust based on your screen width
                    float intensity = lerpedIntensity * distanceIntensityFactor;

                    // Calculate opacity reduction factor
                    float opacityReductionFactor = GetRandomFloat(0.1f, 0.8f); // Varying opacity for each particle

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

}
