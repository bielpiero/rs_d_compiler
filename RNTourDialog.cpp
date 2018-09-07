#include "RNTourDialog.h"

RNTourDialog::RNTourDialog(/*RNGesturesTask* head, DorisLipSync* lips*/){
	std::setlocale(LC_ALL, "es_ES");
	//this->head = head;
	//this->lips = lips;
	file.open("tts/disam-01.text");
	if(!file){
		printf("could not load file\n");
	} else {
		loadPredifinedSymbols();
		lex();
		parse();
	}

}

RNTourDialog::~RNTourDialog(){
	if(file){
		file.close();
	}
}

void RNTourDialog::loadPredifinedSymbols(){
	globalSymbols.emplace("FACE:happy", "ID:0");
	globalSymbols.emplace("FACE:surprise", "ID:1");
	globalSymbols.emplace("FACE:singing", "ID:2");
	globalSymbols.emplace("FACE:despair", "ID:3");
	globalSymbols.emplace("FACE:rejection", "ID:4");
	globalSymbols.emplace("FACE:slyly", "ID:5");
	globalSymbols.emplace("FACE:tired", "ID:6");
	globalSymbols.emplace("FACE:sleepy", "ID:7");
	globalSymbols.emplace("FACE:angry", "ID:8");
	globalSymbols.emplace("FACE:happy-dream", "ID:9");
	globalSymbols.emplace("FACE:shouting", "ID:10");
	globalSymbols.emplace("FACE:talking", "ID:11");
	globalSymbols.emplace("FACE:hypnotic", "ID:12");
	globalSymbols.emplace("FACE:evil", "ID:13");
	globalSymbols.emplace("FACE:afraid", "ID:14");
	globalSymbols.emplace("FACE:suspect", "ID:15");
	globalSymbols.emplace("FACE:neutral", "ID:16");
	globalSymbols.emplace("FACE:wink1", "ID:17");
	globalSymbols.emplace("FACE:wink2", "ID:18");
	globalSymbols.emplace("FACE:wink3", "ID:19");
	globalSymbols.emplace("FACE:shame", "ID:20");
	globalSymbols.emplace("FACE:thinker", "ID:21");
	globalSymbols.emplace("FACE:drunk", "ID:22");
	globalSymbols.emplace("FACE:worried", "ID:23");
	globalSymbols.emplace("FACE:pouts", "ID:24");
	globalSymbols.emplace("FACE:sidelook", "ID:25");
	globalSymbols.emplace("FACE:sympathy", "ID:26");
	globalSymbols.emplace("FACE:med-smile", "ID:27");
	globalSymbols.emplace("FACE:big-smile", "ID:28");
	globalSymbols.emplace("FACE:creepy-smile", "ID:29");
	globalSymbols.emplace("FACE:panic", "ID:30");
	globalSymbols.emplace("FACE:quiet", "ID:31");
	globalSymbols.emplace("FACE:sad", "ID:32");
	globalSymbols.emplace("ATTN:front", "ID:33");
	globalSymbols.emplace("ATTN:right", "ID:34");
	globalSymbols.emplace("ATTN:left", "ID:35");
	globalSymbols.emplace("VAR:begin", "ID:0");
	globalSymbols.emplace("VAR:next", "ID:1");
	globalSymbols.emplace("VAR:previous", "ID:2");
	globalSymbols.emplace("VAR:end", "ID:3");
}

