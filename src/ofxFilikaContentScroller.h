//
//  ofxFilikaContentScroller.h
//  example_ContentScroller
//
//  Created by alp tugan on 22.06.2019.
//  Update: 31.01.2024
//  Update: 04.02.2024
//  Update: 06.02.2024

#ifndef ofxFilikaContentScroller_h
#define ofxFilikaContentScroller_h
#include "ofMain.h"
#include "ofxFilikaDraggableButton.h"

class ofxFilikaContentScroller : public ofRectangle {
private:
	ofBaseDraws * content;
	ofFbo mask;
	ofFbo contentFbo;

	ofRectangle scrollBarRect;
	ofRectangle scrollerRect;

	ofxFilikaDraggableButton scrollerBtn;
	ofxFilikaDraggableButton btnScrollUp;
	ofxFilikaDraggableButton btnScrollDown;

	glm::vec2 btnScrollUpPos;
	glm::vec2 btnScrollDownPos;

	bool isScrollBarVisible;
	bool isScrollingEnabled = false;
    bool isMouseScrollEnabled = false;

	int saveX, saveY, touchYScrollerPos;
	int sbW;
	int sbH;

	int scW;
	int scH;

	int sbRoundness;

	int moveAmty;
	int sbGap;
	ofColor sbColor;
	ofColor scColor, scOverColor, scPressedColor;

	bool isScrollNavEnable;
	bool isPointerDown;
    bool isNavPointerDown = false;
	float lastMoveYpos;
	int dirY;
	int bottomMargin;

    int scNavSz = 0;
    int navCorrection = 0;
    float scrollNavSpeed;
    
    // USER PRESSED SCROLLER BUTTON to move the content up and down
	void isDraggingHandler(dragEvent & _e) {
        // If dragged button id is 0 means scrollerBtn
        if(_e._id == 0)
            moveContent(_e.p);
	}
    
    // USER PRESSED DOWN BUTTON on SCROLLER BAR
    void moveContentDown(int & _id) {
        //moveContent(glm::vec2(0, scrollerRect.y + bottomMargin)); // updated 06.02.2024
        //updateContentFbo();
        isNavPointerDown = false;
    }

    // USER PRESSED UP BUTTON on SCROLLER BAR
    void moveContentUp(int & _id) {
        //moveContent(glm::vec2(0, scrollerRect.y - bottomMargin));
        //updateContentFbo();
        isNavPointerDown = false;
    }
    
    void moveContentUpPRESSED(int & _id) {
        //moveContent(glm::vec2(0, scrollerRect.y - bottomMargin));
        //updateContentFbo();
        isNavPointerDown = true;
    }
    
    void moveContentDownPRESSED(int & _id) {
        //moveContent(glm::vec2(0, scrollerRect.y + bottomMargin)); // updated 06.02.2024
        //updateContentFbo();
        isNavPointerDown = true;
    }

public:
	ofxFilikaContentScroller() {

	}

	~ofxFilikaContentScroller() {
		disableInteraction();
	}

	void enableInteraction() {
		//if (!isScrollingEnabled) {
//#ifdef TOUCH_ENABLE
			ofAddListener(ofEvents().touchMoved, this, &ofxFilikaContentScroller::touchMoveContentTouchHandler);
			ofAddListener(ofEvents().touchDown, this, &ofxFilikaContentScroller::touchDownContentTouchHandler);
			ofAddListener(ofEvents().touchUp, this, &ofxFilikaContentScroller::touchUpContentTouchHandler);
//#else
			ofAddListener(ofEvents().mouseDragged, this, &ofxFilikaContentScroller::moveMouseContent);
			ofAddListener(ofEvents().mousePressed, this, &ofxFilikaContentScroller::moveMouseContentPressed);
			ofAddListener(ofEvents().mouseReleased, this, &ofxFilikaContentScroller::moveMouseContentReleased);
            ofAddListener(ofEvents().mouseScrolled, this, &ofxFilikaContentScroller::moveMouseContentScroll);
//#endif
			ofAddListener(scrollerBtn.BUTTON_DRAGGING, this, &ofxFilikaContentScroller::isDraggingHandler);

			// Scroll Nav buttons enabled
			if (isScrollNavEnable) {
                enableNavButtons();
			}
		//}
	}

