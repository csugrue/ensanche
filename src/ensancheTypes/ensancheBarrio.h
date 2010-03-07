/*
 *  ensancheBarrio.h
 *  ensanche_2.0
 *
 *  Created by Chris on 3/4/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */


#pragma once

#include "ensancheBuilding.h"
#include "ensancheBuildingGroup.h"

class EnsancheBarrio{
	
public:
	EnsancheBarrio();
	~EnsancheBarrio();
	
	void clear();
	void setup();
	void draw();
	
	void addBuilding();
	
	//--- set from xml
	void loadFromXml( string filename);
	
	//--- set from editor functions
	void addBuilding(EnsancheBuildingGroup * mapBuildings, float pixelsPerMeter);
	
	//--- architectural elements
	vector<EnsancheBuilding*>		buildings;
	ofRectangle boundingBox;
	
protected:
	
	ofRectangle getGroupBoundingBox();
	
};