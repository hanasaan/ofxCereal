//
//  Created by Yuya Hanai, https://github.com/hanasaan/
//
#pragma once

#include "ofBaseTypes.h"

#include <cereal/archives/binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/archives/json.hpp>

#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>

/*
 
 Usage : 
 // Most simple usage
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
 
 // Serializable
 class MyClassS : public ofxCereal::JsonSerializable<MyClassS> {
    ofVec3f pos;
    ofQuaternion rot;
    OFX_CEREAL_DEFINE_SERIALIZABLE(MyClassS, pos, rot)
 };
 
    class ofApp : public ofBaseApp{
        MyClass myclass;
        MyClassS s;
    public:
        void setup() {
            // file IO
            ofxCereal::saveJson("myfile.json", myclass);
            ofxCereal::loadJson("myfile.json", myclass);
            s.save("myfile2.json");
            s.load("myfile2.json");
 
            // stream IO
            stringstream ss;
            ofxCereal::jsonout(ss) << myclass;
            ofxCereal::jsonin(ss) >> myclass;
        }
    };
 */

// utility functions for OF
namespace cereal
{
    // conversion functions for of base types
    template<class Archive>
    void serialize(Archive & archive, ofVec2f & m)
    {
        archive(cereal::make_nvp("x", m.x),
                cereal::make_nvp("y", m.y));
    }
    
    template<class Archive>
    void serialize(Archive & archive, ofVec3f & m)
    {
        archive(cereal::make_nvp("x", m.x),
                cereal::make_nvp("y", m.y),
                cereal::make_nvp("z", m.z));
    }
    
    template<class Archive>
    void serialize(Archive & archive, ofVec4f & m)
    {
        archive(cereal::make_nvp("x", m.x),
                cereal::make_nvp("y", m.y),
                cereal::make_nvp("z", m.z),
                cereal::make_nvp("w", m.w));
    }
    
    template<class Archive, class T>
    void serialize(Archive & archive, ofColor_<T> & m)
    {
        archive(cereal::make_nvp("r", m.r),
                cereal::make_nvp("g", m.g),
                cereal::make_nvp("b", m.b),
                cereal::make_nvp("a", m.a));
    }
    
    template<class Archive>
    void serialize(Archive & archive, ofQuaternion & m)
    {
        archive(cereal::make_nvp("v", m._v));
    }
    
    template<class Archive>
    void serialize(Archive & archive, ofRectangle & m)
    {
        archive(cereal::make_nvp("x", m.x),
                cereal::make_nvp("y", m.y),
                cereal::make_nvp("width", m.width),
                cereal::make_nvp("height", m.height));
    }
    
    template<class Archive>
    void serialize(Archive & archive, ofMatrix4x4 & m)
    {
        archive(cereal::make_nvp("mat", m._mat));
    }
    
    // some aliases
    using jsonin  = JSONInputArchive;
    using jsonout = JSONOutputArchive;
    using xmlin   = XMLInputArchive;
    using xmlout  = XMLOutputArchive;
    using binin   = BinaryInputArchive;
    using binout  = BinaryOutputArchive;
    
    // save & load functions
    template<typename oarchive, typename T>
    void save(string filename, const T& obj)
    {
        ofFile file(filename, ofFile::WriteOnly);
        oarchive(file) << obj;
    }
    
    template<typename iarchive, typename T>
    bool load(string filename, T& obj)
    {
        ofFile file(filename);
        if (file.exists()) {
            iarchive(file) >> obj;
            return true;
        }
        return false;
    }
    
    template<typename T>
    inline void saveJson(string filename, const T& obj) { save<jsonout>(filename, obj); }

    template<typename T>
    inline bool loadJson(string filename, T& obj) { return load<jsonin>(filename, obj); }

    template<typename T>
    inline void saveXml(string filename, const T& obj) { save<xmlout>(filename, obj); }
    
    template<typename T>
    inline bool loadXml(string filename, T& obj) { return load<xmlin>(filename, obj); }
    
    template<typename T>
    inline void saveBinary(string filename, const T& obj) { save<binout>(filename, obj); }
    
    template<typename T>
    inline bool loadBinary(string filename, T& obj) { return load<binin>(filename, obj); }
    
    template<class iarchive, class oarchive, class T>
    class Serializable {
    protected:
        bool load(string filename, T& obj)
        {
            return ::cereal::load<iarchive, T>(filename, obj);
        }
        
        void save(string filename, const T& obj) const
        {
            ::cereal::save<oarchive, T>(filename, obj);
        }
    public:
        bool load(string filename) {
            return load(filename, get());
        }
        void save(string filename) const {
            save(filename, get());
        }
        
        virtual T& get() = 0;
        virtual const T& get() const = 0;
    };
    
    template<class T>
    using JsonSerializable = Serializable<jsonin, jsonout, T>;
    
    template<class T>
    using XmlSerializable = Serializable<xmlin, xmlout, T>;
    
    template<class T>
    using BinarySerializable = Serializable<binin, binout, T>;
}

#define OFX_CEREAL_DEFINE_SERIALIZABLE(T,...) \
public:\
const T &get() const override { return *this; } \
T &get() override { return *this; } \
template<class Archive> \
void serialize(Archive & archive) { archive( __VA_ARGS__ ); }

#define OFX_CEREAL_DEFINE(...) \
public:\
template<class Archive> \
void serialize(Archive & archive) { archive( __VA_ARGS__ ); }

namespace ofxCereal = cereal;
