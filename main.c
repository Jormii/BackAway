#include <pspkernel.h>
#include <pspdebug.h> // TODO: Remove0

#include "callbacks.h"

PSP_MODULE_INFO("BackAway", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

int main()
{
    setup_callbacks();
    pspDebugScreenInit();

    pspDebugScreenPrintf("Hello");
    while (running())
    {
    }

    sceKernelExitGame();
    return 0;
}