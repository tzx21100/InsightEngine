#pragma once
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

void init() {
    mono_set_assemblies_path("libraries/mono/lib/4.5");

    MonoDomain* rootDomain = mono_jit_init("MyScriptRuntime");
    if (rootDomain == nullptr)
    {
        // Maybe log some error here
        return;
    }

    // Store the root domain pointer
    MonoDomain*  s_RootDomain = rootDomain;

}