	void disableInteraction() {
		if (isScrollingEnabled) {
//#ifdef TOUCH_ENABLE
			ofRemoveListener(ofEvents().touchMoved, this, &ofxFilikaContentScroller::touchMoveContentTouchHandler);
			ofRemoveListener(ofEvents().touchDown, this, &ofxFilikaContentScroller::touchDownContentTouchHandler);
			ofRemoveListener(ofEvents().touchUp, this, &ofxFilikaContentScroller::touchUpContentTouchHandler);
            ofRemoveListener(ofEvents().mouseScrolled, this, &ofxFilikaContentScroller::moveMouseContentScroll);

//#else
			ofRemoveListener(ofEvents().mousePressed, this, &ofxFilikaContentScroller::moveMouseContentPressed);
			ofRemoveListener(ofEvents().mouseDragged, this, &ofxFilikaContentScroller::moveMouseContent);
			ofRemoveListener(ofEvents().mouseReleased, this, &ofxFilikaContentScroller::moveMouseContentReleased);
///#endif	
			ofRemoveListener(scrollerBtn.BUTTON_DRAGGING, this, &ofxFilikaContentScroller::isDraggingHandler);


			// Scroll Nav buttons enabled
			if (isScrollNavEnable) {
                disableNavButtons();
			}
		}
	}


	void setup(ofRectangle _contentRect = ofRectangle(0, 0, 0, 0), ofBaseDraws * _content = nullptr, int _sbGap = 1, bool _isScrollNavEnable = false) {
		this->set(_contentRect);
		isPointerDown = false;
        isScrollNavEnable = _isScrollNavEnable;
        bottomMargin = 15;
        
		sbGap = _sbGap; // issue
		sbColor = ofColor(150, 150, 150, 60);
		scColor = ofColor(150, 150, 150, 255);
		sbW = 15;
		sbH = 0;
		scW = 15;
		scH = 0;
		moveAmty = 0;
		lastMoveYpos = 0;
        scrollNavSpeed = 1;

        if (_content != nullptr) {
            setContent(_content);
            enableInteraction();
        }
	}

//#ifdef TOUCH_ENABLE
	void touchMoveContentTouchHandler(ofTouchEventArgs  & e) {
		pointerMove(glm::vec2(e.x, e.y));
	}

	void touchDownContentTouchHandler(ofTouchEventArgs  & e) {
		pointerDown(glm::vec2(e.x, e.y));
	}

	void touchUpContentTouchHandler(ofTouchEventArgs & e) {
		pointerUp(glm::vec2(e.x, e.y));
	}

//#else
	void moveMouseContentPressed(ofMouseEventArgs & e) {
		pointerDown(glm::vec2(e.x, e.y));
	}

	void moveMouseContent(ofMouseEventArgs & e) {
		pointerMove(glm::vec2(e.x, e.y));
	}

	void moveMouseContentReleased(ofMouseEventArgs & e) {
		pointerUp(glm::vec2(e.x, e.y));
	}
//#endif	
	void pointerDown(glm::vec2 e) {
		if (this->inside(e)) {
			saveY = e.y + scrollerBtn.getPos().y + bottomMargin;
			isPointerDown = true;
		}
	}

	void pointerUp(glm::vec2 e) {
		isPointerDown = false;
	}

	void pointerMove(glm::vec2 e) {
		//if (isPointerDown && this->inside(e)) {
        if (isPointerDown) {
			if (e.y - saveY < 0) {
				dirY = -1;
			}
			else {
				dirY = 1;
			}
            moveContent(glm::vec2(0, (saveY - e.y) - bottomMargin));
		}
	}
    
    void moveMouseContentScroll(ofMouseEventArgs & e) {
        if(isMouseScrollEnabled) {
            if(e.x > this->getPosition().x && e.x < this->getPosition().x + this->getWidth() + scrollerRect.getWidth() + sbGap) {
                if(e.scrollY < 0) {
                    moveContent(glm::vec2(0, scrollerRect.y + scrollNavSpeed * abs(e.scrollY)));
                }else{
                    moveContent(glm::vec2(0, scrollerRect.y - scrollNavSpeed * abs(e.scrollY)));
                }
            }
        }
    }
    
