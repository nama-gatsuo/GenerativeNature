#include "ofMain.h"
#include "ObjBase.hpp"
#include "CommonUtil.hpp"

class CalatravaStruct : public ObjBase {
public:
    void setup();
    void update(float dt);
    void draw(ofCamera& cam, bool isShadow);
    void randomize(int i);
    
private:
    ofVboMesh mesh;
    ofShader shader;
    
    SmoothPoint fromTrans, fromRotate, fromScale;
    SmoothPoint toTrans, toRotate, toScale;
    
    const int num = 200;
};
