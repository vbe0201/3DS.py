#include <stdio.h>
#include <3ds.h>

#include "Python.h"

#define MAIN_PY "main.py"

int main(int argc, char *argv[])
{
    /* Initialize the hardware. */
    gfxInitDefault();
    gfxSet3D(false);
    consoleInit(GFX_TOP, NULL);

    #ifdef _3DS_PY_DEBUG
    consoleDebugInit(debugDevice_CONSOLE);

    printf("Args: %s\n", !argc ? "None" : "");
    for (int i = 0; i < argc; i++)
        printf("%d: %s\n", i + 1, argv[i]);

    Py_VerboseFlag = 1;
    #endif

    /* Set some interpreter options. */
    Py_NoSiteFlag = 1;
    Py_IgnoreEnvironmentFlag = 1;
    Py_NoUserSiteDirectory = 1;

    /* Calculate the absolute home directory and strip the leading "sdmc:/". */
    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));
    char *stripped_cwd = strchr(cwd, '/');
    if (stripped_cwd == NULL)
        stripped_cwd = cwd;

    Py_SetPythonHome(Py_DecodeLocale(stripped_cwd, NULL));

    Py_SetProgramName(L"3DS.py");

    Py_Initialize();

    #ifdef _3DS_PY_DEBUG
    printf("Python %s running on %s.\n", Py_GetVersion(), Py_GetPlatform());
    #endif

    /* Set import path. */
    PyObject *path = PyUnicode_FromString("");
    PyObject *sys_path = PySys_GetObject("path");
    PyList_Insert(sys_path, 0, path);

    /* Launch our script. */
    FILE *main_py = fopen(MAIN_PY, "r");
    if (main_py == NULL)
        printf("Failed to open %s. Check if the file exists.", MAIN_PY);
    else
        PyRun_AnyFile(main_py, MAIN_PY);

    Py_DECREF(path);
    Py_DECREF(sys_path);

    Py_Finalize();

    /* The main loop of this application. */
    while (aptMainLoop()) {
        hidScanInput();

        if (hidKeysDown() & KEY_START) // Exit application and go back to the Homebrew Launcher.
            break;

        gfxFlushBuffers();
        gfxSwapBuffers();

        gspWaitForVBlank();
    }

    gfxExit();

    return 0;
}
