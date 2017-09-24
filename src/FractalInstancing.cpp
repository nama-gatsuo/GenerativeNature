#include "FractalInstancing.hpp"

void FractInst::setup(){
    shader.load("shader/scene/FractInst.vert", "shader/customShader.frag");
    
    mesh = ofMesh::box(1.,1.,1., 1,1,1);
    
    ofFloatColor col;
    col.set(1.0);
    
    for (int i = 0; i < mesh.getNumIndices(); i++){
        mesh.addColor(col);
    }
    SmoothValue s;
    s.setSpeed(0.005);
    scale.assign(depth, s);
    translate.assign(depth, s);
    division = 6;
}

void FractInst::update(float dt){
    
    boxSize.update(dt);
    
    for (int i = 0; i < depth; i++) {
        
        scale[i].update(dt);
        translate[i].update(dt);
        
    }
    
}

void FractInst::draw(ofCamera& cam, bool isShadow){
    
    shader.begin();
    shader.setUniform1i("isShadow",isShadow?1:0);
    shader.setUniform1f("farClip", cam.getFarClip());
    shader.setUniform1f("nearClip", cam.getNearClip());
    shader.end();
    
    vector<float> sca;
    vector<float> tra;
    
    for (int i = 0; i < depth; i++) {
        sca.clear();
        tra.clear();
        
        for (int j = 0; j < depth; j++) {
            if (j < i) {
                sca.push_back(scale[j].get());
                tra.push_back(translate[j].get());
            } else {
                sca.push_back(1.);
                tra.push_back(0.);
            }
        }
        
        shader.begin();
        shader.setUniform3f("boxSize", boxSize);
        shader.setUniform1i("division", division);
        shader.setUniform1fv("dscale", sca.data(), depth);
        shader.setUniform1fv("translate", tra.data(), depth);
        shader.setUniform1i("index", i);
        mesh.drawInstanced(OF_MESH_FILL, pow(division, i));
        shader.end();
    }
}

void FractInst::randomize(int i){
    
    if (i == 0) {
        float coin = ofRandom(1.0);
        if (coin < 0.33) boxSize.to(ofPoint(5.,5., 200.));
        else if (coin < 0.66) boxSize.to(ofPoint(200., 5.,5.));
        else boxSize.to(ofPoint(5., 200., 5.));
        
//        coin = ofRandom(1.0);
//        if (coin < 0.5) division = 4;
//        else division = 6;
        
    } else if (i == 1) {
        
        for (int i = 0; i < depth; i++) {
            
            scale[i].to(ofRandom(0.5, 1.0));
            translate[i].to(ofRandom(0, 200.));
            
        }
    
    }
}

