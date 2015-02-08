#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    modeManager.setup();
}

//--------------------------------------------------------------
void ofApp::update(){
    modeManager.update();

}

//--------------------------------------------------------------
void ofApp::draw(){
    modeManager.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    ofLog() << key;
    if (key >= 48 && key <= 57) {
        modeManager.next(key-48);
    } else if (key == 32) {
        modeManager.next(-1);
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}


//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}



//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}
