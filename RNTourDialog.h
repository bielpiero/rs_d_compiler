#ifndef RN_TOUR_DIALOG_H
#define RN_TOUR_DIALOG_H

#include "DorisLipSync.h"

class RNTourDialog{
	public:
		RNTourDialog(DorisLipSync* lips);
		virtual ~RNTourDialog();
		void lex();
		void parse();
		void parse(std::list<std::string> functionTokens, std::map<std::string, std::string> *functionSymbols);
	private:
		void printList(std::list<std::string> l);
		void printMap(std::map<std::string, std::string> m);
		std::map<std::string, std::string> processOptions(std::string opts);
	private:
		struct wcontent_t{
			std::map<std::string, std::string> symbols;
			std::list<std::string> tokens;
		};
		std::map<std::string, wcontent_t > functions;
		std::map<std::string, std::string> globalSymbols;

		std::ifstream file;
		DorisLipSync* lips;
};

#endif