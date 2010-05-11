/*
 *  analysisFacadeMaker.h
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
#include "ensancheModelBuilding.h"
#include "analysisExpandAreaFinder.h"
/*
: takes in -> polygon of explanded buliding + data on the expansion
: creates new facade for the current building
: inserts polygons for all floors
: takes the new expansion and determines how far in and out it can make the ripple
: makes varying ripples for each floor 
: inserts new points for the facade

*/

class AnalysisFacadeMaker{
	
	public:
	
		AnalysisFacadeMaker();
		~AnalysisFacadeMaker();
		
		// settings
		// ?
		
		
		// pass in pointer to model to fill in with new data
		// pass in expansion info
		void createFacade(EnsancheModelBuilding * newBuilding, EnsancheBuilding building, enExpandAreas expander);
		
					
	protected:
		
};
