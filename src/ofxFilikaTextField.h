#pragma once
#include "ofMain.h"
#include "ofxTextSuite.h"

class ofxFilikaTextField {
private:
	
	ofColor						color;
	string txt;
	ofTrueTypeFont* f;
    
    TextBlockAlignment alignment; // OF_TEXT_ALIGN_LEFT, OF_TEXT_ALIGN_RIGHT, OF_TEXT_ALIGN_JUSTIFIED, OF_TEXT_ALIGN_CENTER
    ofFbo fbo;
    int fWidth,lineH,fSize;
public:
	ofxTextBlock				content;
    void setup(ofTrueTypeFont & _f, int fSize, int fWidth, string txt, ofColor cll = ofColor(100), int lineH = 2, TextBlockAlignment alignment = OF_TEXT_ALIGN_LEFT) {
		this->color = cll;
		this->txt = txt;
        this->fWidth = fWidth;
        this->alignment = alignment;
        this->lineH = lineH;
        this->fSize = fSize;
		f = &_f;
        
        generateTexture();
	}
    
    void setup(string _f, int fSize, int fWidth, string txt, ofColor cll = ofColor(100), int lineH = 2, TextBlockAlignment alignment = OF_TEXT_ALIGN_LEFT) {
        this->color = cll;
        this->txt = txt;
        this->fWidth = fWidth;
        this->alignment = alignment;
        this->lineH = lineH;
        this->fSize = fSize;
        ofFboSettings s;
        
        content.init(_f,fSize);                                // Set default font for the content text box
        
        //content._mainColor = cll;
        content.setLineHeight(fSize + lineH);
        content.setColor(cll);
        content.setHtmlText(this->txt);
        if(this->fWidth != 0) {
            content.wrapTextX(this->fWidth);  // Wrap textbox width to width
            s.width = content.getWidth();
            s.height = content.getHeight();
        }else{
            // Some random chars to get text height accurately for single line texts.
            s.width = content.getWidth();
            s.height = content.getFont().getStringBoundingBox("QpyRTŞ", 0, 0).getHeight();
        }
        if(this->fWidth != 0)
            content.wrapTextX(this->fWidth);  // Wrap textbox width to width
        
        /*GLenum textureTarget;             ///< GL_TEXTURE_2D or GL_TEXTURE_RECTANGLE_ARB
        GLint internalformat;             ///< GL_RGBA, GL_RGBA16F_ARB, GL_RGBA32F_ARB, GL_LUMINANCE32F_ARB etc.
        GLint depthStencilInternalFormat; ///< GL_DEPTH_COMPONENT(16/24/32)
        int wrapModeHorizontal;           ///< GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER etc.
        int wrapModeVertical;             ///< GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER etc.
        int minFilter;                    ///< GL_NEAREST, GL_LINEAR etc.
        int maxFilter;                    ///< GL_NEAREST, GL_LINEAR etc.
        int numSamples;                   ///< number of samples for multisampling (set 0 to disable)*/
        
        
        
        s.internalformat = GL_RGBA32F_ARB;
        /*s.useDepth = false;
        s.depthStencilAsTexture = false;
        s.textureTarget = GL_TEXTURE_RECTANGLE_ARB;
        s.wrapModeHorizontal = GL_REPEAT;
        s.wrapModeVertical = GL_REPEAT;
        s.minFilter = GL_LINEAR;
        s.maxFilter = GL_LINEAR;*/
        s.numSamples = 4;
        
        fbo.allocate(s);
        fbo.begin();
        
        ofClear(cll.r, cll.g, cll.b,0);
        //ofClear(255,0);
        if(this->alignment == OF_TEXT_ALIGN_LEFT) {
            glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_ALPHA );
            content.drawLeft(0, 0);
            //glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_ALPHA );
            content.drawLeft(0, 0);
        }else if(this->alignment == OF_TEXT_ALIGN_RIGHT) {
            content.drawRight(0, 0);
            content.drawRight(0, 0);
        }else if(this->alignment == OF_TEXT_ALIGN_CENTER) {
            content.drawCenter(0, 0);
            content.drawCenter(0, 0);
        }else if(this->alignment == OF_TEXT_ALIGN_JUSTIFIED) {
            content.drawJustified(0, 0, fWidth);
            content.drawJustified(0, 0, fWidth);
        }
        
