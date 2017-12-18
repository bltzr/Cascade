#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0,0,0);
    ofSetVerticalSync(true);
    ofSetFrameRate(60); 
    
    trame.setPixelFormat(OF_PIXELS_NATIVE);

    dir.open("/data");
    dir.allowExt("mov");
    dir.listDir();
    ofLog() << "Loading Dir: ";
    dir.sort();
        
    //if (send){ 
    // open an outgoing connection to HOST:PORT
    sender.setup(HOST, PORT);
    ofLog() << "Opened OSC Sender";
    //}

    // get host name of this computer
    FILE* stream = popen( "hostname", "r" );  
    ostringstream output;  

    while( !feof( stream ) && !ferror( stream ))  
    {  
        char buf[128];  
        int bytesRead = fread( buf, 1, 128, stream );  
        output.write( buf, bytesRead );  
    }  
    thisHostName = output.str();  

    sendFileList();

    //trame.load("/data/vid.mov");
    trame.load(dir.getPath(0));
    ofLog() << "Loaded Mov: " << dir.getPath(0);// << "/data/"+dir.listDir(0);
    trame.setLoopState(OF_LOOP_NORMAL);

    receiver.setup(PORTIN);

    ofLog() << "Opened OSC Receiver";

    wiringPiSetup();
    if(wiringPiSPISetup(0,7812500)<0){
        ofLog() << "Failed to setup SPI!";
    }
    else{
        ofLog()<<"Open setup SPI!";
        wiringPiSetupSys() ;
    }

    //clearLEDs(width*height);

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

        if( !playing && m.getAddress() == "/image"){
        //    ofLog() << "nArgs" << m.getNumArgs();
            //NetBuffer.clear();
            NetBuffer = m.getArgAsBlob(0);
            setLEDs(NetBuffer.size(), (unsigned char*)NetBuffer.getData());
        } 

        if(m.getAddress() == "/file"){
            //ofLog() << "nArgs" << m.getNumArgs();
            //NetBuffer.clear();
            trame.load(m.getArgAsString(0));
            //ofLog(m.getArgAsString(0));
            trame.play();
        } 

        if(m.getAddress() == "/host"){
            //    ofLog() << "nArgs" << m.getNumArgs();
            //NetBuffer.clear();
            host=m.getArgAsString(0);
            sender.setup(host, PORT);

            // now send list of files 
            sendFileList();
            
        } 
        
    }
 
// get part of the image for the LEDs

    if(playing){
        trame.update();
        ofPixels & pixels = trame.getPixels();
        LEDs = pixels.getData();

        if (pwm>0){
            // get part of the image for the PWMs
            ofPixels PWMPix;
            pixels.cropTo(PWMPix, 15, 0, 1, 16);
        }

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
    
    setLEDs(width*height, LEDs);

    }


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

    clearLEDs(width*height);

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

        for(a=0; a<numLed; a++){
                LEDs[a]=0;
                LEDs[a+1]=0;
                LEDs[a+2]=0;
        }

        setLEDs(numLed, LEDs);
              

    }


//-------------------------------------------------------------- 

void ofApp::sendFileList(){

    ofxOscMessage m;
    m.setAddress("/file");
    m.addStringArg(thisHostName);
    m.addStringArg("start");
    sender.sendMessage(m);

    for(int i = 0; i < (int)dir.size(); i++){
        ofLogNotice(dir.getPath(i));
        ofxOscMessage m;
        m.setAddress("/file");
        m.addStringArg(thisHostName);
        m.addStringArg(dir.getPath(i));
        sender.sendMessage(m);
    }
}    
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
   