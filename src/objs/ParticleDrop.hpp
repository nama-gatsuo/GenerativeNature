#pragma once
#include "ofMain.h"
#include "ObjBase.hpp"
#include "CommonUtil.hpp"

class ParticleDrop : public ObjBase {
public:
    void setup(){
        
        glEnable(GL_POINT_SPRITE);
        glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
        
        shader.load("shader/scene/ParticleDrop");
        
        mesh.setMode(OF_PRIMITIVE_POINTS);
        for (int i = 0; i < num; i++) {
            
            // y: random seed
            mesh.addVertex(ofVec3f(ofRandom(-1600., 1600.), ofRandom(1.), ofRandom(-1600., 1600.)));
            
        }
        
        rot.setSpeed(0.01);
        
    };
    void update(float dt){
        t += dt;
        rot.update(dt);
    };
    void draw(ofCamera& cam, bool isShadow){
        
        if (isShadow) return;
        
		ofEnableBlendMode(OF_BLENDMODE_ADD);
        shader.begin();
        shader.setUniform1f("farClip", cam.getFarClip());
        shader.setUniform1f("nearClip", cam.getNearClip());
        shader.setUniform1f("time", t);
        shader.setUniform3f("rot", rot);
        
        mesh.draw();
        
        shader.end();
		ofDisableBlendMode();
    };
    void randomize(int i){
        
        if (!rot.isMoving()){
            if (ofRandom(1.) < 0.1) {
                rot.to(ofPoint(PI, rot.y, rot.z));
            } else {
                rot.to(ofPoint(0.));
            }
        }
        
    };
private:
    
    int num = 128 * 128;
    ofVboMesh mesh;
    ofShader shader;
    SmoothPoint rot;
    
    float t;
    
};

