#include "pushBrowserApp.h"

//--------------------------------------------------------------
void pushBrowserApp::setup(){
    
	ofBackground(0);
    ofSetFrameRate(FRAME_RATE);
    ofSetVerticalSync(VERTICAL_SYNC);
    ofSetLogLevel(LOG_LEVEL);
	
    bFullScreen = bUpdateURL = bQRControl = bBrowserLoaded = bUpdateQRCode = false;
    currentURL = defaultURL = pushURL = QRCurrentURL = QRDefaultURL = QRPushURL = "";
    
    loadSettings();
    
    if (bFullScreen){ofSetFullscreen(bFullScreen); windowWidth = ofGetWindowWidth(); windowHeight = ofGetWindowHeight();}
    else {ofSetWindowShape(windowWidth, windowHeight); ofSetWindowPosition(30, 50);}
    
    URLReceiver.setup(oscReceivePort);
}

//--------------------------------------------------------------
void pushBrowserApp::update(){

    if ((bBrowserLoaded == false)&&(defaultURL != "")){
        browser.setup(windowWidth, windowHeight);
        browser.loadURL(currentURL);
        bBrowserLoaded = true;
		ofxAwesomium::clearSessionCache();
		ofxAwesomium::clearSessionCookies();
    } else {
        
        ofxAwesomium::updateCore();
        browser.update();
        ofSetWindowTitle(browser.getTitle());
    }
    
    while(URLReceiver.hasWaitingMessages()){

        ofxOscMessage m;
        URLReceiver.getNextMessage(&m);
        if(m.getAddress() == "/pushURL"){
            pushURL = m.getArgAsString(0);
            bUpdateURL = true;
        }
		if(m.getAddress() == "/pushQRURL"){
            QRPushURL = m.getArgAsString(0);
            bUpdateQRCode = true;
        }
    }

    if ((bBrowserLoaded)&&(bUpdateURL)) {

        browser.loadURL(pushURL);
        currentURL = pushURL;
        pushURL = "";
        bUpdateURL = false;
		ofxAwesomium::clearSessionCache();
		ofxAwesomium::clearSessionCookies();
	}
    
	if (bUpdateQRCode) {

		loadQRCode(QRPushURL, QRCurrentSize);
        QRCurrentURL = QRPushURL; QRPushURL = "";
        bUpdateQRCode = false;
	}

    if ((windowWidth != ofGetWindowWidth()) || (windowHeight != ofGetWindowHeight())){
        
		windowWidth = ofGetWindowWidth(); windowHeight = ofGetWindowHeight();
        browser.setup(windowWidth, windowHeight);
        browser.loadURL(currentURL);
    }
}

//--------------------------------------------------------------
void pushBrowserApp::draw(){

	
    if(browser.getIsLoading()) {ofSetColor(0); ofDrawBitmapString("Loading...", 10, 15);}
    ofSetColor(255);
    browser.draw(0, 0);

	if ((bQRControl) && (QRControlCode.isAllocated())){
		ofPushMatrix();
			if (QRCurrentHeight == QR_TOP) ofTranslate(0,(QRCurrentSize/2) + QRCurrentMargin,0); 
			else if (QRCurrentHeight == QR_BOTTOM) ofTranslate(0,ofGetWindowHeight()-(QRCurrentSize/2)-QRCurrentMargin,0); 
			else  ofTranslate(0,ofGetWindowHeight()/2,0);

			if ((QRCurrentSide == QR_LEFT) || (QRCurrentSide == QR_BOTH)) QRControlCode.draw((QRCurrentSize/2) + QRCurrentMargin,0);
			if ((QRCurrentSide == QR_RIGHT) || (QRCurrentSide == QR_BOTH)) QRControlCode.draw(ofGetWindowWidth()-(QRCurrentSize/2)-QRCurrentMargin,0);

		ofPopMatrix();
	}
}

