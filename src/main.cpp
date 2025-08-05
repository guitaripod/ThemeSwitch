#include "Application.h"

#ifdef _WIN32
    #include <windows.h>
    #include <commctrl.h>
    
    #pragma comment(lib, "user32.lib")
    #pragma comment(lib, "shell32.lib")
    #pragma comment(lib, "advapi32.lib")
    #pragma comment(lib, "comctl32.lib")
    
    int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
        INITCOMMONCONTROLSEX icc;
        icc.dwSize = sizeof(icc);
        icc.dwICC = ICC_WIN95_CLASSES;
        InitCommonControlsEx(&icc);
        
        Application app;
        return app.run();
    }
#else
    #include <QApplication>
    
    int main(int argc, char *argv[]) {
        QApplication qtApp(argc, argv);
        qtApp.setApplicationName("ThemeSwitch");
        qtApp.setQuitOnLastWindowClosed(false);
        
        Application app;
        
        return qtApp.exec();
    }
#endif