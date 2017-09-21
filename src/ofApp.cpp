#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    pe.setup();
    setupDeferred();
    updateDeferredParam();
    panel.add(dt.set("dt", 1.0, 0.001, 2.0));
    receiver.setup(7401);
    camPos.setSpeed(0.01);
    lp1.setSpeed(0.01);
    lp2.setSpeed(0.01);
    
    shared_ptr<ObjBase> o1(new NoiseSea());
    o1->setup();
    objs.push_back(o1);
    
    shared_ptr<ObjBase> o2(new WarpGates());
    o2->setup();
    objs.push_back(o2);
    
    shared_ptr<ObjBase> o3(new OctaWarms());
    o3->setup();
    objs.push_back(o3);
    
    shared_ptr<ObjBase> o4(new OctaNest());
    o4->setup();
    objs.push_back(o4);
    
    shared_ptr<ObjBase> o5(new GenTunnel());
    o5->setup();
    objs.push_back(o5);
    
    shared_ptr<ObjBase> o6(new ConnectedLights());
    o6->setup();
    objs.push_back(o6);
    
    RefObj refObj(o1, 1);
    refObj.isActive = true;
    refObjs.push_back(refObj);
    
    refObj.isActive = false;
    refObjs.push_back(refObj);
    refObj.isActive = false;
    refObjs.push_back(refObj);
}

//--------------------------------------------------------------
void ofApp::update(){
    if (isShow) updateDeferredParam();
    camPos.update(dt.get());
    cam.setPosition(camPos);
    cam.lookAt(camLook);
    lp1.update(dt.get());
    lp2.update(dt.get());
    lightingPass->getLightRef(0).position = lp1;
    lightingPass->getLightRef(1).position = lp2;
    
    for (int i = 0; i < refObjs.size(); i++) {
        if (refObjs[i].isActive) refObjs[i].ref->update(dt.get());
    }
    
    while (receiver.hasWaitingMessages()){
        ofxOscMessage m;
        receiver.getNextMessage(m);
        
        string address = m.getAddress();
        vector<string> dirs = ofSplitString(address, "/");
        
        if (dirs[0] == "bang") {
            int index = ofToInt(dirs[1]);
            int bangIndex = m.getArgAsInt(0);
            
            if (refObjs[index].isActive) refObjs[index].ref->randomize(bangIndex);
            
        } else if (dirs[0] == "change") {
            int index = ofToInt(dirs[1]);
            int objIndex = m.getArgAsInt(0);
            
            if (objIndex == 0) refObjs[index].isActive = false;
            else {
                
                refObjs[index].ref = objs[objIndex-1];
                refObjs[index].index = objIndex;
                refObjs[index].isActive = true;
                
                if ((refObjs[0].index == refObjs[1].index) && refObjs[0].isActive && refObjs[1].isActive ) {
                    refObjs[1].isActive = false;
                } else if ((refObjs[0].index == refObjs[2].index) && refObjs[0].isActive && refObjs[2].isActive ) {
                    refObjs[2].isActive = false;
                }
                
            }
            
        } else if (dirs[0] == "cam") {
            float coin = ofRandom(1.);
            if (coin < 0.6) {
                camPos.to(ofPoint(ofRandom(-800, 800), ofRandom(50, 400), ofRandom(600, 1200)));
                camLook.to(ofPoint(ofRandom(-50, 50), ofRandom(30, 800), ofRandom(-50, 50)));
            } else if (coin < 0.8) {
                camPos.to(ofPoint(ofRandom(-200, 200), ofRandom(1000, 1200), ofRandom(-200, 200)));
                camLook.to(ofPoint(0., 0., 0.));
            } else {
                camPos.to(ofPoint(ofRandom(-800, 800), ofRandom(50, 400), ofRandom(0, 600)));
                camLook.to(ofPoint(ofRandom(-50, 50), ofRandom(30, 800), ofRandom(-50, 50)));
            }
            
        } else if (dirs[0] == "vfx") {
            lp1.to(ofPoint(ofRandom(0, 600), ofRandom(30, 500), ofRandom(-600, 600)));
            lp2.to(ofPoint(ofRandom(-600, 0), ofRandom(30, 500), ofRandom(-600, 600)));
            
            float coin = ofRandom(1.);
            if (coin < 0.4) pe.setMode(0);
            else if (coin < 0.45) pe.setMode(1);
            else if (coin < 0.5) pe.setMode(2);
            else if (coin < 0.55) pe.setMode(3);
            
            coin = ofRandom(1.);
            if (coin < 0.4) pe.disableGrey();
            else if (coin < 0.5) pe.enableGrey(false);
            else if (coin < 0.6) pe.enableGrey(true);
            
        } else if (dirs[0] == "dt") {
            float to = m.getArgAsFloat(0);
            dt.set(0.1 + to * 2.);
        }
        
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    shadowLightPass->beginShadowMap(true);
    ofCamera sc = shadowLightPass->getCam();
    for (int i = 0; i < refObjs.size(); i++) {
        if (refObjs[i].isActive) refObjs[i].ref->draw(sc, true);
    }
    lightingPass->drawLights(sc, true);
    shadowLightPass->endShadowMap();
    
    deferred.begin(cam, true);
    for (int i = 0; i < refObjs.size(); i++) {
        if (refObjs[i].isActive) refObjs[i].ref->draw(cam, false);
    }
    lightingPass->drawLights(cam, false);
    deferred.end(false);
    
    pe.begin();
    deferred.draw();
    pe.end();
    pe.draw();
    
    if (isShow) panel.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'h') isShow = !isShow;
    //if (key == ' ') refObjs[0].ref->randomize(0);
}

