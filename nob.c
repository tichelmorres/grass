#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#define NOB_WARN_DEPRECATED
#include "nob.h"

int main(int argc, char **argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);

    Cmd cmd = {0};

    cmd_append(&cmd, "gcc");
    cmd_append(&cmd, "-Wall");
    cmd_append(&cmd, "-Wextra");
    cmd_append(&cmd, "-Wno-unused-parameter");
    cmd_append(&cmd, "-O2");
    cmd_append(&cmd, "main.c");
    cmd_append(&cmd, "grass.c");
    cmd_append(&cmd, "lib/linalc.c");
    cmd_append(&cmd, "-o", "main");
    cmd_append(&cmd, "-lX11");
    cmd_append(&cmd, "-lXext");
    cmd_append(&cmd, "-lm");

    if (!cmd_run(&cmd, .dont_reset = true)) return 1;

    for (int i = 1; i < argc; i++) {
         if (strcmp(argv[i], "-r") == 0 ||
             strcmp(argv[i], "--run") == 0) {
             const char *exec = argv[argc - 1];
             int st = system(exec);
             if (st == -1) printf("[FLAG]: Failed to run %s", exec);
         }
    }

    return 0;
}
