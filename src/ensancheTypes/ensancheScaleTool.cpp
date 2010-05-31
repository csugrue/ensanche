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
	bStraighten = false;
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

void EnsancheScaleTool::addPoly( ofPoint m0, ofPoint m1, float width )
{
	polyGroup::addPoly(m0,m1,width);
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
		float armlen = 4*(1/scale);
		
		if( polyFWs[i]->pts.size() == 1 )
		{
			
			ofPoint m = polyFWs[i]->getMouseAltered(mouse);
			ofFill();
			ofCircle(polyFWs[i]->pts[0].x,polyFWs[i]->pts[0].y,armlen);
			ofLine(polyFWs[i]->pts[0].x,polyFWs[i]->pts[0].y,m.x,m.y);
		
		}else if(polyFWs[i]->pts.size() == 2)
		{
			
			ofLine(polyFWs[i]->pts[0].x,polyFWs[i]->pts[0].y,polyFWs[i]->pts[1].x,polyFWs[i]->pts[1].y);
			
			ofxVec2f pp = ofxVec2f(polyFWs[i]->pts[0].x,polyFWs[i]->pts[0].y)-ofxVec2f(polyFWs[i]->pts[1].x,polyFWs[i]->pts[1].y);
			float rangle = RAD_TO_DEG*atan2(pp.y,pp.x);
			float len = pp.length();
			
			if(rangle<0)
			{
				pp = ofxVec2f(polyFWs[i]->pts[1].x,polyFWs[i]->pts[1].y)-ofxVec2f(polyFWs[i]->pts[0].x,polyFWs[i]->pts[0].y);
				rangle = RAD_TO_DEG*atan2(pp.y,pp.x);
			}
			
			if(rangle > 90 && rangle < 180)
			{
			rangle += 180;
			}
			
			
			
			pp = pp.perpendicular();
			
			// Note: not so efficient to calc everytime
			float xDiff = fabs(polyFWs[i]->pts[0].x-polyFWs[i]->pts[1].x);
			float yDiff = fabs(polyFWs[i]->pts[0].y-polyFWs[i]->pts[1].y);
			
			ofLine( polyFWs[i]->pts[0].x-armlen*pp.x,polyFWs[i]->pts[0].y-armlen*pp.y,polyFWs[i]->pts[0].x+armlen*pp.x,polyFWs[i]->pts[0].y+armlen*pp.y);
			ofLine( polyFWs[i]->pts[1].x-armlen*pp.x,polyFWs[i]->pts[1].y-armlen*pp.y,polyFWs[i]->pts[1].x+armlen*pp.x,polyFWs[i]->pts[1].y+armlen*pp.y);

			/*if( xDiff > yDiff)
			{
				ofLine( polyFWs[i]->pts[0].x,polyFWs[i]->pts[0].y-armlen,polyFWs[i]->pts[0].x,polyFWs[i]->pts[0].y+armlen);
				ofLine( polyFWs[i]->pts[1].x,polyFWs[i]->pts[1].y-armlen,polyFWs[i]->pts[1].x,polyFWs[i]->pts[1].y+armlen);
			}else{
				ofLine( polyFWs[i]->pts[0].x-armlen,polyFWs[i]->pts[0].y,polyFWs[i]->pts[0].x+armlen,polyFWs[i]->pts[0].y);
				ofLine( polyFWs[i]->pts[1].x-armlen,polyFWs[i]->pts[1].y,polyFWs[i]->pts[1].x+armlen,polyFWs[i]->pts[1].y);
			}*/
			
			// draw measurement
			//float len = (xDiff > yDiff) ? xDiff: yDiff;
			float measurement = pixelPerMeter * len;
			string strMeasure = ofToString(measurement,2);
			strMeasure += " m";
			float strLen = font.stringWidth(strMeasure);
			
			ofPoint ctr = ofPoint( .5*(polyFWs[i]->pts[0].x+polyFWs[i]->pts[1].x),.5*(polyFWs[i]->pts[0].y+polyFWs[i]->pts[1].y) );
			
			
			glPushMatrix();
				glTranslatef(ctr.x,ctr.y,0);
				glScalef(1/scale,1/scale,1);
				glRotatef(rangle,0,0,1);
				font.drawString(strMeasure, -.5*strLen, -8);
			glPopMatrix();
			
			/*if( xDiff > yDiff )
			{
				float cX = .5*(polyFWs[i]->pts[0].x+polyFWs[i]->pts[1].x);
				glPushMatrix();
					glTranslatef(cX,polyFWs[i]->pts[0].y,0);
					glScalef(1/scale,1/scale,1);
					font.drawString(strMeasure, -.5*strLen, -4);
				glPopMatrix();

			}else{
				float cY = .5*(polyFWs[i]->pts[0].y+polyFWs[i]->pts[1].y);
				glPushMatrix();
					
					glTranslatef(polyFWs[i]->pts[0].x,cY,0);
					glScalef(1/scale,1/scale,1);
					glRotatef(90,0,0,1);
					
					font.drawString(strMeasure, -.5*strLen, -4);
				glPopMatrix();
			}*/
			
			
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
	if(bStraighten) setStraight();
	

}

void EnsancheScaleTool::setPixelPerMeter(float ppm)
{
	pixelPerMeter = ppm;
	
}

