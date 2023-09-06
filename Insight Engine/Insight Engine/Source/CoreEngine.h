#ifndef GAM200_INSIGHT_ENGINE_SOURCE_COREENGINE_H_
#define GAM200_INSIGHT_ENGINE_SOURCE_COREENGINE_H_

#include "System.h"
#include <unordered_map>
#include <chrono>

namespace IS {
    class InsightEngine: public MessageListener{
    public:

        //override message handling this way the core engine also will recieve and send messages
        virtual void HandleMessage(const Message& message) override;

        //this is to instatiate only ONE engine
        static InsightEngine& Instance() {
            static InsightEngine instance;
            return instance;
        }
        void Run();
        void AddSystem(ParentSystem* system);
        void DestroySystem(const std::string& name);
        void DestroyAllSystems();
        void InitializeAllSystems();
        void SetFPS(int num);

        //eventmanager
        void sendMessage(Message* message) { EventManager::Instance().Broadcast(*message); };
        void Subscribe(MessageType type) { EventManager::Instance().Subscribe(type, this); };

    private:
        //putting this here as a hard cap to fps, could move it to public as well
        std::chrono::high_resolution_clock::time_point LimitFPS(const std::chrono::high_resolution_clock::time_point& frameStart);
        bool is_running;
        std::unordered_map<std::string, ParentSystem*> all_systems;
        unsigned last_runtime;
        int targetFPS{60};
        //follow the singleton pattern for only one engine
        InsightEngine();
        ~InsightEngine();


    };
}

#endif // GAM200_INSIGHT_ENGINE_SOURCE_COREENGINE_H_
