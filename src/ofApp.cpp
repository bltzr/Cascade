#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0,0,0);
    ofSetVerticalSync(true);
    ofSetFrameRate(120);
    
    trame.setPixelFormat(OF_PIXELS_RGBA);
    
    trame.load("movies/oh.mov");
    trame.setLoopState(OF_LOOP_NORMAL);
    trame.play();
    
    // open an outgoing connection to HOST:PORT
    sender.setup(HOST, PORT);


    ofSetFrameRate(60);

    wiringPiSetup();
    if(wiringPiSPISetup(0,7812500)<0){
        ofLog() << "Failed to setup SPI!";
    }
    else{
        ofLog()<<"Open setup SPI!";
        wiringPiSetupSys() ;
    }

}

//--------------------------------------------------------------
void ofApp::update(){

    trame.update();
 
// get part of the image for the LEDs
    ofPixels & pixels = trame.getPixels();
    unsigned char * LEDs = pixels.getData();
    
    int vidWidth = pixels.getWidth();
    int vidHeight = pixels.getHeight();
    int nChannels = pixels.getNumChannels();
    //ofLog()<<"Channels:"<<nChannels;

    imgAsBuffer.clear();
    imgAsBuffer.append((const char*)pixels.getData(),pixels.size()-64);

// send to LEDs
    setLEDs(LEDnumb, LEDs, 3);
    //ofLog() << ofGetFrameRate();
    
 // get part of the image for the PWMs
    ofPixels PWMPix;
    pixels.cropTo(PWMPix, 15, 0, 1, 16);
    
    PWMBuffer.clear();
    PWMBuffer.append((const char*)PWMPix.getData(), PWMPix.size());
    
    ofxOscMessage m;
    m.setAddress("/image");
    m.addBlobArg(imgAsBuffer);
    sender.sendMessage(m);
    /*cout << "ofApp:: ____________________________________ " << endl;
    cout << "ofApp:: NEW FRAME " << endl;
    cout << "ofApp:: qdth / height / nChannels: " << vidWidth << " / " << vidHeight << " / " << nChannels << endl;
    cout << "ofApp:: image's actual size: " << vidWidth * vidHeight * nChannels<< endl;
    cout << "ofApp:: sending image with size: " << imgAsBuffer.size() << endl;
    cout << "ofApp:: WTF factor: " << imgAsBuffer.size() - vidWidth * vidHeight * nChannels<< endl;
    */  
    ofxOscMessage n;
    n.setAddress("/PWM");
    n.addBlobArg(PWMBuffer);
    sender.sendMessage(n);
    //cout << "ofApp:: sending image with size: " << PWMBuffer.size() << endl;
}

//--------------------------------------------------------------
void ofApp::draw(){

  /*
    
    ofSetHexColor(0xFFFFFF);
    
    trame.draw(16,16);
    ofSetHexColor(0x000000);
    
    */

}
//--------------------------------------------------------------
void ofApp::exit() {

    clearLEDs(LEDnumb);

}

//--------------------------------------------------------------
void ofApp::setLEDs(int numLed, unsigned char * LEDs, int BRIGHTNESS) {
        int a;
                uint8_t buffer0[1], buffer1[4];
                srand(time(NULL));
        if(BRIGHTNESS>30)
            BRIGHTNESS=31;

                for(a=0; a<4; a++){
                       buffer0[0]=0b00000000;
                       wiringPiSPIDataRW(0, (unsigned char*)buffer0, 1);
                }
                for(a=0; a<numLed; a++){
                       buffer1[0]=(BRIGHTNESS & 0b00011111) | 0b11100000;
                       buffer1[1]=LEDs[a*4+1];  //green
                       buffer1[2]=LEDs[a*4+2];  //blue
                       buffer1[3]=LEDs[a*4+0];  //red
                       wiringPiSPIDataRW(0, (unsigned char*)buffer1, 4);
                }
                for(a=0; a<4; a++){
                       buffer0[0]=0b11111111;
                       wiringPiSPIDataRW(0, (unsigned char*)buffer0, 1);
                }
              

    }

//--------------------------------------------------------------
void ofApp::clearLEDs(int numLed) {
        int a;
                uint8_t buffer0[1], buffer1[4];
                srand(time(NULL));


                for(a=0; a<4; a++){
                       buffer0[0]=0b00000000;
                       wiringPiSPIDataRW(0, (unsigned char*)buffer0, 1);
                }
                for(a=0; a<numLed; a++){
                       buffer1[0]=(0 & 0b00011111) | 0b11100000;
                       buffer1[1]=0;  //green
                       buffer1[2]=0;  //blue
                       buffer1[3]=0;  //red
                       wiringPiSPIDataRW(0, (unsigned char*)buffer1, 4);
                }
                for(a=0; a<4; a++){
                       buffer0[0]=0b11111111;
                       wiringPiSPIDataRW(0, (unsigned char*)buffer0, 1);
                }
              

    }