#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include <QRect>
#include <opencv2/core/core.hpp>

#include "kcftracker.hpp"

//#include <dirent.h>

using namespace std;
using namespace cv;

QRect runtrack(int argc, char* argv[]){

	if (argc > 5) return -1;

	bool HOG = true;
	bool FIXEDWINDOW = false;
	bool MULTISCALE = true;
	bool SILENT = true;
	bool LAB = false;

	for(int i = 0; i < argc; i++){
		if ( strcmp (argv[i], "hog") == 0 )
			HOG = true;
		if ( strcmp (argv[i], "fixed_window") == 0 )
			FIXEDWINDOW = true;
		if ( strcmp (argv[i], "singlescale") == 0 )
			MULTISCALE = false;
		if ( strcmp (argv[i], "show") == 0 )
			SILENT = false;
		if ( strcmp (argv[i], "lab") == 0 ){
			LAB = true;
			HOG = true;
		}
		if ( strcmp (argv[i], "gray") == 0 )
			HOG = false;
	}
	
	// Create KCFTracker object
	KCFTracker tracker(HOG, FIXEDWINDOW, MULTISCALE, LAB);

	// Frame readed
	Mat frame;

	// Tracker results
	Rect result;

	// Using min and max of X and Y for groundtruth rectangle


	// Frame counter
	int nFrames = 0;

    while (1){
		// First frame, give the groundtruth to the tracker
		if (nFrames == 0) {
			tracker.init( Rect2d(xMin, yMin, width, height), frame );
		}
		// Update
		else{
			result = tracker.update(frame);
       }
		nFrames++;
        return QRect(result.x, result.y, result.width, result.height);
		}
	}
}
