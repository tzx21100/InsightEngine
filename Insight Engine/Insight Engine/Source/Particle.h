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

    private:
       
    };
}


#endif // GAM200_INSIGHT_ENGINE_SOURCE_PARTICLE_H  