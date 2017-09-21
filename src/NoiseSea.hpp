#include "ofMain.h"
#include "ObjBase.hpp"
#include "CommonUtil.hpp"

class NoiseSea : public ObjBase {
public:
    void setup(){
        shader.setGeometryInputType(GL_TRIANGLES);
        shader.setGeometryOutputType(GL_TRIANGLE_STRIP);
        shader.setGeometryOutputCount(3);
        shader.load("shader/scene/NoiseSea.vert",
                    "shader/scene/NoiseSea.frag",
                    "shader/scene/NoiseSea.geom");
        
        mesh = ofMesh::plane(3000, 3000, 64, 64);
        ofMatrix4x4 m = ofMatrix4x4();
        m.glRotate(90, 1, 0, 0);
        for (int i = 0; i < mesh.getNumVertices(); i++) {
            ofVec3f v = mesh.getVertex(i) * m;
            mesh.setVertex(i, v);
            mesh.setNormal(i, ofVec3f(0,1,0));
        }
        randomize(0);
        randomize(1);
    };
    void update(float dt){
        
        heightFactor.update(dt);
        colFactor.update(dt);
        noiseFactor.update(dt);
        wireWidth.update(dt);
        time += 0.03 * dt;
        
    };
    void draw(ofCamera& cam, bool isShadow){
        shader.begin();
        shader.setUniform1f("farClip", cam.getFarClip());
        shader.setUniform1f("nearClip", cam.getNearClip());
        shader.setUniform1f("colFactor", colFactor.get());
        shader.setUniform1f("heightFactor", heightFactor.get());
        shader.setUniform1f("noiseFactor", noiseFactor.get());
        shader.setUniform1f("wireWidth", 1.05 - wireWidth.get());
        shader.setUniform1f("time", time);
        
        mesh.draw();
        shader.end();
    };
    void randomize(int i){
        if (i == 0) {
            heightFactor.addForce(0.6);
            noiseFactor.to(ofRandom(0.01, 0.05));
            
            if (ofRandom(1.) < 0.4) {
                noiseFactor.to(ofRandom(0.01, 0.05));
            }
            if (ofRandom(1.) < 0.2) {
                wireWidth.addForce(ofRandom(0.6, 0.85));
            }
        } else if (i == 1) {
            colFactor.addForce(0.5);
        }
    };
    
private:
    ofVboMesh mesh;
    ofShader shader;
    float time;
    
    PhysicValue colFactor;
    PhysicValue heightFactor;
    SmoothValue noiseFactor;
    PhysicValue wireWidth;
    
};