void RNTourDialog::lex(){
	
	std::stringstream wss;
	wss << file.rdbuf();
	std::string buff = wss.str();
	std::list<char> fileContent(buff.begin(), buff.end());
	std::list<char>::iterator it;
	std::list<std::string> tokens;
	std::list<std::string>::iterator tpos = tokens.end();
	std::string tok = "";
	std::string str = "";
	std::string var = "";
	std::string cnd = "";
	std::string ret = "VOID";
	tokens.clear();
	unsigned long long ifcounter = 0, forcounter = 0, whilecounter = 0;
	int parcount = 0;
	int state = 0, isnumber = 0, varstarted = 0, retstarted = 0;
	int functionStarted = 0;
	int infunction = 0, ifstarted = 0;
	int expr_started = 0, elsestate = 0, equal_started = 0;
	int equalfound = 0;
	std::string functionName = "";
	int functionNameStatus = 0;
	for(it = fileContent.begin(); it != fileContent.end(); it++){
		tok += *it;
		if(*it == ' '){
			tok = tok.substr(0, tok.size() - 1);
			if(functionStarted == 1 and functionNameStatus == 1 and str != ""){
				functionNameStatus = 0;
				functionName = str;
				functions.emplace(functionName, wcontent_t());
				str = "";
				tok = "";
			} else if(varstarted == 1 and var != ""){
				tokens.insert(tpos, "DVR:" + var);
				varstarted = 0;
				var = "";
				tok = "";
			} else if(state == 2 and isnumber == 1){
				if(str != ""){
					tokens.insert(tpos, "NUM:" + str);
				}
				isnumber = 0;
				str = "";
				state = 0;
				tok = "";
			} else if(tok != "" and expr_started == 0 and std::regex_match(tok, std::regex("[a-zA-Z_$][a-zA-Z_$0-9]*"))){
				tokens.insert(tpos, "VAR:" + tok);
				tok = "";
			} else if(state == 1 and expr_started == 1){
				tok = " ";
			} else if(state == 0 or state == 1){
				tok = "";
			} else if (state == 2){
				tok = " ";
			} else if(retstarted == 1 and cnd == ""){
				tok = "";
			}
		} else if(*it == '\n' or *it == '\t'){
			tok = tok.substr(0, tok.size() - 1);
			if(functionStarted == 1 and functionNameStatus == 1 and str != ""){
				functionName = str;
				functionNameStatus = 0;
				functions.emplace(functionName, wcontent_t());
				str = "";
				tok = "";
			} else if(varstarted == 1 and var != ""){
				tokens.insert(tpos, "DVR:" + var);
				varstarted = 0;
				var = "";
				tok = "";
			} else if(state == 2 and isnumber == 1){
				if(str != ""){
					tokens.insert(tpos, "NUM:" + str);
				}
				isnumber = 0;
				str = "";
				state = 0;
				tok = "";
			} else if(tok != "" and expr_started == 0 and std::regex_match(tok, std::regex("[a-zA-Z_$][a-zA-Z_$0-9]*"))){
				tokens.insert(tpos, "VAR:" + tok);
				tok = "";
			} else if(retstarted == 1 and expr_started == 1 and cnd != ""){
				tokens.insert(tpos, "EXP:" + cnd);
				expr_started = 0;
				retstarted = 0;
				state = 0;
				cnd = "";
				tok = "";
			} else if(equal_started == 1 and expr_started == 1 and cnd != ""){
				tokens.insert(tpos, "EXP:" + cnd);
				expr_started = 0;
				equal_started = 0;
				state = 0;
				cnd = "";
				tok = "";
			} else if(state == 1 and expr_started == 1){
				tok = " ";
			} else if(state == 0 or state == 1){
				tok = "";
			} else if (state == 2){
				tok = " ";
			} else if(retstarted == 1 and cnd == ""){
				tok = "";
			}
		} else if(tok == "function"){
			if(functionStarted == 0){
				functionStarted = 1;
				functionName = "";
				functionNameStatus = 1;
				tokens.clear();
			} else {
				functionStarted = -1;
			}
			tok = "";
		} else if(tok == "endfunction"){
			if(functionStarted == 1){
				functionStarted = 0;
				std::cout << functionName << std::endl;
				RNUtils::printList<std::string>(tokens);
				wcontent_t currentContent = functions.at(functionName);
				currentContent.result = ret;
				currentContent.tokens = tokens;
				functions[functionName] = currentContent;
				tokens.clear();
				ifcounter = 0;
				forcounter = 0; 
				whilecounter = 0;
			} else {
				functionStarted = -1;
			}
			tok = "";
		} else if(tok == "if"){
			tokens.insert(tpos, "IF");
			infunction = 1;
			expr_started = 1;
			ifstarted = 1;
			tok = "";
		} else if(tok == "else"){
			elsestate++;
			tpos = std::next(tpos, 1);
			tok = "";
		} else if(tok == "endif"){
			if(elsestate > 0){
				elsestate--;
				tpos = std::next(tpos, 1);
			} else {
				tpos = std::next(tpos, 2);
			}
			tok = "";
		} else if(tok == "move"){
			tokens.insert(tpos, "MOVE");
			infunction = 1;
			tok = "";
		} else if(tok == "goto"){
			tokens.insert(tpos, "GOTO");
			infunction = 1;
			tok = "";
		} else if(tok == "say"){
			tokens.insert(tpos, "SAY");
			infunction = 1;
			tok = "";
		} else if(tok == "turn"){
			tokens.insert(tpos, "TURN");
			infunction = 1;
			tok = "";
		} else if(tok == "attention"){
			tokens.insert(tpos, "ATTENTION");
			infunction = 1;
			tok = "";
		} else if(tok == "var"){
			varstarted = 1;
			var = "";
			tok = "";
		} else if(tok == "return"){
			tokens.insert(tpos, "RETURN");
			retstarted = 1;
			expr_started = 1;
			infunction = 1;
			state = 1;
			tok = "";
		} else if(tok == "(" and state == 0 and functionStarted == 1 and functionNameStatus == 1 and str != ""){
			functionName = str;
			functionNameStatus = 0;
			functions.emplace(functionName, wcontent_t());
			state = 1;
			str = "";
			tok = "";
		} else if((*it) == '(' and state == 0 and tok != "" and infunction == 0){
			tok = tok.substr(0, tok.size() - 1);
			tokens.insert(tpos, "FNC:" + tok);
			str = "";
			tok = "";
		} else if (tok == "(" and state == 0 and infunction == 1){
			state = 1;
			tok = "";
		} else if (tok == "[" and state == 0 and infunction == 1){
			state = 1;
			tok = "";
		} else if(tok == "=" and state == 0){
			state = 1;
			expr_started = 1;
			equal_started = 1;
			tokens.insert(tpos, "EQ");
			tok = "";
		} else if (*it == '\"' and expr_started == 0){
			if(state == 1){
				state = 2;
				str += tok;
			} else if (state == 2){
				if(str != "" and str.substr(0, 1) == "\""){
					tokens.insert(tpos, "STR:" + str.substr(1, str.length() - 1));
				}
				str = "";
				state = 0;
			}
			tok = "";
		} else if(*it == '#' and expr_started == 0){ 
			if(state == 1){
				state = 2;
				isnumber = 1;
			}
			tok = "";
		} else if (tok == ","){
			if(state == 1){
				if(str != ""){
					tokens.insert(tpos, "VAR:" + str);
					str = "";
					tok = "";
				}
			} else if(state == 2){
				if(isnumber == 1){
					tokens.insert(tpos, "NUM:" + str);
					isnumber = 0;
					state = 1;
					str = "";
					tok = "";
				} else {
					str += tok;
					tok = "";
				}

			}
		} else if (tok == ")"){
			if(parcount > 0){
				parcount--;
				if(expr_started == 1){
					cnd += tok;
					tok = "";
				} else if(varstarted == 1){
					var += tok;
					tok = "";
				} else {
					str += tok;
					tok = "";
				}
			} else {
				if(state == 1){
					if(ifstarted == 1){
						tokens.insert(tpos, "EXP:" + cnd);
						tokens.insert(tpos, "BIF:" + std::to_string(ifcounter));
						tokens.insert(tpos, "BEL:" + std::to_string(ifcounter));
						tokens.insert(tpos, "EIF:" + std::to_string(ifcounter++));
						tpos = std::prev(tpos, 2);
						ifstarted = 0;
						expr_started = 0;
						cnd = "";
					} else if(str != ""){
						tokens.insert(tpos, "VAR:" + str);
					}
				} else if(state == 2){
					if(isnumber == 1){
						tokens.insert(tpos, "NUM:" + str);
						isnumber = 0;
					}
				}
				infunction = 0;
				state = 0;
				str = "";
			}
			tok = "";
		} else if(tok == "]"){
			if(state == 1){
				tokens.insert(tpos, "OPT:" + str);
				state = 0;
				tok = "";
				str = "";
			}
		} else if(functionStarted == 1 and functionNameStatus == 1){
			str += tok;
			tok = "";
		} else if(state > 0){
			if(expr_started == 1){
				if(tok == "(" or tok == " ("){
					parcount++;
				}
				cnd += tok;
				tok = "";
			} else if(tok == "#"){
				tok = "";
				if(isnumber == 0){
					isnumber = 1;
				}
			} else {
				str += tok;
				tok = "";
			}
		} else if (varstarted == 1){
			var += tok;
			tok = "";
		}
	}
}

