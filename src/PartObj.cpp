#include "PartObj.hpp"

void PartObj::setup(PartMode mode) {
    
    life = floor(ofRandom(100, 200));
    speed = 3000. / life;
    aSpeed = ofRandom(-2., 2.);
    
    switch (mode) {
        case MULTI_WALL:
            createWalls();
            break;
        case LATTICE:
            createLattice();
            break;
        case BLOCKS: {
            
            int num = 4;
            float radius = 300;
            float depth = 300;
            float w = 8.;
            
            ofFloatColor c(1.2, 1.3, 0.7);
            for (int i = 0; i < num; i++) {
                ofMatrix4x4 m;
                m.glRotate(360. / num * i, 0, 0, 1.);
                m.glTranslate(0, radius, 0);
                createBox(m, c, w, w, depth);
            }
            
            aSpeed = 0.;
            
            break;
        }
        case SPHERES: {
            
            int num = 12;
            float radius = 500.;
            ofFloatColor c(1.5);
            
            for (int i = 0; i < num; i++) {
                ofMatrix4x4 m;
                m.glRotate(360. / num * i, 0, 0, 1.);
                m.glTranslate(0, radius, 0.);
                createSphere(m, c);
            }
            aSpeed = 0.;
            break;
        }
        default: break;
    }
    
    update(0.);
}

void PartObj::createWalls() {
    
    int aNum = floor(ofRandom(6., 12.));
    float radius = 600. * ofRandom(0.6, 1.0);
    float totalAngle = 360.* ofRandom(0.3, 0.6);
    float totalDepth = 400.* ofRandom(0.8, 1.0);
    float w = 8.;
    float x = radius * sin(totalAngle / aNum * 0.5 / 180. * PI) * 2.;
    x *= ofRandom(0.7, 1.1);
    
    ofFloatColor c(.5);
    ofFloatColor c2(1.5);
    for (int i = 0; i < aNum + 1; i++) {
        ofMatrix4x4 mi;
        mi.glRotate(totalAngle / aNum * i, 0, 0, 1.);
        mi.glTranslate(0, radius, 10 * i);
        createBox(mi, c, x, w, totalDepth);
        createBox(mi, c2, x, w*2., w);
        createBox(mi, c, w, w*2., totalDepth);
    }
    
}

void PartObj::createLattice(){
    int aNum = floor(ofRandom(4., 10.));
    int dNum = floor(ofRandom(4., 6.));
    float radius = 600. * ofRandom(0.8, 1.0);
    float totalDepth = 300.* ofRandom(0.8, 1.0);
    float totalAngle = 120. * ofRandom(0.5, 1.0);
    
    float w = 8.;
    float x = radius * sin(totalAngle / aNum * 0.5 / 180. * PI) * 2.;
    
    ofFloatColor c(0.5);
    
    for (int i = 0; i < aNum + 1; i++) {
        ofMatrix4x4 mi;
        mi.glRotate(totalAngle / aNum * i, 0, 0, 1.);
        mi.glTranslate(0, radius, 0);
        createBox(mi, c, w, w, totalDepth);
        
        if (i != aNum) {
            
            for (int j = 0; j < dNum + 1; j++) {
                
                ofMatrix4x4 mj;
                mj.glRotate(totalAngle / aNum * (i + 0.5), 0, 0, 1.);
                mj.glTranslate(0, radius, 0);
                mj.glTranslate(0, 0, totalDepth / dNum * (j - (dNum+1.)*0.5));
                createBox(mj, c, x, w, w);
                
            }
        }
        
    }
    
}

void PartObj::createSphere(ofMatrix4x4 &m, ofFloatColor &c) {
    
    ofVboMesh sphere = ofMesh::sphere(8.);
    for (int i = 0; i < sphere.getNumVertices(); i++) {
        ofVec3f v = sphere.getVertex(i);
        sphere.setVertex(i, v * m);
        sphere.addColor(c);
    }
    
    mesh.append(sphere);
    
}

void PartObj::createBox(ofMatrix4x4 &m, ofFloatColor &c, float w, float h, float d){
    ofMesh box = ofMesh::box(w, h, d, 1, 1, 1);
    int iNum = mesh.getNumVertices();
    
    for (int i = 0; i < box.getNumVertices(); i++) {
        ofVec3f v = box.getVertex(i);
        mesh.addVertex(v * m);
        mesh.addNormal(((box.getNormal(i) + v) * m - (v * m)).normalize());
        mesh.addColor(c);
    }
    
    for (int i = 0; i < box.getNumIndices(); i++) {
        mesh.addIndex(box.getIndex(i) + iNum);
    }
    
}

void PartObj::update(float dt){
    life -= dt;
    angle = aSpeed * life;
    float y = - speed * life + 1500.;
    pos.y = y;
}

void PartObj::draw() {
    
    ofPushMatrix();
    ofTranslate(pos.x, pos.y, pos.z);
    ofRotateX(90);
    ofRotateZ(angle);
    
    mesh.draw();
    ofPopMatrix();
}

bool PartObj::check() {
    return life < 0;
}

