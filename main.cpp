#include <signal.h>
#include "RNTourDialog.h"
#include "SerialPort.h"
#include "DorisLipSync.h"

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
	SerialPort* mc = new SerialPort();
	DorisLipSync* lips = new DorisLipSync(mc, 1);
	RNTourDialog* dialog = new RNTourDialog (lips);
	//lips->textToViseme("ahí");
	return 0;
}