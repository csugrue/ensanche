/*
 *  analysisExpander.h
 *  ensanche_2.0
 *
 *  Created by Chris on 5/11/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

/*
expands sides of bulding a given distance (does not make new facade)
*/
#pragma once

#include "analysisExpandAreaFinder.h"
#include "ensancheExpandData.h"

// note instead of saving buildings, save endpoints + position
/*
//moved to ensancheExpandData
typedef struct endPoints{
	vector<int> ids;
	vector<ofPoint> pts;
};
*/
class AnalysisExpander{
	
	public:
	
		AnalysisExpander();
		~AnalysisExpander();
	
		void clear();
		
		void expand( EnsancheExpandData & expandBuilding, int sideToExpand, enExpandData epData );
		
		
		// takes in: original building, destination building, expand data, side to expand, 
		// takes in building, dest building, sideIds, side to expand, total length possible to expand or expand data
		void expand( 
						EnsancheBuilding building, 
						EnsancheBuilding & dstBuilding, 
						int sideToExpand,
						vector<int>sideIds,
						enExpandData epData,
						endPoints * nEnds);
	
			
		// function finds the end points and adds the data NOT replacing any old data
		void findEndPoints(EnsancheBuilding building, int sideToExpand, vector<int>sideIds,endPoints * nEnds);
		
		// function finds the end points of all sides sets data replacing any old data
		void findAllEndPoints(EnsancheBuilding building, vector<int>sideIds, endPoints * nEnds);
		
		// removes the points in between the endpoints for the given side. also removes walls, sideids, and resets endpoints
		void removeInBetweenPoints(EnsancheBuilding & building, int sideToExpand, vector<int> * sideIds, endPoints * nEnds);
		
		  
		void draw(float scale = 1);
		
		vector<EnsancheBuilding> buildings;
		vector<endPoints> ends;
		map<string,int> uidToIndex;
		
		ofTrueTypeFont font;

	protected:
		

};
