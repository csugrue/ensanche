/*
 *  ensancheScaleTool.cpp
 *  ensanche_2.0
 *
 *  Created by Chris on 3/3/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

#include "ensancheScaleTool.h"

EnsancheScaleTool::EnsancheScaleTool()
{
	polyGroup::polyGroup();
	pixelPerMeter = 1;
	mouse.set(0,0);
	bEnabled = false;
}

EnsancheScaleTool::~EnsancheScaleTool()
{
	for( int i = 0; i < polyFWs.size(); i++)
	{
		delete polyFWs[i];
	}
}

void EnsancheScaleTool::addPoly()
{
	polyGroup::addPoly();
	//setStraight();
}

void EnsancheScaleTool::setup()
{
	polyGroup::setup();
	ofAddListener(ofEvents.mouseMoved, this, &EnsancheScaleTool::mouseMoved);
	
	// color
	setFillColor(255,255,255,255);
	setStrokeColor(40,40,40,255);
	
	// width
	cFWidth = 2.f;
	
	font.loadFont("fonts/verdana.ttf", 8,true,true);
	font.setLineHeight(8+2);
	
}

void EnsancheScaleTool::draw()
{
	for( int i = 0; i < polyFWs.size(); i++)
	{
		if( polyFWs[i]->pts.size() == 1 )
		{
			ofPoint m = polyFWs[i]->getMouseAltered(mouse);
			ofFill();
			ofCircle(polyFWs[i]->pts[0].x,polyFWs[i]->pts[0].y,2);
			ofLine(polyFWs[i]->pts[0].x,polyFWs[i]->pts[0].y,m.x,m.y);
		}else if(polyFWs[i]->pts.size() == 2)
		{
			ofLine(polyFWs[i]->pts[0].x,polyFWs[i]->pts[0].y,polyFWs[i]->pts[1].x,polyFWs[i]->pts[1].y);
			
			// Note: not so efficient to calc everytime
			float xDiff = fabs(polyFWs[i]->pts[0].x-polyFWs[i]->pts[1].x);
			float yDiff = fabs(polyFWs[i]->pts[0].y-polyFWs[i]->pts[1].y);
			
			if( xDiff > yDiff)
			{
				ofLine( polyFWs[i]->pts[0].x,polyFWs[i]->pts[0].y-4,polyFWs[i]->pts[0].x,polyFWs[i]->pts[0].y+4);
				ofLine( polyFWs[i]->pts[1].x,polyFWs[i]->pts[1].y-4,polyFWs[i]->pts[1].x,polyFWs[i]->pts[1].y+4);
			}else{
				ofLine( polyFWs[i]->pts[0].x-4,polyFWs[i]->pts[0].y,polyFWs[i]->pts[0].x+4,polyFWs[i]->pts[0].y);
				ofLine( polyFWs[i]->pts[1].x-4,polyFWs[i]->pts[1].y,polyFWs[i]->pts[1].x+4,polyFWs[i]->pts[1].y);
			}
			
			// draw measurement
			float len = (xDiff > yDiff) ? xDiff: yDiff;
			float measurement = pixelPerMeter * len;
			string strMeasure = ofToString(measurement,2);
			strMeasure += " m";
			float strLen = font.stringWidth(strMeasure);
			
			if( xDiff > yDiff )
			{
				float cX = .5*(polyFWs[i]->pts[0].x+polyFWs[i]->pts[1].x);
				font.drawString(strMeasure, cX - .5*strLen, polyFWs[i]->pts[0].y-4);
			}else{
				float cY = .5*(polyFWs[i]->pts[0].y+polyFWs[i]->pts[1].y);
				glPushMatrix();
					glTranslatef(polyFWs[i]->pts[0].x,cY,0);
					glRotatef(90,0,0,1);
					font.drawString(strMeasure, -.5*strLen, -4);
				glPopMatrix();
			}
			
			
			
		}
		
	}
	
}

void EnsancheScaleTool::mouseMoved(ofMouseEventArgs& event)
{
	polyGroup::mouseMoved(event);
	mouse.x = event.x;
	mouse.y = event.y;
}

void EnsancheScaleTool::mouseReleased(ofMouseEventArgs& event)
{
	polyGroup::mouseReleased(event);
	setStraight();
}

void EnsancheScaleTool::setPixelPerMeter(float ppm)
{
	pixelPerMeter = ppm;
	
}

