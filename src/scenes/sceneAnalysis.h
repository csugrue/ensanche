/*
 *  sceneAnalysis.h
 *  ensanche_2.0
 *
 *  Created by Chris on 3/5/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

#pragma once;

#include "ofMain.h"
#include "baseScene.h"
#include "analysisExpandScene.h"

#define ANALYSIS_NUM_SUB_SCENES 1

#define ANALYSIS_SUB_EXPAND	0


class SceneAnalysis: public baseScene{
	
public: 
	
	~SceneAnalysis();
	void setup();
	void update();
	void resetScene();
	
	
	void preDraw();
	void draw();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	
	string	getStatusMessage();
	
	void startScene();
	void setUserName(string name);
	
	void setupControlPanel();
	void updateControlPanel();
	
	int getSubSceneId( string panelName);
	
	// sub scenes
	baseScene *analysisScenes[ANALYSIS_NUM_SUB_SCENES];
	int currentSubScene;
	string lastPanelOn;
	
};