void ofApp::setupDeferred(){
    deferred.init(ofGetWidth(), ofGetHeight());
    ssaoPass = deferred.createPass<SsaoPass>().get();
    
    shadowLightPass = deferred.createPass<ShadowLightPass>().get();
    shadowLightPass->lookAt(ofVec3f(0.0));
    shadowLightPass->setCam(75, 0.1, 5000);
    shadowLightPass->setPosition(0, 1500.0, 500);
    shadowLightPass->lookAt(ofVec3f(0.0));
    
    lightingPass = deferred.createPass<PointLightPass>().get();
    ofxDeferredShading::PointLight dlight;
    dlight.ambientColor = ofFloatColor(0.005);
    lightingPass->addLight(dlight);
    
    dlight.ambientColor = ofFloatColor(0.0);
    lightingPass->addLight(dlight);
    
    hdrBloomPass = deferred.createPass<HdrBloomPass>().get();
    dofPass = deferred.createPass<DofPass>().get();
    
    // gui
    panel.setup();
    pl1.setName("Point light 1");
    pl1.add(pl1_diff.set("Diffuse Color", ofFloatColor(0.65, 0.4, 0.5)));
    pl1.add(pl1_spe.set("Specular Color", ofFloatColor(0.9)));
    pl1.add(pl1_rad.set("Radius", 400, 100, 2000));
    pl1.add(pl1_int.set("Intensity", 0.44, 0.1, 3.0));
    panel.add(pl1);
    
    pl2.setName("Point light 2");
    pl2.add(pl2_diff.set("Diffuse Color", ofFloatColor(00.3, 0.5, 0.66)));
    pl2.add(pl2_spe.set("Specular Color", ofFloatColor(0.9)));
    pl2.add(pl2_rad.set("Radius", 400, 100, 2000));
    pl2.add(pl2_int.set("Intensity", 0.44, 0.1, 3.0));
    panel.add(pl2);
    
    ao.setName("Ambient Occlusion");
    ao.add(ao_rad.set("Occlusion Radius", 5.0, 0.1, 100.0));
    ao.add(ao_dark.set("Darkness", 0.8, 0.1, 5.0));
    panel.add(ao);
    
    shadow.setName("Shadow Light");
    shadow.add(sha_amb.set("Ambient", 0.1, 0.0, 1.0));
    shadow.add(sha_dif.set("Diffuse", 0.25, 0.0, 1.0));
    shadow.add(sha_dark.set("Shadow Darkness", 0.4, 0.0, 1.0));
    shadow.add(sha_blend.set("Lighting Blend", 0.4, 0.0, 1.0));
    panel.add(shadow);
    
    dof.setName("Defocus Blur");
    dof.add(dof_blur.set("Max Blur", 0.5, 0.0, 1.0));
    dof.add(dof_ape.set("Aperture", 0.1, 0.0, 1.0));
    dof.add(dof_focal.set("Focus Distance", 0.1, 0.0, 1.0));
    panel.add(dof);
    
    
}

void ofApp::updateDeferredParam(){
    
    lightingPass->getLightRef(0).diffuseColor = pl1_diff.get();
    lightingPass->getLightRef(0).specularColor = pl1_spe.get();
    lightingPass->getLightRef(0).radius = pl1_rad.get();
    lightingPass->getLightRef(0).intensity = pl1_int.get();
    
    lightingPass->getLightRef(1).diffuseColor = pl2_diff.get();
    lightingPass->getLightRef(1).specularColor = pl2_spe.get();
    lightingPass->getLightRef(1).intensity = pl2_int.get();
    lightingPass->getLightRef(1).radius = pl2_rad.get();
    
    ssaoPass->setOcculusionRadius(ao_rad.get());
    ssaoPass->setDarkness(ao_dark.get());
    
    shadowLightPass->setAmbientColor(ofFloatColor(sha_amb.get()));
    shadowLightPass->setDiffuseColor(ofFloatColor(sha_dif.get()));
    shadowLightPass->setDarkness(sha_dark.get());
    shadowLightPass->setBlend(sha_blend.get());
    
    dofPass->setFocus(dof_focal.get());
    dofPass->setMaxBlur(dof_blur.get());
    dofPass->setAperture(dof_ape.get());
}
