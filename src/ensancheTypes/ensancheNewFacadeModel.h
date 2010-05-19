/*
 *  ensancheNewFacadeModel.h
 *  ensanche_2.0
 *
 *  Created by Chris on 5/19/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

#include "ensancheModelBuildingAdv.h"

typedef struct newFacadeFloorData{
	vector<int> sideIds;
	vector<ofPoint> endPts;
};

class EnsancheNewFacadeModel : public EnsancheModelBuildingAdv{
	
	public:
		EnsancheNewFacadeModel();
		~EnsancheNewFacadeModel();
		EnsancheNewFacadeModel( const EnsancheNewFacadeModel & mom );
		
		void setupFromModelBuilding(EnsancheModelBuildingAdv & mom);
		
		// insert new facade line into the side of floor
		void insertFacadeLine( vector<ofPoint> fLine, int floorNum, int startPt, int endPt, int sideId );
		
		void draw();
		
		// new facade should be in buildingFloors
		
		// original expanded facade
		vector<polySimple> momBuildingFloors;

		// endpoints + side ids for each floor
		vector<newFacadeFloorData> floorData;
				
		
	
};
