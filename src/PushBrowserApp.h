#pragma once

#include "ofMain.h"
#include "defines.h"
#include "ofxXmlSettings.h"
#include "ofxOsc.h"
#include "ofxQRCodeGenerator.h"
#include "ofxAwesomium.h"

enum QRSide { QR_LEFT, QR_RIGHT, QR_BOTH };
enum QRHeight { QR_TOP, QR_MIDDLE, QR_BOTTOM };

class pushBrowserApp : public ofBaseApp{

	public:
    
        void loadSettings();
		void loadQRCode(string URL, int size);
    
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

		ofxQRCodeGenerator	QRCodeGenerator;
		bool				bQRControl, bUpdateQRCode;
		string				QRCurrentURL, QRDefaultURL, QRPushURL;
		ofImage				QRControlCode;
		QRSide				QRCurrentSide;
		QRHeight			QRCurrentHeight;
		int					QRCurrentSize, QRCurrentMargin;
};
