/*
 *  ensancheLabelGroup.cpp
 *  ensanche_2.0
 *
 *  Created by Chris on 3/3/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

#include "ensancheLabelGroup.h"

EnsancheLabelGroup::EnsancheLabelGroup()
{
	selectedId	= -1;
	scale			= 1;
	gRotation		= 0;
	transPreRotate.set(0,0);
	transPstRotate.set(0,0); 
	bEnabled = false;
	cLabel = "";
	cID= 0;
	
}

EnsancheLabelGroup::~EnsancheLabelGroup()
{
	for( int i = 0; i < labels.size(); i++)
	{
		delete labels[i];
	}
	ofRemoveListener(ofEvents.mousePressed, this, &EnsancheLabelGroup::mousePressed);
	ofRemoveListener(ofEvents.mouseReleased, this, &EnsancheLabelGroup::mouseReleased);
	ofRemoveListener(ofEvents.mouseDragged, this, &EnsancheLabelGroup::mouseDragged);
	ofRemoveListener(ofEvents.keyPressed, this, &EnsancheLabelGroup::keyPressed);
	ofRemoveListener(ofEvents.keyReleased, this, &EnsancheLabelGroup::keyReleased);
}

void EnsancheLabelGroup::setup()
{
	ofAddListener(ofEvents.mousePressed, this, &EnsancheLabelGroup::mousePressed);
	ofAddListener(ofEvents.mouseReleased, this, &EnsancheLabelGroup::mouseReleased);
	ofAddListener(ofEvents.mouseDragged, this, &EnsancheLabelGroup::mouseDragged);
	ofAddListener(ofEvents.keyPressed, this, &EnsancheLabelGroup::keyPressed);
	ofAddListener(ofEvents.keyReleased, this, &EnsancheLabelGroup::keyReleased);
	
	font.loadFont("fonts/verdana.ttf", 8,true,true);
	font.setLineHeight(8+2);
	
}

void EnsancheLabelGroup::clear()
{
	
	
	//for (vector <EnsancheRoomLabel*> mlabel = labels.begin();mlabel != labels.end();mlabel++)
		//delete *mlabel;
	for (int cntr = 0; cntr < labels.size(); cntr++)
		delete labels[ cntr ];
	labels.clear();
	
	selectedId = -1;
	bEnabled = false;
	
}


void EnsancheLabelGroup::addLabel()
{
	
	//if( !bEnabled ) return;
	
	labels.push_back( new EnsancheRoomLabel() );
	selectedId	= labels.size()-1;
	
	labels[selectedId]->setOffset(transPreRotate,transPstRotate);
	labels[selectedId]->setScale(scale);
	labels[selectedId]->setGRotation(gRotation);
	labels[selectedId]->setup();
	labels[selectedId]->enable();
	labels[selectedId]->setFont(&font);
	labels[selectedId]->setLabel(cLabel,cID);
	
}

void EnsancheLabelGroup::addLabel(int typeID, string name)
{
	
	addLabel();
	labels[selectedId]->setLabel(name,typeID);
	
}

void EnsancheLabelGroup::disableAll(bool bDisableMe)
{
	for( int i = 0; i < labels.size(); i++)
	{
		labels[i]->disable();
	}
	
	if(bDisableMe) bEnabled = false;
	
}

void EnsancheLabelGroup::reEnableLast()
{
	bEnabled = true;
	if(selectedId < 0 ) return;
}

void EnsancheLabelGroup::draw()
{
	
	for( int i = 0; i < labels.size(); i++)
	{
		
		
		if( i == selectedId )
		{
			ofNoFill();
			ofSetColor(100,100,100,200);
			
			ofPoint c = labels[i]->getCentroid();
			ofCircle(c.x, c.y, 4);
			
			ofRectangle rect = labels[i]->getBoundingBox();
			ofRect(rect.x,rect.y,rect.width,rect.height);
		}
		
		if( i == selectedId && bEnabled ) ofSetColor(255,0,0,255);
		else ofSetColor(100,100,100,255);
		
		labels[i]->draw();
		
	}
	
}										

void EnsancheLabelGroup::keyPressed(ofKeyEventArgs& event)
{
	if( !bEnabled ) return;
	if( event.key == OF_KEY_BACKSPACE )
	{
		if( labels.size() > selectedId )
		{
			labels.erase(labels.begin()+selectedId);
			selectedId = labels.size()-1;
		}
	}
	
}

void EnsancheLabelGroup::keyReleased(ofKeyEventArgs& event)
{
	if( !bEnabled ) return;
	
}

void EnsancheLabelGroup::mouseMoved(ofMouseEventArgs& event)
{
	if( !bEnabled ) return;
	
}

void EnsancheLabelGroup::mouseDragged(ofMouseEventArgs& event)
{
	if( !bEnabled ) return;
	
}

void EnsancheLabelGroup::mousePressed(ofMouseEventArgs& event)
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
			addLabel();
			ofPoint m = labels[ selectedId ]->getMouseAltered(ofPoint(event.x,event.y));
			labels[ selectedId ]->addPoint( ofPoint(m.x,m.y) );
			
		}else if( labels.size() > selectedId )
		{
			if( labels[ selectedId ]->pts.size() == 0 ) 
			{
				ofPoint m = labels[ selectedId ]->getMouseAltered(ofPoint(event.x,event.y));
				labels[ selectedId ]->addPoint( ofPoint(m.x,m.y) );// add pt
			}else{
				
				addLabel(); // make new if last is finished
				ofPoint m = labels[ selectedId ]->getMouseAltered(ofPoint(event.x,event.y));
				labels[ selectedId ]->addPoint( ofPoint(m.x,m.y) );
			}
		}
		
		if(labels.size() > selectedId) labels[ selectedId ]->enable();
		
	}else{
		
		// reset selectedId to none but remember original to restore after
		int lastId = selectedId;
		selectedId = -1;
		float cDist = 0;
		
		bool bCloseToPt = false;
		
		// find closest and set as selected
		for( int i = 0; i < labels.size(); i++)
		{
			ofPoint m = labels[i]->getMouseAltered( ofPoint(event.x,event.y) );
			ofPoint c;
			
			if( labels[i]->pts.size() < 1 ) continue;
			else c.set(labels[i]->pts[0].x,labels[i]->pts[0].y);
			
			for( int j = 0; j < labels[i]->pts.size() ; j++)
			{
				if( abs(m.x-labels[i]->pts[j].x) < labels[i]->selectDist && 
					abs(m.y-labels[i]->pts[j].y) < labels[i]->selectDist ) 
					
					bCloseToPt = true;
			}
			
			ofRectangle boundingbox = labels[i]->getBoundingBox();
			
			// if meet requirements see if we are closer to this than last and remember
			if( isInsideRect(m.x, m.y, boundingbox) || bCloseToPt )
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
		if( selectedId >= 0 && labels.size() > selectedId )
		{
			labels[ selectedId ]->enable();
			labels[ selectedId ]->mousePressed(event);
		}
		
		// if nobody is close, keep previous
		if(selectedId == -1) selectedId = lastId;
	}
	
	
}

void EnsancheLabelGroup::mouseReleased(ofMouseEventArgs& event)
{
	if( !bEnabled ) return;
}

void EnsancheLabelGroup::setLabel( string name, int id )
{
	if( selectedId < labels.size() )
	{
		//labels[selectedId]->setLabel(name,id);
	}
	
	cLabel = name;
	cID = id;
}

void EnsancheLabelGroup::setScale( float s )
{
	scale = s;
	for( int i = 0; i < labels.size(); i++)
	{
		labels[i]->setScale(s);
	}
	
}

void EnsancheLabelGroup::setOffset( ofPoint preR, ofPoint pstR )
{
	transPreRotate = preR;
	transPstRotate = pstR;
	
	for( int i = 0; i < labels.size(); i++)
	{
		labels[i]->setOffset(preR,pstR);
	}
	
}

void EnsancheLabelGroup::setGRotation( float r )
{
	gRotation = r;
	for( int i = 0; i < labels.size(); i++)
	{
		labels[i]->setGRotation(r);
	}
	
}

int EnsancheLabelGroup::getSelectedId()
{
	if( selectedId < labels.size() )
	{
		return labels[selectedId]->labelID;
	}
	return 0;
}

