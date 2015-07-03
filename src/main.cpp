#include "ofMain.h"
#include "PushBrowserApp.h"
#include "defines.h"
#ifdef TARGET_WIN32
    #pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#endif
//========================================================================
int main( ){
	ofSetupOpenGL(WINDOW_WIDTH_DEFAULT,WINDOW_HEIGHT_DEFAULT,OF_WINDOW);			// <-------- setup the GL context

	ofRunApp(new pushBrowserApp());
}
