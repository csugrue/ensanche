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
#include "ensancheUtils.h"
#include "analysisExpandAreaFinder.h"
#include "ensancheScaleTool.h"
#include "ensancheModelBuilding.h"
#include "facadeBoxRipple.h"
#include "analysisSideAssigner.h"
#include "analysisExpander.h"
#include "ensancheExpandData.h"
#include "ensancheNewFacadeModel.h"

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
	
	void disable();
	void enable();
	
	string	getStatusMessage();
	
	void setupControlPanel();
	void updateControlPanel();
	
	void setUserName( string name );
	void loadUserFile();
	
	// initializes vector of bildings to hold data on endpoints of "sides" of the buliding
	void setupEndPointData();
	
	// initializes vector of buildings to hold data on the initial expansion (sans facade ripple) of each building
	void setupExpandData();
	
	// for all buildings, assigns a side to each wall of building
	void findSideIdData();
	
	// create models that will hold info for all floors, inclding initial expansions
	void setUpModels();
	
	// set the initial expansion for each floor for a building
	void setModelInitialExpansion(int index);
	
	// create a new facade line for the current buildng, side, and floor
	void createFacadeLine( int ixBuilding, int ixFloor, int sideToAlter);
	
	
	bool bSetUserName;
	bool bEnabled;
	
	// model of original layout
	EnsancheBarrio	barrioOriginal;
	
	// tools for making expansion
	AnalysisExpandAreaFinder minRectExpander;
	
	// expanded models
	vector<EnsancheModelBuilding>	buildings;
	vector<EnsancheNewFacadeModel>	fBuildings;
	
	// scale tool
	EnsancheScaleTool	scaleTool;
	
	// update controls
	guiTypeMultiToggle * multiWhichB;
	guiTypeSlider	* guiChooseBuliding;
	guiTypeSlider	* guiChooseSide;
	guiTypeSlider	* guiChooseExpandBuliding;
	guiTypeSlider	* guiChooseExpandSide;
	guiTypeSlider	* guiChooseFacadeBuilding;
	guiTypeSlider	* guiChooseFacadeFloor;
	
	// for making changes manually
	int currBuilding, prevBuilding;
	int currSide, prevSide;
	int prevFBuilding;
	
	// facade makers
	FacadeBoxRipple		facadeMakerBox;
	
	//
	AnalysisSideAssigner	sideAssigner;
	AnalysisExpander		expander;
	
	// save expand data
	vector<EnsancheExpandData> buildingDataEndPoints;		// preserves side ids and original end points
	vector<EnsancheExpandData> buildingDataExpanded;		// removed points in between end points an preserves initial minimum expansion
	
	ofTexture textureWalls[MODEL_T_TEXTURES];
	EnsancheNewFacadeModel fmodel;
	
};