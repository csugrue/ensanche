#include "userComboScene.h"

void userComboScene::setup()
{
	statusMessage	= "none";
	bPanelOn		= true;
	
	setupControlPanel();
	
}

userComboScene::~userComboScene()
{
}

void userComboScene::update()
{
	if(bPanelOn) updateControlPanel();
}


void userComboScene::preDraw()
{
}

void userComboScene::draw()
{
	if(bPanelOn) panel.draw();
}

void userComboScene::keyPressed(int key)
{
	
}

void userComboScene::keyReleased(int key)
{

}

void userComboScene::mouseMoved(int x, int y)
{

}

void userComboScene::mouseDragged(int x, int y, int button)
{
	if(bPanelOn) panel.mouseDragged(x,y,button);
}

void userComboScene::mousePressed(int x, int y, int button)
{
	if(bPanelOn) panel.mousePressed(x,y,button);

}

void userComboScene::mouseReleased(int x, int y, int button)
{
	if(bPanelOn) panel.mouseReleased();

}

string userComboScene::getStatusMessage()
{
	return statusMessage;
}

void userComboScene::setupControlPanel()
{
	
	//panel.setup("user", 250, 10, 530, 270);
	//panel.update();
	
	
}

void userComboScene::updateControlPanel()
{

	panel.update();
	
		
}
