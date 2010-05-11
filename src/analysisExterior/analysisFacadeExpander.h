/*
 *  analysisFacadeExpander.h
 *  ensanche_2.0
 *
 *  Created by Chris on 3/18/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ensancheBarrio.h"
#include "ensancheUtils.h"
#include "ensancheConstants.h"
#include "analysisExpandAreaFinder.h"

/*
Takes in expand areas and buildings
Decides which direction to expand, or set this
Expands out the original building with new measurements, keeping only relevant points
*/

class AnalysisFacadeExpander{
	
	public:
	
		AnalysisFacadeExpander();
		~AnalysisFacadeExpander();

		void setup();
		void createExpansions(vector<EnsancheBuilding> buildings,vector<enExpandAreas> expandersAreas);
		
		
		// buildings with sides expanded
		vector<EnsancheBuilding> buildings;
				
			
	protected:
		
		
	
};
