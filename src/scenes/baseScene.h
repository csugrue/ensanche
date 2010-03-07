/*
 *  baseScene.h
 *  openFrameworks
 *
 *  Created by Zach Lieberman on 9/23/09.
 *  Modified by Chris Sugrue.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */




#pragma once;

#include "ofMain.h"
#include "ofxControlPanel.h"
#include "ensancheConstants.h"

class baseScene{
	
	public: 
		
		virtual void setup(){}
		virtual void update(){}
		virtual void resetScene(){}
	
		virtual void startScene(){}
		virtual void endScene(){}
	
	
		virtual void preDraw(){}			// this is for clearing (or some other stuff)
		virtual void draw(){}
		
		
		virtual void keyPressed(int key){}
		virtual void keyReleased(int key){}
		virtual void mouseMoved(int x, int y){}
		virtual void mouseDragged(int x, int y, int button){}
		virtual void mousePressed(int x, int y, int button){}
		virtual void mouseReleased(int x, int y, int button){}
		
		virtual string	getStatusMessage(){ return "none"; }
		virtual bool	isTextEntryOn(){ return false; }
		
		virtual void disable(){}
		virtual void enable(){}
	
		//----  controls
		ofxControlPanel	panel;
		bool bPanelOn;
		
		string myName;
		string statusMessage;
	
		string username, userdir;
		virtual void setUserName( string name ){ username = name; }
		virtual void setUserDir( string dir ){ userdir = dir; }
		virtual	void loadUserFile(){}
		virtual void saveUserFile(){}
	
	
};