void RNTourDialog::parse(){

	std::map<std::string, wcontent_t>::iterator fit_main;
	fit_main = functions.find("main");
	if(fit_main != functions.end()){
		wcontent_t content = fit_main->second;
		std::map<std::string, std::string> symbols;
		parse(fit_main->first, &content);
		fit_main->second = content;
	} else {
		printf("FUNCTION MAIN NOT FOUND\n");
	}
}

void RNTourDialog::parse(std::string functionName, wcontent_t* content){
	std::list<std::string> functionTokens = content->tokens;
	std::map<std::string, std::string>* functionSymbols = &content->symbols;
	RNUtils::printList<std::string>(functionTokens);
	std::list<std::string>::iterator it = functionTokens.begin();
	std::stack<std::string> ifIdsStack;
	while(it != functionTokens.end()){
		std::list<std::string>::iterator it2 = std::next(it, 1);
		std::list<std::string>::iterator it3 = std::next(it, 2);
		std::list<std::string>::iterator it4 = std::next(it, 3);
		if((*it) == "MOVE"){
			if(it2 != functionTokens.end()){
				if((*it2).substr(0, 3) == "STR"){
					printf("MOVE TO COMMAND\n");
					it = std::next(it, 2);
				} else if((*it2).substr(0, 3) == "VAR"){
					std::map<std::string, std::string>::iterator dir = globalSymbols.find((*it2));
					if(dir != globalSymbols.end()){
						int direction = std::stoi(globalSymbols.at((*it2)).substr(3));
						//moveAround(direction);
					} else {
						fprintf(stderr, "Unidefined VAR: %s\n", (*it2).substr(4).c_str());
					}
					it = std::next(it, 2);
				}
			}
		} else if((*it) == "GOTO"){
			int sector;
			float px, py;
			if((*it2).substr(0, 3) == "NUM" and (*it3).substr(0, 3) == "NUM" and (*it4).substr(0, 3) == "NUM"){
				sector = std::stoi((*it2).substr(4));
				px = std::stof((*it3).substr(4));
				py = std::stof((*it4).substr(4));
			} else if((*it2).substr(0, 3) == "NUM" and (*it3).substr(0, 3) == "NUM" and (*it4).substr(0, 3) == "VAR"){
				sector = std::stoi((*it2).substr(4));
				px = std::stof((*it3).substr(4));
				if(functionSymbols->find((*it4).substr(4)) != functionSymbols->end()){
					py = std::stof(functionSymbols->at((*it4).substr(4)).substr(4));
				}
			} else if((*it2).substr(0, 3) == "NUM" and (*it3).substr(0, 3) == "VAR" and (*it4).substr(0, 3) == "NUM"){
				sector = std::stoi((*it2).substr(4));
				if(functionSymbols->find((*it3).substr(4)) != functionSymbols->end()){
					px = std::stof(functionSymbols->at((*it3).substr(4)).substr(4));
				}
				py = std::stof((*it4).substr(4));
			} else if((*it2).substr(0, 3) == "NUM" and (*it3).substr(0, 3) == "VAR" and (*it4).substr(0, 3) == "VAR"){
				sector = std::stoi((*it2).substr(4));
				if(functionSymbols->find((*it3).substr(4)) != functionSymbols->end()){
					px = std::stof(functionSymbols->at((*it3).substr(4)).substr(4));
				}
				if(functionSymbols->find((*it4).substr(4)) != functionSymbols->end()){
					py = std::stof(functionSymbols->at((*it4).substr(4)).substr(4));
				}
			} else if((*it2).substr(0, 3) == "VAR" and (*it3).substr(0, 3) == "NUM" and (*it4).substr(0, 3) == "NUM"){
				if(functionSymbols->find((*it2).substr(4)) != functionSymbols->end()){
					sector = std::stoi(functionSymbols->at((*it2).substr(4)).substr(4));
				}
				px = std::stof((*it3).substr(4));
				py = std::stof((*it4).substr(4));
			} else if((*it2).substr(0, 3) == "VAR" and (*it3).substr(0, 3) == "NUM" and (*it4).substr(0, 3) == "VAR"){
				if(functionSymbols->find((*it2).substr(4)) != functionSymbols->end()){
					sector = std::stoi(functionSymbols->at((*it2).substr(4)).substr(4));
				}
				px = std::stof((*it3).substr(4));
				if(functionSymbols->find((*it4).substr(4)) != functionSymbols->end()){
					py = std::stof(functionSymbols->at((*it4).substr(4)).substr(4));
				}
			} else if((*it2).substr(0, 3) == "VAR" and (*it3).substr(0, 3) == "VAR" and (*it4).substr(0, 3) == "NUM"){
				if(functionSymbols->find((*it2).substr(4)) != functionSymbols->end()){
					sector = std::stoi(functionSymbols->at((*it2).substr(4)).substr(4));
				}
				if(functionSymbols->find((*it3).substr(4)) != functionSymbols->end()){
					px = std::stof(functionSymbols->at((*it3).substr(4)).substr(4));
				}
				py = std::stof((*it4).substr(4));
			} else if((*it2).substr(0, 3) == "VAR" and (*it3).substr(0, 3) == "VAR" and (*it4).substr(0, 3) == "VAR"){
				if(functionSymbols->find((*it2).substr(4)) != functionSymbols->end()){
					sector = std::stoi(functionSymbols->at((*it2).substr(4)).substr(4));
				}
				if(functionSymbols->find((*it3).substr(4)) != functionSymbols->end()){
					px = std::stof(functionSymbols->at((*it3).substr(4)).substr(4));
				}
				if(functionSymbols->find((*it4).substr(4)) != functionSymbols->end()){
					py = std::stof(functionSymbols->at((*it4).substr(4)).substr(4));
				}
			}
			printf("Going to Sector: %d, {x: %f, y: %f}\n", sector, px, py);
			//tripTo(sector, px, py);
			
			it = std::next(it, 4);
		} else if((*it) == "TURN"){
			if(it2 != functionTokens.end()){
				if((*it2).substr(0, 3) == "NUM"){
					printf("TURN %s DEGREES COMMAND\n", (*it2).substr(4).c_str());
					it = std::next(it, 2);
				}
			} 
		} else if((*it).substr(0, 3) == "FNC"){
			printf("CALL FUNCTION\n");
			std::string nextFunctionName = (*it).substr(4);
			std::map<std::string, wcontent_t>::iterator fit_call;
			fit_call = functions.find(nextFunctionName);
			if(fit_call != functions.end()){
				wcontent_t content = fit_call->second;
				parse(nextFunctionName, &content);
				fit_call->second = content;
			} else {
				printf("FUNCTION %s NOT FOUND\n", nextFunctionName.c_str());
			}
			it = std::next(it, 1);			
		} else if((*it) == "SAY"){
			if(it2 != functionTokens.end()){
				if((*it2).substr(0, 3) == "STR"){
					printf("SAY STRING: %s\n", (*it2).substr(4).c_str());
					//lips->textToViseme((*it2).substr(4));
					it = std::next(it, 2);
				} else if((*it2).substr(0, 3) == "VAR"){
					if(functionSymbols->find((*it2).substr(4)) != functionSymbols->end()){
						printf("SAY VAR: %s\n", functionSymbols->at((*it2).substr(4)).c_str());
						//lips->textToViseme((*it2).substr(4));
					} else {
						fprintf(stderr, "Unidefined VAR: %s\n", (*it2).substr(4).c_str());
					}
					it = std::next(it, 2);
				} else if((*it2).substr(0, 3) == "OPT"){
					if(it3 != functionTokens.end() and (*it3).substr(0, 3) == "STR"){
						printf("SAY STRING %s WITH OPTIONS: %s\n", (*it3).substr(4).c_str(), (*it2).substr(4).c_str());
						std::map<std::string, std::string> opts = createOptionsMap((*it2).substr(4));
						processOptions(opts);
						//lips->textToViseme((*it3).substr(4));
						it = std::next(it, 3);
					}
				}
			} 
		} else if((*it).substr(0, 3) == "DVR"){
			if(it2 != functionTokens.end() and it3 != functionTokens.end()){
				if((*it2) == "EQ" and (*it3).substr(0, 3) == "EXP"){
					printf("VAR WITH EXP\n");
					std::string r = evaluateExpression((*it3).substr(4), *functionSymbols);
					if(functionSymbols->find((*it).substr(4)) == functionSymbols->end()){
						functionSymbols->emplace((*it).substr(4), r);
					} else {
						functionSymbols->at((*it).substr(4)) = r;
					}
					it = std::next(it, 3);
				} else {
					printf("DECLARED VAR %s\n", (*it).substr(4).c_str());
					if(functionSymbols->find((*it).substr(4)) == functionSymbols->end()){
						functionSymbols->emplace((*it).substr(4), "UNK:nil");
					} else {
						fprintf(stderr, "VAR: %s, already exists\n", (*it).substr(4).c_str());
					}
					it++;
				}
			} else {
				printf("DECLARED VAR %s\n", (*it).substr(4).c_str());
				if(functionSymbols->find((*it).substr(4)) == functionSymbols->end()){
					functionSymbols->emplace((*it).substr(4), "UNK:nil");
				} else {
					fprintf(stderr, "VAR: %s, already exists\n", (*it).substr(4).c_str());
				}
				it++;
			}
		} else if((*it).substr(0, 3) == "VAR"){
			if(functionSymbols->find((*it).substr(4)) != functionSymbols->end()){
				if((*it2) == "EQ" and (*it3).substr(0, 3) == "EXP"){
					printf("UPDATING VALUE OF VAR %s FROM EXP\n", (*it).substr(4).c_str());
					std::string r = evaluateExpression((*it3).substr(4), *functionSymbols);
					functionSymbols->at((*it).substr(4)) = r;
					it = std::next(it, 3);
				}
			} else {
				fprintf(stderr, "Undefined VAR: %s\n", (*it).substr(4).c_str());
				it++;
			}

		} else if((*it) == "RETURN"){
			if(it2 != functionTokens.end() and (*it2).substr(0, 3) == "EXP"){
				std::string r = evaluateExpression((*it2).substr(4), *functionSymbols);
				functions.at(functionName).result = r;
			}
			it = functionTokens.end();
		} else if((*it) == "IF"){
			std::string r = evaluateExpression((*it2).substr(4), *functionSymbols); //hacer recursive descent parser

			if(r == "NUM:1"){
				it = std::next(it, 3);
				ifIdsStack.push((*it3).substr(4));
			} else {
				std::string elsename = "BEL:" + (*it3).substr(4);
				while((*it) != elsename){
					it++;
				}

			}
		} else if((*it).substr(0, 3) == "BEL"){
			if(not ifIdsStack.empty()){
				if(ifIdsStack.top() == (*it).substr(4)){
					std::string elsename = "EIF:" + ifIdsStack.top();
					while((*it) != elsename){
						it++;
					}
					ifIdsStack.pop();
				}
			} else {
				it++;
			}
		} else if((*it).substr(0, 3) == "EIF"){
			if(not ifIdsStack.empty()){
				if(ifIdsStack.top() == (*it).substr(4)){
					ifIdsStack.pop();
				}
			}
			it++;
		} else {
			it++;
		}
	}
}

