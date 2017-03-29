#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0,0,0);
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    
    trame.setPixelFormat(OF_PIXELS_NATIVE);
    
    trame.load("/data/vid.mov");
    ofLog() << "Loaded Mov";
    trame.setLoopState(OF_LOOP_NORMAL);
    
    if (send){ 
        // open an outgoing connection to HOST:PORT
        sender.setup(HOST, PORT);
        ofLog() << "Opened OSC Sender";
    }

    receiver.setup(PORTIN);

    ofLog() << "Opened OSC Receiver";

    

    if(wiringPiSPISetup(0,7812500)<0){
        ofLog() << "Failed to setup SPI!";
    }
    else{
        ofLog()<<"Open setup SPI!";
        wiringPiSetupSys() ;
    }

    wiringPiSetup();

    pinMode (pwmMap[0], PWM_OUTPUT) ;
    //ofLog() << pwmMap [i] ;
    //softPwmCreate (1, 0, 100) ;
    for (i = 1 ; i < NUM_PWMs ; ++i)
        {   
        softPwmCreate (pwmMap [i], 0, PWM_RANGE) ;
        //  printf ("%3d, %3d, %3d\n", i, pwmMap [i], values [i]) ;
        }
    



    
    if(playing){
        trame.play();
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
            else if(!m.getArgAsBool(0)){trame.stop(); playing = 0; clearLEDs(width*height);}
        }

        if(m.getAddress() == "/image"){
        //    ofLog() << "nArgs" << m.getNumArgs();
            //NetBuffer.clear();
            NetBuffer = m.getArgAsBlob(0);
        }    
        
        if(m.getAddress() == "/PWMs"){
        //    ofLog() << "nArgs" << m.getNumArgs();
            //NetBuffer.clear();
            PWMBuffer = m.getArgAsBlob(0);
        }    

    }

    trame.update();
 


    if(playing){

        // get part of the image for the LEDs
        pixels = trame.getPixels();
        LEDs = pixels.getData();

        // get part of the image for the PWMs
        pixels.cropTo(PWMPix, 0, 31, NUM_PWMs, 1);
        PWMs = PWMPix.getData();
        /*   for (i = 0 ; i < NUM_PWMs ; ++i)
        {
            ofLog() <<  "frame:" << i << ":  " << (int)PWMs[i*4] << "  " << (int)PWMs[i*4+1] << "  " <<  (int)PWMs[i*4+2] << "  " <<  (int)PWMs[i*4+3] ;
        }  ;
        */
        if (send){     
            imgAsBuffer.clear();
            imgAsBuffer.append((const char*)pixels.getData(),pixels.size());

            // PWMBuffer.clear();
            // PWMBuffer.append((const char*)PWMPix.getData(), PWMPix.size());
        
            ofxOscMessage m;
            m.setAddress("/image");
            m.addBlobArg(imgAsBuffer);
            sender.sendMessage(m);
        }
        
    }

    else{
        LEDs = (unsigned char*) NetBuffer.getData();
        PWMs = (unsigned char*) PWMBuffer.getData();
    }


// send to LEDs
    setLEDs(width*height, LEDs);
    //ofLog() << ofGetFrameRate();

// send to PWMs

    //if (j > 1023) {j=0;}
    //else j+=1;
    
    //pwmWrite (pwmMap [0], j) ;
    //pwmWrite (pwmMap [0], j) ;
    //softPwmWrite (pwmMap [0], 300) ;
    //softPwmWrite (1, 20) ;
    //delay (10) ;
    //softPwmWrite (pwmMap [4], j) ;
    //softPwmWrite (4, 70) ;
    //delay (10) ;
    //ofLog() <<  j ;
    
    pwmWrite (pwmMap [0], (int)(PWMs[0]+PWMs[1]+PWMs[2]+PWMs[3])) ;
    //ofLog() << "PWM " << 0 << ":    " << pwmMap [0] <<  " ->    " << (int)(PWMs[0]+PWMs[1]+PWMs[2]+PWMs[3]);
    for (i = 1 ; i < NUM_PWMs ; ++i)
        {
            softPwmWrite (pwmMap [i], PWMs[i*4]+PWMs[i*4+1]+PWMs[i*4+2]+PWMs[i*4+3]) ;
            //ofLog() << "PWM " << i << ":    " << pwmMap [i] <<  " ->    " <<  (int)(PWMs[i*4]+PWMs[i*4+1]+PWMs[i*4+2]+PWMs[i*4+3]);
        //  delay (10) ; // from wiringPi's example code -is this necessary ?
        }    
   
    //ofLog() <<  "next frame";
    

}

//--------------------------------------------------------------
void ofApp::draw(){

  /*
    
    // Clear with alpha, so we can capture via syphon and composite elsewhere should we want.
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    ofSetHexColor(0xFFFFFF);
    
    img.setFromPixels(pixels);
    img.draw(100, 100, 320, 320);
    
    PWMimg.setFromPixels(PWMPix);
    PWMimg.draw(100, 500, 320, 50);
    
    */

}
//--------------------------------------------------------------
void ofApp::exit() {

   clearLEDs(width*height);
    pwmWrite (pwmMap [0], 0) ;
    //ofLog() << "PWM " << 0 << ":    " << pwmMap [0] <<  " ->    " << (int)(PWMs[0]+PWMs[1]+PWMs[2]+PWMs[3]);
    for (i = 1 ; i < NUM_PWMs ; ++i)
        {
            softPwmWrite (pwmMap [i], 0) ;
            //ofLog() << "PWM " << i << ":    " << pwmMap [i] <<  " ->    " <<  (int)(PWMs[i*4]+PWMs[i*4+1]+PWMs[i*4+2]+PWMs[i*4+3]);
        //  delay (10) ; // from wiringPi's example code -is this necessary ?
        }   

}

//--------------------------------------------------------------
void ofApp::setLEDs(int numLed, unsigned char * LEDs) {
        int a;
            uint8_t buffer0[1], buffer1[4];
            srand(time(NULL));
            int BRIGHTNESS;

                //ofLog() << "mapBright: " << mapBright[BRIGHTNESS] ;
                //ofLog() << "col: " << mapCol[BRIGHTNESS] ;

                for(a=0; a<4; a++){
                    buffer0[0]=0b00000000;
                    wiringPiSPIDataRW(0, (unsigned char*)buffer0, 1);
                }
                for(a=0; a<numLed; a++){
                    BRIGHTNESS=(int)LEDs[a*4+3];
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


//-------------------------------------------------------------- 
/*void ofApp::parseYAML(){
    yaml.load("m/data/init.yml");
    
    yaml.doc["width"] >> width;
    yaml.doc["height"] >> height;
    yaml.doc["pwm"] >> pwm;
    yaml.doc["host"] >> host;
    yaml.doc["send"] >> send;
    
    cout << dec << "width: " << width << "* height: " << height << endl; 
    cout << "send: " << send << endl;

}
*/
   