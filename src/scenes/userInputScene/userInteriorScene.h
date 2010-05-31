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
#include "polyEditable.h"
#include "ensancheUtils.h"
#include "polyFixWidth.h"
#include "polyGroup.h"
#include "ensancheLabelGroup.h"
#include "ensancheScaleTool.h"
#include "ensancheFloorPlan.h"



class userInteriorScene: public baseScene{
	
	public: 
		
	~userInteriorScene();
	void setup();
	void update();
	void resetScene();	
	
	void disable();
	
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
	
	// file loading
	void loadFloorPlanImage( string filename );	// loads image from file
	void loadUserFile();	// loads floor plan image + tracing and data from a uesr file
	void saveUserFile();
	void setUserName(string name);
	
	// floor plan
	ofImage imgFloorPlan;
	polyEditable fpPoly;
	string fpFileName;
	
	// status vars
	bool bFloorPlanLoaded;						// is there a floorplan loaded to memory
	
	// interior elements
	polyEditable	fpLimits;					// outer limits of apartment
	
	// pilars/windows
	void calcWindowsToWalls();
	void alignWindowsToWalls();
	polyGroup pilars;
	polyGroup windows;
	
	// for windows.. 
	vector<windowToWall> closeWindowWall;
	
	// room labels etc.
	EnsancheLabelGroup	rmLabels;
	vector<string> room_labels;
	
	// scale tool
	EnsancheScaleTool	scaleTool;
	
	EnsancheFloorPlan	enFloorPlan;
	
	//interface
	ofTrueTypeFont	titleFont;
	string title_limits;
	string title_windows;
	string title_walls;
	string title_agua;
	string title_rooms;
	string title_scale;
	
	void loadTextFromXML();
	
protected:
	
	bool bSetUserName;
	int lastSaveTime;
	
};