//
//  ModeManager.cpp
//  Finding Stillness
//
//  Created by Lauren McCarthy on 2/7/15.
//
//

#include "ModeManager.h"


void ModeManager::setup() {
    reset();
    modes.push_back(new PixelMode("Pixel", 15));
    modes.push_back(new VideoMode("Grass", 5));
    modes.push_back(new FlockingMode("Flocking", 5));
    modes.push_back(new SeparationMode("Separate", 5));
    modes.push_back(new SwipeMode("Swipe", 5));
    modes.push_back(new CircleMode("Circles", 5));
    modes.push_back(new FaderMode("Fader", 5));
    
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
}

void ModeManager::draw() {
    for (int i=0; i<modes.size(); i++) {
        if (modes[i]->playing) {
            modes[i]->draw();
        }
    }
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
    modes[curMode]->exit();
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

