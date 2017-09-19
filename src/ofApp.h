#pragma once

#include "ofMain.h"

#include "ofxOsc.h"
#include "ofxDeferredShading.h"
#include "ofxGui.h"

using namespace ofxDeferredShading;

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);

private:
    void setupDeferred();
    void updateDeferredParam();
    
    ofxDeferredProcessing deferred;
    PointLightPass* lightingPass;
    SsaoPass* ssaoPass;
    ShadowLightPass* shadowLightPass;
    HdrBloomPass* hdrBloomPass;
    DofPass* dofPass;
    
    SmoothPoint lp1, lp2;
    SmoothPoint camPos;
    SmoothPoint camLook;
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
    
    ofParameterGroup dof;
    ofParameter<float> dof_blur;
    ofParameter<float> dof_ape;
    ofParameter<float> dof_focal;
    
    ofParameter<float> dt;
};