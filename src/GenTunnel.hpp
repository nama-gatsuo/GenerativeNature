#include "ofMain.h"
#include "PartObj.hpp"
#include "ObjBase.hpp"

class GenTunnel : public ObjBase {
public:
    void setup(){};
    void update(float dt){
        shader.load("shader/gbuffer.vert", "shader/customShader.frag");
        vector<PartObj>::iterator it;
        for (it = parts.begin(); it < parts.end(); it++) {
            
            if (it->check()) {
                it = parts.erase(it);
            } else {
                it->update(dt);
            }
            
        }
        
        time += dt;
        if (time > 20.) {
            time = 0.;
            randomize(1);
        }
    };
    void draw(ofCamera& cam, bool isShadow){
        vector<PartObj>::iterator it;
        shader.begin();
        
        shader.setUniform1f("farClip", cam.getFarClip());
        shader.setUniform1f("nearClip", cam.getNearClip());
        shader.setUniform1i("isShadow", isShadow?1:0);
        for (it = parts.begin(); it < parts.end(); it++) {
            it->draw();
            
        }
        shader.end();
    };
    void randomize(int i){
        if (parts.size() > 20) return;
        
        PartObj p;
        float coin = ofRandom(1.0);
        if (i == 0) {
            if (coin < .5) p.setup(MULTI_WALL);
            else p.setup(LATTICE);
        } else if (i == 1) {
            if (coin < .5) p.setup(SPHERES);
            else p.setup(BLOCKS);
        }
        
        parts.push_back(p);
    };
    
private:
    vector<PartObj> parts;
    float time = 0;
    ofShader shader;
};