	void moveContent(glm::vec2 _p, string _dir = "default") {

		int maxYVal = scrollBarRect.y + scrollBarRect.height - scrollerBtn.getHeight();

		// Moves scroll button
		scrollerRect.y = _p.y ;

		// Change moving direction
		if (_dir == "default")
		{
			// Stop if the content reaches to top border
			if (_p.y < scrollBarRect.y) {
				scrollerRect.y = scrollBarRect.y;
			}

			// Stop, if the content reaches to end
			if (_p.y > maxYVal) {
				scrollerRect.y = maxYVal;
			}
            
			moveAmty = scrollBarRect.y + ofMap(scrollerRect.y, scrollBarRect.y, maxYVal, scrollBarRect.y, scrollBarRect.y + content->getHeight() - scrollBarRect.height - navCorrection + bottomMargin)*-1;
		} else {
            // BU KISIM KULLANILMIYOR
            // DO NOT DELETE
            //moveAmty = scrollBarRect.y + ofMap(scrollerRect.y, maxYVal, scrollBarRect.y, scrollBarRect.y + content->getHeight() - scrollBarRect.height + 20, scrollBarRect.y)*-1;
			moveAmty = lastMoveYpos;

			// To stop content moving upwards
			if (moveAmty < (content->getHeight() - scrollBarRect.height) * -1) {
				moveAmty = (content->getHeight() - scrollBarRect.height) * -1;
				lastMoveYpos = moveAmty;
			}
			// To stop content moving downwards
			if (moveAmty > 0) {
				moveAmty = 0;
				lastMoveYpos = moveAmty;
			}
            
			scrollerRect.y = ofMap(moveAmty, (content->getHeight() - scrollBarRect.height) * -1, 0, maxYVal, scrollBarRect.y);

			// Stop if the content reaches to top border
			if (scrollerRect.y > scrollBarRect.y) {
				//scrollerRect.y = scrollBarRect.y;
			}

			// Stop, if the content reaches to end
			if (scrollerRect.y > maxYVal) {
				//scrollerRect.y = maxYVal;
			}
		}
		

		updateContentFbo();
	}

private:
	void updateContentFbo() {
		// Update & slide the content fbo
		contentFbo.begin();
		ofClear(0, 0);
		content->draw(0, moveAmty);
        content->draw(0, moveAmty);
		contentFbo.end();
	}
public:
	void draw() {
		if (content != nullptr)
		{
            if(isScrollNavEnable && isNavPointerDown) {
                if(btnScrollUp.getButtonState())
                    moveContent(glm::vec2(0, scrollerRect.y - scrollNavSpeed));
                else if(btnScrollDown.getButtonState())
                    moveContent(glm::vec2(0, scrollerRect.y + scrollNavSpeed));
                
                updateContentFbo();
            }
            
            // DRAW THE MASKED CONTENT
			contentFbo.getTexture().setAlphaMask(mask.getTexture());
            contentFbo.draw(x, y);
			//ofDrawRectangle(x, y, width, height );

			if (isScrollBarVisible) {
				ofPushStyle();

				// Scroll Bar
				ofSetColor(sbColor);
				ofDrawRectRounded(scrollBarRect, sbRoundness, sbRoundness, sbRoundness, sbRoundness);

				// Scroller
				//ofSetColor(scColor);
				//ofDrawRectRounded(scrollerRect, sbRoundness, sbRoundness, sbRoundness, sbRoundness);
				scrollerBtn.draw(scrollerRect.x, scrollerRect.y);

				if (isScrollNavEnable) {
					btnScrollUp.draw(btnScrollUpPos.x, btnScrollUpPos.y);
					btnScrollDown.draw(btnScrollDownPos.x, btnScrollDownPos.y);
				}

				ofPopStyle();
			}
		}
	}

