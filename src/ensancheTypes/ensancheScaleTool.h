/*
 *  ensancheScaleTool.h
 *  ensanche_2.0
 *
 *  Created by Chris on 3/3/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

#pragma once

#include "polyGroup.h"

// NOTE: needs copy constructor!


class EnsancheScaleTool : public polyGroup
{
	public:
		EnsancheScaleTool();
		~EnsancheScaleTool();
		
		void setup();
		void addPoly();
		void addPoly( ofPoint m0, ofPoint m1, float width );
		
		void draw();
		void setPixelPerMeter(float ppm);
	
		void mouseMoved(ofMouseEventArgs& event);
		void mouseReleased(ofMouseEventArgs& event);
	
		float pixelPerMeter;
		ofPoint mouse;
		bool bStraighten;
		
	protected:
		
		ofTrueTypeFont font;
};