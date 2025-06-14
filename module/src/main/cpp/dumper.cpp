#include "dumper.h"
#include <cstdio>
#include <cstdarg>
#include <string>
#include <fstream>
#include "log.h"

static const char *dump_path = "/data/local/tmp/redteam/il2cpp-dump.json";
static const char *log_path = "/data/local/tmp/dump.log";

static FILE *log_fp = nullptr;

static void open_log() {
    if (!log_fp) {
        log_fp = fopen(log_path, "a");
    }
}

static void log_print(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    __android_log_vprint(ANDROID_LOG_INFO, LOG_TAG, fmt, args);
    if (log_fp) {
        vfprintf(log_fp, fmt, args);
        fprintf(log_fp, "\n");
        fflush(log_fp);
    }
    va_end(args);
}

void dump_registrations(void *code_reg, void *metadata_reg) {
    open_log();
    log_print("CodeRegistration=%p MetadataRegistration=%p", code_reg, metadata_reg);
    std::ofstream out(dump_path);
    if (!out.is_open()) {
        log_print("Failed to open dump file %s", dump_path);
        return;
    }
    out << "{\n  \"CodeRegistration\": \"" << code_reg << "\",\n";
    out << "  \"MetadataRegistration\": \"" << metadata_reg << "\"\n}";
    out.close();
}
