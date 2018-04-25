#pragma once
#include "ofMain.h"
#include "ObjBase.hpp"
#include "CommonUtil.hpp"
using namespace glm;

class ComplexFlower : public ObjBase {
public:
    ComplexFlower(){};
    void setup(){
        shader.load("shader/scene/broken.vert", "shader/customShader.frag");
        mesh.clear();
        
		mat4 m(1.f);
		m = m * scale(vec3(500.f, 500.f, 500.f));
		m = m * translate(vec3(0., -2., 0));
		
        for (int i = 0; i < 8; i++) {
            
			m = m * rotate(ofDegToRad(30), vec3(0, 1, 0));
			m = m * scale(vec3(0.9));
			m = m * translate(vec3(0, 0.3, 0));
			
            ring(m);
            
        }
        seed.to(0.);
    };
    
    void update(float dt) {
        seed.update(dt);
    }
    
    void draw(ofCamera& cam, bool isShadow){
        
        shader.begin();
        shader.setUniform1i("isShadow",isShadow?1:0);
        shader.setUniform1f("farClip", cam.getFarClip());
        shader.setUniform1f("nearClip", cam.getNearClip());
        shader.setUniform1f("seed", seed.get());
        
        mesh.draw();
        
        shader.end();
        
    };
    
    void randomize(int i){
        float coin = ofRandom(1.);
        if (coin < 0.4) {
            seed.to(ofRandom(1.));
        } else if (coin < 0.9) {
            seed.to(0.);
        }
        
    };
private:
    void ring(const mat4& _m){
        
        for (int i = 0; i < 5; i++) {
            
			mat4 m(_m);
            m = m * rotate(ofDegToRad(72.f * i), vec3(0, 1.f, 0));
            m = m * translate(vec3(1.f,0,0));
            m = m * rotate((float)HALF_PI, vec3(0, 1., 0));
            petal(m);
            
        }
    }
    
    void petal(const mat4& _m){
        
        mat4 m(_m);
        m = m * rotate(ofDegToRad(80), vec3(1.f, 0, 0));
        for (int i = 0; i < 9; i++) {
            m = m * rotate(ofDegToRad(-6), vec3(1, 0, 0));
            m = m * translate(vec3(0, 1, 0));
            pane(m);
        }
        
        m = _m;
		m = m * rotate(ofDegToRad(80), vec3(1.f, 0, 0));
		m = m * rotate(ofDegToRad(-25), vec3(0, 0, 1.0f));
        for (int i = 0; i < 9; i++) {
            m = m * rotate(ofDegToRad(4), vec3(0, 0, 1.f));
            m = m * rotate(ofDegToRad(-6), vec3(1.f, 0, 0));
            m = m * translate(vec3(0, 1.f, 0));
            pane(m);
        }
        
        m = _m;
		m = m * rotate(ofDegToRad(80), vec3(1.f, 0, 0));
        m = m * rotate(ofDegToRad(25), vec3(0, 0, 1.0f));
        for (int i = 0; i < 9; i++) {
			m = m * rotate(ofDegToRad(-4), vec3(0, 0, 1.f));
			m = m * rotate(ofDegToRad(-6), vec3(1.f, 0, 0));
			m = m * translate(vec3(0, 1.f, 0));
            pane(m);
        }
    };
    
    void pane(const mat4& _m){
        
		mat4 m(_m);
        ofFloatColor c(0.1, 0.1, 0.1);
        
        float coin = ofRandom(1.);
        if (coin < 0.3) {

           
			m = m * scale(vec3(1.f, 1.f, 0.05f));
			addBox(m, c);
            
            coin = ofRandom(1.);
            if (coin < 0.4) {

				m = m * scale(vec3(0.8f, 0.8f, 2.f));
                addBox(m, c);
            }
        }
        
        m = _m;
        coin = ofRandom(1.);
        if (coin < 0.9) {
            beam(m);
        }
        
    }
    
    void beam(const mat4& _m){
        
        ofFloatColor c(1.);
        ofFloatColor bc(0.1);
        ofFloatColor lc(1.5, 0.8, 1.2);
        
        // vertical
        mat4 m(_m);
        m = m * translate(vec3(0, 0, -0.2f));
        m = m * scale(vec3(0.05f, 1.f, 0.05f));
        addBox(m, c);
        
        m = _m;
		m = m * translate(vec3(0, 0, 0.2f));
		m = m * scale(vec3(0.05f, 1.f, 0.05f));
        addBox(m, c);
        
        m = _m;
		m = m * scale(vec3(0.05f, 0.05f, 0.4f));
		addBox(m, c);
        
        m = _m;
        m = m * translate(vec3(0, 0.5f, 0));
        m = m * scale(vec3(0.05f, 0.05f, 0.4f));
		addBox(m, c);
        
        m = _m;
        m = m * translate(vec3(0, 0.5f, 0));
        m = m * scale(vec3(0.05f, 0.05f, 0.4f));
		addBox(m, c);
        
        // horizontal
        float coin = ofRandom(1.);
        if (coin < 0.1) {
            m = _m;
            m = m * scale(vec3(1.f, 0.01f, 0.3f));
			addBox(m, c);
            
            m = _m;
            m = m * translate(vec3(0, 0, -0.22f));
            m = m * scale(vec3(1.f, 0.05f, 0.05f));
			addBox(m, lc);
            
            m = _m;
            m = m * translate(vec3(0, 0, 0.22f));
            m = m * scale(vec3(1.f, 0.03f, 0.05f));
			addBox(m, c);
            
            m = _m;
            m = m * translate(vec3(0, 0.03f, 0));
            m = m * scale(vec3(0.05f, 0.05f, 0.4f));
			addBox(m, c);
        }
        
    };
    
    void addBox(const mat4& _m, const ofFloatColor& _c){
        
        ofMesh box = ofMesh::box(1,1,1, 1,1,1);
        for (int i = 0; i < box.getNumVertices(); i++) {
            
            vec4 v = _m * vec4(box.getVertex(i), 1.f);
            vec4 n = inverse(transpose(_m)) * vec4(box.getNormal(i), 0.f);
            
            box.setVertex(i, v);
            box.setNormal(i, n);
            box.addColor(_c);
        }
        
        mesh.append(box);
        
    };
    
    ofVboMesh mesh;
    ofShader shader;
    SmoothValue seed;
};
