#ifndef ANDROID_MOD_MENU_MACROS_H
#define ANDROID_MOD_MENU_MACROS_H

// thanks to shmoo and joeyjurjens for the usefull stuff under this comment.

#include <dobby.h>

#define HOOK(offset, ptr, orig) DobbyHook((void *)getAbsoluteAddress(targetLibName, string2Offset(OBFUSCATE(offset))), (void *)ptr, (void **)&orig)
#define HOOK_LIB(lib, offset, ptr, orig) DobbyHook((void *)getAbsoluteAddress(OBFUSCATE(lib), string2Offset(OBFUSCATE(offset))), (void *)ptr, (void **)&orig)

#define HOOK_NO_ORIG(offset, ptr) DobbyHook((void *)getAbsoluteAddress(targetLibName, string2Offset(OBFUSCATE(offset))), (void *)ptr, NULL)
#define HOOK_LIB_NO_ORIG(lib, offset, ptr) DobbyHook((void *)getAbsoluteAddress(OBFUSCATE(lib), string2Offset(OBFUSCATE(offset))), (void *)ptr, NULL)

#define HOOKSYM(sym, ptr, org) DobbyHook(dlsym(dlopen(targetLibName, 4), OBFUSCATE(sym)), (void *)ptr, (void **)&org)
#define HOOKSYM_LIB(lib, sym, ptr, org) DobbyHook(dlsym(dlopen(OBFUSCATE(lib), 4), OBFUSCATE(sym)), (void *)ptr, (void **)&org)

#define HOOKSYM_NO_ORIG(sym, ptr)  DobbyHook(dlsym(dlopen(targetLibName, 4), OBFUSCATE(sym)), (void *)ptr, NULL)
#define HOOKSYM_LIB_NO_ORIG(lib, sym, ptr)  DobbyHook(dlsym(dlopen(OBFUSCATE(lib), 4), OBFUSCATE(sym)), (void *)ptr, NULL)

// Obfuscate offset
#define OBFUSCATEOFFSET(str) string2Offset(OBFUSCATE(str)) // Encrypt offset

// Patching a offset without switch.
void patchOffset(const char *fileName, uint64_t offset, std::string hexBytes) {
    MemoryPatch patch = MemoryPatch::createWithHex(fileName, offset, hexBytes);
    if(!patch.isValid()){
        LOGE(OBFUSCATE("Failing offset: 0x%llu, please re-check the hex you entered."), offset);
        return;
    }
    if(!patch.Modify()) {
        LOGE(OBFUSCATE("Something went wrong while patching this offset: 0x%llu"), offset);
        return;
    }
}

void patchOffset(uint64_t offset, std::string hexBytes) {
    MemoryPatch patch = MemoryPatch::createWithHex("libil2cpp.so", offset, hexBytes);
    if(!patch.isValid()){
        LOGE(OBFUSCATE("Failing offset: 0x%llu, please re-check the hex you entered."), offset);
        return;
    }
    if(!patch.Modify()) {
        LOGE(OBFUSCATE("Something went wrong while patching this offset: 0x%llu"), offset);
        return;
    }
}

#define PATCHOFFSET(offset, hex) patchOffset(string2Offset(OBFUSCATE(offset)), OBFUSCATE(hex))
#define PATCHOFFSET_LIB(lib, offset, hex) patchOffset(OBFUSCATE(lib), string2Offset(OBFUSCATE(offset)), OBFUSCATE(hex))

#endif //ANDROID_MOD_MENU_MACROS_H
