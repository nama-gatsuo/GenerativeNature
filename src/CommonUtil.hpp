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
