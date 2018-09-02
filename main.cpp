#include <signal.h>
#include "RNTourDialog.h"
//#include "SerialPort.h"
//#include "DorisLipSync.h"
#include "RNGesturesTask.h"

RNTourDialog* dialog;

void signalHandler(int s){
	printf("Shutdown application Requested.\n");
	
	if(dialog != NULL){
    	delete dialog;
    }
	printf("Succesfully closed...\n");
}

int main(int argc, char** argv){
	std::setlocale(LC_ALL, "es_ES");
	//SerialPort* mc = new SerialPort();
	//uint8_t cid = atoi(argv[1]);
	//uint8_t sid = atoi(argv[2]);
	//uint16_t value = atoi(argv[3]);
	//mc->setTarget(cid, sid, value);
	//DorisLipSync* lips = new DorisLipSync(mc, 1);
	//RNGesturesTask* head = new RNGesturesTask(mc);
	RNTourDialog* dialog = new RNTourDialog (/*head, lips*/);

	//lips->textToViseme("biénvenidos al /\\/\\ centro de >>>>>>>> automática y /\\/\\robótica de la <<<<<<<<<<<<<< universidad politécnica de madrid");
	return 0;
}