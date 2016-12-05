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
		float mapCol[256] = { 0., 0.007874, 0.008124, 0.008874, 0.010124, 0.011874, 0.014123, 0.016873, 0.020122, 0.023872, 0.028121, 0.032871, 0.03812, 0.04387, 0.050119, 0.056868, 0.064117, 0.071866, 0.080115, 0.088864, 0.098113, 0.107862, 0.11811, 0.128859, 0.140107, 0.151856, 0.164104, 0.176853, 0.190101, 0.20385, 0.218098, 0.232846, 0.248094, 0.263842, 0.28009, 0.296838, 0.314086, 0.331834, 0.350081, 0.368829, 0.388076, 0.407824, 0.428071, 0.448819, 0.470066, 0.491814, 0.514061, 0.536808, 0.560055, 0.583802, 0.608049, 0.632796, 0.658043, 0.683789, 0.710036, 0.736783, 0.76403, 0.791776, 0.820023, 0.848769, 0.878015, 0.907762, 0.938008, 0.968754, 0.5, 0.500488, 0.501953, 0.504395, 0.507812, 0.512207, 0.517578, 0.523926, 0.53125, 0.539551, 0.548828, 0.559082, 0.570312, 0.58252, 0.595703, 0.609863, 0.625, 0.641113, 0.658203, 0.67627, 0.695312, 0.715332, 0.736328, 0.758301, 0.78125, 0.805176, 0.830078, 0.855957, 0.882812, 0.910645, 0.939453, 0.969238, 0.666667, 0.667423, 0.66969, 0.673469, 0.67876, 0.685563, 0.693878, 0.703704, 0.715042, 0.727891, 0.742252, 0.758125, 0.77551, 0.794407, 0.814815, 0.836735, 0.860166, 0.88511, 0.911565, 0.939531, 0.96901, 0.75, 0.750977, 0.753906, 0.758789, 0.765625, 0.774414, 0.785156, 0.797852, 0.8125, 0.829102, 0.847656, 0.868164, 0.890625, 0.915039, 0.941406, 0.969727, 0.8, 0.801183, 0.804734, 0.810651, 0.818935, 0.829586, 0.842604, 0.857988, 0.87574, 0.895858, 0.918343, 0.943195, 0.970414, 0.833333, 0.835, 0.84, 0.848333, 0.86, 0.875, 0.893333, 0.915, 0.94, 0.968333, 0.857143, 0.858907, 0.864198, 0.873016, 0.885362, 0.901235, 0.920635, 0.943563, 0.970018, 0.875, 0.876953, 0.882812, 0.892578, 0.90625, 0.923828, 0.945312, 0.970703, 0.888889, 0.890625, 0.895833, 0.904514, 0.916667, 0.932292, 0.951389, 0.973958, 0.9, 0.902778, 0.911111, 0.925, 0.944444, 0.969444, 0.909091, 0.911616, 0.919192, 0.931818, 0.949495, 0.972222, 0.916667, 0.92, 0.93, 0.946667, 0.97, 0.923077, 0.926154, 0.935385, 0.950769, 0.972308, 0.928571, 0.931429, 0.94, 0.954286, 0.974286, 0.933333, 0.9375, 0.95, 0.970833, 0.9375, 0.941406, 0.953125, 0.972656, 0.941176, 0.944853, 0.955882, 0.974265, 0.944444, 0.950617, 0.969136, 0.947368, 0.950658, 0.960526, 0.976974, 0.95, 0.955556, 0.972222, 0.952381, 0.957672, 0.973545, 0.954545, 0.959596, 0.974747, 0.956522, 0.967391, 0.958333, 0.962963, 0.976852, 0.96, 0.964444, 0.977778, 0.961538, 0.971154, 0.962963, 0.967078, 0.979424, 0.964286, 0.973214, 0.965517, 0.974138, 0.966667, 0.975, 0.967742};

    
		
};
