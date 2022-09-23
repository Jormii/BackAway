#include <pspkernel.h>

#include "callbacks.h"

static int exit_request = 0;

int exit_callback(int arg1, int arg2, void *common);
int callback_thread(SceSize args, void *argp);

int setup_callbacks()
{
    int thid = 0;

    thid = sceKernelCreateThread("update_thread", callback_thread, 0x11, 0xFA0, 0, 0);
    if (thid >= 0)
    {
        sceKernelStartThread(thid, 0, 0);
    }

    return thid;
}

int callback_thread(SceSize args, void *argp)
{
    int cbid;

    cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
    sceKernelRegisterExitCallback(cbid);

    sceKernelSleepThreadCB();

    return 0;
}

int exit_callback(int arg1, int arg2, void *common)
{
    exit_request = 1;
    return 0;
}

int running()
{
    return !exit_request;
}
