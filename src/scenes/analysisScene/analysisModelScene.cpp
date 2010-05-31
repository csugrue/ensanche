/*
 *  analysisModelScene.cpp
 *  ensanche_2.0
 *
 *  Created by Chris on 3/9/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

#include "analysisModelScene.h"

void AnalysisModelScene::setup()
{
	statusMessage	= "none";
	bPanelOn		= true;
	bSetUserName	= false;
	bEnabled		= false;
	
	modelBarrio.setup();
	modelBarrio.enable();
	
	setupControlPanel();
	
}

AnalysisModelScene::~AnalysisModelScene()
{
}

void AnalysisModelScene::update()
{
	if(bPanelOn) updateControlPanel();
	
	//if( panel.getSelectedPanelName() == "" ) 
	modelBarrio.enable();
}

void AnalysisModelScene::disable()
{
	bEnabled = false;
	modelBarrio.disable();
}

void AnalysisModelScene::enable()
{
	bEnabled = true;
}

void AnalysisModelScene::preDraw()
{
}

void AnalysisModelScene::draw()
{
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	
	
	//------ 3d model
	ofEnableAlphaBlending();
	glEnable(GL_DEPTH_TEST);
	
	// camera view 3d model
	//glPolygonMode(GL_FRONT, GL_FILL);
	ofNoFill();
	glColor4f(1,1,1,1.f);
	
	glPushMatrix();
	modelBarrio.drawWithCamera();
	glPopMatrix();
	
	glDisable(GL_DEPTH_TEST);
	
	//------ 2d model
	glPushMatrix();
	
	glTranslatef(10,400,0);
	glScalef(4,4,1);
	
	ofNoFill();
	ofSetColor(255,0,0);
	modelBarrio.draw2D();
	ofSetColor(255,0,0);
	ofCircle(modelBarrio.pov.x, modelBarrio.pov.z, 1.f);
	
	glPopMatrix();
	
	
	// control panel
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(bPanelOn) panel.draw();
}

void AnalysisModelScene::keyPressed(int key)
{
	
}

void AnalysisModelScene::keyReleased(int key)
{
	
}

void AnalysisModelScene::mouseMoved(int x, int y)
{
	
}

void AnalysisModelScene::mouseDragged(int x, int y, int button)
{
	if(bPanelOn) panel.mouseDragged(x,y,button);
	if( panel.isMouseInPanel(x, y)  )
	{
		modelBarrio.disable();
	}
}

void AnalysisModelScene::mousePressed(int x, int y, int button)
{
	if(bPanelOn) panel.mousePressed(x,y,button);
	if( panel.isMouseInPanel(x, y) )
	{
		modelBarrio.disable();
	}
	
}

void AnalysisModelScene::mouseReleased(int x, int y, int button)
{
	if(bPanelOn) panel.mouseReleased();
	
}

string AnalysisModelScene::getStatusMessage()
{
	return statusMessage;
}

void AnalysisModelScene::setupControlPanel()
{
	
	panel.setup("expand", ofGetWidth()-260, ofGetHeight()-450, 240, 370);
	panel.addPanel("adjust pos",1, false);
	
	panel.setWhichPanel("adjust pos");
	panel.addSlider("pov x","POV_X",20.f,0.f,100.f,false);
	panel.addSlider("pov y","POV_Y",10.f,0.f,100.f,false);
	panel.addSlider("pov z","POV_Z",100.f,0.f,100.f,false);
	panel.update();
	
	
}

void AnalysisModelScene::updateControlPanel()
{
	
	panel.update();
	modelBarrio.pov.x = panel.getValueF("POV_X");
	modelBarrio.pov.y = panel.getValueF("POV_Y");
	modelBarrio.pov.z = panel.getValueF("POV_Z");
	
}


void AnalysisModelScene::setUserName( string name )
{
	if( name.length() > 0 )
	{
		username = name;
		bSetUserName = true;
	}
}

void AnalysisModelScene::loadUserFile()
{	
	if( bSetUserName )
	{
		//EnsancheBarrio	barrioOriginal;
		
		// load model
		string dir = USER_DIRECTORY;
		string filename = dir+username+"/"+username+XML_FILE_FACADE;
		
		cout << "---------------------" << endl;
		cout << "Analysis Model Scene: loading " << filename << endl;
		
		//barrioOriginal.loadFromXml(filename);
		modelBarrio.clear();
		modelBarrio.loadFromXml(filename);
		
		cout << "---------------------" << endl;
		
		//if( barrioOriginal.buildings.size() > 0 )
		//{
			
			//modelBarrio.clear();
			//modelBarrio.addBuildings(&barrioOriginal);
		//}
	}
	
	
}
