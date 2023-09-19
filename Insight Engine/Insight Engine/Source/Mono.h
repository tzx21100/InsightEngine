#pragma once
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

void init() {
    mono_set_assemblies_path("mono/lib");
}