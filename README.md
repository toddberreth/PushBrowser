# PushBrowser

Simple web browser (using the Awesomium HTML/UI libraries based on Chromium/Webkit). Allows you to present a webpage full screen without menus, disabling the cursor, and overlaying a QR Code (if desired).  The browser listens for osc messages which may relay new URLs/QR Code URLs to load on the display.

Generally useful for public displays (driven by a OSX/Windows machine), which do not have any UI devices hooked to them. 

PushBrowser can display a web-driven exhibition, and direct the user to obtain more info (via the QR Code) or connect to a hand-held UI.

Flexible configuration via an .xml configuration file. 

PushBrowser was written in C++ using openFrameworks 008+

PushBrowser requires OF addons -> ofxAwesomium, ofxOsc, ofxQRCodeGenerator, and ofxXmlSettings

