#ifndef GAM200_INSIGHT_ENGINE_SOURCE_COREENGINE_H_
#define GAM200_INSIGHT_ENGINE_SOURCE_COREENGINE_H_

#include "System.h"
#include <unordered_map>
#include <chrono>
#include "EventManager.h"

namespace IS {
    class InsightEngine {
    public:
        InsightEngine();
        ~InsightEngine();
        void Run();
        void AddSystem(ParentSystem* system);
        void DestroySystem(const std::string& name);
        void DestroyAllSystems();
        void InitializeAllSystems();
        void BroadcastMessage(Message* message);
        void SetFPS(int num);
    private:
        //putting this here as a hard cap to fps, could move it to public as well
        void LimitFPS(const std::chrono::high_resolution_clock::time_point& frameStart);
        //this will be the only event manager the system refers to
        EventManager eventManager;
        bool is_running;
        std::unordered_map<std::string, ParentSystem*> all_systems;
        unsigned last_runtime;
        int targetFPS{60};
    };
}

#endif // GAM200_INSIGHT_ENGINE_SOURCE_COREENGINE_H_
