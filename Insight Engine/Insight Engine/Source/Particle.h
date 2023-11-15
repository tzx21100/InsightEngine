/*                                                                   guard
----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_SOURCE_PARTICLE_H
#define GAM200_INSIGHT_ENGINE_SOURCE_PARTICLE_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "System.h"
#include "ParticleEmitter.h"

namespace IS {

    class Particle : public ParentSystem {
    public:
        //override parent
        void Initialize() override;
        void Update(float deltaTime) override;
        std::string GetName() override;
        void HandleMessage(const Message& message) override;

        // void InitParticles(); //Set Default Values
        void EmitParticles(const Vec2D& velocity, const float& size, const float& lifespan, const Color& color, const float& alpha, const float& emmision_rate);
        void EmitParticlesRandomValues(const Vec2D& min_velocity, const Vec2D& max_velocity, const float& min_size, const float& max_size, const float& min_lifespan, const float& max_lifespan,
                                       const float& min_alpha, const float& max_alpha, const float& min_emission_rate, const float& max_emission_rate);
        
        Vec2D RandomVelocity(const Vec2D& min_velocity, const Vec2D& max_velocity);
        float RandomSize(const float& min_size, const float& max_size);
        float RandomLifespan(const float& min_lifespan, const float& max_lifespan);
        Color RandomColor();
        float RandomAlpha(const float& min_alpha, const float& max_alpha);
        float RandomEmissionRate(const float& min_emission_rate, const float& max_emission_rate);

        float RandomFloatValue(float min, float max) {
            float random = ((float)rand()) / (float)RAND_MAX;
            float range = max - min;
            return (random * range) + min;
        }

        void ClearParticles();

        //void StoreParticles(Entity& entity, const ParticleEmitter& particle);

    private:
        std::vector<ParticleEmitter*> mParticles{};
        std::unordered_map<Entity, ParticleEmitter*> mParticlesStorage{};
    };
}


#endif // GAM200_INSIGHT_ENGINE_SOURCE_PARTICLE_H  