std::string RNTourDialog::evaluateExpression(std::string expr, std::map<std::string, std::string> symbols){
	std::stack<std::string> exprStack;
	std::string result = "";
	std::list<std::string> tokens = tokenizeExpCond(expr);

	std::list<std::string>::iterator itokens;
	bool ok_sofar = true;
	for(itokens = tokens.begin(); itokens != tokens.end() and ok_sofar; itokens++){
		if((*itokens).substr(0, 3) == "VAR"){
			std::map<std::string, std::string>::iterator var = symbols.find((*itokens).substr(4));
			if(var != symbols.end()){
				*itokens = var->second;
			} else {
				fprintf(stderr, "error: Undefined variable %s in this scope\n", (*itokens).substr(4).c_str());
				ok_sofar = false;
			}
		} else if((*itokens).substr(0, 3) == "FNC"){
			std::map<std::string, wcontent_t>::iterator itFnc;
			itFnc = functions.find((*itokens).substr(4));
			if(itFnc != functions.end()){
				wcontent_t content = itFnc->second;
				parse(itFnc->first, &content);
				if(functions.at(itFnc->first).result != "VOID"){
					*itokens = functions.at(itFnc->first).result;
				} else {
					fprintf(stderr, "error: Function %s does not return value\n", (*itokens).substr(4).c_str());
				}
				
			} else {
				fprintf(stderr, "error: Undefined function %s in this scope\n", (*itokens).substr(4).c_str());
				ok_sofar = false;
			}
		}
	}

	solveExpParenthesis(&tokens);
	if(tokens.size() > 1){
		solveExp(&tokens);
	}
	result = tokens.front();
	return result;
}

