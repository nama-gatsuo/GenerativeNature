#pragma once
#include "ofMain.h"
#include "CommonUtil.hpp"
#include "ObjBase.hpp"

class Particle {
public:
    
    Particle(){
        pos.setSpeed(ofRandom(0.02, 0.05));
        
        lines.setMode(OF_PRIMITIVE_POINTS);
        pos.to(ofPoint(ofRandom(-300, 300), ofRandom(-300, 300), ofRandom(-300, 300)));
    };
    
    void update(vector<Particle> * ps, float dt){
        pos.update(dt);
        lines.clearVertices();
        
        for (int i = 0; i < ps->size(); i++) {
            
            float length = pos.distance(ps->at(i).pos);
            if (length > 0. && length < threshold) {
                lines.addVertex(ps->at(i).pos);
            }
        }
    };
    
    SmoothPoint pos;
    ofVboMesh lines;
    float threshold = 360.;
};

class ConnectedLights : public ObjBase {
public:
    void setup(){
        
        gbShader.load("shader/gbuffer.vert", "shader/customShader.frag");
        
        lShader.setGeometryInputType(GL_POINTS);
        lShader.setGeometryOutputType(GL_TRIANGLE_STRIP);
        lShader.setGeometryOutputCount(24);
        lShader.load("shader/scene/ConnectedLights.vert", "shader/scene/ConnectedLights.frag", "shader/scene/ConnectedLights.geom");
        
        for (int i = 0; i < num; i++) {
            
            ps.push_back(Particle());
            
        }
        
        p = ofMesh::sphere(10);
        for (int i = 0; i < p.getNumVertices(); i++) {
            p.addColor(ofFloatColor(0.1, 0.4, 4.0));
        }
        
    };
    void update(float dt){
        
        for (int i = 0; i < num; i++) {
            ps[i].update(&ps, dt);
        }
        
    };
    void draw(ofCamera &cam, bool isShadow){
        
        gbShader.begin();
        gbShader.setUniform1f("farClip", cam.getFarClip());
        gbShader.setUniform1f("nearClip", cam.getNearClip());
        lShader.setUniform1i("isShadow", isShadow?1:0);
        
        for (auto part : ps) {
            ofPushMatrix();
            ofTranslate(part.pos);
            
            p.draw();
            ofPopMatrix();
        }
        
        gbShader.end();
        
        for (auto part : ps) {
            
            lShader.begin();
            lShader.setUniform3f("to", part.pos);
            lShader.setUniform1i("isShadow", isShadow?1:0);
            lShader.setUniform1f("farClip", cam.getFarClip());
            lShader.setUniform1f("nearClip", cam.getNearClip());
            part.lines.draw();
            lShader.end();
        }
        
    };
    
    void randomize(int i){
        if (i == 0) {
            for (int i = 0; i < num; i++) {
                ps[i].pos.to(ofPoint(ofRandom(-300, 300), ofRandom(-300, 300), ofRandom(-300, 300)));
            }
        } else if (i == 1) {
            for (int i = 0; i < num; i++) {
                ps[i].pos.to(ofPoint(ofRandom(-600, 600), ofRandom(-600, 600), ofRandom(-600, 600)));
            }
        }
        
    };
private:
    ofShader lShader;
    ofShader gbShader;
    
    ofVboMesh p;
    const unsigned num = 40;
    vector<Particle> ps;
    
};
