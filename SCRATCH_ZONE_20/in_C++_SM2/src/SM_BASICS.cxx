#define DEBUG 1

// #include <jsapi.h>

#include <js/Initialization.h>
int main(void) {
    if (!JS_Init()) return 1;
    JS_ShutDown();
    return 0;
}



