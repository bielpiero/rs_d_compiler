#ifndef RN_TOUR_DIALOG_H
#define RN_TOUR_DIALOG_H

//#include "DorisLipSync.h"
#include "RNGesturesTask.h"

class RNTourDialog{
	private:
		struct wcontent_t{
			std::map<std::string, std::string> symbols;
			std::list<std::string> arguments;
			std::list<std::string> tokens;
			int requiredArguments;
			std::string result;
		};
	public:
		RNTourDialog(/*RNGesturesTask* head, DorisLipSync* lips*/);
		virtual ~RNTourDialog();
		void lex();
		void parse();
		void parse(std::string functionName, wcontent_t* content);
	private:
		void loadPredifinedSymbols();

		std::string evaluateExpression(std::string condition, std::map<std::string, std::string> symbols);
		std::map<std::string, std::string> createOptionsMap(std::string opts);
		void processOptions(std::map<std::string, std::string> opts);
		std::list<std::string> tokenizeExpCond(std::string expr_cond);
		void solveExpParenthesis(std::list<std::string>* tokens);
		void solveExp(std::list<std::string>* tokens);

		void factor(std::list<std::string>* tokens);
		void term(std::list<std::string>* tokens);
		void simpExpr(std::list<std::string>* tokens);
	private:
		std::map<std::string, wcontent_t > functions;
		std::map<std::string, std::string> globalSymbols;
		std::ifstream file;
		//DorisLipSync* lips;
		//RNGesturesTask* head;
};

#endif