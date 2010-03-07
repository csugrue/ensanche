/*
 *  ensancheRoomLabel.h
 *  ensanche_2.0
 *
 *  Created by Chris on 3/3/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

#pragma once

#include "polyEditable.h"

class EnsancheRoomLabel : public polyEditable{

	public:
		EnsancheRoomLabel();
		~EnsancheRoomLabel();
		
		void clear();
		void setup();
		void draw();
		
		void addPoint( ofPoint pt );
		ofPoint getCentroid();
		ofRectangle getBoundingBox();
	
		void keyPressed(ofKeyEventArgs& event);
		void mouseDragged(ofMouseEventArgs& event);
		void mousePressed(ofMouseEventArgs& event);
		
		void setLabel( string name, int id );
		void setFont( ofTrueTypeFont * myFont);
	
		int		labelID;
		string	labelName;
	
		float w,h;
		
	protected:
	
		ofTrueTypeFont * font;
		bool bSetFont;
};