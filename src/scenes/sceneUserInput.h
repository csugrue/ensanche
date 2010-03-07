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
#include "ofxDirList.h"
#include "pocoDirectoryLister.h"
#include "userMapScene.h"
#include "userInteriorScene.h"
#include "userComboScene.h"
#include "ensancheConstants.h"

#define USER_NUM_SUB_SCENES 3

#define USER_SUB_MAP	0
#define USER_SUB_INT	1
#define USER_SUB_COM	2

#define LOAD_DATA_ALL		0
#define LOAD_DATA_MAP		1
#define LOAD_DATA_BUILDINGS	2
#define LOAD_DATA_FLOORPLAN 3
#define LOAD_DATA_INTERIOR	4
#define LOAD_DATA_COMBO		5

typedef struct userFileData{
	string username;
	string address;	
};
	
class sceneUserInput: public baseScene{
	
	public: 
		
	~sceneUserInput();
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
	
	void setupControlPanel();
	void updateControlPanel();
	
	void triggerEvent(string event);
	void changeScene();
	int  getSubSceneId( string panelName);
	
	// file io
	void saveUserData();
	void updateSavedUserList();
	bool loadUserData( string userFolderName, int level = 0 );
	
	simpleFileLister lister;
	ofxPocoDirectoryLister pocoLister;
	string userDir;
	vector<string> savedUserFolders;
	vector<userFileData> userData;
	guiTypeFileLister * savedUserLister;
	
	// sub scenes
	baseScene *userScenes[USER_NUM_SUB_SCENES];
	int currentSubScene;
	string lastPanelOn;
	
	// loading floor plans
	void updateSavedFloorPlanList();
	simpleFileLister fpLister;
	guiTypeFileLister * savedFloorPlansLister;
	int nSavedFPlans;
	
};