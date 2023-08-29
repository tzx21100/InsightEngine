#ifndef GAM200_INSIGHT_ENGINE_SOURCE_COREENGINE_H_
#define GAM200_INSIGHT_ENGINE_SOURCE_COREENGINE_H_


#include "System.h"
#include <vector>

//the engine that handles all the main functions like loading assets, systems, gameplay loop and destroying them.
namespace Zx_Framework {
    //core engine will manage all systems in the game
    class InsightEngine{
        public:
            InsightEngine();//constructor
            ~InsightEngine();//destructor
            void addSystem(ParentSystem *system); //adds a system to the game
            void destroySystem(ParentSystem*system); //removes a system from game
            void destroyAllSystem();//destroys all virtual systems
            void initializeAllSystem();//inits all systems
            void initSystem(ParentSystem* system);//initializes one specific system
        private:
            bool is_running = false;
            std::vector<ParentSystem> all_systems;
            unsigned last_runtime;  
    
    };

}




#endif //GAM200_INSIGHT_ENGINE_SOURCE_COREENGINE_H_
