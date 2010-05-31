/*
 *  ensancheBuildingGroup.cpp
 *  ensanche_2.0
 *
 *  Created by Chris on 3/4/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

#include "ensancheBuildingGroup.h"

EnsancheBuildingGroup::EnsancheBuildingGroup()
{
	bDrawData = true;
}

EnsancheBuildingGroup::~EnsancheBuildingGroup()
{
	for( int i = 0; i< buildings.size(); i++)
		delete buildings[i];
	
	ofRemoveListener(ofEvents.mousePressed, this, &EnsancheBuildingGroup::mousePressed);
	ofRemoveListener(ofEvents.mouseReleased, this, &EnsancheBuildingGroup::mouseReleased);
	ofRemoveListener(ofEvents.mouseDragged, this, &EnsancheBuildingGroup::mouseDragged);
	ofRemoveListener(ofEvents.keyPressed, this, &EnsancheBuildingGroup::keyPressed);
	ofRemoveListener(ofEvents.keyReleased, this, &EnsancheBuildingGroup::keyReleased);
}

void EnsancheBuildingGroup::clear()
{
	for( int i = 0; i< buildings.size(); i++)
		buildings[i]->clear();
	
	for( int i = 0; i< buildings.size(); i++)
		delete buildings[i];
	
	buildings.clear();
}

void EnsancheBuildingGroup::setup()
{
	selectedId = -1;
	ofAddListener(ofEvents.mousePressed, this, &EnsancheBuildingGroup::mousePressed);
	ofAddListener(ofEvents.mouseReleased, this, &EnsancheBuildingGroup::mouseReleased);
	ofAddListener(ofEvents.mouseDragged, this, &EnsancheBuildingGroup::mouseDragged);
	ofAddListener(ofEvents.keyPressed, this, &EnsancheBuildingGroup::keyPressed);
	ofAddListener(ofEvents.keyReleased, this, &EnsancheBuildingGroup::keyReleased);
	
}

/*void EnsancheBuildingGroup::clear()
{
	for( int i = 0; i< buildings.size(); i++)
		delete buildings[i];
	
	buildings.clear();
	
}*/

void EnsancheBuildingGroup::draw()
{
	for( int i = 0; i < buildings.size(); i++)
	{
		ofFill();
		
		int alpha = ( i == selectedId ) ? 120 : 100;
		//ofSetColor(255,255,255,alpha);		
		
		if( buildings[i]->type == EN_BUILDING_ME )  ofSetColor(255,255,255,alpha);
		else										ofSetColor(255,180,180,alpha);
		
		//buildings[i]->bClosed = false;
		//buildings[i]->polySimple::draw();
		buildings[i]->polyEditable::draw(false);
		
		ofNoFill();
		
		if( i != selectedId )	ofSetColor(255,255,255,255);
		else					ofSetColor(255,0,0,255);
		
		
		buildings[i]->polySimple::draw();
		if(bDrawData) buildings[i]->drawData();
	}
}

void EnsancheBuildingGroup::addBuilding()
{
	
	if( selectedId >= 0 && buildings.size() > selectedId ) buildings[ selectedId ]->disable();
	
	buildings.push_back( new EnsancheMapBuilding() );
	
	selectedId = buildings.size()-1;
	
	buildings[selectedId]->enable();
	buildings[selectedId]->setOffset(transPreRotate,transPstRotate);
	buildings[selectedId]->setScale(scale);
	buildings[selectedId]->setGRotation(gRotation);
	buildings[selectedId]->setup();
	
	if( buildings.size() == 1 ) setIsMe();
	
}

void EnsancheBuildingGroup::removeBuilding()
{
}

void EnsancheBuildingGroup::keyPressed(ofKeyEventArgs& event)
{
	if( !bEnabled ) return;
	if( event.key == OF_KEY_BACKSPACE )
	{
		if( buildings.size() > selectedId )
		{
			buildings.erase(buildings.begin()+selectedId);
			selectedId = buildings.size()-1;
		}
	}
	
}

void EnsancheBuildingGroup::keyReleased(ofKeyEventArgs& event)
{
}

