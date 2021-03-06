//
//  FlockingMode.h
//  FindingStillness
//
//  Created by Lauren McCarthy on 2/7/15.
//
//
#pragma once

#include "ofMain.h"
#include "Mode.h"



class Boid {
public:
    ofVec2f position;
    ofVec2f velocity;
    ofVec2f acceleration;
    float maxspeed;
    float maxforce;
    float r;
    float w, h;
    float i;
    float n;
    float desiredseparation;
    
    void setup(float x, float y, float _w, float _h) {
        i = ofRandom(0, 200);
        w = _w;
        h = _h;
        n = 127;
        position.set(x, y);
        velocity.set(ofRandom(-15, -13), ofRandom(-1, 1));//width*0.5-x, height*0.5-y);
        borders();
        r = 30;
        maxspeed = 10;    // Maximum speed
        maxforce = 0.5; // Maximum steering force
        desiredseparation = 45;
    }

    void run(const vector<Boid>& boids) {
        flock(boids);
        update();
        grow();
        borders();
    }
    
    void applyForce(ofVec2f force) {
        // We could add mass here if we want A = F / M
        acceleration += force;
    }
    
    // We accumulate a new acceleration each time based on three rules
    void flock(const vector<Boid>& boids) {
        ofVec2f sep = separate(boids);   // Separation
        ofVec2f ali = align(boids);      // Alignment
        ofVec2f coh = cohesion(boids);   // Cohesion
        // Arbitrarily weight these forces
        sep *= 1.5;
        ali *= 1.0;
        coh *= 1.0;
        // Add the force vectors to acceleration
        applyForce(sep);
        applyForce(ali);
        applyForce(coh);
    }
    
    // Method to update location
    void update() {
        // Update velocity
        velocity += acceleration;
        // Limit speed
        velocity.limit(maxspeed);
        position += velocity;
        // Reset accelertion to 0 each cycle
        acceleration.set(0, 0);
    }
    
    void grow() {
        if (maxspeed <= 9.02) {
            r += ofRandom(0.2, 0.38);
            desiredseparation += 0.185;
        }
        if (n<255) n+=0.35;
        if (maxspeed > 4) maxspeed -= 0.005;
    }
    
    // A method that calculates and applies a steering force towards a target
    // STEER = DESIRED MINUS VELOCITY
    ofVec2f seek(ofVec2f target) {
        ofVec2f desired = target - position;  // A vector pointing from the location to the target
        // Normalize desired and scale to maximum speed
        desired.normalize();
        desired *= maxspeed;
        // Steering = Desired minus Velocity
        ofVec2f steer = desired - velocity;
        steer.limit(maxforce);  // Limit to maximum steering force
        return steer;
    }
    
    void draw() {
        // Draw a triangle rotated in the direction of velocity
        ofVec2f zero(0, -1);
        float theta = zero.angle(velocity);
        ofSetColor(n+(255-n)*sin((ofGetFrameNum()+i)*0.05));
        ofPushMatrix();
        ofTranslate(position.x,position.y);
        ofRotate(theta);
        //ofTriangle(0, -r*2, -r, r*2, r, r*2);
        ofEllipse(0, 0, r, r);
        ofPopMatrix();
    }
    
    // Wraparound
    void borders() {
        if (position.x < -r)  velocity.x = abs(velocity.x);//position.x = width +r;
        if (position.y < -r)  velocity.y  = abs(velocity.y);//position.y = height +r;
        if (position.x > w +r) velocity.x = -abs(velocity.x);//position.x = -r;
        if (position.y > h +r) velocity.y = -abs(velocity.y);//position.y = -r;
    }
    
    // Separation
    // Method checks for nearby boids and steers away
    ofVec2f separate(const vector<Boid>& boids) {
        ofVec2f steer(0, 0);
        int count = 0;
        // For every boid in the system, check if it's too close
        for (int i = 0; i < boids.size(); i++) {
            float d = position.distance(boids[i].position);
            // If the distance is greater than 0 and less than an arbitrary amount (0 when you are yourself)
            if ((d > 0) && (d < desiredseparation)) {
                // Calculate vector pointing away from neighbor
                ofVec2f diff = position - boids[i].position;
                diff.normalize();
                diff /= d;        // Weight by distance
                steer += diff;
                count++;            // Keep track of how many
            }
        }
        // Average -- divide by how many
        if (count > 0) {
            steer /= count;
        }
        
        // As long as the vector is greater than 0
        if (steer.length() > 0) {
            // Implement Reynolds: Steering = Desired - Velocity
            steer.normalize();
            steer *= maxspeed;
            steer -= velocity;
            steer.limit(maxforce);
        }
        return steer;
    }
    
    // Alignment
    // For every nearby boid in the system, calculate the average velocity
    ofVec2f align(const vector<Boid>& boids) {
        float neighbordist = 50;
        ofVec2f sum(0, 0);
        int count = 0;
        for (int i = 0; i < boids.size(); i++) {
            float d = position.distance(boids[i].position);
            if ((d > 0) && (d < neighbordist)) {
                sum += boids[i].velocity;
                count++;
            }
        }
        if (count > 0) {
            sum /= count;
            sum.normalize();
            sum *= maxspeed;
            ofVec2f steer = sum - velocity;
            steer.limit(maxforce);
            return steer;
        } else {
            ofVec2f empty(0, 0);
            return empty;
        }
    }
    
    // Cohesion
    // For the average location (i.e. center) of all nearby boids, calculate steering vector towards that location
    ofVec2f cohesion(const vector<Boid>& boids) {
        float neighbordist = 30;
        ofVec2f sum(0, 0);
        int count = 0;
        for (int i = 0; i < boids.size(); i++) {
            float d = position.distance(boids[i].position);
            if ((d > 0) && (d < neighbordist)) {
                sum += boids[i].position; // Add location
                count++;
            }
        }
        if (count > 0) {
            sum /= count;
            return seek(sum);  // Steer towards the location
        } else {
            ofVec2f empty(0, 0);
            return empty;
        }
    }
};


class FlockingMode : public Mode {
    
public:
    FlockingMode(string _name, float _duration);
    void update(float hr);
    void draw();
    void reset();
    void preExit();
    
        
private:
    vector<Boid> boids;
    ofFbo fbo;
    
};


