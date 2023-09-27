///////////////////////////////////////////////////////////////////////////////////////
//
//	Main
//	The main entry point for the game--everything starts here.
//	
//	Authors:  Tan Zheng Xun
//	Copyright 2023, Digipen Institute of Technology
//
///////////////////////////////////////////////////////////////////////////////////////
//pch has to go to the top of every cpp
#include "Pch.h"
#include "InsightEngine.h"
#include "MemoryLeakCheck.h"

using namespace IS;

int main() {
    // Enable memory leaks check
    ENABLE_MEMORY_CHECK();
    // Initialize ScriptEngine
    //ScriptEngine::Init();
    // Initialize log
    Log::Init();
    // Run the engine
    RunInsightEngine();

    return 0;
}