void RNTourDialog::solveExpParenthesis(std::list<std::string>* tokens){
	//RNUtils::printList<std::string>(*tokens);
	std::list<std::string> mini_tokens;
	std::list<std::string>::reverse_iterator itoks_s = std::find(tokens->rbegin(), tokens->rend(), "(");
	int idx_start = RN_NONE, idx_end = RN_NONE;
	if(itoks_s != tokens->rend()){
		idx_start = (tokens->size() - 1) - std::distance(tokens->rbegin(), itoks_s);
	}
	std::list<std::string>::iterator itoks_e = std::find(std::next(tokens->begin(), idx_start), tokens->end(), ")");
	if(itoks_e != tokens->end()){
		idx_end = std::distance(tokens->begin(), itoks_e);
	}

	if(idx_start != RN_NONE and idx_end != RN_NONE){
		
		mini_tokens.insert(mini_tokens.end(), std::next(tokens->begin(), idx_start + 1), std::next(tokens->begin(), idx_end));
		solveExp(&mini_tokens);
		tokens->erase(std::next(tokens->begin(), idx_start), std::next(tokens->begin(), idx_end + 1));
		tokens->insert(std::next(tokens->begin(), idx_start), mini_tokens.front());
		
		solveExpParenthesis(tokens);
	}
	
	
}

