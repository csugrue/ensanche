/*
 *  analysisExpandScene.h
 *  ensanche_2.0
 *
 *  Created by Chris on 3/5/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */


#pragma once;

#include "ofMain.h"
#include "baseScene.h"
#include "ensancheBarrio.h"
#include "ensancheModelRoom.h"


class AnalysisExpandScene: public baseScene{
	
public: 
	
	~AnalysisExpandScene();
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
	
	void setUserName( string name );
	void loadUserFile();
	
	bool bSetUserName;
	
	// model of original layout
	EnsancheBarrio	barrioOriginal;
	EnsancheModelRoom modelRoom;
};