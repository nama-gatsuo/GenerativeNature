#include "ofMain.h"
#include "ObjBase.hpp"

class ArcObj {
public:
    ArcObj(){
        startDelay = floor(ofRandom(40));
        startAngle = PI * ofRandom(1.5);
        SA = startAngle;
        angle = PI * ofRandom(0.5, 1.);
        radius = 30 + 200 * ofRandom(1.);
        morpDuration = 10;
        stayDuration = 30;
    };
    void update(float dt){
        t += dt;
        
        if (t < startDelay) mode = 0;
        else if (t < startDelay + morpDuration) mode = 1;
        else if (t < startDelay + morpDuration + stayDuration) mode = 2;
        else if (t < startDelay + morpDuration * 2 + stayDuration) mode = 3;
        else mode = 4;
        
        switch (mode) {
            case 0:
                endAngle = startAngle;
                break;
            case 1:
                endAngle = startAngle + angle * sin(HALF_PI * (t - startDelay) / morpDuration);
                break;
            case 2: break;
            case 3:
                startAngle = SA + angle * sin(HALF_PI * (t - (startDelay+morpDuration+stayDuration)) / morpDuration);
                break;
            case 4: break;
            default: break;
        }
        
    };
    
    bool isDead(){
        return mode == 4;
    };
    
    float radius;
    float angle;
    float startAngle, endAngle;
    int startDelay;
    int stayDuration, morpDuration;
    float t = 0;
    int mode = 0;
    float SA;
};

class Line {
public:
    Line(){
        
        angle = TWO_PI * ofRandom(1.);
        r1 = 50 + 50 * ofRandom(1.);
        dist = 50 + 120 * ofRandom(1.);
        SR = r1;
        
        startDelay = floor(ofRandom(40));
        morpDuration = 10;
        stayDuration = 30;
    };
    void update(float dt){
        t += dt;
        
        if (t < startDelay) mode = 0;
        else if (t < startDelay + morpDuration) mode = 1;
        else if (t < startDelay + morpDuration + stayDuration) mode = 2;
        else if (t < startDelay + morpDuration * 2 + stayDuration) mode = 3;
        else mode = 4;
        
        switch (mode) {
            case 0:
                r2 = r1;
                break;
            case 1:
                r2 = SR + dist * sin(HALF_PI * (t - startDelay) / morpDuration);
                break;
            case 2: break;
            case 3:
                r1 = SR + dist * sin(HALF_PI * (t - (startDelay+morpDuration+stayDuration)) / morpDuration);
                break;
            case 4: break;
            default: break;
        }
        
    };
    bool isDead(){
        return mode == 4;
    };
    
    float r1, r2, dist;
    float SR;
    float angle;
    int startDelay;
    int stayDuration, morpDuration;
    float t = 0;
    int mode = 0;
};

class WarpGate {
public:
    WarpGate(ofShader &ls, ofShader &as, ofVec3f pos){
        center = pos;
        
        lineShader = ls;
        arcShader = as;
        for (int i = 0; i < 30; i++) {
            arcs.push_back( ArcObj() );
        }
        
        for (int i = 0; i < 15; i++) {
            lines.push_back( Line() );
        }
        
        mesh = ofMesh::box(1., 1., 1., 1, 1, 1);
        
    };
    void update(float dt){
        vector<ArcObj>::iterator it;
        for (it = arcs.begin(); it < arcs.end();) {
            it->update(dt);
            if (it->isDead()) it = arcs.erase(it);
			else ++it;
        }
        
        vector<Line>::iterator itl;
        for (itl = lines.begin(); itl < lines.end();) {
            itl->update(dt);
            if (itl->isDead()) itl = lines.erase(itl);
			else  ++itl;
        }
    };
    void draw(ofCamera &cam, bool isShadow){
        
        ofPushMatrix();
        ofTranslate(center);
        
        for (auto arc : arcs) {
            arcShader.begin();
            arcShader.setUniform1i("isShadow", isShadow?1:0);
            arcShader.setUniform1i("total", 20);
            arcShader.setUniform1f("time", arc.t*0.04);
            arcShader.setUniform1f("radius", arc.radius);
            arcShader.setUniform1f("startAngle", arc.startAngle);
            arcShader.setUniform1f("endAngle", arc.endAngle);
            arcShader.setUniform1f("nearClip", cam.getNearClip());
            arcShader.setUniform1f("farClip", cam.getFarClip());
            
            mesh.drawInstanced(OF_MESH_FILL, 20);
            
            arcShader.end();
        }
        
        for (auto line : lines) {
            lineShader.begin();
            lineShader.setUniform1i("isShadow", isShadow?1:0);
            lineShader.setUniform1f("time", line.t*0.04);
            lineShader.setUniform1f("angle", line.angle);
            lineShader.setUniform1f("r1", line.r1);
            lineShader.setUniform1f("r2", line.r2);
            lineShader.setUniform1f("nearClip", cam.getNearClip());
            lineShader.setUniform1f("farClip", cam.getFarClip());
            
            mesh.draw();
            
            lineShader.end();
        }
        
        ofPopMatrix();
    };
    bool isDead(){
        return arcs.size() == 0;
    }
    
private:
    ofShader lineShader;
    ofShader arcShader;
    std::vector<ArcObj> arcs;
    vector<Line> lines;
    ofVec3f center;
    ofVboMesh mesh;
};

class WarpGates : public ObjBase {
public:
    void setup(){
        arcShader.load("shader/scene/arc.vert", "shader/scene/output.frag");
        lineShader.load("shader/scene/line.vert", "shader/scene/output.frag");
    };
    void update(float dt){
        vector<WarpGate>::iterator it;
        for (it = wgs.begin(); it < wgs.end(); it++) {
            it->update(dt);
            if (it->isDead()) wgs.erase(it);
        }
    };
    void draw(ofCamera& cam, bool isShadow){
        for (auto wg : wgs) {
            wg.draw(cam, isShadow);
        }
    };
    void randomize(int i){
        if (wgs.size() > MAX_COUNT) return;
        
        if (i == 0) {
            ofVec3f pos = ofVec3f(ofRandom(-1,1) * 500., ofRandom(0.1,2.)*300, ofRandom(-1,1) * 500.);
            wgs.push_back(WarpGate(lineShader,arcShader,pos));
        } else if (i == 1) {
            ofVec3f pos = ofVec3f(ofRandom(-1,1) * 500., ofRandom(0.0,1.)*200, ofRandom(-1,1) * 500.);
            wgs.push_back(WarpGate(lineShader,arcShader,pos));
        }
    };
private:
    ofShader lineShader;
    ofShader arcShader;
    vector<WarpGate> wgs;
    int MAX_COUNT = 16;
    
};
