#pragma once
#include "ofMain.h"
#include "ObjBase.hpp"
#include "CommonUtil.hpp"

class TriWall : public ObjBase {
public:
    void setup(){
        
        shader.load("shader/gbuffer.vert", "shader/customShader.frag");
        
        unsigned yn = 6;
        unsigned xn = 6;
        
        for (int i = 0; i < N; i++) {
            mesh.clear();
            num = floor(ofRandom(2.0, 8.0));
            
            if (i % 3 == 1) c.set(0.9, 0.92, 1.2);
            else c.set(0.6, 0.6, 0.7);
            
            randomSeeds.clear();
            createRandomSeed();
            
            float r = 400;
            float dh = r * 1.5;
            float dw = r * sqrt(3.);
            for (int i = 0; i < yn; i++) {
                ofVec3f v[3];
                
                for (int j = 0; j < xn; j++) {
                    
                    ofVec3f shift(dw * (j-xn*0.5), dh * (i-yn*0.5), 0.);
                    if (i%2==0) shift.x -= dw * 0.5;
                    
                    v[0] = ofVec3f(0,r,0) + shift;
                    v[1] = ofVec3f(-r*sqrt(3.)*0.5,-r*0.5,0) + shift;
                    v[2] = ofVec3f(r*sqrt(3.)*0.5,-r*0.5,0) + shift;
                    createTri(v[0], v[1], v[2]);
                    
                    v[0] = ofVec3f(r*sqrt(3.)*0.5,-r*0.5,0) + shift;
                    v[1] = ofVec3f(0,r,0) + shift;
                    v[2] = ofVec3f(r*sqrt(3.),r,0) + shift;
                    createTri(v[0], v[1], v[2]);
                }
                
            }
            meshes.push_back(mesh);
            SmoothValue s;
            s.setSpeed(0.005);
            zs.push_back(s);
        }
        randomize(0);
    };
    void update(float dt){
        for (int i = 0; i < N; i++) {
            zs[i].update(dt);
        }
    };
    void draw(ofCamera& cam, bool isShadow){
        
        shader.begin();
        shader.setUniform1i("isShadow",isShadow?1:0);
        shader.setUniform1f("farClip", cam.getFarClip());
        shader.setUniform1f("nearClip", cam.getNearClip());
        
        for (int i = 0; i < N; i++) {
            ofPushMatrix();
            ofTranslate(ofVec3f(0, 0, zs[i].get()));
            meshes[i].draw();
            ofPopMatrix();
        }
        
        shader.end();
    };
    void randomize(int i){
        
        float coin = ofRandom(1.0);
        
        if (coin <  0.8) {
            for (int i = 0; i < N; i++) {
                zs[i].to(ofRandom(-1200, 1200));
            }
        } else {
            for (int i = 0; i < N; i++) {
                zs[i].to(0.);
            }
        }
        
    };
private:
    void createRandomSeed(){
        randomSeeds.clear();
        for (int i = 0; i < num; i++) {
            vector<float> trs;
            for (int j = 0; j < num-i; j++) {
                trs.push_back(ofRandom(1.));
            }
            randomSeeds.push_back(trs);
        }
    };
    void createTri(ofVec3f v1, ofVec3f v2, ofVec3f v3){
        ofVec3f v[3];
        v[0] = v1;
        v[1] = v2;
        v[2] = v3;
        
        ofVec3f center = (v[0] + v[1] + v[2]) / 3.0;
        
        for (int i = 0; i < 3; i++) {
            int i1 = i, i2 = i+1;
            if (i == 2) i2 = 0;
            
            createPat(v[i1], center, (v[i2] + v[i1]) * 0.5);
            createPat(v[i2], center, (v[i2] + v[i1]) * 0.5);
            
        }
        
    };
    void createPat(ofVec3f v1, ofVec3f v2, ofVec3f v3){
        
        for (int i = 0; i < num; i++) {
            
            ofVec3f start = v1.getInterpolated(v3, (float)i/num);
            ofVec3f next;
            for (int j = 0; j < num-i; j++){
                
                ofVec3f target;
                if (randomSeeds[i][j] < 0.5) {
                    target = (v2 - v1) * (1.0 / num);
                } else if (randomSeeds[i][j] < 0.85) {
                    target = (v3 - v1) * (1.0 / num);
                } else {
                    ofVec3f t1 = (v2 - v1) * (1.0 / num);
                    ofVec3f t2 = (v3 - v1) * (1.0 / num);
                    
                    target = t1 * 2.0 - t2;
                }
                
                next = start + target;
                
                addBox(start, next, ofNoise(i*randomSeeds[i][j], j*randomSeeds[i][j]));
                
                start = next;
            }
        }
        
    };
    void addBox(glm::vec3 start, glm::vec3 end, float width){
        
		glm::vec3 rel = glm::normalize(end - start);
        float lat = acos(rel.y) - HALF_PI;
        float lon = atan2(rel.x, rel.z);
        
        glm::mat4 rot(1.f);

		rot = rot * glm::rotate(lat, glm::vec3(1.f, 0.f, 0.f));
		rot = rot * glm::rotate(lon, glm::vec3(0.f, 1.f, 0.f));
        
        float d = glm::distance(start, end);
        float h = 24. / num;
        float w = 24. / num;
        
        ofMesh box = ofMesh::box(w,h,d, 1,1,1);
        for (int i = 0; i < box.getNumVertices(); i++) {
			glm::mat4 model = glm::translate(start) * rot * glm::translate(glm::vec3(0, 0, d*0.5f));

			glm::vec4 v = model * glm::vec4(box.getVertex(i), 1.f);
            glm::vec4 n = glm::inverse(glm::transpose(model)) * glm::vec4(box.getNormal(i), 1.f);
            
            box.setVertex(i, v);
            box.setNormal(i, n);
            
            box.addColor(c);
        }
        mesh.append(box);
        
    };
    
    ofFloatColor c;
    vector<ofVboMesh> meshes;
    vector<SmoothValue> zs;
    ofVboMesh mesh;
    ofShader shader;
    
    unsigned num = 6;
    unsigned N = 8;
    vector<vector<float>> randomSeeds;
};
