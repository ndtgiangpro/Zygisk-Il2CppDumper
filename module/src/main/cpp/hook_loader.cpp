#include "hook_loader.h"
#include "dumper.h"
#include "xdl.h"
#include "log.h"

static int (*orig_il2cpp_init)(const char *domain_name) = nullptr;

static int hooked_il2cpp_init(const char *domain_name) {
    int ret = 0;
    if (orig_il2cpp_init) {
        ret = orig_il2cpp_init(domain_name);
    }
    void *handle = xdl_open("libil2cpp.so", 0);
    if (handle) {
        void *code_reg = xdl_sym(handle, "g_CodeRegistration", nullptr);
        void *metadata_reg = xdl_sym(handle, "g_MetadataRegistration", nullptr);
        dump_registrations(code_reg, metadata_reg);
        xdl_close(handle);
    } else {
        LOGW("failed to open libil2cpp.so");
    }
    return ret;
}

void setup_il2cpp_hook(zygisk::Api *api) {
    api->pltHookRegister("libil2cpp.so", "il2cpp_init", (void *)hooked_il2cpp_init, (void **)&orig_il2cpp_init);
    if (!api->pltHookCommit()) {
        LOGE("pltHookCommit failed");
    }
}
