//
//  ofxFontManager.cpp
//  example_ofxFilikaCounter
//
//  Created by alp tugan on 6.02.2024.
//
#include <stdio.h>

#include "ofxFontManager.h"

std::vector<std::shared_ptr<ofxFontManager>> ofxFontManager::mFonts;

void ofxFontManager::draw(std::string s, int x, int y)
{
    ttf->drawString(s, x, y);
}

std::string ofxFontManager::getName() {
    return mName;
}

void ofxFontManager::setName(std::string name) {
    mName = name;
}

std::string ofxFontManager::getFile() {
    return mFile;
}

int ofxFontManager::getSize() {
    return mSize;
}

ofRectangle ofxFontManager::getFontRect(std::string s, int x, int y) {
    return ttf->getStringBoundingBox(s, x, y);
}

float ofxFontManager::getWidth(std::string s, int x, int y) {
    return ttf->getStringBoundingBox(s, x, y).width;
}

float ofxFontManager:: getHeight(std::string s, int x, int y) {
    return ttf->getStringBoundingBox(s, x, y).height;
}

float ofxFontManager::getLineHeight()
{
   return ttf->getLineHeight();
}

/*
    static methods
*/

std::shared_ptr<ofxFontManager> ofxFontManager::add(std::string file, int size, std::string name, bool _unicode)
{
   for(auto f:mFonts){
       if (f->getFile()==file && f->getSize()==size){
          // log(f->name() + "@ pt size "+std::to_string(f->size()) + " is already in memory.");
           return f;
       }
   }
   struct make_shared_sf : public ofxFontManager {
       make_shared_sf(std::string file, int size, std::string name, bool _unicode) : ofxFontManager(file, size, name, _unicode){}
   };
   mFonts.push_back(std::make_shared<make_shared_sf>(file, size, name, _unicode));
   return mFonts.back();
}

std::shared_ptr<ofxFontManager> ofxFontManager::get(std::string name)
{
   for(auto f:mFonts){
       if (f->getName()==name) return f;
   }
   ofLogError("requested font : "+ name + " not found");
   return nullptr;
}

std::shared_ptr<ofxFontManager> ofxFontManager::get(std::string name, int size)
{
   for(auto f:mFonts){
       if (f->getFile().find(name) != std::string::npos && f->getSize()==size){
           return f;
       }
   }
   ofLogError("requested font : "+ name +" @ " +std::to_string(size)+ " not found");
   return nullptr;
}

std::shared_ptr<ofxFontManager> ofxFontManager::get(std::vector<std::string> keys, int size)
{
    for(auto f:mFonts){
        bool match = true;
        for(auto k:keys){
            if (ofToLower(f->getFile()).find(ofToLower(k)) == std::string::npos) match = false;
        }
        if (match && f->getSize()==size) return f;
    }
    std::stringstream ss;
    for(int i=0; i<keys.size(); i++){
        ss << keys[i];
        if (i<keys.size()-1) ss << ",";
    }
    ofLogError("nothing found in search for : "+ ss.str() +" @ " +std::to_string(size)+ "pt");
    return nullptr;
}

void ofxFontManager::getFontList() {
    ofLogNotice() << "----------------------------------";
    ofLogNotice() << "total # of fonts cached = " << std::to_string(mFonts.size());
    for(auto f:mFonts){
        ofLogNotice(f->getName() + " (" + std::to_string(f->getSize()) +"pt -> "+f->getFile()+")");
    }
    ofLogNotice() << "----------------------------------";
}
