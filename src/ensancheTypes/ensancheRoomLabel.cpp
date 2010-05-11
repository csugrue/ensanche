/*
 *  ensancheRoomLabel.cpp
 *  ensanche_2.0
 *
 *  Created by Chris on 3/3/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

#include "ensancheRoomLabel.h"

EnsancheRoomLabel::EnsancheRoomLabel()
{
	polyEditable::polyEditable();
	labelID = 0;
	labelName  = "";
	bSetFont = false;
	w = 2;
	h = 2;
}

EnsancheRoomLabel::EnsancheRoomLabel( const EnsancheRoomLabel & mom )
{
	polyEditable::polyEditable((polyEditable)mom);
	
	labelID = mom.labelID;
	labelName = mom.labelName;
	
	 w = mom.w;
	 h = mom.h;
	
	// NOTE: how to copy pointer??
	//ofTrueTypeFont * font;
	bSetFont = false;
}

void EnsancheRoomLabel::clear()
{
	polyEditable::clear();
	bSetFont = false;
	labelName  = "";
}

void EnsancheRoomLabel::setup()
{
	polyEditable::setup();
}

void EnsancheRoomLabel::draw()
{
	if( pts.size() <= 0 ) return;
	
	ofCircle(pts[0].x, pts[0].y, 2);
	
	if( bSetFont )
	{
		if( pts.size() > 0 ) font->drawString(labelName, pts[0].x + 4, pts[0].y + 4);
	}else{
		if( pts.size() > 0 ) ofDrawBitmapString(labelName, pts[0].x + 4, pts[0].y + 4);
	}
	
}

void EnsancheRoomLabel::addPoint( ofPoint pt )
{
	if( pts.size() == 0 )
	{
		pushVertex( pt );
		selectedPoint = pts.size()-1;
	}
	
}

void EnsancheRoomLabel::keyPressed(ofKeyEventArgs& event)
{
	polyEditable::keyPressed(event);
}

void EnsancheRoomLabel::mouseDragged(ofMouseEventArgs& event)
{
	polyEditable::mouseDragged(event);
}

void EnsancheRoomLabel::mousePressed(ofMouseEventArgs& event)
{
	polyEditable::mousePressed(event);
}

void EnsancheRoomLabel::setLabel( string name, int id )
{
	labelName = name;
	labelID = id;
	
	if(bSetFont)
	{
		w = 8+ font->stringWidth(labelName);
		h = 2+ font->getLineHeight();
		
	}else
	{
		w = labelName.length();
		h = 8;
	}
}

void EnsancheRoomLabel::setFont( ofTrueTypeFont * myFont)
{
	font = myFont;
	bSetFont = true;
}

ofPoint EnsancheRoomLabel::getCentroid()
{
	if( pts.size() > 0 )
	{
		return pts[0];
	}else
		return ofPoint(0,0);
}

ofRectangle EnsancheRoomLabel::getBoundingBox()
{
	if( pts.size() > 0 )
	{
		ofRectangle rect;
		rect.x = pts[0].x - 2;
		rect.y = pts[0].y - 4;
		rect.width = w;
		rect.height = h;
		
		return rect;
		
	}else
		return ofRectangle(0,0,0,0);
}
