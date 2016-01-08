#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0,0,0);
    ofSetVerticalSync(true);
    
    trame.setPixelFormat(OF_PIXELS_RGBA);
    
    trame.load("movies/testRaw3.mov");
    //pour tester avec le son:
    //     trame.load("movies/oh.mov");
    trame.setLoopState(OF_LOOP_NORMAL);
    trame.play();
    
    // open an outgoing connection to HOST:PORT
    sender.setup(HOST, PORT);
    
    // imgAsBuffer = ofBufferFromFile("sendImageTest.jpg", true);
}

//--------------------------------------------------------------
void ofApp::update(){

    trame.update();
}

//--------------------------------------------------------------
void ofApp::draw(){

    
    ofSetHexColor(0xFFFFFF);
    
    trame.draw(20,20);
    ofSetHexColor(0x000000);
    ofPixels & pixels = trame.getPixels();
    
    int vidWidth = pixels.getWidth();
    int vidHeight = pixels.getHeight();
    int nChannels = pixels.getNumChannels();
    
    // ofSaveImage(pixels, imgAsBuffer, OF_IMAGE_FORMAT_JPEG, OF_IMAGE_QUALITY_BEST);
    
    // img.load(imgAsBuffer);

    //ofBuffer pixBuf;
    imgAsBuffer.clear();
    imgAsBuffer.append((const char*)pixels.getData(),pixels.size()-64);
    
    
    ofxOscMessage m;
    m.setAddress("/image");
    m.addBlobArg(imgAsBuffer);
    sender.sendMessage(m);
    cout << "ofApp:: ____________________________________ " << endl;
    cout << "ofApp:: NEW FRAME " << endl;
    cout << "ofApp:: qdth / height / nChannels: " << vidWidth << " / " << vidHeight << " / " << nChannels << endl;
    cout << "ofApp:: image's actual size: " << vidWidth * vidHeight * nChannels<< endl;
    cout << "ofApp:: sending image with size: " << imgAsBuffer.size() << endl;
    cout << "ofApp:: WTF factor: " << imgAsBuffer.size() - vidWidth * vidHeight * nChannels<< endl;

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
