//
//  ofxFilikaScrollingText.h
//  example_ofxFilikaScrollingText
//
//  Created by alp tugan on 22.05.2024.
//

#ifndef ofxFilikaScrollingText_h
#define ofxFilikaScrollingText_h
#include "ofMain.h"
#include "ofxFilikaConstants.h"
#include "ofxFilikaTextField.h"

struct scrollEvent {
    ofxFilikaScrollDir direction;
    int lastPosX;
    int lastPosY;
};

class ofxFilikaScrollingText: public ofxFilikaTextField {
private:
    float speed;
    float posX;
    float newX;
    int dir;
    int wTxt;
    ofxFilikaScrollDir direction;
    
    ofEvent<scrollEvent> SCROLL_DONE;
public:
    ofxFilikaScrollingText() {
        speed = 2;
        dir = -1;
        posX = 0;
        newX = 0;
    }
    
    ~ofxFilikaScrollingText() {
        
    }
    
    void setup(ofTrueTypeFont & _f, string txt, ofColor cll = ofColor(200), ofxFilikaScrollDir direction = ofxFilikaScrollDir::RIGHT_TO_LEFT) {
        
        setDirection(direction);
        
        ofxFilikaTextField::setup(_f, _f.getSize(), 0, txt, cll);
        
        wTxt = getWidthFbo();
    }
    
    void setup(string _f, string txt, int sz, ofColor cll = ofColor(200), ofxFilikaScrollDir direction = ofxFilikaScrollDir::RIGHT_TO_LEFT) {
        
        setDirection(direction);
        
        ofxFilikaTextField::setup(_f, sz, 0, txt, cll);


        wTxt = content.getWidth();
    }
    
   void draw(float x, float y, bool isTexture = true) {
       
       ofPushMatrix();
       ofPushStyle();
       ofTranslate(newX + posX * dir, 0);
       if(isTexture)
           ofxFilikaTextField::drawTexture(x, y);
       else
           ofxFilikaTextField::draw(x, y);
       ofPopStyle();
       ofPopMatrix();
       
       posX = posX + speed;
       
       if(direction == ofxFilikaScrollDir::RIGHT_TO_LEFT) {
           if(x + posX > wTxt + newX) {
               posX = 0;
               newX = ofGetWidth();
               
               /*scrollEvent e;
               
               e.direction = ofxFilikaScrollDir::RIGHT_TO_LEFT;
               ofNotifyEvent(SCROLL_DONE, e, this);*/
           }
       }
       
       else if(direction == ofxFilikaScrollDir::LEFT_TO_RIGHT) {
           if(x + posX > ofGetWidth() - newX) {
               posX = 0;
               newX = -wTxt;
               /*scrollEvent e;
               
               e.direction = ofxFilikaScrollDir::LEFT_TO_RIGHT;
               ofNotifyEvent(SCROLL_DONE, e, this);*/
           }
       }
       
    }
    
    void setContent(string _str) {
        ofxFilikaTextField::setContent(_str, 0, true);
        wTxt = getTexture().getWidth();
    }
    
    void setSpeed(float val) {
        speed = val;
    }
    
    void setDirection(ofxFilikaScrollDir d) {
        direction = d;
        if(direction == ofxFilikaScrollDir::RIGHT_TO_LEFT) {
            dir = -1;
        }else if(direction == ofxFilikaScrollDir::LEFT_TO_RIGHT) {
            dir = 1;
        }
    }
   
};

#endif /* ofxFilikaScrollingText_h */
