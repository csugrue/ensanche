/*
 *  ensancheFloorPlan.h
 *  ensanche_2.0
 *
 *  Created by Chris on 3/3/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

#pragma once

#include "polyGroup.h"
#include "polyUtils.h"
#include "polyEditable.h"
#include "ensancheRoomLabel.h"
#include "ofxXmlSettings.h"
#include "uidGenerator.h"

//-- types
typedef struct enWall{
	ofPoint pts[2];
	string uid;
	int posInPoly;
	int type;
};
	
typedef struct enPilar{
	ofPoint pts[2];
	float width;
	string uid;
};

typedef struct enWindow{
	ofPoint pts[2];
	float width;
	string uid;
	int wallIndex;
	string wallUID;
};

typedef struct enEntry{
	ofPoint pt;
	float width;
	float height;
	string uid;
	int wallIndex;
};

typedef struct windowToWall{
	int wallindex;
	float closeDist;
	ofPoint closePt;
};

class EnsancheFloorPlan
{
	public:
		EnsancheFloorPlan();
		~EnsancheFloorPlan();
		EnsancheFloorPlan(const EnsancheFloorPlan & mom);
		
		void clear();
		void setup();
		void draw();
		
		//--- set from xml
		void loadFromXml( string filename);
	
		void clearWindows(){windows.clear();}
		void clearPilars(){	pilars.clear(); }
	
		//--- set from editor functions
		void addWalls(polyEditable & fpLimits, float pixelsPerMeter);
		void addPilar(polyFixWidth * fpPilar, float pixelsPerMeter);
		void addWindow(polyFixWidth * fpWindow, windowToWall closeWindowWall,float pixelsPerMeter);
		void addEntry(EnsancheRoomLabel fpEntry, float pixelsPerMeter);
		
		//--- architectural elements
		vector<enWall>		walls;
		vector<enPilar>		pilars;
		vector<enWindow>	windows;
		vector<enEntry>		entries;
	
		ofPoint	getTransformsApplied(ofPoint me, ofPoint pre, ofPoint pst, float rotation);

	protected:
		
		polySimple polyLimitsTrans;
		ofRectangle boundingbox;
};