#include "ofApp.h"
// OSCVideoplayer v0.1 ------- rychrd@thelimen.com 6/2019 -------------

// Plays videos based on a numerical index received over OSC
// Based on the ofxOMXPlayer demo of the ability to play multiple files with the Non-Texture Player
// Also accepts alpha values to fade up and down
 
// OSC Commands on port 54321:	/pi1/play [file index]
//				/pi1/fadeUp [time in secs]
//				/pi1/fadeDown [time in secs]


bool doLoadNextMovie = false;

void ofApp::onVideoEnd(ofxOMXPlayer* player)
{
    ofLog() << "onVideoEnd: " << player->isLoopingEnabled();
    doLoadNextMovie = false;
    bFadeDown = true;
    bFadeUp = false;
}

void ofApp::onVideoLoop(ofxOMXPlayer* player)
{
    
}

//--------------------------------------------------------------
void ofApp::setup()
{
    	ofSetLogLevel(OF_LOG_VERBOSE);
	ofHideCursor();    
	ofBackground(ofColor::black);

	ofLog() << "C++ Version" << __cplusplus ;	

//	omxPlayer.engine.m_config_audio.device = "omx:alsa";
//	omxPlayer.engine.m_config_audio.subdevice = "hw:1,0";

	oscReceiver.setup(PORT);
	ofLog() << "listening for OSC on port " << PORT;  

	loadVideoFiles();

	bFadeUp = true;
	bFadeDown = false;
	fadeRate = 1.0;
	alpha = 255.0;

}
//--------------------------------------------------------------
void ofApp::loadVideoFiles()
{
    	string directoryPath = ofToDataPath("video/", true); 	//this will let us just grab a video without recompiling
	ofDirectory currentVideoDirectory(directoryPath);

	if (currentVideoDirectory.exists()) 
	{
		currentVideoDirectory.listDir();
		currentVideoDirectory.sort();

		files = currentVideoDirectory.getFiles();

		if (files.size()>0) 
		{
			videoCounter = 0;
			settings.videoPath = files[videoCounter].path();
			settings.useHDMIForAudio = true;
			settings.enableLooping = false;
			settings.enableTexture = false;
			settings.autoStart = true;
			settings.listener = this;
			omxPlayer.setup(settings);
		}		
	} else

	{
        	ofLog() << directoryPath << "DOES NOT EXIST - does bin/data exist?";
	}
}
//--------------------------------------------------------------

void ofApp::loadNextMovie()
{
	if(videoCounter+1<files.size())
	{
		videoCounter++;
	}else
	{
		videoCounter = 0;
	}
	ofLog() << "LOADING MOVIE" << files[videoCounter].path();
	omxPlayer.loadMovie(files[videoCounter].path());
	doLoadNextMovie = false;
}

//--------------------------------------------------------------
void ofApp::update()
{

	if(oscReceiver.hasWaitingMessages())
	{
	    ofxOscMessage m;
	    oscReceiver.getNextMessage(m);

	    if(m.getAddress() == "/update") loadVideoFiles();

	    if(m.getAddress() == "/fadeDown")
	    {
		setFade(DOWN);
		fadeTime = m.getArgAsFloat(0);
		fadeRate = timeScale(fadeTime);
		ofLog() << "received message " << m << " fadeRate(frames):" << fadeRate << " fade time(secs):" << fadeTime <<" current alpha:" << alpha;

	    }


	    if(m.getAddress() == "/fadeUp")
	    {
		setFade(UP);
		fadeTime = m.getArgAsFloat(0);
		fadeRate = timeScale(fadeTime); 

		ofLog() << "received message " << m << " fadeRate(frames):" << fadeRate << " fade time(secs):" << fadeTime;
	    }

	    if(m.getAddress() == "/liveFade")
	    {
		alpha = m.getArgAsInt(0);
		omxPlayer.setAlpha(alpha);
		ofLog() << "received message " << m << " liveFade:alpha value:" << alpha;
	    }


	    if(m.getAddress() == "/play")
	    {
		int fileIndex = (m.getArgAsInt(0) - 1);
		fileIndex = fileIndex % files.size();
		omxPlayer.loadMovie(files[fileIndex].path());
		alpha = 255.0;
		setFade(UP);
		ofLog() << "received message" << m << "alpha:" << alpha;
	    }

	}

	if(bFadeUp   && alpha < 255.0) 
	    {
		alpha += fadeRate;
		if(alpha>255.0) alpha=255.0;
		omxPlayer.setAlpha(floor(alpha));
		ofLog() << "fade in " << alpha;
	    }

	else

	if(bFadeDown && alpha > 0.0) 
	    {
		alpha -= fadeRate;
		if(alpha<0) alpha=0;
		omxPlayer.setAlpha(floor(alpha));
		ofLog() << "fade out " << alpha;

	    }

}

//--------------------------------------------------------------
void ofApp::draw(){
	
//	stringstream info;
//	info << "alpha:" << alpha << endl;
// 	info << omxPlayer.getInfo() << endl;
//	ofDrawBitmapStringHighlight(info.str(), 60, 60, ofColor(ofColor::black, 90), ofColor::yellow);

}

//--------------------------------------------------------------

void ofApp::keyPressed  (int key){

}
//--------------------------------------------------------------

void ofApp::setFade(int fadeDirection){

	if(fadeDirection==UP)
	{
		bFadeUp=true;
		bFadeDown=false;

	} else {

	if(fadeDirection==DOWN)
	{
		bFadeUp=false;
		bFadeDown=true;
	}
    }
}
//--------------------------------------------------------------

float ofApp::timeScale(float _seconds){

	float fadeStep = (255 / ofGetFrameRate()) / _seconds; 

	return fadeStep;
}
	
