#include "sceneUserInput.h"

#ifdef TARGET_OSX
#include <sys/stat.h>
#endif


void sceneUserInput::setup()
{
	userDir			= USER_DIRECTORY;//"userData";
	
	statusMessage	= "none";
	bPanelOn		= true;
	
	setupControlPanel();
	
	// set up sub scenes
	userScenes[ USER_SUB_MAP ] = new userMapScene();
	userScenes[ USER_SUB_INT ] = new userInteriorScene();
	userScenes[ USER_SUB_COM ] = new userComboScene();
	
	for( int i = 0; i < USER_NUM_SUB_SCENES; i++)
		userScenes[i]->setup();
	
	currentSubScene = -1;
	lastPanelOn = panel.getSelectedPanelName();
	
	nSavedFPlans = 0;
	
}

sceneUserInput::~sceneUserInput()
{
	for( int i = 0; i < USER_NUM_SUB_SCENES; i++)
	{
	delete userScenes[i];
	}
	
}

void sceneUserInput::resetScene()
{
}

void sceneUserInput::update()
{
	if(bPanelOn) updateControlPanel();
	if( currentSubScene >= 0 ) userScenes[currentSubScene]->update();
	
	currentSubScene = getSubSceneId( panel.getSelectedPanelName() );
	
	/*if(			panel.getSelectedPanelName() == "google map") currentSubScene = USER_SUB_MAP;
	else if (	panel.getSelectedPanelName() == "interior") currentSubScene = USER_SUB_INT;
	else if(	panel.getSelectedPanelName() == "combo") currentSubScene = USER_SUB_COM;
	else		currentSubScene = -1;*/
			
}

int  sceneUserInput::getSubSceneId( string panelName)
{
	if(			panelName == "google map")	return USER_SUB_MAP;
	else if (	panelName == "interior")	return USER_SUB_INT;
	else if(	panelName == "combo")		return USER_SUB_COM;
	else		return -1;
	
}


void sceneUserInput::preDraw()
{
}

void sceneUserInput::draw()
{
	ofBackground(200,200,200);
	if( currentSubScene >= 0 ) userScenes[currentSubScene]->draw();
	if(bPanelOn) panel.draw();

}

void sceneUserInput::keyPressed(int key)
{
	if( panel.isAnyTextBoxActive() ){
		if( key == OF_KEY_BACKSPACE) panel.deleteLastChar();
		else{
			panel.addChar(key);
		}
		return;
	}
	
	if( currentSubScene >= 0 ) userScenes[currentSubScene]->keyPressed(key);
	if( currentSubScene >= 0 &&  !userScenes[currentSubScene]->panel.isAnyTextBoxActive() ){
		if( key == 's'  ) userScenes[currentSubScene]->saveUserFile();
		if( key == '!' && currentSubScene == USER_SUB_INT ) userScenes[currentSubScene]->loadUserFile();
		if( key == '@' && currentSubScene == USER_SUB_INT ) userScenes[currentSubScene]->saveUserFile();
	}
	
	if( key == OF_KEY_F1) bPanelOn = !bPanelOn;

}

void sceneUserInput::keyReleased(int key)
{
	if( currentSubScene >= 0 ) userScenes[currentSubScene]->keyReleased(key);
}

void sceneUserInput::mouseMoved(int x, int y)
{
	if(bPanelOn && panel.isMouseInPanel(x,y) )
	{
		for( int i = 0; i < USER_NUM_SUB_SCENES; i++)
			userScenes[i]->disable();
	}else{
	
		if( currentSubScene >= 0 ) userScenes[currentSubScene]->mouseMoved(x,y);
	}
}

void sceneUserInput::mouseDragged(int x, int y, int button)
{
	if(bPanelOn) panel.mouseDragged(x,y,button);
	
	if(bPanelOn && panel.isMouseInPanel(x,y) )
	{
		for( int i = 0; i < USER_NUM_SUB_SCENES; i++)
			userScenes[i]->disable();
	}else{

		if( currentSubScene >= 0 ) userScenes[currentSubScene]->mouseDragged(x,y,button);
	}
}

void sceneUserInput::mousePressed(int x, int y, int button)
{
	
	if(bPanelOn) panel.mousePressed(x,y,button);
	
	if(bPanelOn && panel.isMouseInPanel(x,y) )
	{
		for( int i = 0; i < USER_NUM_SUB_SCENES; i++)
			userScenes[i]->disable();
	}
	
	
	if( currentSubScene >= 0 ) userScenes[currentSubScene]->mousePressed(x,y,button);

}

