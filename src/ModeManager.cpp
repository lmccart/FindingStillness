//
//  ModeManager.cpp
//  Finding Stillness
//
//  Created by Lauren McCarthy on 2/7/15.
//
//

#include "ModeManager.h"


void ModeManager::setup() {
    
    // PEND
    dmx.connect(1); // or use a number
    // dmx.connect("tty.usbserial-ENS8KBA6"); // or use the name
    //dmx.connect(port, modules * channelsPerModule);
    dmx.update(true); // black on startup
    
    reset();
    
    Tweenzor::init();
    
    modes.push_back(new PixelMode("Pixel", 15, false));
    modes.push_back(new VideoMode("Nature", 15, false));
    modes.push_back(new FlockingMode("Flocking", 10, true));
    modes.push_back(new SeparationMode("Separate", 5, false, 0));
    modes.push_back(new SeparationMode("Separate", 5, false, 255));
    
    modes.push_back(new FaderMode("Fader", 30, false));
    
    modes.push_back(new FlickerMode("Flicker", 10, false));
    modes.push_back(new WashMode("White", 100, false, 100));
    
    cur_hr = 0;
}

void ModeManager::update() {
    float now = ofGetElapsedTimef();
    if (now - modeStartTime >= modes[curMode]->duration) {
        next(-1);
    }
    for (int i=0; i<modes.size(); i++) {
        if (modes[i]->playing) {
            modes[i]->update();
        }
    }
    Tweenzor::update( ofGetElapsedTimeMillis() );
    
    if(dmx.isConnected()) {
        int val = modes[curMode]->floorValue;
        dmx.setLevel(1, val); // PEND channel?
        dmx.update();
    }
}

void ModeManager::draw() {
    // draw mode
    for (int i=0; i<modes.size(); i++) {
        if (modes[i]->playing) {
            modes[i]->drawWithHR();
        }
    }
    
    // draw bg
    ofPushStyle();
    ofSetColor(0);
    ofRect(1024, 0, ofGetWidth()-1024, ofGetHeight());
    
    // draw dmx
    int val = modes[curMode]->floorValue;
    int w = modes[curMode]->width;
    ofSetColor(val);
    ofRect(w+10, 10, ofGetWidth()-20-w, 100);
    ofPopStyle();
    
}

void ModeManager::reset() {
    if (playing) {
        modes[curMode]->exit();
    }
    curMode = 0;
    playing = false;
    modeStartTime = 0;
}

void ModeManager::start() {
    reset();
    next(0);
    modeStartTime = ofGetElapsedTimef();
    playing = true;
}

void ModeManager::next(int i) {
    ofLog() << "next";
    modeStartTime = ofGetElapsedTimef();
    if (playing) modes[curMode]->exit();
    else modes[curMode]->exit();
    if (i == -1) {
        curMode++;
        if (curMode >= modes.size()) {
            curMode = 0;
        }
    } else {
        if (i >= 0 && i < modes.size()) {
            curMode = i;
        } else ofLog() << "ModeManager::mode out of range";
    }
    modes[curMode]->enter();
}

void ModeManager::updateHeartrate(float hr) {
    cur_hr = hr;
    ofLog() << "heartrate updated to " << hr;
}

void ModeManager::exit() {
    dmx.clear();
    dmx.update(true); // black on shutdown
}