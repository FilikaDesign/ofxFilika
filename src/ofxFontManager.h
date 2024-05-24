//
//  ofxFontManager.h
//  example_ofxFilikaCounter
//
//  Created by alp tugan on 6.02.2024.
//

// EXAMPLE
/*
 In ofApp.h declare
 shared_ptr<ofxFontManager> f;

 In setup(), ofApp.cpp initialize
 ofxFontManager::add("/Users/alptugan/oF/assets/fonts/OverpassMono-Bold.ttf", siz, "regular", true);
 ofxFontManager::add("/Users/alptugan/oF/assets/fonts/OverpassMono-Bold.ttf", 15, "italic", false);

 f = ofxFontManager::mFonts[1];
 
 In draw()
 string typeStr = "ABCÇDEFGĞHIİJKLMNOÖPQRSŞTUÜVWXYZ\nabcçdefgğhıijklmnoöpqrsştuüvwxyz\n0123456789,:&!?";
 f->draw(typeStr, 100, 100);

 
 */

#ifndef ofxFontManager_h
#define ofxFontManager_h

#include "ofTrueTypeFont.h"

class ofxFontManager {
public:

    /*
        instance getters & setters
    */
    int getSize();
    std::string getFile();
    
    std::string getName();
    
    ofRectangle getFontRect(std::string s, int x=0, int y=0);
    
    
    float getWidth(std::string s, int x=0, int y=0);
    
    
    float getHeight(std::string s, int x=0, int y=0);
    
    float getLineHeight();
    
    /*
        instance methods
    */
    void setName(std::string name);
    
    void draw(std::string s, int x, int y);


/*
    static methods
*/
    static std::shared_ptr<ofxFontManager> add(std::string file, int size, std::string name = "", bool _unicode = true);
    
    
    static std::shared_ptr<ofxFontManager> get(std::string name);
    
    
    static std::shared_ptr<ofxFontManager> get(std::string name, int size);
    
    
    static std::shared_ptr<ofxFontManager> get(std::vector<std::string> keys, int size);
    
    static void getFontList();

    static std::vector<std::shared_ptr<ofxFontManager>> mFonts;
    
    ofTrueTypeFont& getTrueTypeFont() {
        return *ttf;
    }
private:

    ofxFontManager(std::string file, int size, std::string name, bool _unicode)
    {
        mSize = size;
        mFile = file;
        ttf = new ofTrueTypeFont();
        
        ofTrueTypeFontSettings s(mFile, mSize);
        s.antialiased = true;
        if(_unicode) s.addRanges(ofAlphabet::Latin);
        
        
        if (name != ""){
            mName = name;
        }   else {
            mName = file.substr(file.find_last_of("/\\") + 1);
        }
        if (ttf->load(s) == false){
            ofLogError("ERROR!! file : " + mFile + " NOT FOUND");
        }   else{
            ofLogNotice("new font added : " +mName+" @ pt size "+std::to_string(mSize));
        }
    }

    int mSize;
    std::string mFile;
    std::string mName;
    ofTrueTypeFont * ttf;
};
#endif /* ofxFontManager_h */