	// SETTERTS & GETTERS
	void setContent(ofBaseDraws * _content) {
		content = _content;
		
        // Clear the content
        mask.clear();
        contentFbo.clear();
        
        // Init Mask
        mask.allocate(content->getWidth(), content->getHeight());
		mask.begin();
		ofClear(0, 0);
		ofDrawRectangle(0, 0, width, height);
		mask.end();

        // Init Content Fbo
		contentFbo.allocate(content->getWidth(), content->getHeight(), GL_RGBA32F_ARB, 4);
		contentFbo.begin();
		ofClear(0, 0);
		content->draw(0, 0);
        content->draw(0, 0);
		contentFbo.end();

		// content height > Holder size ? enable scrollbar;
		if (contentFbo.getHeight() > height) {
            calculateSizes();

			// ofVec2f _size, int _id, ofColor _mainColor = ofColor(0), bool _isAnimatable = false
			scrollerBtn.setup(glm::vec2(scrollerRect.width, scrollerRect.height), 0, scColor);
            scrollerBtn.setDraggingVertical(true);
            scrollerBtn.setColorReleased(scColor);
			scrollerBtn.setColorPressed(scPressedColor);
			scrollerBtn.setColorOver(scOverColor);
			scrollerBtn.setRoundness(sbRoundness);
            scrollerBtn.setPivot("tl");
            //ofLog() << "button w: " << scrollerRect.width;
			scrollerBtn.setInteractionArea(glm::vec2(scrollerRect.width + 40 , scrollerRect.height));
            //scrollerBtn.setInteractionArea(glm::vec2(scrollerRect.width, scrollerRect.height));
            scrollerBtn.enableInteraction();
			//if (!isScrollingEnabled) {
			//	ofAddListener(scrollerBtn.BUTTON_DRAGGING, this, &ofxFilikaContentScroller::isDraggingHandler);
				
			//}

			isScrollBarVisible = true;
			isScrollingEnabled = true;

		}
		else {
			isScrollBarVisible = false;
			disableInteraction();
			isScrollingEnabled = false;
		}
	}
    
    void calculateSizes() {
        scNavSz = 0;
        navCorrection = 0;
        
        // Scroll Bar
        sbH = height;
        
        if(isScrollNavEnable) {
            scNavSz = scW;
            btnScrollUpPos.x = x + width + sbGap;
            btnScrollUpPos.y = y;
            
            btnScrollDownPos.x = x + width + sbGap;
            btnScrollDownPos.y = y + sbH - scNavSz;
            
            sbH -= (scNavSz*2 + 2);
            navCorrection = scNavSz + 1;
        }
        
        scrollBarRect.set(x + width + sbGap, y + navCorrection, sbW, sbH);
        
        // Scroller
        int diff = contentFbo.getHeight() - sbH;

        // 50 is the minimum size of the available scroller height
        //scH = ofMap(diff, 0, contentFbo.getHeight(), 10, sbH);
        scH = ofMap(diff, 0, contentFbo.getHeight(), sbH, 10);
        scrollerRect.set(scrollBarRect.x, scrollBarRect.y, scW, scH);
    }
    
    void setBottomMargin(int val) {
        bottomMargin = val;
    }

	
	void setPositon(int _x, int _y) {
		x = _x;
		y = _y;

		scrollBarRect.x = x + width + sbGap;
		scrollBarRect.y = y;

		scrollerRect.x = scrollBarRect.x;
        scrollerRect.y = y;
        
	}

	void setContentRect(ofRectangle & _r) {
		this->set(_r);
	}
    
	void setScrollingEnable(bool _isScrollingEnabled) {
		isScrollingEnabled = _isScrollingEnabled;

		if (!isScrollingEnabled)
			disableInteraction();
		else
			enableInteraction();
	}

    /* SCROLLER BAR BACKGROUND SETTERS */
    void setScrollbarVisible(bool _v) {
        isScrollBarVisible = _v;
    }
    
	void setScrollbarW(int _v) {
		scW = _v;
		sbW = scW;
        scrollBarRect.setWidth(sbW);
	}

	void setScrollbarCornerRadius(int _v) {
		sbRoundness = _v;
	}

	void setScrollbarColor(ofColor _v) {
		sbColor = _v;
	}
    
    
    /* SCROLLER BUTTON SETTERS */
    void setScrollerGap(int  _v) {
        sbGap = _v;
    }
    
    void setScrollerW(int w) {
        scrollerRect.setWidth(w);
        scrollerBtn.setButtonSize(w, scrollerRect.getHeight());
    }
    
    void setScrollerCornerRadius(int v) {
        sbRoundness = v;
        scrollerBtn.setRoundness(sbRoundness);
    }
    
	void setScrollerColor(ofColor _idleColor, ofColor _pressedColor, ofColor _overColor) {
		scColor = _idleColor;
		scPressedColor = _pressedColor;
		scOverColor = _overColor;
        
        scrollerBtn.setColorReleased(scColor);
        scrollerBtn.setColorPressed(scPressedColor);
        scrollerBtn.setColorOver(scOverColor);
        scrollerBtn.mainColor = scColor;
	}

