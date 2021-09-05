#include "muffin_app/app.mod.h"

#include "muffin_platform/monitor.h"
#include "muffin_platform/window.h"

extern MufWindow *_mufWindow;

static void _load() {
    MufMonitor *monitor = mufMonitorGetPrimary();
    MufExtent2i size = mufMonitorGetPhysicalSize(monitor);
    _mufWindow = mufCreateWindow(size.width / 2, size.height / 2, 800, 600, "muffin");
    mufWindowSetCurrentContext(_mufWindow);    
}

static void _unload() {
    mufDestroyWindow(_mufWindow);
}

MUF_MODULE_REGISTRY_DEF(App,
    "MF_app",
    "",
    mufCreateVersion(1, 0, 0),
    0,
    _load,
    _unload
);