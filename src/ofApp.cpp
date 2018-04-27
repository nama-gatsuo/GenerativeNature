#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
    ofSetFrameRate(30);
    
    pe.setup();
    setupDeferred();
    updateDeferredParam();
    panel.add(dt.set("dt", 2.0, 0.001, 4.0));
    receiver.setup(7401);
    
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
    
    shared_ptr<ObjBase> o7(new CalatravaStruct());
    o7->setup();
    objs.push_back(o7);
    
    shared_ptr<ObjBase> o8(new FractInst());
    o8->setup();
    objs.push_back(o8);
    
    shared_ptr<ObjBase> o9(new RotateArc());
    o9->setup();
    objs.push_back(o9);
    
    shared_ptr<ObjBase> o10(new TriWall());
    o10->setup();
    objs.push_back(o10);
    
    shared_ptr<ObjBase> o11(new ParticleDrop());
    o11->setup();
    objs.push_back(o11);
    
    shared_ptr<ObjBase> o12(new SubdivIcosa());
    o12->setup();
    objs.push_back(o12);
    
    shared_ptr<ObjBase> o13(new ComplexFlower());
    o13->setup();
    objs.push_back(o13);
    
    shared_ptr<ObjBase> o14(new ComplexBuilding());
    o14->setup();
    objs.push_back(o14);
    
//    RefObj refObj(o1, 1);
    RefObj refObj(o11, 11);
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
    
    cam.update(dt.get());
    
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
            
            cam.bang();
            
        } else if (dirs[0] == "lights") {
            lp1.to(ofPoint(ofRandom(0, 600), ofRandom(30, 500), ofRandom(-600, 600)));
            lp2.to(ofPoint(ofRandom(-600, 0), ofRandom(30, 500), ofRandom(-600, 600)));
        } else if (dirs[0] == "vfx") {
            
            int index = m.getArgAsInt(0);
            
            if (index == -1) {
                float coin = ofRandom(1.);
                if (coin < 0.4) pe.setMode(0);
                else if (coin < 0.45) pe.setMode(1);
                else if (coin < 0.5) pe.setMode(2);
                else if (coin < 0.55) pe.setMode(3);
                
                coin = ofRandom(1.);
                if (coin < 0.4) pe.disableColor();
                else if (coin < 0.5) pe.enableColor(ofFloatColor(1.), false);
                else if (coin < 0.6) pe.enableColor(ofFloatColor(1.), true);

            } else if (index == 0) {
                pe.setMode(0);
                pe.disableColor();
            }
            else if (index == 1) pe.setMode(1);
            else if (index == 2) pe.setMode(2);
            else if (index == 3) pe.setMode(3);
            else if (index == 4) pe.enableColor(ofFloatColor(1.), true);
            else if (index == 5) pe.enableColor(ofFloatColor(1.), false);
            else if (index == 6) pe.enableColor(ofFloatColor(227./225.,84./225., 68./225.) , true);
            else if (index == 7) pe.enableColor(ofFloatColor(68./225.,  227/225.,84./225.) , false);
            
        } else if (dirs[0] == "dt") {
            float to = m.getArgAsFloat(0);
            dt.set(0.1 + to * 4.);
        }
        
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
	shadowLightPass->beginShadowMap(true);
    for (int i = 0; i < refObjs.size(); i++) {
        if (refObjs[i].isActive) refObjs[i].ref->draw(shadowLightPass->getCamera(), true);
    }
    lightingPass->drawLights(shadowLightPass->getCamera(), true);
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
    
    if (isShow) {
        panel.draw();
        shadowLightPass->debugDraw();
        deferred.debugDraw();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'h') isShow = !isShow;
    //if (key == ' ') refObjs[0].ref->randomize(0);
}

