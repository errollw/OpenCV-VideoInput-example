// TestVideoInput.cpp : Defines the entry point for the console application.
//
// see: http://www.codeproject.com/Articles/559437/Capturing-video-from-web-camera-on-Windows-and

#include "stdafx.h"
#include "videoInput.h"
#include <opencv2/highgui/highgui.hpp>

void StopEvent(int deviceID, void *userData) {
    videoInput vidInput = videoInput::getInstance();
    vidInput.closeDevice(deviceID);
}
 
int main(int argc, const char** argv) {

    videoInput vidInput = videoInput::getInstance();
    int num_devices = vidInput.listDevices();
	int cam_id = 0;

	// check we actually have attached devices, if not exit
	if (num_devices == 0){
		return -1;		
	}

	// if we fail to setup a camera, exit
	if(!vidInput.setupDevice(cam_id, 1280, 720, 60)) {
		return -1;	
	}

	// extract camera parameters, and set a default brightness
	CamParametrs CP = vidInput.getParametrs(cam_id);                        
    CP.Brightness.CurrentValue = 180; 
    CP.Brightness.Flag = 0; 
    vidInput.setParametrs(cam_id, CP);

	// sets callback function for emergency stop, e.g. pulling out camera
    vidInput.setEmergencyStopEvent(cam_id, NULL, StopEvent);
 
	cv::namedWindow("VideoInput Test", 1);
    CvSize size = cvSize(vidInput.getWidth(cam_id), vidInput.getHeight(cam_id));

    IplImage* frame;
 
	// VideoInput library uses IplImage
    frame = cvCreateImage(size, 8, 3);
 
	// main loop
    while(true) {

        if(vidInput.isFrameNew(cam_id)) {
            vidInput.getPixels(cam_id, (unsigned char *)frame->imageData);        
			cv::Mat image(frame);
			cv::imshow("VideoInput Test", image);
        }
 
        char c = cv::waitKey(5);
 
        if(c == 27) {
            break;
		} else if(c == 49) {                     
            CP.Brightness.CurrentValue = 50; 
            CP.Brightness.Flag = 1; 
            vidInput.setParametrs(cam_id, CP);
        }else if(c == 50) {                      
            CP.Brightness.CurrentValue = 0; 
            CP.Brightness.Flag = 2; 
            vidInput.setParametrs(cam_id, CP);
        }
 
        if(!vidInput.isDeviceSetup(cam_id)) {
            break;
        }
    }
 
	// tidy up on exit
    vidInput.closeDevice(cam_id);
    cv::destroyAllWindows();

    
    return 0;
}