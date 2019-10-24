#pragma once

#include "ofMain.h"
#include "ofxOMXPlayer.h"
#include "ofxOsc.h"

#define PORT 54321
#define UP 1
#define DOWN 0


class ofApp : public ofBaseApp, public ofxOMXPlayerListener {

	public:

		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		
		void onVideoEnd(ofxOMXPlayer* player);
	        void onVideoLoop(ofxOMXPlayer* player);
		void loadNextMovie();
		void loadVideoFiles();
		void setFade(int);
		float timeScale(float);
			
		ofxOscReceiver 		oscReceiver;
		ofxOMXPlayerSettings 	settings;
		ofxOMXPlayer 		omxPlayer;
		vector<ofFile> 		files;
		int 			videoCounter;
		float 			alpha, fadeTime, fadeRate;
		bool 			bFadeUp, bFadeDown;	
};

