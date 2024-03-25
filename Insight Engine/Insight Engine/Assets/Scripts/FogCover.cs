using System;
using System.Runtime.CompilerServices;
namespace IS
{
    class FogCover
    {

        static public float fog_width = 100; // Width of the fog area
        static public float fog_height = 100; // Height of the fog area
        static public float fog_density = 10; // Density of the fog, more particles for higher values
        static public float fog_size = 10; // Density of the fog, more particles for higher values
        static public Vector2D fog_position = new Vector2D(0, 0); // Starting position of the fog
        static public float fog_timer = 2f;
        static public float fog_timer_set = 2f;

        static public void Init()
        {
            // Initialization code here, if necessary
        }

        static public void Update()
        {

        }


        static public void CleanUp(){

        }


        static MyRandom randomfloat = new MyRandom(12314);

        static public void SpawnFog()
        {

            if (InternalCalls.GetRandomFloat()+randomfloat.NextFloat() < 0.6f)
            {
                return;
            }

            if(fog_timer > 0)
            {
                fog_timer -= InternalCalls.GetDeltaTime();
                return; 
            }
            fog_timer = fog_timer_set;

            float lifetime = fog_timer_set;
            float direction = InternalCalls.GetRandomFloat()*360; // Direction of the particles, can be randomized for more natural effect
            float size = fog_size; // Size of the particles
            float scale = 2f; // Scale of the particles
            float dash_particle_alpha = 1f; // Alpha transparency of the particles
            float growth = -0.05f; // How the particles grow or shrink over time
            float speed = 100f; // Speed of the particles
            int top_layer = InternalCalls.GetTopLayer();

            // Determine the number of particles to spawn based on the fog density
            int numParticlesX = (int)(fog_width / fog_size)/2;
            int numParticlesY = (int)(fog_height / fog_size)/2;
            numParticlesX= Math.Max(numParticlesX, 1);
            numParticlesY = Math.Max(numParticlesY, 1);
            
            // Loop over the specified area and spawn particles
            for (int x = -numParticlesX; x < numParticlesX; x++)
            {
                for (int y = -numParticlesY; y < numParticlesY; y++)
                {
                    // Calculate the position for each particle
                    float posX = fog_position.x + (x* fog_size);
                    float posY = fog_position.y + (y* fog_size);


                    float loops = randomfloat.NextFloat() * fog_density;
                    loops=Math.Max(loops, 1);
                    // Spawn particle at the calculated position
                    for(int i=0; i < loops; i++) {

                        size = CustomMath.max(fog_size /4f, randomfloat.NextFloat() * size);
                        dash_particle_alpha = CustomMath.max(0.2f, randomfloat.NextFloat());
                        lifetime = CustomMath.max(fog_timer_set, randomfloat.NextFloat() *2f);
                        posX += size * randomfloat.NextFloat();
                        posY += size * randomfloat.NextFloat();
                        InternalCalls.GameSpawnParticleExtraLayerFrames(posX, posY,
                        direction, size, scale, dash_particle_alpha,
                        growth, lifetime, speed, "ParticleDust.txt",
                        (1f, 1f, 1f), top_layer, 0, 4);
                        if (InternalCalls.GetRandomFloat() + randomfloat.NextFloat() < 0.6f)
                        {
                            InternalCalls.GameSpawnParticleExtraFramesFull(posX, posY,
                                            direction, size / 2f, scale, 1f,
                                            0f, lifetime * 1.5f, 0f, "ParticleGlitch.txt"
                                            , 0, 0, direction, 0, 1f, 1f, top_layer,
                                            (1f, 1f, 1f)
                                            );

                        }

                    }

                }
            }
        }




    }
}
