#pragma once
#include "ofMain.h"
#include "ofxOsc.h"
#include "wiringPi.h"
#include "wiringPiSPI.h"

#define HOST "bltzr.local"
#define PORT 12345
#define PORTIN 98765

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void exit();
		void setLEDs(int numLed, unsigned char * LEDs, int BRIGHTNESS);
		void clearLEDs(int numLed);
    
        ofVideoPlayer trame;
        ofxOscSender sender;
        ofxOscReceiver receiver;		
        ofBuffer imgAsBuffer;
        ofBuffer NetBuffer;
        ofBuffer PWMBuffer;
        ofImage img;
        unsigned char * LEDs;

    	const int LEDnumb = 280;
    	const int PWMnumb = 16;   
		int brightness = 0;
		int brightnessNet = 0; 
		int playing = 0;

		int mapBright[256] = { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 9, 9, 9, 10, 10, 10, 10, 10, 10, 11, 11, 11, 11, 11, 11, 12, 12, 12, 12, 12, 13, 13, 13, 13, 13, 14, 14, 14, 14, 14, 15, 15, 15, 15, 16, 16, 16, 16, 17, 17, 17, 17, 18, 18, 18, 19, 19, 19, 19, 20, 20, 20, 21, 21, 21, 22, 22, 22, 23, 23, 24, 24, 24, 25, 25, 25, 26, 26, 27, 27, 27, 28, 28, 29, 29, 30, 30, 31};
		float mapCol[256] = { 0., 0.007874, 0.023876, 0.039878, 0.05588, 0.071882, 0.087884, 0.103886, 0.119888, 0.13589, 0.151892, 0.167894, 0.183896, 0.199898, 0.2159, 0.231902, 0.247904, 0.263907, 0.279909, 0.295911, 0.311913, 0.327915, 0.343917, 0.359919, 0.375921, 0.391923, 0.407925, 0.423927, 0.439929, 0.455931, 0.471933, 0.487935, 0.503937, 0.519939, 0.535941, 0.551943, 0.567945, 0.583947, 0.599949, 0.615951, 0.631953, 0.647955, 0.663957, 0.679959, 0.695961, 0.711963, 0.727965, 0.743967, 0.75997, 0.775972, 0.791974, 0.807976, 0.823978, 0.83998, 0.855982, 0.871984, 0.887986, 0.903988, 0.91999, 0.935992, 0.951994, 0.967996, 0.983998, 1., 0.5, 0.516129, 0.532258, 0.548387, 0.564516, 0.580645, 0.596774, 0.612903, 0.629032, 0.645161, 0.66129, 0.677419, 0.693548, 0.709677, 0.725806, 0.741935, 0.758065, 0.774194, 0.790323, 0.806452, 0.822581, 0.83871, 0.854839, 0.870968, 0.887097, 0.903226, 0.919355, 0.935484, 0.951613, 0.967742, 0.983871, 1., 0.666667, 0.683333, 0.7, 0.716667, 0.733333, 0.75, 0.766667, 0.783333, 0.8, 0.816667, 0.833333, 0.85, 0.866667, 0.883333, 0.9, 0.916667, 0.933333, 0.95, 0.966667, 0.983333, 1., 0.75, 0.766667, 0.783333, 0.8, 0.816667, 0.833333, 0.85, 0.866667, 0.883333, 0.9, 0.916667, 0.933333, 0.95, 0.966667, 0.983333, 1., 0.8, 0.816667, 0.833333, 0.85, 0.866667, 0.883333, 0.9, 0.916667, 0.933333, 0.95, 0.966667, 0.983333, 1., 0.833333, 0.851852, 0.87037, 0.888889, 0.907407, 0.925926, 0.944444, 0.962963, 0.981481, 1., 0.857143, 0.875, 0.892857, 0.910714, 0.928571, 0.946429, 0.964286, 0.982143, 1., 0.875, 0.892857, 0.910714, 0.928571, 0.946429, 0.964286, 0.982143, 1., 0.888889, 0.904762, 0.920635, 0.936508, 0.952381, 0.968254, 0.984127, 1., 0.9, 0.92, 0.94, 0.96, 0.98, 1., 0.909091, 0.927273, 0.945455, 0.963636, 0.981818, 1., 0.916667, 0.9375, 0.958333, 0.979167, 1., 0.923077, 0.942308, 0.961538, 0.980769, 1., 0.928571, 0.946429, 0.964286, 0.982143, 1., 0.933333, 0.955556, 0.977778, 1., 0.9375, 0.958333, 0.979167, 1., 0.941176, 0.960784, 0.980392, 1., 0.944444, 0.972222, 1., 0.947368, 0.964912, 0.982456, 1., 0.95, 0.975, 1., 0.952381, 0.97619, 1., 0.954545, 0.977273, 1., 0.956522, 1., 0.958333, 0.979167, 1., 0.96, 0.98, 1., 0.961538, 1., 0.962963, 0.981481, 1., 0.964286, 1., 0.965517, 1., 0.966667, 1., 1.};

    
		
};
