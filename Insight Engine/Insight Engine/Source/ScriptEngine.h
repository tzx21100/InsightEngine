#ifndef GAM200_INSIGHT_ENGINE_SOURCE_SCRIPTENGINE_H
#define GAM200_INSIGHT_ENGINE_SOURCE_SCRIPTENGINE_H

namespace IS {
    class ScriptEngine {
    public:
        static void Init();
        static void Shutdown();

    private:
        static void InitMono();

        static void ShutdownMono();

    };
}

#endif // GAM200_INSIGHT_ENGINE_SOURCE_SCRIPTENGINE_H  