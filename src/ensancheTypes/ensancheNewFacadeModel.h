/*
 *  ensancheNewFacadeModel.h
 *  ensanche_2.0
 *
 *  Created by Chris on 5/19/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

#include "ensancheModelBuildingAdv.h"
#include "ensancheConstants.h"

typedef struct newFacadeFloorData{
	vector<int> sideIds;
	ofPoint endPtIndex[4];
	bool sideAltered[4];
};

class EnsancheNewFacadeModel : public EnsancheModelBuildingAdv{
	
	public:
		EnsancheNewFacadeModel();
		~EnsancheNewFacadeModel();
		EnsancheNewFacadeModel( const EnsancheNewFacadeModel & mom );
		
		void setupFromBuilding(EnsancheBuilding buildingOriginal );

		void setFloor(EnsancheBuilding buildingFloor, int floorNum, vector<int>sideIds, map<int,ofPoint> sideToEndPt);

		// insert new facade line into the side of floor
		void insertFacadeLine( vector<ofPoint> fLine, int floorNum, int startPt, int endPt, int sideId );
		
		void draw2D(bool bDrawSideColorCoded, bool bDrawWOffset);
		void draw();
		void drawAllFloors(bool bDrawSideColorCoded);
		
		// new facade should be in buildingFloors
		
		// original expanded facade
		vector<polySimple> momBuildingFloors;

		// endpoints + side ids for each floor
		vector<newFacadeFloorData> floorData;
				
		
	
};
