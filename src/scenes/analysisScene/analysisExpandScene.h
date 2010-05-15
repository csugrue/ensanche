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
	
	// core function to prepare for making new facades. finds sides + expands out minimum possible
	void makeInitialExpansion();
	
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
	
	// for making changes manually
	int currBuilding, prevBuilding;
	int currSide, prevSide;
	
	// facade makers
	FacadeBoxRipple		facadeMakerBox;
	
	//
	AnalysisSideAssigner	sideAssigner;
	AnalysisExpander		expander;
	
	// save expand data
	vector<EnsancheExpandData> expandBuildingData;
	
	
};