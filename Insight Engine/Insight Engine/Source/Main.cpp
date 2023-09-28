///////////////////////////////////////////////////////////////////////////////////////
//
//	Main
//	The main entry point for the game engine--everything starts here.
// : )
//	
//	Authors:  Tan Zheng Xun
//	Copyright 2023, Digipen Institute of Technology
//
///////////////////////////////////////////////////////////////////////////////////////
#include "Pch.h"
#include "InsightEngine.h"
#include "MemoryLeakCheck.h"

int main() {
    // Enable memory leaks check
    ENABLE_MEMORY_CHECK();
    // Run the engine
    RunInsightEngine();
}

