#pragma once

#ifndef OFXFILIKAIMAGEMAGNIFIER_H
#define OFXFILIKAIMAGEMAGNIFIER_H

#include "ofMain.h"
#include "ofxThreadedImageLoader.h"


class ofxFilikaImageMagnifier {
private:
    ofImage img;
    int cw,ch,sx,sy,sw,sh;
    float sclFac;
    int frameThickness = 3;
    ofColor frameCol = ofColor::red;
	ofxThreadedImageLoader loader;
	ofRectangle scaledImage;

public:
    ofxFilikaImageMagnifier() {

    }

    ~ofxFilikaImageMagnifier() {
		loader.stopThread();
    }

	/*void setSize(int w, int h) {

	}*/
	// pass loaded image large scale
    void setup(ofImage & src, int frameW = 200, int frameH = 200, float _sclFac = 0.3) {
        img = src;

        cw = frameW;     // Magnifier Frame width
        ch = frameH;     // Magnifier Frame height
        sclFac = _sclFac; // Manifier scale amount <smaller the value larger the image>
        sw = cw * sclFac;
        sh = ch * sclFac;
    }

	// SETUP and load large scale
	void setup(string src, int frameW = 200, int frameH = 200, float _sclFac = 0.3) {
		

		loader.loadFromDisk(img, src);

		cw = frameW;     // Magnifier Frame width
		ch = frameH;     // Magnifier Frame height
		sclFac = _sclFac; // Manifier scale amount <smaller the value larger the image>
		sw = cw * sclFac;
		sh = ch * sclFac;
	}
	void setScaledRect(ofRectangle & rect) {
		//ofLog() << rect.x << "-" << rect.y << "-" << rect.getWidth() << "-" << rect.getHeight();
		scaledImage = rect;
	}


    void draw(float x, float y, float gridX1, float gridX2, float gridY1, float gridY2) {
		//sx = x - cw * 0.5;
		//sy = y - ch * 0.5;
		sx = x;
		sy = y;

       
        
		
		///
   /// \param x X position to draw cropped image at.
   /// \param y Y position to draw cropped image at.
   /// \param w Width of subsection to draw.
   /// \param h Height of subsection to draw.
   /// \param sx X position in image to begin cropping from.
   /// \param sy Y position in image to begin cropping from.
   /// \param sw Source width of cropped area.
   /// \param sh Source height of cropped area.
	//	void drawSubsection(float x, float y, float w, float h, float sx, float sy, float sw, float sh) const;


		if (img.isAllocated()) {
			//img.drawSubsection(0, 0, cw, ch, sx, sy, sw, sh);
			//img.drawSubsection(0, 0, cw, ch, sx + sw, sy + sh, sw, sh);
			//int xx = ofMap(sx + sw*0.5, xs - sw * 0.5, xe + sw * 0.5, 0, img.getWidth());
			//int yy = ofMap(sy + sh*0.5, ys - sh * 0.5, ye + sh * 0.5, 0, img.getHeight());
			float xRatio, yRatio; //pad position ratio
			if ((sx - gridX1 > 0) && (sy - gridY1) > 0) {
				xRatio = (sx - gridX1) / (gridX2 - gridX1);
				yRatio = (sy - gridY1) / (gridY2 - gridY1);

				//ofLog() << xRatio << "-" << yRatio;
			}

			float xx = scaledImage.x + xRatio * scaledImage.getWidth();
			float yy = scaledImage.y + yRatio * scaledImage.getHeight();

			ofPushStyle();
			ofPushMatrix();
			ofFill();
			

			ofTranslate(xx,yy);

			//param sx X position in image to begin cropping from.
			float paramSx = ofMap(xx, scaledImage.x , scaledImage.x + scaledImage.getWidth(), 0, img.getWidth());
			float paramSy = ofMap(yy, scaledImage.y, scaledImage.y + scaledImage.getHeight(), 0, img.getHeight());

			//img.drawSubsection(xx, yy, cw, ch, paramSx, paramSy, sw, sh);
			img.drawSubsection(0, 0, cw, ch, paramSx, paramSy, sw, sh);
			ofNoFill();
			ofSetLineWidth(frameThickness);
			ofSetColor(frameCol);
			ofDrawRectangle(0, 0, cw, ch);
			ofFill();
			ofSetLineWidth(0);
			ofDrawTriangle(glm::vec2(0,0), glm::vec2(30,0), glm::vec2(0,30));
			ofPopMatrix();
			ofPopStyle();
		}
        
    }

private:
};

#endif // OFXFILIKAIMAGEMAGNIFIER_H

/*
EXAMPLE
---------------------------------------------------------------

ofApp.h

---------------------------------------------------------------

#include "ofMain.h"
#include "ofxFilikaImageMagnifier.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    ofImage img;
    bool isPressed ;
    ofxFilikaImageMagnifier mag;
};


---------------------------------------------------------------

ofApp.cpp

---------------------------------------------------------------

void ofApp::setup(){
    ofSetVerticalSync(true);
    string str =  "/Users/alptugan/oF/assets/test.jpg";

    img.load(str);

    mag.setup(img);
}

//--------------------------------------------------------------
void ofApp::draw(){
    // draw the original image
    ofSetColor(ofColor::white);
    img.draw(0, 0);

    if(isPressed) {
        int x = ofGetWidth()*0.5;
        int y = img.getHeight()*0.5;
        if(mouseX > 0 && mouseX < ofGetWidth() && mouseY > img.getHeight()) {
            x = ofMap(mouseX, 0, ofGetWidth(), 0, ofGetWidth());
            y = ofMap(mouseY, img.getHeight(), ofGetHeight(), 0, img.getHeight());
        }

        mag.draw(x,y);
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    isPressed = true;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    isPressed = false;
}


*/