void ofApp::setupDeferred(){
    deferred.init(ofGetWidth(), ofGetHeight());
    ssaoPass = deferred.createPass<SsaoPass>().get();
	ssaoPass->setOcculusionRadius(10.f);
	ssaoPass->setDarkness(1.f);
    
    lightingPass = deferred.createPass<PointLightPass>().get();
    ofxDeferredShading::PointLight dlight;
    dlight.ambientColor = ofFloatColor(0.005f);
	dlight.radius = 500.f;
	lightingPass->addLight(dlight);
    
    dlight.ambientColor = ofFloatColor(0.005f);
    lightingPass->addLight(dlight);
    
	shadowLightPass = deferred.createPass<ShadowLightPass>().get();
	shadowLightPass->setEnabled(true);
	shadowLightPass->setNear(1.f);
	shadowLightPass->setFar(4000.f);
	//shadowLightPass->setPosition(200., 2500.0, -800.);
	shadowLightPass->setDirection(normalize(vec3(-200., -2500.0, 800.)));
	shadowLightPass->setDarkness(0.9f);
	shadowLightPass->setAmbientColor(0.2f);
	shadowLightPass->setDiffuseColor(0.3f);

    hdrBloomPass = deferred.createPass<HdrBloomPass>().get();
	//hdrBloomPass->setEnabled(false);

    dofPass = deferred.createPass<DofPass>().get();
    
    // gui
    panel.setup();
    pl1.setName("Point light 1");
    pl1.add(pl1_diff.set("Diffuse Color", ofFloatColor(0.65, 0.4, 0.5)));
    pl1.add(pl1_spe.set("Specular Color", ofFloatColor(0.9)));
    pl1.add(pl1_int.set("Intensity", 0.44, 0.1, 3.0));
    panel.add(pl1);
    
    pl2.setName("Point light 2");
    pl2.add(pl2_diff.set("Diffuse Color", ofFloatColor(00.3, 0.5, 0.66)));
    pl2.add(pl2_rad.set("Radius", 400, 100, 2000));
    pl2.add(pl2_int.set("Intensity", 0.44, 0.1, 3.0));
    panel.add(pl2);
    
    ao.setName("Ambient Occlusion");
    /*ao.add(ao_rad.set("Occlusion Radius", 8.0, 0.1, 100.0));
    ao.add(ao_dark.set("Darkness", 0.8, 0.1, 5.0));*/
    panel.add(ao);
    
    shadow.setName("Shadow Light");
    shadow.add(sha_pos.set("Position", ofVec3f(200., 2500.0, -800.), ofVec3f(0.), ofVec3f(4000)));
    panel.add(shadow);
    
    shadowLightPass->setPosition(sha_pos.get());
    shadowLightPass->lookAt(ofVec3f(0.));
    
    dof.setName("Defocus Blur");
    dof.add(dof_blur.set("Max Blur", 0.5, 0.0, 1.0));
    dof.add(dof_ape.set("Aperture", 0.1, 0.0, 1.0));
    dof.add(dof_focal.set("Focus Distance", 0.1, 0.0, 1.0));
    panel.add(dof);
    
}

void ofApp::updateDeferredParam(){
    
    lightingPass->getLightRef(0).diffuseColor = pl1_diff.get();
    lightingPass->getLightRef(0).specularColor = pl1_spe.get();
    lightingPass->getLightRef(0).intensity = pl1_int.get();
    
    lightingPass->getLightRef(1).diffuseColor = pl2_diff.get();
    lightingPass->getLightRef(1).specularColor = pl2_spe.get();
    lightingPass->getLightRef(1).intensity = pl2_int.get();
    
    /*ssaoPass->setOcculusionRadius(ao_rad.get());
    ssaoPass->setDarkness(ao_dark.get());*/
	
    /*shadowLightPass->setPosition(sha_pos.get());
    shadowLightPass->lookAt(ofVec3f(0.));*/
    
    dofPass->setFocus(dof_focal.get());
    dofPass->setMaxBlur(dof_blur.get());
    dofPass->setAperture(dof_ape.get());
}
