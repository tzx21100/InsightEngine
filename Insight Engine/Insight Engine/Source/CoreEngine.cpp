#include "CoreEngine.h"


namespace Zx_Framework {

    class InsightEngine {
    public:
        InsightEngine() {
            is_running = false;
            last_runtime = 0;
        }

        ~InsightEngine() {
            destroyAllSystem();
        }

        void addSystem(ParentSystem* system) {
            all_systems.push_back(system);
        }

        void destroySystem(ParentSystem* system) {
            // Remove and delete a specific system
            auto it = std::find(all_systems.begin(), all_systems.end(), system);
            if (it != all_systems.end()) {
                delete* it;
                all_systems.erase(it);
            }
        }

        void destroyAllSystem() {
            for (ParentSystem* system : all_systems) {
                delete system;
            }
            all_systems.clear();
        }

        void initializeAllSystem() {
            for (ParentSystem* system : all_systems) {
                initSystem(system);
            }
        }

        void initSystem(ParentSystem* system) {
            // Initialize a specific system
            system->initializeSystem();
        }

        void run() {
            is_running = true;
            //last_runtime = timeGetTime();

            while (is_running) {
               // unsigned currenttime = timeGetTime();
                //float dt = (currenttime - last_runtime) / 1000.0f;
                //last_runtime = currenttime;
                for (ParentSystem* system : all_systems) {
                    //system->updateSystem(dt);
                }
            }
        }

        void broadcastMessage(Message* message) {
            if (message->MessageId == aMessageIdType::Quit) {
                is_running = false;
            }

            for (ParentSystem* system : all_systems) {
                system->sendMessage(message);
            }
        }

    private:
        bool is_running;
        std::vector<ParentSystem*> all_systems;
        unsigned last_runtime;
    };

} // namespace Zx_Framework