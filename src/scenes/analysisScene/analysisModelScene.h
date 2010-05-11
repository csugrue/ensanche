/*
 *  analysisModelScene.h
 *  ensanche_2.0
 *
 *  Created by Chris on 3/9/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

#pragma once;

#include "ofMain.h"
#include "baseScene.h"
#include "ensancheModelBarrio.h"
#include "ensancheBarrio.h"

class AnalysisModelScene: public baseScene{
	
public: 
	
	~AnalysisModelScene();
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
	
	void disable();
	void enable();
	
	string	getStatusMessage();
	
	void setupControlPanel();
	void updateControlPanel();
	
	void setUserName( string name );
	void loadUserFile();
	
	bool bSetUserName;
	bool bEnabled;
	
	// model w/ 3d drawing etc.
	EnsancheModelBarrio modelBarrio;
	
	//EnsancheModelBarrio modelBarrioExpanded;
	
};