void RNTourDialog::solveExp(std::list<std::string>* tokens){
	factor(tokens);
	term(tokens);
	simpExpr(tokens);
}

void RNTourDialog::factor(std::list<std::string>* tokens){
	bool fac = true;
	//RNUtils::printList<std::string>(*tokens);
	while(fac){
		fac = false;
		std::list<std::string>::iterator itTokens = tokens->begin();
		int idx_start = RN_NONE, idx_end = RN_NONE, idx = 0;
		std::string op1 = "", op2 = "", oper = "", result = "";
		while(itTokens != tokens->end()){
			if(*itTokens == "*" or *itTokens == "/" or *itTokens == "%" or *itTokens == "AND"){
				fac = true;
				op1 = *std::prev(itTokens);
				op2 = *std::next(itTokens, 1);
				oper = *itTokens;
				idx_start = idx - 1;
				idx_end = idx + 1;
				itTokens = tokens->end();
			} else {
				itTokens++;
				idx++;
			}
		}
		if(fac){
			if(op1 != "" and op2 != "" and oper != ""){
				if(op1.substr(0, 3) == op2.substr(0, 3)){
					if(op1.substr(0, 3) == "NUM"){
						float res;
						if(oper == "*"){
							res = std::stof(op1.substr(4)) * std::stof(op2.substr(4));
						} else if(oper == "/"){
							res = std::stof(op1.substr(4)) / std::stof(op2.substr(4));
						} else if(oper == "%"){
							res = (float)(static_cast<int>(std::stof(op1.substr(4))) % static_cast<int>(std::stof(op2.substr(4))));
						} else if(oper == "AND"){
							res = std::stof(op1.substr(4)) and std::stof(op2.substr(4));
						}
						result = "NUM:" + std::to_string(res);
					} else {
						fprintf(stderr, "error: invalid type operation\n");
					}
				} else {
					fprintf(stderr, "error: invalid type operation\n");
				}
			}

			if(idx_start != RN_NONE and idx_end != RN_NONE){
				tokens->erase(std::next(tokens->begin(), idx_start), std::next(tokens->begin(), idx_end + 1));
				tokens->insert(std::next(tokens->begin(), idx_start), result);
				
			}
		}
		
	}
}

