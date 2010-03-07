/*
 *  polyGroup.cpp
 *  ensanche_2.0
 *
 *  Created by Chris on 3/1/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

#include "polyGroup.h"

polyGroup::polyGroup()
{
	selectedId	= -1;
	cFWidth		= 1;
	bFWSquare	= false;
	scale			= 1;
	invScale		= 1.f/scale;
	gRotation		= 0;
	transPreRotate.set(0,0);
	transPstRotate.set(0,0); 
	bEnabled = false;
	
	setFillColor(200,200,200,200);
	setStrokeColor(80,80,80,255);
}

polyGroup::~polyGroup()
{
	for( int i = 0; i < polyFWs.size(); i++)
	{
		delete polyFWs[i];
	}
	ofRemoveListener(ofEvents.mousePressed, this, &polyGroup::mousePressed);
	ofRemoveListener(ofEvents.mouseReleased, this, &polyGroup::mouseReleased);
	ofRemoveListener(ofEvents.mouseDragged, this, &polyGroup::mouseDragged);
	ofRemoveListener(ofEvents.keyPressed, this, &polyGroup::keyPressed);
	ofRemoveListener(ofEvents.keyReleased, this, &polyGroup::keyReleased);
}

void polyGroup::setup()
{
	ofAddListener(ofEvents.mousePressed, this, &polyGroup::mousePressed);
	ofAddListener(ofEvents.mouseReleased, this, &polyGroup::mouseReleased);
	ofAddListener(ofEvents.mouseDragged, this, &polyGroup::mouseDragged);
	ofAddListener(ofEvents.keyPressed, this, &polyGroup::keyPressed);
	ofAddListener(ofEvents.keyReleased, this, &polyGroup::keyReleased);
	
}

void polyGroup::clear()
{
	polyFWs.clear();
}

void polyGroup::addPoly()
{
	
	//if( !bEnabled ) return;
	
	polyFWs.push_back( new polyFixWidth() );
	selectedId	= polyFWs.size()-1;
	polyFWs[selectedId]->setOffset(transPreRotate,transPstRotate);
	polyFWs[selectedId]->setScale(scale);
	polyFWs[selectedId]->setGRotation(gRotation);
	polyFWs[selectedId]->setWidth(cFWidth);
	polyFWs[selectedId]->setup();
	polyFWs[selectedId]->enable();
	
	polyFWs[selectedId]->setFillColor(fillColor.r, fillColor.g, fillColor.b, fillColor.a);
	polyFWs[selectedId]->setStrokeColor(strokeColor.r, strokeColor.g, strokeColor.b, strokeColor.a);

}

void polyGroup::addPoly( ofPoint m0, ofPoint m1, float width  )
{
	
	setWidth(width);
	addPoly();
	
	polyFWs[selectedId]->addPoint( m0 );
	polyFWs[selectedId]->addPoint( m1 );
	
}

void polyGroup::disableAll(bool bDisableMe)
{
	for( int i = 0; i < polyFWs.size(); i++)
	{
		polyFWs[i]->disable();
	}
	
	if(bDisableMe) bEnabled = false;
	
}

void polyGroup::reEnableLast()
{
	bEnabled = true;
	if(selectedId < 0 ) return;
}

void polyGroup::draw()
{
	
	for( int i = 0; i < polyFWs.size(); i++)
	{
		
		
		if( i == selectedId )
		{
			ofRectangle rect = polyFWs[i]->getBoundingBox();
			ofNoFill();
			ofSetColor(100,100,100,200);
			ofRect( rect.x-4,rect.y-4,rect.width+8, rect.height+8);
		}
		
		polyFWs[i]->draw();
		
	}
	
}										

void polyGroup::keyPressed(ofKeyEventArgs& event)
{
	if( !bEnabled ) return;
	if( event.key == OF_KEY_BACKSPACE )
	{
		if( polyFWs.size() > selectedId )
		{
			polyFWs.erase(polyFWs.begin()+selectedId);
			selectedId = polyFWs.size()-1;
		}
	}
	
}

void polyGroup::keyReleased(ofKeyEventArgs& event)
{
	if( !bEnabled ) return;

}

void polyGroup::mouseMoved(ofMouseEventArgs& event)
{
	if( !bEnabled ) return;

}

void polyGroup::mouseDragged(ofMouseEventArgs& event)
{
	if( !bEnabled ) return;

}

void polyGroup::mousePressed(ofMouseEventArgs& event)
{
		
	if( !bEnabled ) return;
	
	disableAll();
	
	//--- check shift key
	int modifier = glutGetModifiers();
		
	//--- if left-click and no modifiers, create new, add points. if r-click choose the closest if inside boundingbox to move it
	if( event.button == 0 && modifier != GLUT_ACTIVE_SHIFT && modifier != GLUT_ACTIVE_CTRL )
	{
			if( selectedId == -1 )
			{
				// make new if we have none
				addPoly();
				ofPoint m = polyFWs[ selectedId ]->getMouseAltered(ofPoint(event.x,event.y));
				polyFWs[ selectedId ]->addPoint( ofPoint(m.x,m.y) );
				
			}else if( polyFWs.size() > selectedId )
			{
				if( polyFWs[ selectedId ]->pts.size() < 2 ) 
				{
					ofPoint m = polyFWs[ selectedId ]->getMouseAltered(ofPoint(event.x,event.y));
					polyFWs[ selectedId ]->addPoint( ofPoint(m.x,m.y) );// add pt
				}else{
					
					addPoly(); // make new if last is finished
					ofPoint m = polyFWs[ selectedId ]->getMouseAltered(ofPoint(event.x,event.y));
					polyFWs[ selectedId ]->addPoint( ofPoint(m.x,m.y) );
				}
			}
		
			if(polyFWs.size() > selectedId) polyFWs[ selectedId ]->enable();
		
	}else{
				
		// reset selectedId to none but remember original to restore after
		int lastId = selectedId;
		selectedId = -1;
		float cDist = 0;
		
		// find closest and set as selected
		for( int i = 0; i < polyFWs.size(); i++)
		{
			ofPoint m = polyFWs[i]->getMouseAltered( ofPoint(event.x,event.y) );
			ofRectangle boundingbox = polyFWs[i]->getBoundingBox();
			
			// make bounding area bigger so we can select more easily
			boundingbox.x -= 4;
			boundingbox.y -= 4;
			boundingbox.width	+= 8;
			boundingbox.height	+= 8;
			
			// dist to point
			bool bCloseToCenter = false;
			bool bCloseToEndPt  = false;
			
			// if we are editing points, also check if we are close to point (but out of bounding box ok)
			if( modifier == GLUT_ACTIVE_SHIFT )
			{
				for( int j = 0; j < polyFWs[i]->pts.size() ; j++)
				{
					if( abs(m.x-polyFWs[i]->pts[j].x) < polyFWs[i]->selectDist && 
					    abs(m.y-polyFWs[i]->pts[j].y) < polyFWs[i]->selectDist ) 
						bCloseToEndPt = true;
				}
			}
			
			// check dist to centroid in case bounding very small
			ofPoint c = polyFWs[i]->getCentroid();
			
			for( int j = 0; j < polyFWs[i]->pts.size(); j++)
			{
				
				if( abs(m.x-c.x) < polyFWs[i]->selectDist && 
				    abs(m.y-c.y) < polyFWs[i]->selectDist ) 
					 bCloseToCenter = true;
				
			}	
			
			// if meet requirements see if we are closer to this than last and remember
			if( isInsideRect(m.x, m.y, boundingbox) || bCloseToEndPt || bCloseToCenter)
			{
				float distSq = ( (m.x-c.x)*(m.x-c.x) + (m.y-c.y)*(m.y-c.y) );
				if( selectedId == -1 || distSq < cDist )
				{
					selectedId = i;
					cDist = distSq;
				}
			}
			
		}
		
		// pick the closest
		if( selectedId >= 0 && polyFWs.size() > selectedId )
		{
			polyFWs[ selectedId ]->enable();
			polyFWs[ selectedId ]->mousePressed(event);
		}
		
		// if nobody is close, keep previous
		if(selectedId == -1) selectedId = lastId;
	}
	
	
}

void polyGroup::mouseReleased(ofMouseEventArgs& event)
{
	if( !bEnabled ) return;
}

void polyGroup::setScale( float s )
{
	scale = s;
	for( int i = 0; i < polyFWs.size(); i++)
	{
		polyFWs[i]->setScale(s);
	}
	
}

void polyGroup::setOffset( ofPoint preR, ofPoint pstR )
{
	transPreRotate = preR;
	transPstRotate = pstR;
	
	for( int i = 0; i < polyFWs.size(); i++)
	{
		polyFWs[i]->setOffset(preR,pstR);
	}
	
}

void polyGroup::setGRotation( float r )
{
	gRotation = r;
	for( int i = 0; i < polyFWs.size(); i++)
	{
		polyFWs[i]->setGRotation(r);
	}
	
}

void  polyGroup::setWidth( float w, bool bAll )
{
	if( bAll )
	{
		for( int i = 0; i < polyFWs.size(); i++)
			polyFWs[i]->setWidth(w);
	}
	
	cFWidth = w;
	
	if(selectedId >= 0 && polyFWs.size() > selectedId ) polyFWs[selectedId]->setWidth(cFWidth);
}

void  polyGroup::setSquare( bool bSq, bool bAll )
{
	if( bAll )
	{
		for( int i = 0; i < polyFWs.size(); i++)
			polyFWs[i]->setSquare(bSq);
	}
	
	if(selectedId >= 0 && polyFWs.size() > selectedId ) polyFWs[selectedId]->setSquare(bSq);

	bFWSquare = bSq;
}

void  polyGroup::toggleSquare()
{
	if(selectedId >= 0 && polyFWs.size() > selectedId ){
		bool bSq = !polyFWs[selectedId]->bSquare;
		polyFWs[selectedId]->setSquare(bSq);
	}
	
}

void polyGroup::setStraight()
{
	if(selectedId >= 0 && polyFWs.size() > selectedId ){
		polyFWs[selectedId]->setStraight();
	}
}

void polyGroup::setFillColor(int r, int g, int b, int a)
{
	fillColor.r = r;
	fillColor.g = g;
	fillColor.b = b;
	fillColor.a = a;
}

void polyGroup::setStrokeColor(int r, int g, int b, int a)
{
	strokeColor.r = r;
	strokeColor.g = g;
	strokeColor.b = b;
	strokeColor.a = a;
}

