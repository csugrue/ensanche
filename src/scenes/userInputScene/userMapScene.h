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
#include "ensancheConstants.h"
#include "googleMapLoader.h"
#include "polyEditable.h"
#include "ensancheUtils.h"
#include "ensancheMapBuilding.h"
#include "ensancheBuildingGroup.h"
#include "ensancheBuilding.h"
#include "ensancheBarrio.h"

/*
 - map
 - google map loader
 - buildings
*/

class userMapScene: public baseScene{
	
	public: 
		
	~userMapScene();
	void setup();
	void update();
	
	void findMap( string address);
		
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
	
	void disable();
	
	void saveUserFile();
	void loadUserFile();
	
	//--- map functions
	void saveTiles(string dir);
	void loadTileFromFolder( string path);
	
	//--- map
	ofImage		myMap;
	float		rotation;
	polyEditable mapPoly;
	
	
	//--- google map loading
	googleMapLoader	googleLoader;
	bool	bUsingGoogleMap;
	int		nTilesLoaded;
	bool	bSavedTiles;

	//--- io
	void setUserName(string name);
	bool bSetUserName;
	
	//--- buildings
	EnsancheBuildingGroup buildingGroup;
	
	//--- interface
	ofPoint mouse;
	int lastSelectedBuilding;
	
};