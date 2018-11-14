#ifndef RN_GESTURES_TASK_H
#define RN_GESTURES_TASK_H


#include "FaceGesture.h"
#include "SerialPort.h"

#define XML_GESTURE_FILE_PATH "BeaConSuerte.xml"
#define XML_FACE_LABEL "{face}"
#define XML_NECK_LABEL "{neck}"

using namespace rapidxml;

class RNGesturesTask{
private:
    std::vector<FaceGesture*> *gestures;
    std::string gestureId;
    SerialPort* maestroController;
public:
	RNGesturesTask(SerialPort* mc, const char* name = "Gestures Task", const char* description = "Doris Faces");
	~RNGesturesTask();
	virtual void task();

	void setGesture(std::string gestureId);
    void getGestures(std::string& jsonResponse);
    void saveGesture(std::string token);
    void modifyGesture(std::string token);
    void removeGesture(std::string face_id);
};

#endif