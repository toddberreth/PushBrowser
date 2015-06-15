#pragma once

#include "ofMain.h"
#include "defines.h"
#include "ofxXmlSettings.h"
#include "ofxAwesomium.h"
#include "ofxOsc.h"



class pushBrowserApp : public ofBaseApp{

	public:
    
        void loadSettings();
    
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
		ofxAwesomium    browser;
    
        int             windowWidth, windowHeight, origWindowWidth, origWindowHeight;
        bool            bFullScreen, bBrowserLoaded;
    
        string          currentURL, defaultURL , pushURL;
        bool            bUpdateURL;
    
        int             oscReceivePort;
    
        ofxOscReceiver  URLReceiver;
};
