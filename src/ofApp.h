#pragma once

#include "ofMain.h"
#include "CommonUtil.hpp"
#include "ObjBase.hpp"
#include "NoiseSea.hpp"
#include "WarpGate.hpp"
#include "OctaWarms.hpp"
#include "OctaNest.hpp"
#include "GenTunnel.hpp"
#include "ConnectedLights.hpp"
#include "CalatravaStruct.hpp"
#include "FractalInstancing.hpp"
#include "RotateArc.hpp"
#include "TriWall.hpp"
#include "ParticleDrop.hpp"
#include "SubdivIcosa.hpp"
#include "ComplexFlower.hpp"
#include "ComplexBuilding.hpp"

#include "PostEffect.hpp"

#include "ofxOsc.h"
#include "ofxDeferredShading.h"
#include "ofxGui.h"

using namespace ofxDeferredShading;

struct RefObj {
    RefObj(shared_ptr<ObjBase> _ptr, int _index):ref(_ptr), index(_index){};
    
    shared_ptr<ObjBase> ref;
    bool isActive = false;
    int index;
};

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);

private:
    void setupDeferred();
    void updateDeferredParam();
    
    int refNum = 3;
    vector<RefObj> refObjs;
    
    vector<shared_ptr<ObjBase>> objs;
    ofxOscReceiver receiver;
    PostEffect pe;
    
    ofxDeferredProcessing deferred;
    PointLightPass* lightingPass;
    SsaoPass* ssaoPass;
    ShadowLightPass* shadowLightPass;
    HdrBloomPass* hdrBloomPass;
    DofPass* dofPass;
    
    SmoothPoint lp1, lp2;
    MovingCam cam;
    
    // gui
    bool isShow = false;
    ofxPanel panel;
    ofParameterGroup pl1;
    ofParameter<ofFloatColor> pl1_diff;
    ofParameter<ofFloatColor> pl1_spe;
    ofParameter<float> pl1_int;
    ofParameter<float> pl1_rad;
    
    ofParameterGroup pl2;
    ofParameter<ofFloatColor> pl2_diff;
    ofParameter<ofFloatColor> pl2_spe;
    ofParameter<float> pl2_int;
    ofParameter<float> pl2_rad;
    
    ofParameterGroup ao;
    ofParameter<float> ao_rad;
    ofParameter<float> ao_dark;
    
    ofParameterGroup shadow;
    ofParameter<float> sha_amb;
    ofParameter<float> sha_dif;
    ofParameter<float> sha_dark;
    ofParameter<float> sha_blend;
    ofParameter<float> sha_far;
    ofParameter<ofVec3f> sha_pos;
    
    ofParameterGroup dof;
    ofParameter<float> dof_blur;
    ofParameter<float> dof_ape;
    ofParameter<float> dof_focal;
    
    ofParameter<float> dt;
};