	/* SCROLL NAVIGATION */
	void setScrollNavEnable(bool _isEnable) {
		isScrollNavEnable = _isEnable;
	}
    
    void setScrollNavSpeed(float _s) {
        scrollNavSpeed = _s;
    }
    
    void setScrollNavigation(bool _enable = true){
        setScrollNavEnable(_enable);
        
        // Up Button
        btnScrollUp.setup(glm::vec2(scrollerRect.width, scrollerRect.width), 1, scColor);
        btnScrollUp.setDraggingVertical(false);
        btnScrollUp.setDraggingHorizontal(false);
        btnScrollUp.setColorReleased(scColor);
        btnScrollUp.setColorPressed(scPressedColor);
        btnScrollUp.setColorOver(ofColor(scOverColor.r, scOverColor.g, scOverColor.b, 180));
        btnScrollUp.setRoundness(sbRoundness);
        btnScrollUp.setPivot("tl");
        btnScrollUp.setInteractionArea(glm::vec2(scrollerRect.width , scrollerRect.width));
        
        
        // Down Button
        btnScrollDown.setup(glm::vec2(scrollerRect.width, scrollerRect.width), 2, scColor);
        btnScrollDown.setDraggingVertical(false);
        btnScrollDown.setDraggingHorizontal(false);
        btnScrollDown.setColorReleased(scColor);
        btnScrollDown.setColorPressed(scPressedColor);
        btnScrollDown.setColorOver(ofColor(scOverColor.r, scOverColor.g, scOverColor.b, 180));
        btnScrollDown.setRoundness(sbRoundness);
        btnScrollDown.setPivot("tl");
        btnScrollDown.setInteractionArea(glm::vec2(scrollerRect.width , scrollerRect.width));
        
        // Enable Interaction
        btnScrollDown.enableInteraction();
        btnScrollUp.enableInteraction();
        
        // Enable Listeners
        enableNavButtons();
        
        calculateSizes();
    }

    void setScrollNavigation(string _btnUpSrc, string _btnDownSrc, bool _enable = true) {
        isScrollNavEnable = _enable;
        btnScrollDown.setup(_btnDownSrc, 0);
        btnScrollUp.setup(_btnUpSrc, 0);
        if (isScrollNavEnable) enableNavButtons();
    }

	void setScrollNavPos(glm::vec2 _upPos, glm::vec2 _downPos) {
		btnScrollDownPos = _downPos;
		btnScrollUpPos = _upPos;
	}
    
    /* Mac touchpad or mouse scroll */
    void setMouseScrollEnabled(bool val) {
        isMouseScrollEnabled = val;
    }
    
    // GETTERS
    /* Mac touchpad or mouse scroll */
    bool getMouseScrollEnabled(bool val) {
        return isMouseScrollEnabled;
    }
    
private:
    void enableNavButtons() {
        ofAddListener(btnScrollUp.BUTTON_TOUCH_UP, this, &ofxFilikaContentScroller::moveContentUp);
        ofAddListener(btnScrollDown.BUTTON_TOUCH_UP, this, &ofxFilikaContentScroller::moveContentDown);
        ofAddListener(btnScrollUp.BUTTON_TOUCH_DOWN, this, &ofxFilikaContentScroller::moveContentUpPRESSED);
        ofAddListener(btnScrollDown.BUTTON_TOUCH_DOWN, this, &ofxFilikaContentScroller::moveContentDownPRESSED);
    }
    
    void disableNavButtons() {
        ofRemoveListener(btnScrollUp.BUTTON_TOUCH_UP, this, &ofxFilikaContentScroller::moveContentUp);
        ofRemoveListener(btnScrollDown.BUTTON_TOUCH_UP, this, &ofxFilikaContentScroller::moveContentDown);
        ofRemoveListener(btnScrollUp.BUTTON_TOUCH_DOWN, this, &ofxFilikaContentScroller::moveContentUpPRESSED);
        ofRemoveListener(btnScrollDown.BUTTON_TOUCH_DOWN, this, &ofxFilikaContentScroller::moveContentDownPRESSED);
    }
};

#endif /* ofxFilikaContentScroller_h */
