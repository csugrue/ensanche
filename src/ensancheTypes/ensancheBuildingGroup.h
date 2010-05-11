/*
 *  ensancheBuildingGroup.h
 *  ensanche_2.0
 *
 *  Created by Chris on 3/4/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

#pragma once

#include "ensancheMapBuilding.h"

// NOTE: needs copy constructor!


class EnsancheBuildingGroup{

	public:
		EnsancheBuildingGroup();
		~EnsancheBuildingGroup();
		
		void setup();
		void clear();
		void draw();
		
		void addBuilding();
		void removeBuilding();
		
		void keyPressed(ofKeyEventArgs& event);
		void keyReleased(ofKeyEventArgs& event);
		void mouseMoved(ofMouseEventArgs& event);
		void mouseDragged(ofMouseEventArgs& event);
		void mousePressed(ofMouseEventArgs& event);
		void mouseReleased(ofMouseEventArgs& event);
		
		void disableAll(bool bDisableMe = false);
		void reEnableLast();
		void setScale( float s );
		void setOffset( ofPoint preR, ofPoint pstR );
		void setGRotation( float r );
		void setIsMe();
	
		bool isEnabled(){ return bEnabled; }
		
		float scale;								// a rendering scale
		float gRotation;							// for mouse input, if we are rotating in draw, need proper mouse coords
		ofPoint transPreRotate,transPstRotate;		// this is to reverse engineer translation outside draw for mouse input...maybe there is a matri math op to do this better
		
		vector<EnsancheMapBuilding*> buildings;	// all buildings added by user
		int selectedId; // which being edited now 
		
		bool bDrawData;
	
	protected:
	
		bool bEnabled;

};
