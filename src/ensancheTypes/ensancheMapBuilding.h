/*
 *  ensancheBuilding.h
 *  ensanche_2.0
 *
 *  Created by Chris on 1/26/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */
#pragma once

#include "polyEditable.h"
#include "uidGenerator.h"

#define EN_BUILDING_ME			0
#define EN_BUILDING_NEIGHBOR	1
#define EN_BUILDING_OTHER		2

class EnsancheMapBuilding: public polyEditable{
	
	public:
		EnsancheMapBuilding();
		//~EnsancheMapBuilding(){};
		EnsancheMapBuilding( const EnsancheMapBuilding & mom );
		
		void drawData();
		
		// type of building
		int type;
		
		// number of floors
		int nFloors;
		
		// unique id
		string uid;
		
		
		// 
};
