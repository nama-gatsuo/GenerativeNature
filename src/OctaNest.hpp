#include "ofMain.h"
#include "ObjBase.hpp"
#include "CommonUtil.hpp"

class OctaNest : public ObjBase {
public:
    void setup(){
        shader.setGeometryInputType(GL_TRIANGLES);
        shader.setGeometryOutputType(GL_TRIANGLE_STRIP);
        shader.setGeometryOutputCount(3);
        shader.load("shader/scene/OctaNest.vert",
             "shader/scene/wireframe.frag",
             "shader/scene/wireframe.geom");
        
        ofVec3f ov[6] = {
            ofVec3f(1,0,0), ofVec3f(-1,0,0), ofVec3f(0,1,0),
            ofVec3f(0,-1,0), ofVec3f(0,0,1), ofVec3f(0,0,-1)
        };
        unsigned oi[24] = {
            0, 2, 4, 0, 4, 3, 0, 3, 5, 0, 5, 2,
            1, 2, 5, 1, 5, 3, 1, 3, 4, 1, 4, 2
        };
        
        for (int i = 0; i < 24; i += 3) {
            ofVec3f v1 = ov[oi[i]] * 0.1;
            ofVec3f v2 = ov[oi[i+1]] * 0.1;
            ofVec3f v3 = ov[oi[i+2]] * 0.1;
            
            mesh.addVertex(v1);
            mesh.addVertex(v2);
            mesh.addVertex(v3);
            
        }
        mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    
        size = ofVec2f(18, 36);
        angleFactor.setSpeed(0.01);
    };
    void update(float dt){
        angleFactor.update(dt);
        scaleFactor.update(dt);
        phaseFactor.update(dt);
        wireWidth.update(dt);
    };
    void draw(ofCamera &cam, bool isShadow){
        
        shader.begin();
        shader.setUniform1f("nearClip", cam.getNearClip());
        shader.setUniform1f("farClip", cam.getFarClip());
        shader.setUniform1f("wireWidth", 0.05+wireWidth.get());
        shader.setUniform1i("isDrawFace", 0);
        shader.setUniform1i("isShadow", isShadow?1:0);
        shader.setUniform2f("size", size);
        shader.setUniform1f("scaleFactor", scaleFactor.get());
        shader.setUniform1f("phaseFactor", phaseFactor.get());
        shader.setUniform1f("angleFactor", angleFactor.get());
        mesh.drawInstanced(OF_MESH_FILL, size.x * size.y);
        shader.end();
    
    };
    void randomize(int i){
        if (i == 0) {
            angleFactor.to(ofRandom(PI, TWO_PI));
            scaleFactor.to(ofRandom(1., 10.));
            phaseFactor.to(ofRandom(0., PI));
        } else if (i == 1) {
            wireWidth.addForce(0.2);
        }
    };
private:
    ofVboMesh mesh;
    ofShader shader;
    ofVec2f size;
    
    SmoothValue angleFactor;
    SmoothValue scaleFactor;
    SmoothValue phaseFactor;
    PhysicValue wireWidth;
};
