#pragma once
#include "ofMain.h"

class PostEffect {
public:
    void setup();
    void begin();
    void end();
    void draw();
    
    void bang();
    void setMode(int mode);
    void setParam(int ch, float val);
    
    void enableGrey(bool isNega);
    void disableGrey();
    
private:
    ofFbo base;
    ofFbo composite;
    
    int mode = 0;
    float params[3] = { 1.0, 0.1, 0.0 };
    ofShader complexConv;
    ofShader mirrorConv;
    
    bool isGrey = false;
    bool isNega = false;
    ofShader greyConv;
    
};
