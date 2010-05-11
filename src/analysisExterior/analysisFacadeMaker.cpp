/*
 *  AnalysisFacadeExpander.cpp
 *  ensanche_2.0
 *
 *  Created by Chris on 3/18/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

#include "analysisFacadeMaker.h"


AnalysisFacadeMaker::AnalysisFacadeMaker()
{
}


AnalysisFacadeMaker::~AnalysisFacadeMaker()
{
}

void AnalysisFacadeMaker::createFacade(EnsancheModelBuilding * newBuilding, EnsancheBuilding building, enExpandAreas expander)
{

	
	// insert the floors
	for( int i = 0; i < building.nFloors; i++)
		newBuilding->addBuildingFloor(building.buildingPoly);
	

	if(building.nFloors > 1)
	{
		
	}
	
	// check the expansion on each side and see limits of rippling
	// expand
	// add points
	// extrude
	
	
}