void RNTourDialog::term(std::list<std::string>* tokens){
	bool trm = true;
	//RNUtils::printList<std::string>(*tokens);
	while(trm){
		trm = false;
		std::list<std::string>::iterator itTokens = tokens->begin();
		int idx_start = RN_NONE, idx_end = RN_NONE, idx = 0;
		std::string op1 = "", op2 = "", oper = "", result = "";
		while(itTokens != tokens->end()){
			if(*itTokens == "+" or *itTokens == "-" or *itTokens == "OR"){
				trm = true;
				op1 = *std::prev(itTokens);
				op2 = *std::next(itTokens, 1);
				oper = *itTokens;
				idx_start = idx - 1;
				idx_end = idx + 1;
				std::cout << oper << std::endl;
				itTokens = tokens->end();
			} else {
				itTokens++;
				idx++;
			}
		}
		if(trm){
			if(op1 != "" and op2 != "" and oper != ""){
				if(op1.substr(0, 3) == op2.substr(0, 3)){
					if(op1.substr(0, 3) == "NUM"){
						float res;
						if(oper == "+"){
							res = std::stof(op1.substr(4)) + std::stof(op2.substr(4));
						} else if(oper == "-"){
							res = std::stof(op1.substr(4)) - std::stof(op2.substr(4));
						} else if(oper == "OR"){
							res = std::stof(op1.substr(4)) or std::stof(op2.substr(4));
						}
						result = "NUM:" + std::to_string(res);
					} else {
						fprintf(stderr, "error: invalid type operation\n");
					}
				} else {
					fprintf(stderr, "error: invalid type operation\n");
				}
			}

			if(idx_start != RN_NONE and idx_end != RN_NONE){
				tokens->erase(std::next(tokens->begin(), idx_start), std::next(tokens->begin(), idx_end + 1));
				tokens->insert(std::next(tokens->begin(), idx_start), result);
			}
		}
	}
}

void RNTourDialog::simpExpr(std::list<std::string>* tokens){
	bool se = true;

	while(se){
		se = false;
		std::list<std::string>::iterator itTokens = tokens->begin();
		int idx_start = RN_NONE, idx_end = RN_NONE, idx = 0;
		std::string op1 = "", op2 = "", oper = "", result = "";
		while(itTokens != tokens->end()){
			if(*itTokens == "EQU" or *itTokens == "NEQ" or *itTokens == "LT" or *itTokens == "LEQ" or *itTokens == "GT" or *itTokens == "GEQ"){
				se = true;
				op1 = *std::prev(itTokens);
				op2 = *std::next(itTokens, 1);
				oper = *itTokens;
				idx_start = idx - 1;
				idx_end = idx + 1;
				std::cout << oper << std::endl;
				itTokens = tokens->end();
			} else {
				itTokens++;
				idx++;
			}
		}
		if(se){
			if(op1 != "" and op2 != "" and oper != ""){
				if(op1.substr(0, 3) == op2.substr(0, 3)){
					int res;
					if(oper == "EQU"){
						if(op1.substr(0, 3) == "NUM"){
							res = std::stof(op1.substr(4)) == std::stof(op2.substr(4));
						} else {
							res = op1.substr(4) == op2.substr(4);
						}
					} else if(oper == "NEQ"){
						if(op1.substr(0, 3) == "NUM"){
							res = std::stof(op1.substr(4)) != std::stof(op2.substr(4));
						} else {
							res = op1.substr(4) != op2.substr(4);
						}
					} else if(oper == "LT"){
						if(op1.substr(0, 3) == "NUM"){
							res = std::stof(op1.substr(4)) < std::stof(op2.substr(4));
						} else {
							res = op1.substr(4) < op2.substr(4);
						}
					} else if(oper == "LEQ"){
						if(op1.substr(0, 3) == "NUM"){
							res = std::stof(op1.substr(4)) <= std::stof(op2.substr(4));
						} else {
							res = op1.substr(4) <= op2.substr(4);
						}
					} else if(oper == "GT"){
						if(op1.substr(0, 3) == "NUM"){
							res = std::stof(op1.substr(4)) > std::stof(op2.substr(4));
						} else {
							res = op1.substr(4) > op2.substr(4);
						}
					} else if(oper == "GEQ"){
						if(op1.substr(0, 3) == "NUM"){
							res = std::stof(op1.substr(4)) >= std::stof(op2.substr(4));
						} else {
							res = op1.substr(4) >= op2.substr(4);
						}
					}
					result = "NUM:" + std::to_string(res);
					
				} else {
					fprintf(stderr, "error: invalid type operation\n");
				}
			}

			if(idx_start != RN_NONE and idx_end != RN_NONE){
				tokens->erase(std::next(tokens->begin(), idx_start), std::next(tokens->begin(), idx_end + 1));
				tokens->insert(std::next(tokens->begin(), idx_start), result);
			}
		}
	}
}

