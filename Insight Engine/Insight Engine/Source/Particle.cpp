#include "Pch.h"

namespace IS {
    std::string Particle::GetName() {
        return "Particle";
    }

    void Particle::Initialize() {
        //Subscirbe to messages
        Subscribe(MessageType::DebugInfo);
    }

    void Particle::Update([[maybe_unused]] float deltaTime) {

    }

    void Particle::HandleMessage(const Message& message) {
        if (message.GetType() == MessageType::DebugInfo) {
            // Handle collision logic here
            //IS_CORE_INFO("Handling Debug");
        }
    }

    //void Particle::InitParticles(){
    //    int max_particles = 1000;

    //    for (int i = 0; i < max_particles; ++i) {
    //        ParticleEmitter particle;

    //        //init with default values
    //        particle.mVelocity = Vec2D(0, 0);
    //        particle.mSize = 0.f;
    //        particle.mLifespan = 0.f;
    //        particle.mColor = Color::BLACK;
    //        particle.mAlpha = 0.f;
    //        particle.mEmissionRate = 0.f;
    //        particle.mTimeSinceLastEmission = 0.f;

    //        mParticles.emplace_back(particle);
    //    }
    //}

    void Particle::EmitParticles(const Vec2D& velocity, const float& size, const float& lifespan, const Color& color, const float& alpha, const float& emission_rate){
        for (auto& particle : mParticles) {
            particle->mVelocity = velocity;
            particle->mSize = size;
            particle->mLifespan = lifespan;
            particle->mColor = color;
            particle->mAlpha = alpha;
            particle->mEmissionRate = emission_rate;

            particle->mTimeSinceLastEmission = 0.f;

            //StoreParticles(particle);
        }
    }

    void Particle::EmitParticlesRandomValues(const Vec2D& min_velocity, const Vec2D& max_velocity, const float& min_size, const float& max_size, const float& min_lifespan, const float& max_lifespan, 
                                             const float& min_alpha, const float& max_alpha, const float& min_emission_rate, const float& max_emission_rate){
        for (auto& particle : mParticles) {
            particle->mVelocity = RandomVelocity(min_velocity,max_velocity);
            particle->mSize = RandomSize(min_size,max_size);
            particle->mLifespan = RandomLifespan(min_lifespan,max_lifespan);
            particle->mColor = RandomColor();
            particle->mAlpha = RandomAlpha(min_alpha,max_alpha);
            particle->mEmissionRate = RandomEmissionRate(min_emission_rate,max_emission_rate);

            particle->mTimeSinceLastEmission = 0.f;

            //StoreParticles(particle);
        }
    }

    Vec2D Particle::RandomVelocity(const Vec2D& min_velocity, const Vec2D& max_velocity){
        return Vec2D(RandomFloatValue(min_velocity.x, max_velocity.x), RandomFloatValue(min_velocity.y, max_velocity.y));
    }

    float Particle::RandomSize(const float& min_size, const float& max_size){
        return RandomFloatValue(min_size, max_size);
    
    }

    float Particle::RandomLifespan(const float& min_lifespan, const float& max_lifespan){
        return RandomFloatValue(min_lifespan, max_lifespan);
       
    }

    Color Particle::RandomColor(){
        return RandomColor();
      
    }

    float Particle::RandomAlpha(const float& min_alpha, const float& max_alpha){
        return RandomFloatValue(min_alpha, max_alpha);
    }

    float Particle::RandomEmissionRate(const float& min_emission_rate, const float& max_emission_rate){
        return RandomFloatValue(min_emission_rate, max_emission_rate);
      
    }


    //void Particle::StoreParticles(Entity& entity,const ParticleEmitter& particle){
       /* auto& engine = InsightEngine::Instance();

        if (engine.HasComponent<ParticleEmitter>(entity)){
            
        }*/
    //}

    void Particle::ClearParticles(){
        mParticles.clear();
    }


}