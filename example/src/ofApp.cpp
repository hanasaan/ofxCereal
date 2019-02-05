#include "ofMain.h"
#include "ofxCereal.h"

using namespace ofxCereal;

struct CustomData
{
    ofMatrix4x4 transform;
    vector<ofVec3f> points;
    ofColor color;
    
    OFX_CEREAL_DEFINE(CEREAL_NVP(transform), CEREAL_NVP(points), CEREAL_NVP(color))
    
    void draw() {
        ofPushStyle();
        ofSetColor(color);
        ofPushMatrix();
        ofMultMatrix(transform);
        
        glBegin(GL_LINE_STRIP);
        for (auto v : points) {
            glVertex3fv(&v.x);
        }
        glEnd();
        
        ofPopMatrix();
        ofPopStyle();
    }
};

class ofApp : public ofBaseApp{
    ofPolyline poly;
    CustomData data;
    ofEasyCam ecam;
public:
    void setup()
    {
        ofSetVerticalSync(true);
        ofSetFrameRate(60);
    }
    
    void update()
    {
    }
    
    
    void draw()
    {
        ofClear(0);
        poly.draw();
        
        ecam.begin();
        data.draw();
        ofDrawAxis(100);
        ecam.end();
    }
    
    void save()
    {
        ofFile file("poly.json", ofFile::WriteOnly);
        jsonout jo(file);
        jo << poly.getVertices();
        jo << data;
    }
    
    void load()
    {
        ofFile file("poly.json");
        if (file.exists()) {
            jsonin ji(file);
            ji >> poly.getVertices();
            ji >> data;
        }
    }
    
    void keyPressed(int key)
    {
        if (key == 's') {
            save();
        }
        if (key == 'l') {
            load();
        }
        if (key == 'c') {
            poly.clear();
            data = CustomData();
        }
    }
    
    void mouseDragged(int x, int y, int button)
    {
        poly.addVertex(glm::vec3(x,y,0));
        data.points.push_back(ofVec3f(x,0,y));
        data.color = ofColor::fromHsb(ofRandom(255), 255, 255);
    }
};

//========================================================================
int main( ){
    ofSetupOpenGL(1280,720,OF_WINDOW);
    ofRunApp(new ofApp());
}