void EnsancheBuildingGroup::mouseMoved(ofMouseEventArgs& event)
{
}

void EnsancheBuildingGroup::mouseDragged(ofMouseEventArgs& event)
{
}

void EnsancheBuildingGroup::mousePressed(ofMouseEventArgs& event)
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
			addBuilding();
			ofPoint m = buildings[ selectedId ]->getMouseAltered(ofPoint(event.x,event.y));
			buildings[ selectedId ]->addPoint( ofPoint(m.x,m.y) );
			
		}else if( buildings.size() > selectedId )
		{
			if( !buildings[ selectedId ]->bClosed ) 
			{
				//ofPoint m = buildings[ selectedId ]->getMouseAltered(ofPoint(event.x,event.y));
				//buildings[ selectedId ]->addPoint( ofPoint(m.x,m.y) );// add pt
			}else{
				
				addBuilding(); // make new if last is finished
				ofPoint m = buildings[ selectedId ]->getMouseAltered(ofPoint(event.x,event.y));
				buildings[ selectedId ]->addPoint( ofPoint(m.x,m.y) );
			}
		}
		
		if(buildings.size() > selectedId) buildings[ selectedId ]->enable();
		
	}else{
		
		// reset selectedId to none but remember original to restore after
		int lastId = selectedId;
		selectedId = -1;
		float cDist = 0;
		
		// find closest and set as selected
		for( int i = 0; i < buildings.size(); i++)
		{
			ofPoint m = buildings[i]->getMouseAltered( ofPoint(event.x,event.y) );
			ofRectangle boundingbox = buildings[i]->getBoundingBox();
			
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
				for( int j = 0; j < buildings[i]->pts.size() ; j++)
				{
					if( abs(m.x-buildings[i]->pts[j].x) < buildings[i]->selectDist && 
					   abs(m.y-buildings[i]->pts[j].y) < buildings[i]->selectDist ) 
						bCloseToEndPt = true;
				}
			}
			
			// check dist to centroid in case bounding very small
			ofPoint c = buildings[i]->getCentroid();
			
			for( int j = 0; j < buildings[i]->pts.size(); j++)
			{
				
				if( abs(m.x-c.x) < buildings[i]->selectDist && 
				   abs(m.y-c.y) < buildings[i]->selectDist ) 
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
		if( selectedId >= 0 && buildings.size() > selectedId )
		{
			buildings[ selectedId ]->enable();
			buildings[ selectedId ]->mousePressed(event);
		}
		
		// if nobody is close, keep previous
		if(selectedId == -1) selectedId = lastId;
	}
	
	
}

void EnsancheBuildingGroup::mouseReleased(ofMouseEventArgs& event)
{
}

void EnsancheBuildingGroup::disableAll(bool bDisableMe)
{
	for( int i = 0; i< buildings.size(); i++)
		buildings[i]->disable();
	
	if(bDisableMe) bEnabled = false;

		
}

void EnsancheBuildingGroup::reEnableLast()
{
	bEnabled = true;
}

void EnsancheBuildingGroup::setScale( float s )
{
	scale = s;
	for( int i = 0; i < buildings.size(); i++)
	{
		buildings[i]->setScale(s);
	}
	
}

void EnsancheBuildingGroup::setOffset( ofPoint preR, ofPoint pstR )
{
	transPreRotate = preR;
	transPstRotate = pstR;
	
	for( int i = 0; i < buildings.size(); i++)
	{
		buildings[i]->setOffset(preR,pstR);
	}
}

void EnsancheBuildingGroup::setGRotation( float r )
{
	gRotation = r;
	for( int i = 0; i < buildings.size(); i++)
	{
		buildings[i]->setGRotation(r);
	}
}


void EnsancheBuildingGroup::setIsMe()
{
	for( int i = 0; i < buildings.size(); i++)
	{
		if( buildings[i]->type == EN_BUILDING_ME ) buildings[i]->type = EN_BUILDING_NEIGHBOR;
	}
	 
	if( buildings.size() > selectedId ) buildings[selectedId]->type = EN_BUILDING_ME;
}