        fbo.end();
    }
    
    void setupBold(string fontName, int size, ofColor colorBold = ofColor (255,255,255,255)){
        textFormat bold;

        bold.tagOpened = "<b>";
        bold.tagClosed = "</b>";
        bold.font = fontName;
        bold.size = size;
        bold.color = colorBold;
        bold.antiAliesed = true;
        content.setFormat(bold);
    }
    
    void setupItalic(string fontName, int size, ofColor colorItalic = ofColor (255,0,0,255)) {
        textFormat italic;
        italic.tagOpened = "<i>";
        italic.tagClosed = "</i>";
        italic.font = fontName;
        italic.size = size;
        italic.color = colorItalic;
        italic.antiAliesed = true;
        content.setFormat(italic);
    }
    
    // Use the following method to define custom text formats other than built-in formats
    void setupCustomFontType(string fontName, int size, string beginTag, string endTag, ofColor colorCustom = ofColor (255,255,255,255)) {
        textFormat c;
        c.tagOpened = beginTag;
        c.tagClosed = endTag;
        c.font = fontName;
        c.size = size;
        c.color = colorCustom;
        c.antiAliesed = true;
        
        content.setFormat(c);
    }
    
    void draw(int x = 0, int y = 0, TextBlockAlignment alignment = OF_TEXT_ALIGN_LEFT) {
		ofPushMatrix();

		// Content Text
		ofPushStyle();
		//content.setColor(color.r, color.g, color.b, color.a);
		if (alignment == OF_TEXT_ALIGN_LEFT) {
			content.drawLeft(x, y);
		}
        else if (alignment == OF_TEXT_ALIGN_RIGHT) {
            content.drawRight(x, y);
        }
		else if (alignment == OF_TEXT_ALIGN_CENTER) {
			content.drawCenter(x, y);
		}
        else if (alignment == OF_TEXT_ALIGN_JUSTIFIED) {
            ofPushMatrix();
            ofTranslate(x,y);
            content.drawJustified(0, 0, fWidth);
            ofPopMatrix();
        }
        
		
		ofPopStyle();

		ofPopMatrix();
	}
    
    // Use this one if you have too many words
    // No animation applicable
    void drawTexture(int x = 0, int y = 0) {
        ofPushMatrix();
        ofPushStyle();
        content.setColor(color.r, color.g, color.b, color.a);
        ofTranslate(x,y);
        fbo.draw(0,0);
        ofPopStyle();
        ofPopMatrix();
    }
    
private:
    void generateTexture() {
        fbo.clear();
        // content = new ofxTextBlock();
         content.init(this->f,this->fSize);  // Set default font for the content text box

         content.setLineHeight(this->fSize + this->lineH);
         content.setColor(this->color);
         content.setHtmlText(this->txt);
         if(this->fWidth != 0) {
             content.wrapTextX(this->fWidth);  // Wrap textbox width to width
             fbo.allocate(content.getWidth(), content.getHeight(), GL_RGBA32F_ARB, 2);
         }else{
             // Some random chars to get text height accurately for single line texts.
             fbo.allocate(content.getWidth(), content.getHeight() * 1.1, GL_RGBA32F_ARB, 2);
         }
         fbo.begin();
         ofClear(255,0);
         if(this->alignment == OF_TEXT_ALIGN_LEFT) {
             content.drawLeft(0, 0);
             content.drawLeft(0, 0);
         }else if(this->alignment == OF_TEXT_ALIGN_RIGHT) {
             content.drawRight(0, 0);
             content.drawRight(0, 0);
         }else if(this->alignment == OF_TEXT_ALIGN_CENTER) {
             content.drawCenter(0, 0);
             content.drawCenter(0, 0);
         }else if(this->alignment == OF_TEXT_ALIGN_JUSTIFIED) {
             content.drawJustified(0, 0, fWidth);
         }
         fbo.end();
    }
public:
    // -------------------------------------------------------
    // Setters & Getters
    // -------------------------------------------------------
    // SETTERS
    void setAnimatedText(bool val = true) {
        content.enableAnimatedText(val);
    }

    void animateText(string _type = "fadein", float timePerWord = 3, float delayAnimTime = 0, float animRate = 1.2) {
        content.animateText(timePerWord, delayAnimTime, animRate, _type);
    }

    void setContent(string _str, int _w, bool refreshFboTexture = false) {
        
        this->txt    = _str;
        this->fWidth = _w;

        if(refreshFboTexture) {
            //
            this->f = &content.getFont();
            generateTexture();
        } else {
            content.setHtmlText(this->txt);
        }
            
        
        if(this->fWidth != 0) content.wrapTextX(this->fWidth);
    }
    
    // GETTERS
    int getWidth() {
        return content.getWidth();
    }
    
    int getWidthFbo() {
        return fbo.getWidth();
    }
    
    int getHeight() {
        return content.getHeight();
    }
    
    int getHeightFbo() {
        return fbo.getHeight();
    }
    
    glm::vec2 getPos() {
        return content.getPos();
    }
    
    ofFbo & getTexture() {
        return fbo;
    }
    
    ofColor & getColor() {
        return color;
    }
};