std::list<std::string> RNTourDialog::tokenizeExpCond(std::string expr_cond){
	std::string::iterator it = expr_cond.begin();
	std::list<std::string> tokens;
	std::string tok = "";
	std::string str = "";
	int state = 0;
	int evalfunc = 0;
	while(it != expr_cond.end()){
		if(tok == "and" and (*it == '(' or *it == ')' or *it == '\"' or *it == '#')){
			tokens.emplace_back("AND");
			tok = "";
		} else if(tok == "or" and (*it == '(' or *it == ')' or *it == '\"' or *it == '#')){
			tokens.emplace_back("OR");
			tok = "";
		} else if(tok == "not" and (*it == '(' or *it == ')' or *it == '\"' or *it == '#')){
			tokens.emplace_back("NOT");
			tok = "";
		} 

		if(*it == ' '){
			if(state == 2){
				if(str != ""){
					tokens.emplace_back("NUM:" + str);
				}
				str = "";
				state = 0;
				tok = "";
			} else if(tok != "" and (tok != "and" and tok != "or" and tok != "not") and std::regex_match(tok, std::regex("[a-zA-Z_$][a-zA-Z_$0-9]*"))){
				tokens.emplace_back("VAR:" + tok);
			} else if(tok == "and"){
				tokens.emplace_back("AND");
			} else if(tok == "or"){
				tokens.emplace_back("OR");
			} else if(tok == "not"){
				tokens.emplace_back("NOT");
			}
			tok = "";
		} else if(*it == '#'){ 
			if(state == 0){
				state = 2;
			}
			tok = "";
		} else if (*it == '\"'){
			if(state == 0){
				state = 1;
			} else if (state == 1){
				if(str != ""){
					tokens.emplace_back("STR:" + str);
				}
				str = "";
				state = 0;
			}
			tok = "";
		} else if(*it == '('){
			if(tok != ""){
				tokens.emplace_back("FNC:" + tok);
				evalfunc = 1;
			} else {
				tokens.emplace_back("(");
			}
			tok = "";
		
		} else if(*it == ')'){
			if(evalfunc == 1){
				evalfunc = 0;
			} else {
				if(tok != "" and std::regex_match(tok, std::regex("[a-zA-Z_$][a-zA-Z_$0-9]*"))){
					tokens.emplace_back("VAR:" + tok);
				} else if(state == 2){
					if(str != ""){
						tokens.emplace_back("NUM:" + str);
					}
					str = "";
					state = 0;
				}
				tokens.emplace_back(")");
			}
			tok = "";
		} else if(*it == '+' or *it == '-' or *it == '*' or *it == '/' or *it == '%'){
			if(tok != "" or str != ""){
				if(state == 2){
					if(str != ""){
						tokens.emplace_back("NUM:" + str);
					}
					str = "";
					state = 0;
				} else if(tok != "" and std::regex_match(tok, std::regex("[a-zA-Z_$][a-zA-Z_$0-9]*"))){
					tokens.emplace_back("VAR:" + tok);
				}
				tok = "";
			}
			std::string oper = "";
			oper += *it;
			tokens.emplace_back(oper);
			tok = "";
		} else if(*it == '=' or *it == '!' or *it == '<'  or *it == '>'){
			if(tok != "" or str != ""){
				if(state == 2){
					if(str != ""){
						tokens.emplace_back("NUM:" + str);
					}
					str = "";
					state = 0;
				} else if(tok != "" and std::regex_match(tok, std::regex("[a-zA-Z_$][a-zA-Z_$0-9]*"))){
					tokens.emplace_back("VAR:" + tok);
				}
				tok = "";
			}
			std::string::iterator it2 = std::next(it, 1);
			if(*it2 == '='){
				if(*it == '='){
					tokens.emplace_back("EQU");
					tok = "";
				} else if(*it == '!'){
					tokens.emplace_back("NEQ");
					tok = "";
				} else if(*it == '<'){
					tokens.emplace_back("LEQ");
					tok = "";
				} else if(*it == '>'){
					tokens.emplace_back("GEQ");
					tok = "";
				}
				it++;
			} else {
				if(*it == '='){
					
				} else if(*it == '!'){
					
				} else if(*it == '<'){
					tokens.emplace_back("LT");
					tok = "";
				} else if(*it == '>'){
					tokens.emplace_back("GT");
					tok = "";
				}
			}
		} else if(state > 0){
			str += *it;
			tok = "";
		} else if(state == 0){
			tok += *it;
		}
		it++;
	}
	if(tok != "" or str != ""){
		if(tok != "" and std::regex_match(tok, std::regex("[a-zA-Z_$][a-zA-Z_$0-9]*"))){
			tokens.emplace_back("VAR:" + tok);
		} else if(state == 2){
			if(str != ""){
				tokens.emplace_back("NUM:" + str);
			}
			str = "";
			state = 0;
		}
	}
	return tokens;
}

std::map<std::string, std::string> RNTourDialog::createOptionsMap(std::string opts){
	std::map<std::string, std::string> dic;
	std::vector<std::string> sopts = RNUtils::split(opts, ";");
	for(int i = 0; i < sopts.size(); i++){
		std::vector<std::string> opt_value = RNUtils::split(sopts.at(i), "=");
		if(opt_value.size() == 2){
			if(dic.find(opt_value.at(0)) == dic.end()){
				dic.emplace(opt_value.at(0), opt_value.at(1));
			} else {
				dic[opt_value.at(0)] = opt_value.at(1);
			}
		}
	}
	return dic;
}

void RNTourDialog::processOptions(std::map<std::string, std::string> opts){
	std::map<std::string, std::string>::iterator optsIt;
	std::map<std::string, std::string>::iterator op;
	for(optsIt = opts.begin(); optsIt != opts.end(); optsIt++){
		if(optsIt->first == "face"){
			op = globalSymbols.find("FACE:" + optsIt->second);
			if(op != opts.end()){
				//head->setGesture(op->second.substr(3));
			}
		} else if(optsIt->first == "attention"){
			op = globalSymbols.find("ATTN:" + optsIt->second);
			if(op != opts.end()){
				//head->setGesture(op->second.substr(3));
			}
		}
	}
}