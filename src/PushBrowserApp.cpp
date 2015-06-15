#include "pushBrowserApp.h"

//--------------------------------------------------------------
void pushBrowserApp::setup(){
    
    ofSetFrameRate(FRAME_RATE);
    ofSetVerticalSync(VERTICAL_SYNC);
    ofSetLogLevel(LOG_LEVEL);
    
    bFullScreen = bUpdateURL = bBrowserLoaded = false;
    currentURL = defaultURL = pushURL = "";
    
    loadSettings();
    
    if (bFullScreen){ofSetFullscreen(bFullScreen); windowWidth = ofGetWindowWidth(); windowHeight = ofGetWindowHeight();}
    else {ofSetWindowShape(windowWidth, windowHeight); ofSetWindowPosition(0, 0);}
    
    URLReceiver.setup(oscReceivePort);

    currentURL = defaultURL;
}

//--------------------------------------------------------------
void pushBrowserApp::update(){

    if ((bBrowserLoaded == false)&&(defaultURL != "")){
        
        browser.setup(windowWidth, windowHeight);
        browser.loadURL(currentURL);
        bBrowserLoaded = true;
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
    }
    
    if ((bBrowserLoaded)&&(bUpdateURL)) {
        browser.loadURL(pushURL);
        currentURL = pushURL;
        pushURL = "";
        bUpdateURL = false;}
    
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
}

//--------------------------------------------------------------
void pushBrowserApp::keyPressed(int key){

    browser.keyPressed(key);
}

//--------------------------------------------------------------
void pushBrowserApp::keyReleased(int key){

    browser.keyReleased(key);
}

//--------------------------------------------------------------
void pushBrowserApp::mouseMoved(int x, int y ){

    browser.mouseMoved(x, y);
}

//--------------------------------------------------------------
void pushBrowserApp::mouseDragged(int x, int y, int button){

    browser.mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void pushBrowserApp::mousePressed(int x, int y, int button){

    browser.mousePressed(x, y, button);
}

//--------------------------------------------------------------
void pushBrowserApp::mouseReleased(int x, int y, int button){

    browser.mouseReleased(x, y, button);
}

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
    origWindowWidth = settings.getValue("SETTINGS:WINDOW:WIDTH", WINDOW_WIDTH_DEFAULT);
    origWindowHeight = settings.getValue("SETTINGS:WINDOW:HEIGHT", WINDOW_HEIGHT_DEFAULT);
    windowWidth = origWindowWidth; windowHeight = origWindowHeight;
    string _bFullScreen = ofToLower(settings.getValue("SETTINGS:WINDOW:FULLSCREEN", DEFAULT_FULLSCREEN));
    if (_bFullScreen == "true") bFullScreen = true; else bFullScreen = false;
    defaultURL = settings.getValue("SETTINGS:DEFAULT_URL", DEFAULT_URL);
    oscReceivePort = ofToInt(settings.getValue("SETTINGS:OSC_RECEIVE_PORT", DEFAULT_OSC_RECEIVE_PORT));
}
