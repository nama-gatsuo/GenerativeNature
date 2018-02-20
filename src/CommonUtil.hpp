#pragma once
#include "ofMain.h"

class SmoothPoint : public ofPoint{
public:
    SmoothPoint():mSpeed(0.05){};
    void update(float dt){
        this->x += (mTarget.x - this->x) * mSpeed * dt;
        this->y += (mTarget.y - this->y) * mSpeed * dt;
        this->z += (mTarget.z - this->z) * mSpeed * dt;
    };
    void to(ofPoint target){
        mTarget = target;
    };
    void setSpeed(float speed){
        mSpeed = speed;
    };
    bool isMoving(){
        float d2 = pow(mTarget.x - this->x, 2.) + pow(mTarget.y - this->y, 2.) + pow(mTarget.z - this->z, 2.);
        return sqrt(d2) > 100.;
    };
    
private:
    ofPoint mTarget;
    float mSpeed;
};

class SmoothValue {
public:
    SmoothValue():mSpeed(0.05),mValue(0),mTarget(0){}
    
    void update(float dt){
        mValue += (mTarget - mValue) * mSpeed * dt;
    };
    void to(float target){
        mTarget = target;
    };
    float get(){
        return mValue;
    };
    void setSpeed(float speed){
        mSpeed = speed;
    };
    
private:
    float mValue;
    float mTarget;
    float mSpeed;
};

class PhysicValue {
public:
    PhysicValue():pos(0.),vel(0.),acce(0.){};
    void update(float dt){
        vel += acce;
        vel -= 0.002;
        pos += vel * dt;
        if (vel > .3) {
            vel = .3;
        }
        
        if (pos < 0.) {
            vel = 0.;
            pos = 0.;
        } else if (pos > 1.) {
            pos = 0.99;
        }
        acce = 0.;
    };
    void addForce(float force){
        acce += force * 0.1;
    };
    float get(){ return pos; };
private:
    float pos;
    float vel;
    float acce;
};

class WireFrameShader : public ofShader {
public:
    void setup(){
        setGeometryInputType(GL_TRIANGLES);
        setGeometryOutputType(GL_TRIANGLE_STRIP);
        setGeometryOutputCount(3);
        load("shader/scene/wireframe.vert",
             "shader/scene/wireframe.frag",
             "shader/scene/wireframe.geom");
    };
    void begin(ofCamera &cam, float wireWidth, bool isDrawFace, bool isShadow){
        ofShader::begin();
        setUniform1f("nearClip", cam.getNearClip());
        setUniform1f("farClip", cam.getFarClip());
        setUniform1f("wireWidth", wireWidth);
        setUniform1i("isDrawFace", isDrawFace?1:0);
        setUniform1i("isShadow", isShadow?1:0);
    };
    
};

class MovingCam : public ofCamera {
public:
    MovingCam(){
        pos.setSpeed(0.02);
        bang();
    };
    void update(float dt){
        
        pos.update(dt);
        look.update(dt);
        ofCamera::setPosition(pos);
        ofCamera::lookAt(look);
    
    };
    void bang(){
        
        if (!pos.isMoving()) {
            float coin = ofRandom(1.);
            if (coin < 0.7) {
                
                pos.to(ofPoint(ofRandom(-1000, 1000), ofRandom(50, 400), ofRandom(-1000, 1000)));
                look.to(ofPoint(ofRandom(-50, 50), ofRandom(10, 10), ofRandom(-50, 50)));
            } else if (coin < 0.9) {
                // looking down on
                pos.to(ofPoint(ofRandom(-200, 200), ofRandom(1000, 1200), ofRandom(-200, 200)));
                look.to(ofPoint(0., 0., 0.));
            } else {
                // look from near center
                pos.to(ofPoint(ofRandom(-80, 80), ofRandom(10, 80), ofRandom(-80, 80)));
                look.to(ofPoint(ofRandom(-50, 50), ofRandom(-10, 10), ofRandom(-50, 50)));
            }
        }
    };
    
private:
    SmoothPoint pos;
    SmoothPoint look;
    
};


