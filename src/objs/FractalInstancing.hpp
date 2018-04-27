#include "ofMain.h"
#include "CommonUtil.hpp"
#include "ObjBase.hpp"

class FractInst : public ObjBase {
public:
    void setup();
    void update(float dt);
    void draw(ofCamera& cam, bool isShadow);
    void randomize(int i);
    
private:
    const int depth = 5;
    ofShader shader;
    ofVboMesh mesh;
    
    int division;
    vector<SmoothValue> scale;
    vector<SmoothValue> translate;
    
    SmoothPoint boxSize;
    
};