void sceneUserInput::mouseReleased(int x, int y, int button)
{
	if(bPanelOn) panel.mouseReleased();
	if( currentSubScene >= 0 ) userScenes[currentSubScene]->mouseReleased(x,y,button);
	

	
}

string sceneUserInput::getStatusMessage()
{
	return statusMessage;
}

void sceneUserInput::setupControlPanel()
{
	
	panel.setup("user", 250, 10, 530, 400);

	panel.addPanel("user data", 1, false);
	panel.addPanel("google map", 1, false);
	panel.addPanel("interior", 1, false);
	panel.addPanel("combo", 1, false);
	
	panel.setWhichPanel("user data");
	panel.addTextInput("username", "username", "", 200, 15);
	panel.addTextInput("address", "address", "", 480, 15);
	panel.addToggle("find address", "find_address", false);
	
	updateSavedUserList();
	savedUserLister = panel.addFileLister("saved user data", &lister, 400, 100);
	
	panel.setWhichPanel("interior");
	
	updateSavedFloorPlanList();
	savedFloorPlansLister = panel.addFileLister("saved floor plans", &fpLister, 400, 100);
	
	panel.update();
	
	
}

void sceneUserInput::updateControlPanel()
{
	
	
	panel.update();
	
	
	if( lastPanelOn != panel.getSelectedPanelName() )
	{
		changeScene();
		lastPanelOn = panel.getSelectedPanelName();
	}
	
	
	if( panel.getValueB("find_address") )
	{
		panel.setValueB("find_address", false);
		triggerEvent("find_address");

	}
	
	//--- loading floor plan
	if( panel.getSelectedPanelName() == "interior" )
	{
		if( savedFloorPlansLister->lister->selectedHasChanged() )
		{
			int which = savedFloorPlansLister->selection;
			string floorPlanFile = savedFloorPlansLister->lister->getName( which );
			savedFloorPlansLister->lister->clearChangedFlag();
			
			// load floorplan
			( (userInteriorScene*)userScenes[USER_SUB_INT])->loadFloorPlanImage( FLOOR_PLAN_DIRECTORY+floorPlanFile);
			panel.setMinimized(true);
			
		}
		
	}
	

	//--- user file selection
	if( savedUserLister->lister->selectedHasChanged() )
	{
		int which = savedUserLister->selection;
		string newUser = savedUserLister->lister->getName( which );
		savedUserLister->lister->clearChangedFlag();
		loadUserData( newUser );
		
		
		for( int i = 0; i < USER_NUM_SUB_SCENES; i++) 
			userScenes[i]->setUserName( panel.getValueS("username") );
		
		cout << "Loading map data for " << username << endl;
		userScenes[USER_SUB_MAP]->loadUserFile();
		
		cout << "Loading interior data for " << username << endl;
		userScenes[USER_SUB_INT]->loadUserFile();
		
		setUserName(newUser);

		//cout << "New user file: " << newUser << endl;
	}
	
	
	// keep current username updated
	for( int i = 0; i < USER_NUM_SUB_SCENES; i++) 
		userScenes[i]->setUserName( panel.getValueS("username") );
	
}

void sceneUserInput::changeScene()
{
	// panel dimensions
	if( panel.getSelectedPanelName() == "google map")		panel.setDimensions(530, 75);
	else if( panel.getSelectedPanelName() == "user data")	panel.setDimensions(530, 400);
	else panel.setDimensions(530, 270);
	
	if( lastPanelOn == "user data" ) saveUserData();
	if( lastPanelOn == "google map" ){
		userScenes[USER_SUB_MAP]->saveUserFile();
		//( (userMapScene*)userScenes[USER_SUB_MAP])->saveUserMapData();
	}
	
	//if( panel.getSelectedPanelName() == "google map") panel.setMinimized(true);
	
	if( panel.getSelectedPanelName() == "google map")
		userScenes[USER_SUB_MAP]->bPanelOn = true;
	else
		userScenes[USER_SUB_MAP]->bPanelOn = false;
}

void  sceneUserInput::triggerEvent(string event)
{
	if( event == "find_address" )
	{
		// search google maps
		string myaddress = panel.getValueS("address");
		cout << "myaddress " << myaddress << endl;
		
		( (userMapScene*)userScenes[USER_SUB_MAP])->findMap( myaddress );
		
		// set to google panel
		panel.setSelectedPanel("google map");
	
	}else if( event == "open_map")
	{
		// open files
		panel.setSelectedPanel("saved map");
	
	}else if( event == "load_floor_plan")
	{
		( (userInteriorScene*)userScenes[USER_SUB_INT])->loadFloorPlanImage("");
		
		panel.setMinimized(true);

	}else if( event == "new panel" )
	{
		changeScene();

	}
	
	
}

