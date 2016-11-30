#pragma once
#include "ofMain.h"
#include "ofxOsc.h"
#include "wiringPi.h"
#include "wiringPiSPI.h"

#define HOST "bltzr.local"
#define PORT 12345

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void exit();
		void setLEDs(int numLed, unsigned char * LEDs, int BRIGHTNESS);
		void clearLEDs(int numLed);
    
        ofVideoPlayer trame;
        ofxOscSender sender;
        ofxOscSender PWMsender;
        ofBuffer imgAsBuffer;
        ofBuffer PWMBuffer;
        ofImage img;

    const int LEDnumb = 280;
    const int PWMnumb = 16;    
    
		
};
