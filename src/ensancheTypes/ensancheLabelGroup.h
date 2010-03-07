/*
 *  ensancheLabelGroup.h
 *  ensanche_2.0
 *
 *  Created by Chris on 3/3/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

#pragma once

#include "ensancheRoomLabel.h"

class EnsancheLabelGroup{
	
	public:
	
		EnsancheLabelGroup();
		~EnsancheLabelGroup();
		
		void setup();
		void clear();
		void draw();
	
		void addLabel();
		void addLabel(int typeID, string name);
	
		void keyPressed(ofKeyEventArgs& event);
		void keyReleased(ofKeyEventArgs& event);
		void mouseMoved(ofMouseEventArgs& event);
		void mouseDragged(ofMouseEventArgs& event);
		void mousePressed(ofMouseEventArgs& event);
		void mouseReleased(ofMouseEventArgs& event);
		
		void disableAll(bool bDisableMe = false);
		void reEnableLast();
		void setLabel( string name, int id );
		void setScale( float s );
		void setOffset( ofPoint preR, ofPoint pstR );
		void setGRotation( float r );
	
		int getSelectedId();
	
		int selectedId, lastSelected;
		bool bEnabled;
	
		float scale;								// a rendering scale
		float gRotation;							// for mouse input, if we are rotating in draw, need proper mouse coords
		ofPoint transPreRotate,transPstRotate;		// this is to reverse engineer translation outside draw for mouse input...maybe there is a matri math op to do this better
		string cLabel;
		int cID;
	
	
		vector<EnsancheRoomLabel *> labels;
	
	protected:
		
		
		ofTrueTypeFont font;

};