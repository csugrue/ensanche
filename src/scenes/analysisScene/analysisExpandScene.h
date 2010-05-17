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
	
	void setupEndPointData();
	void setupExpandData();
	
	void findSideIdData();
	void finInitialExpandData();
	
	bool bSetUserName;
	bool bEnabled;
	
	// model of original layout
	EnsancheBarrio	barrioOriginal;
	
	// tools for making expansion
	AnalysisExpandAreaFinder minRectExpander;
	
	// expanded models
	vector<EnsancheModelBuilding>	buildings;
	
	// scale tool
	EnsancheScaleTool	scaleTool;
	
	// update controls
	guiTypeMultiToggle * multiWhichB;
	guiTypeSlider	* guiChooseBuliding;
	guiTypeSlider	* guiChooseSide;
	guiTypeSlider	* guiChooseExpandBuliding;
	guiTypeSlider	* guiChooseExpandSide;
	
	// for making changes manually
	int currBuilding, prevBuilding;
	int currSide, prevSide;
	
	// facade makers
	FacadeBoxRipple		facadeMakerBox;
	
	//
	AnalysisSideAssigner	sideAssigner;
	AnalysisExpander		expander;
	
	// save expand data
	vector<EnsancheExpandData> buildingDataEndPoints;		// preserves side ids and original end points
	vector<EnsancheExpandData> buildingDataExpanded;		// removed points in between end points an preserves initial minimum expansion
	
	
	
};