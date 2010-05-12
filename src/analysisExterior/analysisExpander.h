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

#include "analysisExpandAreaFinder.h"

// note instead of saving buildings, save endpoints + position
typedef struct endPoints{
	vector<int> ids;
	vector<ofPoint> pts;
};

class AnalysisExpander{
	
	public:
	
		AnalysisExpander();
		~AnalysisExpander();
	
		void clear();
		
		// takes in: original building, destination building, expand data, side to expand, 
		// 	takes in building, dest building, sideIds, side to expand, total length possible to expand or expand data
		void expand( 
						EnsancheBuilding building, 
						EnsancheBuilding & dstBuilding, 
						int sideToExpand,
						vector<int>sideIds,
						enExpandData epData
					);
	
	
		void draw(float scale = 1);
		
		vector<EnsancheBuilding> buildings;
		vector<endPoints> ends;
		map<string,int> uidToIndex;
		
	protected:
		
		ofTrueTypeFont font;

};
