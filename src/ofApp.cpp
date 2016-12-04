#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0,0,0);
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    
    trame.setPixelFormat(OF_PIXELS_NATIVE);
    
    trame.load("movies/rougebleuN.mov");
    trame.setLoopState(OF_LOOP_NORMAL);
    
    // open an outgoing connection to HOST:PORT
    sender.setup(HOST, PORT);

    receiver.setup(PORTIN);

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

    while(receiver.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(m);
        
        if(m.getAddress() == "/play"){
            //ofLog() << "b" << m.getArgAsInt32(0);
            if(m.getArgAsBool(0)){trame.play(); playing = 1;}
            else if(!m.getArgAsBool(0)){trame.stop(); playing = 0; clearLEDs(LEDnumb);}
        }
       
        if(m.getAddress() == "/brightness"){
            brightnessNet = m.getArgAsInt(0);
            //ofLog() << "b" << brightnessNet;
        }
        if(m.getAddress() == "/image"){
        //    ofLog() << "nArgs" << m.getNumArgs();
            //NetBuffer.clear();
            NetBuffer = m.getArgAsBlob(0);
        }    
        
    }

    trame.update();
 
// get part of the image for the LEDs

    if(playing){
        ofPixels & pixels = trame.getPixels();
        LEDs = pixels.getData();
        brightness = LEDs[4095];

        imgAsBuffer.clear();
        imgAsBuffer.append((const char*)pixels.getData(),pixels.size());

         // get part of the image for the PWMs
        ofPixels PWMPix;
        pixels.cropTo(PWMPix, 15, 0, 1, 16);
        
        PWMBuffer.clear();
        PWMBuffer.append((const char*)PWMPix.getData(), PWMPix.size());
        
        ofxOscMessage m;
        m.setAddress("/image");
        m.addBlobArg(imgAsBuffer);
        sender.sendMessage(m);

        ofxOscMessage n;
        n.setAddress("/PWM");
        n.addBlobArg(PWMBuffer);
        sender.sendMessage(n);
    }

    else{
        LEDs = (unsigned char*) NetBuffer.getData();
        brightness = brightnessNet;
    }
    //ofLog()<<brightness;



    //cout << "ofApp:: sending image with size: " << PWMBuffer.size() << endl;


// send to LEDs
    setLEDs(LEDnumb, LEDs, brightness);
    //ofLog() << ofGetFrameRate();
    

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
        if(BRIGHTNESS>254)
            BRIGHTNESS=255;

                //ofLog() << "mapBright: " << mapBright[BRIGHTNESS] ;
                //ofLog() << "col: " << mapCol[BRIGHTNESS] ;

                for(a=0; a<4; a++){
                       buffer0[0]=0b00000000;
                       wiringPiSPIDataRW(0, (unsigned char*)buffer0, 1);
                }
                for(a=0; a<numLed; a++){
                       buffer1[0]=(mapBright[BRIGHTNESS] & 0b00011111) | 0b11100000;
                       buffer1[1]=(int)LEDs[a*4+2]*mapCol[BRIGHTNESS];  //green
                       buffer1[2]=(int)LEDs[a*4+1]*mapCol[BRIGHTNESS];  //blue
                       buffer1[3]=(int)LEDs[a*4+0]*mapCol[BRIGHTNESS];  //red
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