void sceneUserInput::saveUserData()
{
	// check if we have a user name
	//string 
	username = panel.getValueS("username");
	
	if( username == "none" ) username = getTimeString();
	else
	{
		cout << "username " << username << " " << username.size() << endl;
		// remove any spaces
	}
	
	panel.setValueS("username",username);
	
	// check if user directory exists
	bool bDirExists = pocoLister.existsDirectory( ofToDataPath(userDir), username );
	
	string myDir = userDir + username;
	
	// if not create it
	if( !bDirExists )
	{
		// Note: works for mac
		mkdir(ofToDataPath(myDir).c_str(),0777);
	}
	
	// save data
	ofxXmlSettings xml;
	
	xml.addTag("ensanche");
	xml.pushTag("ensanche");
	
		xml.addTag("client");
		xml.pushTag("client");
			xml.setValue("username", username);
			xml.setValue("address", panel.getValueS("address") );
			xml.setValue("date", getTimeString() );
		xml.popTag();
	
	xml.popTag();
	
	myDir += "/";
	xml.saveFile(myDir+username+"_userData.xml");
	
						 
	/*( (userMapScene*) userScenes[USER_SUB_MAP])->userDir.clear();
	( (userMapScene*) userScenes[USER_SUB_MAP])->userDir = myDir;
	( (userMapScene*) userScenes[USER_SUB_MAP])->username.clear();
	( (userMapScene*) userScenes[USER_SUB_MAP])->username = username;*/
	userScenes[USER_SUB_MAP]->setUserName( username );
	userScenes[USER_SUB_MAP]->setUserDir( myDir );
	
}

void sceneUserInput::updateSavedFloorPlanList()
{
	// check in floor plan folder
	fpLister.entries.clear();
	nSavedFPlans = fpLister.listDir(FLOOR_PLAN_DIRECTORY);
	
}

void sceneUserInput::updateSavedUserList()
{
	// get list of all folders in userData
	savedUserFolders.clear();
	userData.clear();
	
	pocoLister.setPath( ofToDataPath("userData/", true) );
	pocoLister.findSubDirectories( savedUserFolders );
	
	ofxXmlSettings xml;
	if( savedUserFolders.size() > 0 )
	{
		entry newUserEntry;
		lister.entries.clear();
		
		
		for( int i = 0; i < savedUserFolders.size(); i++)
		{
			//cout << "userData/"+savedUserFolders[i]+"/"+savedUserFolders[i]+"_userData.xml" << endl;
			
			newUserEntry.filename = savedUserFolders[i];
			newUserEntry.fullpath = ofToDataPath("userData/"+savedUserFolders[i]+"_userData.xml",true);
			lister.entries.push_back(newUserEntry);
			
			userFileData myData;
			
			// open client xml file
			xml.loadFile( "userData/"+savedUserFolders[i]+"/"+savedUserFolders[i]+"_userData.xml");
			xml.pushTag("ensanche");
				xml.pushTag("client");
				myData.username = xml.getValue("username", "none");
				myData.address = xml.getValue("address", "none");
				userData.push_back(myData);
				xml.popTag();
			xml.popTag();
			
			//cout << "xml uesrname " << myData.username << endl;
		}
		
		
	}
	
}


bool sceneUserInput::loadUserData( string userFolderName, int level )
{
	string path = "userData/"+userFolderName+"/";
	
	// open client data
	ofxXmlSettings xml;
	xml.loadFile(path+userFolderName+"_userData.xml");
	
	xml.pushTag("ensanche");
		xml.pushTag("client");
		panel.setValueS( "username", xml.getValue("username","none") );
		panel.setValueS("address", xml.getValue("address", ""));
		xml.popTag();
	xml.popTag();
	
	if( level == LOAD_DATA_MAP )
	{
		// check that we have all the tiles
		pocoLister.setPath(ofToDataPath(path,true));
		pocoLister.setExtensionToLookFor("jpg");
		int numFiles = pocoLister.getNumberOfFiles();
		
		// load tiles
		if( numFiles == GOOGLE_MAP_T_TILES )
		{
			( (userMapScene*) userScenes[USER_SUB_MAP])->loadTileFromFolder(path);
			path = path+panel.getValueS("username")+XML_FILE_MAP;
			//( (userMapScene*) userScenes[USER_SUB_MAP])->loadMapData(path);
			return true;
		}
		else{
			cout << "num files is not enough " << numFiles << endl;
			return false;
		}
	}
	
	return true;
}