//--------------------------------------------------------------
void pushBrowserApp::keyPressed(int key){ browser.keyPressed(key);}
//--------------------------------------------------------------
void pushBrowserApp::keyReleased(int key){ browser.keyReleased(key);}
//--------------------------------------------------------------
void pushBrowserApp::mouseMoved(int x, int y ){ browser.mouseMoved(x, y);}
//--------------------------------------------------------------
void pushBrowserApp::mouseDragged(int x, int y, int button){ browser.mouseDragged(x, y, button);}
//--------------------------------------------------------------
void pushBrowserApp::mousePressed(int x, int y, int button){browser.mousePressed(x, y, button);}
//--------------------------------------------------------------
void pushBrowserApp::mouseReleased(int x, int y, int button){browser.mouseReleased(x, y, button);}
//--------------------------------------------------------------
void pushBrowserApp::mouseEntered(int x, int y){}
//--------------------------------------------------------------
void pushBrowserApp::mouseExited(int x, int y){}
//--------------------------------------------------------------
void pushBrowserApp::windowResized(int w, int h){}
//--------------------------------------------------------------
void pushBrowserApp::gotMessage(ofMessage msg){}
//--------------------------------------------------------------
void pushBrowserApp::dragEvent(ofDragInfo dragInfo){}

void pushBrowserApp::loadSettings(){
    
    ofxXmlSettings  settings;
    settings.loadFile(DEFAULT_SETTINGS);
    windowWidth = origWindowWidth = settings.getValue("SETTINGS:WINDOW:WIDTH", WINDOW_WIDTH_DEFAULT);
	windowHeight = origWindowHeight = settings.getValue("SETTINGS:WINDOW:HEIGHT", WINDOW_HEIGHT_DEFAULT);
    string _bFullScreen = ofToLower(settings.getValue("SETTINGS:WINDOW:FULLSCREEN", DEFAULT_FULLSCREEN));
    if (_bFullScreen == "true") bFullScreen = true; else bFullScreen = false;
	string _bHideCursor = ofToLower(settings.getValue("SETTINGS:WINDOW:HIDE_CURSOR", DEFAULT_HIDE_CURSOR));
    if (_bHideCursor == "true") ofHideCursor(); else ofShowCursor();
    
	currentURL = defaultURL = settings.getValue("SETTINGS:DEFAULT_URL", DEFAULT_URL);
	
	QRCurrentURL = QRDefaultURL = settings.getValue("SETTINGS:QR_CONTROL:DEFAULT_QR_URL", DEFAULT_QR_URL);
	string _bQRControl = ofToLower(settings.getValue("SETTINGS:QR_CONTROL:QR_ON", DEFAULT_QR_ON));
    if (_bQRControl == "true") bQRControl = true; else bQRControl = false;
	string _QRSide = ofToLower(settings.getValue("SETTINGS:QR_CONTROL:QR_SIDE", DEFAULT_QR_SIDE));
    if (_QRSide == "left") QRCurrentSide = QR_LEFT; if (_QRSide == "right") QRCurrentSide = QR_RIGHT; if (_QRSide == "both") QRCurrentSide = QR_BOTH;
	string _QRHeight = ofToLower(settings.getValue("SETTINGS:QR_CONTROL:QR_HEIGHT", DEFAULT_QR_HEIGHT));
    if (_QRHeight == "top") QRCurrentHeight = QR_TOP; if (_QRHeight == "middle") QRCurrentHeight = QR_MIDDLE; if (_QRHeight == "bottom") QRCurrentHeight = QR_BOTTOM;
	QRCurrentSize = settings.getValue("SETTINGS:QR_CONTROL:QR_SIZE", DEFAULT_QR_SIZE);
	QRCurrentMargin = settings.getValue("SETTINGS:QR_CONTROL:QR_MARGIN", DEFAULT_QR_MARGIN);
	if (bQRControl) {loadQRCode(QRCurrentURL, QRCurrentSize);}
	oscReceivePort = ofToInt(settings.getValue("SETTINGS:OSC_RECEIVE_PORT", DEFAULT_OSC_RECEIVE_PORT));
}

void pushBrowserApp::loadQRCode(string url, int size){

	QRControlCode = QRCodeGenerator.generate(url, size);
	QRControlCode.setAnchorPercent(0.5, 0.5);
}