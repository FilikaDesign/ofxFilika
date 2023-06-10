#pragma once
#include "ofMain.h"
#include "ofxTextSuite.h"

class ofxFilikaTextField {
private:
	
	ofColor						color;
	string txt;
	ofTrueTypeFont* f;
public:
	ofxTextBlock				content;
	void setup(ofTrueTypeFont & _f, int fSize, int fWidth, string txt, ofColor color = ofColor(100), int lineH = 2) {
		this->color = color;
		this->txt = txt;

		f = &_f;

		content.init(f);								// Set default font for the content text box
		content.setLineHeight(fSize + lineH);
		content.setText(this->txt);
		content.wrapTextX(fWidth);							// Wrap textbox width to width
	
	}

	void setAnimatedText() {
		content.enableAnimatedText(true);
	}

	void animateText() {
		content.animateText(5);
	}

	void setContent(string _str, int _w) {
		content.setText(_str);
		content.wrapTextX(_w);

	}

	void draw(int x = 0, int y = 0, string alignment = "LEFT") {
		ofPushMatrix();

		// Content Text
		ofPushStyle();
		content.setColor(color.r, color.g, color.b, color.a);
		if (alignment == "LEFT") {
			content.drawLeft(x, y);
		}
		else if (alignment == "CENTER") {
			content.drawCenter(x, y);
		}
		
		ofPopStyle();

		ofPopMatrix();

	}
};