#ifndef RN_TOUR_DIALOG_H
#define RN_TOUR_DIALOG_H

//#include "DorisLipSync.h"
#include "definitions.h"
#include "RNUtils.h"
#include "xml/rapidxml_print.hpp"
#include "xml/rapidxml.hpp"



class RNTourDialog{
	private:
		struct wcontent_t{
			std::map<std::string, std::string> symbols;
			std::list<std::string> arguments;
			std::list<std::string> tokens;
			int requiredArguments;
			std::string result;
		};
		struct wevent_t{
			int requiredArguments;
			std::string eventFunction;
			//std::list<std::string> arguments;
		};
	public:
		RNTourDialog(/*RNGesturesTask* head, DorisLipSync* lips*/);
		virtual ~RNTourDialog();
		void lex();
		void parseEvents();
		void parse();
		void parse(std::string functionName, wcontent_t* content);
		void parseEvents(std::string eventName, wevent_t* content, int cont);
		
	private:
		void loadPredifinedSymbols();

		std::string evaluateExpression(std::string condition, std::map<std::string, std::string> symbols);
		std::map<std::string, std::string> createOptionsMap(std::string opts);
		void processOptions(std::map<std::string, std::string> opts);
		std::list<std::string> tokenizeExpCond(std::string expr_cond);
		void solveExpParenthesis(std::list<std::string>* tokens);
		void solveExp(std::list<std::string>* tokens);
		std::string arrayValue(std::map<std::string, std::string> simbolos, std::string nombre, std::string position);
		std::string assignArray(std::map<std::string, std::string> simbolo, std::string array, std::string pos, std::string exprs);
		void factor(std::list<std::string>* tokens);
		void term(std::list<std::string>* tokens);
		void simpExpr(std::list<std::string>* tokens);
		std::string assignSize(std::string word_size);

	private:
		std::map<std::string, wcontent_t > functions;
		std::map<std::string, std::string> globalSymbols;
		std::ifstream file;
		std::map<std::string, wevent_t> events;
		//DorisLipSync* lips;
		//RNGesturesTask* head;
};

#endif