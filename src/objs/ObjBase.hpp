#pragma once
#include "ofMain.h"

class ObjBase{
public:
    virtual void setup(){};
    virtual void update(float dt){};
    virtual void draw(ofCamera& cam, bool isShadow){};
    virtual void randomize(int i){};
};
