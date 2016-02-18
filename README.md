## ofxCereal
Addon for serialization using cereal http://uscilab.github.io/cereal/

## Note
WIP. OF version >= 0.9.0

## How To Use
```cpp
 // Custom class serialize
 struct MyClassSimple {
    ofVec3f pos;
    ofQuaternion rot;
    OFX_CEREAL_DEFINE(pos, rot);
 };
 
 // With Name-Value Pair
 struct MyClass {
    ofVec3f pos;
    ofQuaternion rot;
    OFX_CEREAL_DEFINE(CEREAL_NVP(pos), CEREAL_NVP(rot));
 };
 
 // Serializable class
 class MyClassS : public ofxCereal::JsonSerializable<MyClassS> {
    ofVec3f pos;
    ofQuaternion rot;
    OFX_CEREAL_DEFINE_SERIALIZABLE(MyClassS, pos, rot)
 };
 
    class ofApp : public ofBaseApp{
        MyClass myclass;
        MyClassS s;
        map<int, ofVec3f> pts; // support default OF base types and stl containers
    public:
        void setup() {
            // file IO
            ofxCereal::saveJson("myfile.json", myclass);
            ofxCereal::loadJson("myfile.json", myclass);
            s.save("myfile2.json");
            s.load("myfile2.json");
 
            // stream IO
            stringstream ss;
            ofxCereal::jsonout jo(ss);
            jo << myclass;
            jo << pts;
            ofxCereal::jsonin ji(ss);
            ji >> myclass;
            ji << pts;
        }
    };

```
