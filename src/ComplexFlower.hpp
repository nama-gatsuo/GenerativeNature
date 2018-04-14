#pragma once
#include "ofMain.h"
#include "ObjBase.hpp"
#include "CommonUtil.hpp"

class ComplexFlower : public ObjBase {
public:
    ComplexFlower(){};
    void setup(){
        shader.load("shader/scene/broken.vert", "shader/customShader.frag");
        mesh.clear();
        
		glm::mat4 m(1.f);
		m = m * glm::scale(glm::vec3(500.f, 500.f, 500.f));
		m = m * glm::translate(glm::vec3(0., -2., 0));
		
        for (int i = 0; i < 8; i++) {
            
			m = m * glm::rotate(ofDegToRad(30), glm::vec3(0, 1, 0));
			m = m * glm::scale(glm::vec3(0.9));
			m = m * glm::translate(glm::vec3(0, 0.3, 0));
			
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
    void ring(glm::mat4 _m){
        ofMatrix4x4 m = _m;
        
        for (int i = 0; i < 5; i++) {
            
            ofMatrix4x4 mm = m;
            mm.glRotate(72 * i, 0, 1, 0);
            mm.glTranslate(1,0,0);
            mm.glRotate(90,0,1,0);
            petal(mm);
            
        }
    }
    
    void petal(ofMatrix4x4 _m){
        
        ofMatrix4x4 m = _m;
        m.glRotate(80, 1, 0, 0);
        for (int i = 0; i < 9; i++) {
            m.glRotate(-6, 1, 0, 0);
            m.glTranslate(0, 1, 0);
            pane(m);
        }
        
        m = _m;
        m.glRotate(80, 1, 0, 0);
        m.glRotate(-25, 0, 0, 1);
        for (int i = 0; i < 9; i++) {
            m.glRotate(4, 0, 0, 1);
            m.glRotate(-6, 1, 0, 0);
            m.glTranslate(0, 1, 0);
            pane(m);
        }
        
        m = _m;
        m.glRotate(80, 1, 0, 0);
        m.glRotate(25, 0, 0, 1);
        for (int i = 0; i < 9; i++) {
            m.glRotate(-4, 0, 0, 1);
            m.glRotate(-6, 1, 0, 0);
            m.glTranslate(0, 1, 0);
            pane(m);
        }
    };
    
    void pane(ofMatrix4x4 _m){
        
		glm::mat4 m = _m;
        ofFloatColor c(0.1, 0.1, 0.1);
        
        float coin = ofRandom(1.);
        if (coin < 0.3) {

           
			m = m * glm::scale(glm::vec3(1.f, 1.f, 0.05f));
			addBox(m, c);
            
            coin = ofRandom(1.);
            if (coin < 0.4) {

				m = m * glm::scale(glm::vec3(0.8f, 0.8f, 2.f));
                addBox(m, c);
            }
        }
        
        m = _m;
        coin = ofRandom(1.);
        if (coin < 0.9) {
            beam(m);
        }
        
    }
    
    void beam(ofMatrix4x4 _m){
        
        ofFloatColor c(1.);
        ofFloatColor bc(0.1);
        ofFloatColor lc(1.5, 0.8, 1.2);
        
        // vertical
        ofMatrix4x4 m = _m;
        m.glTranslate(0, 0, -0.2);
        m.glScale(0.05, 1, 0.05);
		glm::mat4 gm = _m;
        addBox(gm, c);
        
        m = _m;
        m.glTranslate(0, 0, 0.2);
        m.glScale(0.05, 1, 0.05);
		gm = _m;
        addBox(gm, c);
        
        m = _m;
        m.glScale(0.05, 0.05, 0.4);
		gm = _m;
		addBox(gm, c);
        
        m = _m;
        m.glTranslate(0, 0.5, 0);
        m.glScale(0.05, 0.05, 0.4);
		gm = _m;
		addBox(gm, c);
        
        m = _m;
        m.glTranslate(0, 0.5, 0);
        m.glScale(0.05, 0.05, 0.4);
		gm = _m;
		addBox(gm, c);
        
        
        // horizontal
        float coin = ofRandom(1.);
        if (coin < 0.1) {
            m = _m;
            m.glScale(1., 0.01, 0.3);
			gm = _m;
			addBox(gm, c);
            
            m = _m;
            m.glTranslate(0, 0, -0.22);
            m.glScale(1., 0.05, 0.05);
			gm = _m;
			addBox(gm, lc);
            
            m = _m;
            m.glTranslate(0, 0, 0.22);
            m.glScale(1., 0.03, 0.05);
			gm = _m;
			addBox(gm, c);
            
            m = _m;
            m.glTranslate(0, 0.03, 0);
            m.glScale(0.05, 0.05, 0.4);
			gm = _m;
			addBox(gm, c);
        }
        
    };
    
    void addBox(glm::mat4& _m, ofFloatColor& _c){
        
        ofMesh box = ofMesh::box(1,1,1, 1,1,1);
        for (int i = 0; i < box.getNumVertices(); i++) {
            
            glm::vec4 v = _m * glm::vec4(box.getVertex(i), 1.f);
            glm::vec4 n = glm::inverse(glm::transpose(_m)) * glm::vec4(box.getNormal(i), 1.f);
            
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
