/*
 *  sceneUserInput.h
 *  openFrameworks
 *
 *  Created by Chris Sugrue on 10.01.10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */




#pragma once;

#include "ofMain.h"
#include "baseScene.h"

class userComboScene: public baseScene{
	
	public: 
		
	~userComboScene();
	void setup();
	void update();
		
	
	void preDraw();
	void draw();
		
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
		
	string	getStatusMessage();
	
	void setupControlPanel();
	void updateControlPanel();
	


	
};