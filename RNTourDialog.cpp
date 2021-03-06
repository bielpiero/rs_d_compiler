#include "RNTourDialog.h"
#include <iostream>
#include <vector>
#include <cstring>


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
	int argCount = 0;
	int parcount = 0;
	int state = 0, isnumber = 0, varstarted = 0, retstarted = 0;
	int functionStarted = 0, functionarguments = 0, function_call = 0;
	int infunction = 0, ifstarted = 0, whilestarted = 0, forstarted = 0;
	int expr_started = 0, elsestate = 0, equal_started = 0; 
	int array_started = 0;
	int equalfound = 0;
	int double_dot_counter = 0;
	int lines_counter = 1;
	int get_pos = 0; 
	int current_vector = 0;
	std::string functionName = "";
	int functionNameStatus = 0;
	int event = 0;
	int func_arg = 0;
	int size = 0;
	std::string eventName = "";
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
				tokens.insert(tpos, TOK_DVR2PTS_STR + var);
				varstarted = 0;
				var = "";
				tok = "";
			} else if(state == 2 and isnumber == 1){
				if(array_started == 0){
					if(str != ""){
						tokens.insert(tpos, TOK_NUM2PTS_STR + str);
					}
					isnumber = 0;
					str = "";
					state = 0;
					tok = "";
				} else {
					tok ="";
					state = 1;
					isnumber = 0;
				}
			} else if(tok != "" and expr_started == 0 and std::regex_match(tok, std::regex(REGEX_STR))){
				if(get_pos == 0){	
					if(functionarguments == 1){
						tokens.insert(tpos, TOK_ARG2PTS_STR + tok);
						argCount++;
					} else {
						tokens.insert(tpos, TOK_VAR2PTS_STR + tok);
					}
				
					tok = "";
				}
			} else if(state == 1 and expr_started == 1){
				tok = " ";
				cnd +=tok;
				tok="";

			} else if(state == 0 or state == 1){
				tok = "";
			} else if (state == 2){
				
				tok = " ";

												
			} else if(retstarted == 1 and cnd == ""){
				tok = "";
			}
		} else if(*it == '\n' or *it == '\t'){
			tok = tok.substr(0, tok.size() - 1);
			if(*it == '\n'){
				lines_counter++; 

			}

			if(functionStarted == 1 and functionNameStatus == 1 and str != ""){
				functionName = str;
				functionNameStatus = 0;
				functions.emplace(functionName, wcontent_t());
				str = "";
				tok = "";
			} else if(varstarted == 1){
				if(var != ""){
					tokens.insert(tpos, TOK_DVR2PTS_STR + var);
					varstarted = 0;
					var = "";
					tok = "";
				} else{
					fprintf(stderr,"Error: variable no declarada\n");
				}

			} else if(state == 2 and isnumber == 1){
				if(str != ""){
					tokens.insert(tpos, TOK_NUM2PTS_STR + str);
				}
				isnumber = 0;
				str = "";
				state = 0;
				tok = "";
			} else if(tok != "" and expr_started == 0 and std::regex_match(tok, std::regex(REGEX_STR))){
				if(functionarguments == 1){
					tokens.insert(tpos, TOK_ARG2PTS_STR + tok);
					argCount++;
				} else {
					tokens.insert(tpos, TOK_VAR2PTS_STR + tok);
				}
				tok = "";
			} else if(retstarted == 1 and expr_started == 1){

				if(cnd != ""){
					tokens.insert(tpos, TOK_EXP2PTS_STR + cnd);
					expr_started = 0;
					retstarted = 0;
					state = 0;
					cnd = "";
					tok = "";
				} else {
					fprintf(stderr, "Error: expresion expected in line %d \n", lines_counter);
				}

			} else if(equal_started == 1 and expr_started == 1){

				if(cnd != ""){
					tokens.insert(tpos, TOK_EXP2PTS_STR + cnd);
					expr_started = 0;
					equal_started = 0;
					state = 0;
					cnd = "";
					tok = "";
				} else {
					fprintf(stderr, "Error: Expresion expected in line %d \n", lines_counter);
				} 

			} else if(state == 1 and expr_started == 1){
				tok = " ";
			} else if(state == 0 or state == 1){
				tok = ""; 
			} else if (state == 2){
				tok = " ";
			} else if(retstarted == 1 and cnd == ""){
				tok = "";
			}
		} else if(tok == RW_FUNCTION_STR){
			if(functionStarted == 0){
				functionStarted = 1;
				functionName = "";
				functionNameStatus = 1;
				functionarguments = 1;
				tokens.clear();
			} else {
				functionStarted = -1;
			}
			tok = "";
		} else if(tok == RW_END_FUNCTION_STR){
			if(functionStarted == 1){
				functionStarted = 0;
				std::cout << functionName << std::endl;
				RNUtils::printList<std::string>(tokens);
				wcontent_t currentContent = functions.at(functionName);
				currentContent.result = ret;
				currentContent.tokens = tokens;
				currentContent.requiredArguments = argCount;
				argCount = 0;
				functions[functionName] = currentContent;
				tokens.clear();
				ifcounter = 0;
				forcounter = 0; 
				whilecounter = 0;
				tpos = tokens.end();

				if(tpos != tokens.end()){
					fprintf(stderr, "Error: Expected symbol )\n");
				}
			} else {
				functionStarted = -1;
			}
			tok = "";
		} else if(tok == RW_IF_STR){
			tokens.insert(tpos, TOK_IF_STR);
			infunction = 1;
			expr_started = 1;
			ifstarted = 1;
			tok = "";
		} else if(tok == RW_ELSE_STR){
			elsestate++;
			tpos = std::next(tpos, 1);
			tok = "";
		} else if(tok == RW_ENDIF_STR){
			if(elsestate > 0){
				elsestate--;
				tpos = std::next(tpos, 1);
			} else {
				tpos = std::next(tpos, 2);
			}
			if (expr_started == 1){
				fprintf(stderr, "Error: Expected ) in expresion \n");
			}
			tok = "";
		} else if(tok == RW_WHILE_STR){
			tokens.insert(tpos, TOK_WHILE_STR);
			infunction = 1;
			expr_started = 1;
			whilestarted = 1;
			tok = "";
		} else if(tok == RW_ENDWHILE_STR){
			tpos = std::next(tpos, 1);
			tok = "";
			if (expr_started == 1){
				fprintf(stderr, "Error: Expected ) in expresion \n");
			}

		} else if(tok == RW_FOR_STR){
			tokens.insert(tpos, TOK_FOR_STR);
			infunction = 1;
			expr_started = 1;
			forstarted = 1;
			tok= "";
		} else if(tok == RW_ENDFOR_STR){
			tpos = std::next(tpos,1);
			tok = "";
			if (expr_started == 1){
				fprintf(stderr, "Error: Expected ) in expresion \n");
			}

		}else if(tok == RW_MOVE_STR){
			tokens.insert(tpos, TOK_MOVE_STR);
			infunction = 1;
			tok = "";
		} else if(tok == RW_GOTO_STR){
			tokens.insert(tpos, TOK_GOTO_STR);
			infunction = 1;
			tok = "";
		} else if(tok == RW_SAY_STR){
			tokens.insert(tpos, TOK_SAY_STR);
			infunction = 1;
			tok = "";
		} else if(tok == RW_TURN_STR){
			tokens.insert(tpos, TOK_TURN_STR);
			infunction = 1;
			tok = "";
		} else if(tok == RW_ATTENTION_STR){
			tokens.insert(tpos, TOK_ATTENTION_STR);
			infunction = 1;
			tok = "";
		} else if(tok == RW_WAIT_STR){
			tokens.insert(tpos, TOK_WAIT_STR);
			infunction = 1;
			tok = "";
		} else if(tok == RW_VAR_STR){
			varstarted = 1;
			var = "";
			tok = "";
		} else if(tok == RW_RETURN_STR){
			tokens.insert(tpos, TOK_RETURN_STR);
			retstarted = 1;
			expr_started = 1;
			infunction = 1;
			state = 1;
			tok = "";
		} else if(tok == RW_SIZEOF_STR){
			tokens.insert(tpos, TOK_SIZEOF_STR);
			size = 1;
		} else if(tok == EVENT_ON_RFID_TAGS_DETECTED_STR or tok == EVENT_ON_SECTOR_CHANGED_STR){
			eventName = tok;
			event = 1;
			tok = "";
		} else if(tok == "(" and state == 0 and functionStarted == 1 and functionNameStatus == 1 and str != ""){
			functionName = str;
			functionNameStatus = 0;
			functions.emplace(functionName, wcontent_t());
			state = 1;
			str = "";
			tok = "";
		} else if((*it) == '(' and size == 1){
			state = 1;
			tok = "";
			expr_started = 1;
		} else if((*it) == '(' and state == 0 and tok != "" and infunction == 0 and event == 0){
			tok = tok.substr(0, tok.size() - 1);
			tokens.insert(tpos, TOK_FNC2PTS_STR + tok);
			tokens.insert(tpos, TOK_EFC_STR);
			tpos = std::prev(tpos, 1);
			function_call = 1;
			expr_started = 1;
			state = 1;
			str = "";
			tok = "";
		} else if (tok == "(" and state == 0 and infunction == 1){
			state = 1;
			tok = "";
		} else if((*it) == '(' and state == 0 and event == 1){
			state = 1;
			tok = "";

		} else if (tok == "[" and state == 0 and infunction == 1 and expr_started == 0){
			state = 1;
			tok = "";
		} else if(tok == "[" and equal_started == 1 and expr_started == 0){
			it = std::prev(it,1);
			if(*it != ' ' or *it != '='){
				current_vector = 1;
				cnd += tok;
				tok = "";
			}
			it = std::next(it,1);


		} else if(tok == "{" and equal_started == 1){
			it = std::prev(it,1);
			if(*it == ' ' or *it == '='){
				array_started = 1;
				expr_started = 0;
				tok = "";
				state = 1;
				cnd = "";
				
			}
			it = std::next(it,1);
		} else if(*it == '[' and tok != "" and expr_started == 0){
			if(get_pos == 0){
				it = std::prev(it,1);
				if(*it != ' '){
					
					tok = tok.substr(0,(tok.size()-1));
					tokens.insert(tpos, TOK_VAR2PTS_STR + tok);
					get_pos = 1;
					tok= "";
				}
				it = std::next(it,1);
			} else{
				cnd += *it;
				tok = "";
				get_pos += 1;
			}
				

		} else if(tok == "=" and state == 0){
			state = 1;
			expr_started = 1;
			equal_started = 1;
			tokens.insert(tpos, TOK_EQ_STR);
			tok = "";
		} else if (*it == '\"' and expr_started == 0 and array_started == 0){
			if(state == 1){
				state = 2;
				str += tok;
			} else if (state == 2){
				if(str != "" and str.substr(0, 1) == "\""){
					tokens.insert(tpos, TOK_STR2PTS_STR + str.substr(1, str.length() - 1));
				}
				str = "";
				state = 1;
			}
			tok = "";
		} else if(*it == '\"' and array_started == 1){
			if(state == 1){
				state = 2;
				cnd += TOK_STR2PTS_STR;
				tok = "";
			}else {
				tok = "";
				state = 1;
			}


		} else if(*it == '#' and expr_started == 0){
			if(state == 1){
				state = 2;
				isnumber = 1;
			}if(array_started == 1){

				cnd += TOK_NUM2PTS_STR;
				//cnd += *it;
			}if(get_pos >= 1){
				cnd += *it;
			} 
			tok = "";
		} else if (tok == ","){
			if(function_call == 1 and expr_started == 1 and cnd != "" ){
				
				tokens.insert(tpos, TOK_EXP2PTS_STR + cnd);
				cnd = "";
				tok = "";
			}
			if(state == 1){

				if(array_started == 0){	
					if(functionarguments == 1 ){
						if(str != ""){
							tokens.insert(tpos, TOK_ARG2PTS_STR + str);
							argCount++; 
					
						} else {
							fprintf(stderr, "Error: Expected argument in line %d \n", lines_counter);
						} 
					} else {
						if(str !=""){
							tokens.insert(tpos, TOK_VAR2PTS_STR + str);
						}
					}
					str = "";
					tok = "";
				} else {
					cnd += tok;
					tok = "";
				}
				
			} else if(state == 2){
				if(isnumber == 1){
					if(array_started == 0){
						tokens.insert(tpos, TOK_NUM2PTS_STR + str);
						isnumber = 0;
						state = 1;
						str = "";
						tok = "";
					} else {
						cnd += tok;
						state = 1;
						isnumber = 0;
						tok = "";
					}
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
					if(functionarguments == 1){
						if(str != ""){
							tokens.insert(tpos, TOK_ARG2PTS_STR + str);
							argCount++;
						}else if(str == "" and argCount > 0){
							fprintf(stderr, "Error: Expected argument in line %d \n", lines_counter);
						}
						functionarguments = 0;
					} else if(ifstarted == 1){
						if(cnd != ""){
							tokens.insert(tpos, TOK_EXP2PTS_STR + cnd);
							tokens.insert(tpos, TOK_BIF2PTS_STR + std::to_string(ifcounter));
							tokens.insert(tpos, TOK_BEL2PTS_STR + std::to_string(ifcounter));
							tokens.insert(tpos, TOK_EIF2PTS_STR + std::to_string(ifcounter++));
							tpos = std::prev(tpos, 2);
							ifstarted = 0;
							expr_started = 0;
							cnd = "";
						} else{
							fprintf(stderr, "Error: no condition in line: %d \n", lines_counter);
						}
					} else if(whilestarted == 1){
						if(cnd != ""){
							tokens.insert(tpos, TOK_EXP2PTS_STR + cnd);
							tokens.insert(tpos, TOK_BWH2PTS_STR + std::to_string(whilecounter));
							tokens.insert(tpos, TOK_EWH2PTS_STR + std::to_string(whilecounter++));
							tpos = std::prev(tpos, 1);
							whilestarted = 0;
							expr_started = 0;
							cnd = "";
						} else{
							fprintf(stderr, "Error: no condition, line: %d \n", lines_counter);
						}
					} else if(forstarted == 1){
						if( cnd != ""){
							if (double_dot_counter == 2){
								tokens.insert(tpos, TOK_EXP2PTS_STR + cnd + ")");
								tokens.insert(tpos, TOK_BFOR2PTS_STR + std::to_string(forcounter));
								tokens.insert(tpos, TOK_EFOR2PTS_STR + std::to_string(forcounter++));
								tpos = std::prev(tpos, 1);
								forstarted = 0;
								expr_started = 0;
								double_dot_counter = 0;
								cnd = "";
							} else {
								fprintf(stderr, "Error: no third condition in line %d \n", lines_counter);
								expr_started = 0;
							}
						} else{
							fprintf(stderr, "Error: No condition\n");
						}
					} else if(event == 1){
						event = 0;
						wevent_t currentContEvent = events[eventName];
						currentContEvent.eventFunction = str;
						str = "";
						events[eventName] = currentContEvent;
					} else if(size == 1){
						size = 0;
						expr_started = 0;
						tokens.insert(tpos, TOK_EXP2PTS_STR + cnd);
						cnd = "";
					} else if(str != ""){
						tokens.insert(tpos, TOK_VAR2PTS_STR + str);
					}
				} else if(state == 2){
					if(isnumber == 1){
						tokens.insert(tpos, TOK_NUM2PTS_STR + str);
						isnumber = 0;
					}
				}

				if(function_call == 1 and expr_started == 1 and cnd != ""){
					tokens.insert(tpos, TOK_EXP2PTS_STR + cnd);
					cnd = "";
					function_call = 0;
					expr_started = 0;
					tpos = std::next(tpos, 1);
				}
				infunction = 0;
				state = 0;
				str = "";
			}
			tok = "";
		} else if(tok == "]" and expr_started == 0){
			if(state == 1 or state == 2){
				if(array_started == 0 and current_vector == 0){
					tokens.insert(tpos, TOK_OPT2PTS_STR + str);
					state = 0;
					tok = "";
					str = "";	
				} else if (current_vector == 1){
					cnd += tok;
					tok = "";
				}
			}
		} else if(tok == "}"){
			if(array_started == 1){
					array_started = 0;
					tokens.insert(tpos, TOK_ARR2PTS_STR + cnd );
					tok="";
					cnd ="";
					state = 0;
				}

		} else if(*it == ']' and expr_started == 0){
			if(get_pos == 1){

				tokens.insert(tpos, TOK_POS2PTS_STR + cnd);
				cnd = "";
				get_pos = 0; 
				tok = "";
			} else{
				cnd += *it;
				get_pos -= 1;
			}


		} else if(functionStarted == 1 and functionNameStatus == 1){
			str += tok;
			tok = "";
		} else if(state > 0){
			if((expr_started == 1 or array_started == 1) and tok != ":"){
				if(tok == "(" or tok == " ("){
					parcount++;
				}
				/*if(*it == '0' or *it == '1' or *it == '2' or *it == '3' or *it == '4' or *it == '5' or *it == '6' or *it == '7' or *it == '8' or *it == '9'){
					it = std::prev(it,1);
					if( *it != '#' and *it != '0'  and *it != '1' and *it != '2' and *it != '3' and *it != '4' and *it != '5' and *it != '6' and *it != '7' and *it != '8' and *it != '9'){
						fprintf(stderr, "Error: Expected # symbol in line %d \n", lines_counter ); //MIO
					}
					it = std::next(it,1);
				}*/
				cnd += tok;
				tok = "";
			} else if(tok == "#"){
				tok = "";
				if(isnumber == 0){
					isnumber = 1;
				}
			} else if(expr_started == 1 and tok == ":" and forstarted == 1) {
				if (double_dot_counter == 0){
					if(cnd != "" or cnd !=" "){
					tokens.insert(tpos, TOK_EXP2PTS_STR + cnd);
					cnd ="";
					tok ="";
					} else{
						fprintf(stderr,"Error: no first condition in line %d \n", lines_counter);
					}
				} else if(double_dot_counter == 1 ){
					if(cnd != "" or cnd != " "){
						tokens.insert(tpos, TOK_EXP2PTS_STR + cnd);
						cnd = "";
						tok = "";
					} else{
						fprintf(stderr, "Error: no second condition in line %d\n", lines_counter );
					}
				}
				double_dot_counter = double_dot_counter + 1;
			
			} else {
				str += tok;
				tok = "";
			}
		} else if(get_pos >= 1){
			cnd += *it;
			
		} else if (varstarted == 1){
			var += tok;
			tok = "";
		} else if(state == 0 and expr_started == 1 and infunction ==1 and tok != "(" and tok != " "){ // FUNCION MIA me sirve para ver que me no venga nada raro detras de un if while...
			fprintf(stderr, "Error: Expected ( in line %d \n", lines_counter);

		} else if(state == 0 and functionarguments == 1 and functionNameStatus == 0 and tok != "(" and tok != " "){
			fprintf(stderr, "Error: Expected ( in line %d \n", lines_counter);
		
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
	}else {
		printf("FUNCTION MAIN NOT FOUND\n");
	}
}




void RNTourDialog::parse(std::string functionName, wcontent_t* content){
	std::list<std::string>::iterator argumentsIdx = content->arguments.begin();
	std::list<std::string> functionTokens = content->tokens;
	std::map<std::string, std::string>* functionSymbols = &content->symbols;
	//RNUtils::printList<std::string>(functionTokens);
	std::list<std::string>::iterator it = functionTokens.begin();
	std::stack<std::string> ifIdsStack;
	std::stack<std::string> whileIdsStack;
	std::stack<std::string> forIdsStack;
	std::vector<int>  vect_for = {0,0,0,0,0};
	while(it != functionTokens.end()){
		std::list<std::string>::iterator it2 = std::next(it, 1);
		std::list<std::string>::iterator it3 = std::next(it, 2);
		std::list<std::string>::iterator it4 = std::next(it, 3);
		if((*it) == TOK_MOVE_STR){
			if(it2 != functionTokens.end()){
				if((*it2).substr(0, 3) == TOK_STR_STR){
					printf("MOVE TO COMMAND\n");
					it = std::next(it, 2);
				} else if((*it2).substr(0, 3) == TOK_VAR_STR){
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
		} else if((*it) == TOK_GOTO_STR){
			int sector;
			float px, py;
			if((*it2).substr(0, 3) == TOK_NUM_STR and (*it3).substr(0, 3) == TOK_NUM_STR and (*it4).substr(0, 3) == TOK_NUM_STR){
				sector = std::stoi((*it2).substr(4));
				px = std::stof((*it3).substr(4));
				py = std::stof((*it4).substr(4));
			} else if((*it2).substr(0, 3) == TOK_NUM_STR and (*it3).substr(0, 3) == TOK_NUM_STR and (*it4).substr(0, 3) == TOK_VAR_STR){
				sector = std::stoi((*it2).substr(4));
				px = std::stof((*it3).substr(4));
				if(functionSymbols->find((*it4).substr(4)) != functionSymbols->end()){
					py = std::stof(functionSymbols->at((*it4).substr(4)).substr(4));
				}
			} else if((*it2).substr(0, 3) == TOK_NUM_STR and (*it3).substr(0, 3) == TOK_VAR_STR and (*it4).substr(0, 3) == TOK_NUM_STR){
				sector = std::stoi((*it2).substr(4));
				if(functionSymbols->find((*it3).substr(4)) != functionSymbols->end()){
					px = std::stof(functionSymbols->at((*it3).substr(4)).substr(4));
				}
				py = std::stof((*it4).substr(4));
			} else if((*it2).substr(0, 3) == TOK_NUM_STR and (*it3).substr(0, 3) == TOK_VAR_STR and (*it4).substr(0, 3) == TOK_VAR_STR){
				sector = std::stoi((*it2).substr(4));
				if(functionSymbols->find((*it3).substr(4)) != functionSymbols->end()){
					px = std::stof(functionSymbols->at((*it3).substr(4)).substr(4));
				}
				if(functionSymbols->find((*it4).substr(4)) != functionSymbols->end()){
					py = std::stof(functionSymbols->at((*it4).substr(4)).substr(4));
				}
			} else if((*it2).substr(0, 3) == TOK_VAR_STR and (*it3).substr(0, 3) == TOK_NUM_STR and (*it4).substr(0, 3) == TOK_NUM_STR){
				if(functionSymbols->find((*it2).substr(4)) != functionSymbols->end()){
					sector = std::stoi(functionSymbols->at((*it2).substr(4)).substr(4));
				}
				px = std::stof((*it3).substr(4));
				py = std::stof((*it4).substr(4));
			} else if((*it2).substr(0, 3) == TOK_VAR_STR and (*it3).substr(0, 3) == TOK_NUM_STR and (*it4).substr(0, 3) == TOK_VAR_STR){
				if(functionSymbols->find((*it2).substr(4)) != functionSymbols->end()){
					sector = std::stoi(functionSymbols->at((*it2).substr(4)).substr(4));
				}
				px = std::stof((*it3).substr(4));
				if(functionSymbols->find((*it4).substr(4)) != functionSymbols->end()){
					py = std::stof(functionSymbols->at((*it4).substr(4)).substr(4));
				}
			} else if((*it2).substr(0, 3) == TOK_VAR_STR and (*it3).substr(0, 3) == TOK_VAR_STR and (*it4).substr(0, 3) == TOK_NUM_STR){
				if(functionSymbols->find((*it2).substr(4)) != functionSymbols->end()){
					sector = std::stoi(functionSymbols->at((*it2).substr(4)).substr(4));
				}
				if(functionSymbols->find((*it3).substr(4)) != functionSymbols->end()){
					px = std::stof(functionSymbols->at((*it3).substr(4)).substr(4));
				}
				py = std::stof((*it4).substr(4));
			} else if((*it2).substr(0, 3) == TOK_VAR_STR and (*it3).substr(0, 3) == TOK_VAR_STR and (*it4).substr(0, 3) == TOK_VAR_STR){
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
		} else if((*it) == TOK_TURN_STR){
			if(it2 != functionTokens.end()){
				if((*it2).substr(0, 3) == TOK_NUM_STR){
					printf("TURN %s DEGREES COMMAND\n", (*it2).substr(4).c_str());
					it = std::next(it, 2);
				}
			} 
		} else if((*it).substr(0, 3) == TOK_FNC_STR){ // NO ENTIENDO PORQUE USA substr Y EN EL RESTO NO
			printf("CALL FUNCTION\n");
			std::string nextFunctionName = (*it).substr(4);
			std::map<std::string, wcontent_t>::iterator fit_call;
			fit_call = functions.find(nextFunctionName);
			if(fit_call != functions.end()){
				wcontent_t content = fit_call->second;
				std::list<std::string>::iterator itArgs = std::next(it, 1);
				while(*itArgs != TOK_EFC_STR){
					std::cout << (*itArgs).substr(4) << std::endl;
					RNUtils::printMap<std::string, std::string>(*functionSymbols);
					content.arguments.emplace_back(evaluateExpression((*itArgs).substr(4), *functionSymbols));
					itArgs++;
				}
				
				if(content.requiredArguments != content.arguments.size()){
					fprintf(stderr, "error: no matching funtion to call to %s. expects %d arguments, provided %d.\n", (*it).substr(4).c_str(), content.requiredArguments, content.arguments.size());
				} else {
					parse(nextFunctionName, &content);
					it = itArgs;
				}
				fit_call->second = content;
			} else {
				printf("FUNCTION %s NOT FOUND\n", nextFunctionName.c_str());
			}
			it = std::next(it, 1);			
		} else if((*it) == TOK_SAY_STR){
			if(it2 != functionTokens.end()){
				if((*it2).substr(0, 3) == TOK_STR_STR){
					printf("SAY STRING: %s\n", (*it2).substr(4).c_str());
					//lips->textToViseme((*it2).substr(4));
					it = std::next(it, 2);
				} else if((*it2).substr(0, 3) == TOK_VAR_STR){
					if(functionSymbols->find((*it2).substr(4)) != functionSymbols->end()){
						printf("SAY VAR: %s\n", functionSymbols->at((*it2).substr(4)).c_str());
						//lips->textToViseme((*it2).substr(4));
					} else {
						fprintf(stderr, "Unidefined VAR: %s\n", (*it2).substr(4).c_str());
					}
					it = std::next(it, 2);
				} else if((*it2).substr(0, 3) == TOK_OPT_STR){
					if(it3 != functionTokens.end() and (*it3).substr(0, 3) == TOK_STR_STR){
						printf("SAY STRING %s WITH OPTIONS: %s\n", (*it3).substr(4).c_str(), (*it2).substr(4).c_str());
						std::map<std::string, std::string> opts = createOptionsMap((*it2).substr(4));
						processOptions(opts);
						//lips->textToViseme((*it3).substr(4));
						it = std::next(it, 3);
					}
				}
			}
		} else if((*it).substr(0, 3) == TOK_ARG_STR){
			if(argumentsIdx != content->arguments.end()){
				std::cout << *argumentsIdx << std::endl;
				functionSymbols->emplace((*it).substr(4), *argumentsIdx);
				argumentsIdx++;
			} else {
				fprintf(stderr, "error: invalid number of arguments in function %s\n", functionName.c_str());
				exit(-1);
			}
			it++;
		} else if((*it).substr(0, 3) == TOK_DVR_STR){
			if(it2 != functionTokens.end() and it3 != functionTokens.end()){
				if((*it2) == TOK_EQ_STR and (*it3).substr(0, 3) == TOK_EXP_STR){
					//printf("DECLARED VAR %s WITH INITIALIZATION\n", (*it).substr(4).c_str());
					std::string r = evaluateExpression((*it3).substr(4), *functionSymbols);
					if(functionSymbols->find((*it).substr(4)) == functionSymbols->end()){
						functionSymbols->emplace((*it).substr(4), r);
					} else {
						functionSymbols->at((*it).substr(4)) = r;
					}
					it = std::next(it, 3);
				} else if((*it2) == TOK_EQ_STR and (*it3).substr(0, 3) == TOK_ARR_STR){
					//printf("DECLARED ARRAY VAR %s\n", (*it).substr(4).c_str());
					if(functionSymbols->find((*it).substr(4)) == functionSymbols->end()){
						functionSymbols->emplace((*it).substr(4),*it3);
					}
					it = std::next(it, 3);
				} else if((*it2).substr(0,3) == TOK_POS_STR and (*it3).substr(0,3) == TOK_EQ_STR and (*it4).substr(0,3) == TOK_EXP_STR){
					std::string cadena;
					if(functionSymbols->find((*it).substr(4)) == functionSymbols->end()){
					cadena = assignArray(*functionSymbols, (*it).substr(4), (*it2).substr(4), (*it4).substr(4));
					functionSymbols->emplace((*it).substr(4), cadena);
					}else {
						functionSymbols->at((*it).substr(4)) = cadena;
					}

				}else {
					//printf("DECLARED VAR %s\n", (*it).substr(4).c_str());
					if(functionSymbols->find((*it).substr(4)) == functionSymbols->end()){
						functionSymbols->emplace((*it).substr(4), "UNK:nil");
					} else {
						fprintf(stderr, "VAR: %s, already exists\n", (*it).substr(4).c_str());
					}
					it++;
				}
			} else {
				//printf("DECLARED VAR %s\n", (*it).substr(4).c_str());
				if(functionSymbols->find((*it).substr(4)) == functionSymbols->end()){
					functionSymbols->emplace((*it).substr(4), "UNK:nil");
				} else {
					fprintf(stderr, "VAR: %s, already exists\n", (*it).substr(4).c_str());
				}
				it++;
			}
		} else if((*it).substr(0, 3) == TOK_VAR_STR){
			if(functionSymbols->find((*it).substr(4)) != functionSymbols->end()){
				if((*it2) == TOK_EQ_STR and (*it3).substr(0, 3) == TOK_EXP_STR){
					std::string r = evaluateExpression((*it3).substr(4), *functionSymbols);
					functionSymbols->at((*it).substr(4)) = r;
					it = std::next(it, 3);
				} else if((*it2) == TOK_EQ_STR and (*it3).substr(0, 3) == TOK_ARR_STR){
					if(functionSymbols->find((*it).substr(4)) == functionSymbols->end()){
						functionSymbols->emplace((*it).substr(4),*it3);
					}
					it = std::next(it, 3);
				} else if((*it2).substr(0,3) == TOK_POS_STR and (*it3).substr(0,3) == TOK_EQ_STR and (*it4).substr(0,3) == TOK_EXP_STR){
						std::string cadenados;
						cadenados = assignArray(*functionSymbols, (*it).substr(4), (*it2).substr(4), (*it4).substr(4));
						functionSymbols->at((*it).substr(4)) = cadenados;

						it = std::next(it, 4);
				}
			} else {
				fprintf(stderr, "Undefined VAR: %s\n", (*it).substr(4).c_str());
				it++;
			}
		} else if((*it) == TOK_RETURN_STR){
			if(it2 != functionTokens.end() and (*it2).substr(0, 3) == TOK_EXP_STR){
				std::string r = evaluateExpression((*it2).substr(4), *functionSymbols);
				functions.at(functionName).result = r;
			}
			it = functionTokens.end();
		} else if((*it) == TOK_IF_STR){
			std::string r = evaluateExpression((*it2).substr(4), *functionSymbols); //hacer recursive descent parser

			if(r == TOK_TRUE){
				it = std::next(it, 3);
				ifIdsStack.push((*it3).substr(4));
			} else {
				std::string elsename = TOK_BEL2PTS_STR + (*it3).substr(4);
				while((*it) != elsename){
					it++;
				}
			}
		} else if((*it).substr(0, 3) == TOK_BEL_STR){
			if(not ifIdsStack.empty()){
				if(ifIdsStack.top() == (*it).substr(4)){
					std::string elsename = TOK_EIF2PTS_STR + ifIdsStack.top();
					while((*it) != elsename){
						it++;
					}
					ifIdsStack.pop();
				}
			} else {
				it++;
			}
		} else if((*it).substr(0, 3) == TOK_EIF_STR){
			if(not ifIdsStack.empty()){
				if(ifIdsStack.top() == (*it).substr(4)){
					ifIdsStack.pop();
				}
			}
			it++;
		} else if(*it == TOK_WHILE_STR){
			std::string r = evaluateExpression((*it2).substr(4), *functionSymbols);
			if(r == TOK_TRUE){
				it = std::next(it, 3);
				whileIdsStack.push((*it3).substr(4));
			} else {
				std::string endwname = TOK_EWH2PTS_STR + (*it3).substr(4);
				while((*it) != endwname){
					it++;
				}
			}
		} else if((*it).substr(0, 3) == TOK_EWH_STR){
			if(not whileIdsStack.empty()){
				if(whileIdsStack.top() == (*it).substr(4)){
					whileIdsStack.pop();
					std::string bwhname = TOK_BWH2PTS_STR + (*it).substr(4);
					while((*it) != bwhname){
						it = std::prev(it);
					}
					it = std::prev(it, 2);
				}
			} else {
				it++;
			}
		}else if(*it == "FOR"){

			std::list<std::string>::iterator it5 = std::next(it, 4);
			std::vector<std::string> for_var;

			int pos_for;

			if(not forIdsStack.empty()){

			
				std::string for_cont = forIdsStack.top();
				int num = std::stof(for_cont);
			 	pos_for = num + 1;

			}else{
				pos_for = 0;

			}
			if(vect_for[pos_for] == 0){
		

				std::string r1 = evaluateExpression((*it2).substr(4), *functionSymbols);
				for_var = RNUtils::split(r1, ",");
				

				if(functionSymbols->find(for_var[1]) == functionSymbols->end()){
						functionSymbols->emplace(for_var[1], for_var[0]);
				} else {
					
					functionSymbols->at(for_var[1]) = for_var[0];
				
				}

				std::string r2 = evaluateExpression((*it3).substr(4), *functionSymbols);
				std::string r3 = evaluateExpression((*it4).substr(4), *functionSymbols);
				for_var = RNUtils::split(r3, ",");

				if(functionSymbols->find(for_var[1]) == functionSymbols->end()){
						functionSymbols->emplace(for_var[1], for_var[0]);
					} else {
						functionSymbols->at(for_var[1]) = for_var[0];
					}


				if(r2 == TOK_TRUE){

				it = std::next(it,5);
				forIdsStack.push((*it5).substr(5));
				
			

				} else {
					std::string endforname = TOK_EFOR2PTS_STR + (*it5).substr(5);
					while((*it) != endforname){
						it++;
					}
				}
			
			} else {
				std::string r2 = evaluateExpression((*it3).substr(4), *functionSymbols);
				std::cout << r2 << '\n';
				std::string r3 = evaluateExpression((*it4).substr(4), *functionSymbols);
				for_var = RNUtils::split(r3, ",");
				if(functionSymbols->find(for_var[1]) == functionSymbols->end()){
						functionSymbols->emplace(for_var[1], for_var[0]);
					} else {
						functionSymbols->at(for_var[1]) = for_var[0];
					}


				if(r2 == TOK_TRUE){
					it = std::next(it,5);
					forIdsStack.push((*it5).substr(5));
					
					


				}else{
					std::string endforname = TOK_EFOR2PTS_STR + (*it5).substr(5);
					vect_for[std::stof((*it5).substr(5))] = 0;
					
				
					while((*it) != endforname){

					it++;
					}

					it++;
				}
			}
		} else if((*it).substr(0,4) == "EFOR"){

			if(not forIdsStack.empty()){
				if(forIdsStack.top() == (*it).substr(5)){
					vect_for[std::stof((*it).substr(5))] += 1;
					forIdsStack.pop();
					std::string bforname = TOK_BFOR2PTS_STR + (*it).substr(5);
					while((*it) != bforname){
						it = std::prev(it);
					}
					it = std::prev(it, 4);
				}
			} else {
				it++;
			}

		}else if((*it) == "SIZEOF"){
			if((*it2).substr(0,3) == TOK_EXP_STR){
				std::string size = evaluateExpression((*it2).substr(4),*functionSymbols);
				assignSize(size);

			}else{
				fprintf(stderr, "Error: no es lo que se espera\n");
			}
			it = std::next(it, 2);
		}else {
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
		if((*itokens).substr(0, 3) == TOK_VAR_STR){
			
			std::list<std::string>::iterator it2 = std::next(itokens, 1);
			std::map<std::string, std::string>::iterator var = symbols.find((*itokens).substr(4));
			if(var != symbols.end()){
				if(it2 == tokens.end() or ((*it2).substr(0, 3) != TOK_POS_STR and (*it2) != TOK_EQ_STR)){
					*itokens = var->second;

				} else if((*it2) == TOK_EQ_STR){
					
					std::list<std::string>::iterator it3 = std::next(itokens, 2);
					std::string rad = evaluateExpression((*it3).substr(4), symbols);
					it3 = tokens.erase(it3);
					it2 = tokens.erase(it2);
					std::string chain = rad + "," + (*itokens).substr(4);
					*itokens = chain;
				}

			} else {
				fprintf(stderr, "error: Undefined variable %s in this scope\n", (*itokens).substr(4).c_str());
				ok_sofar = false;
			}
		
		} else if((*itokens).substr(0, 3) == TOK_POS_STR){	
			std::cout << "entra aqui en la reverga esta..." << std::endl;
			std::list<std::string>::iterator it2 = std::prev(itokens,1); //MIO
			
			std::map<std::string, std::string>::iterator var_array = symbols.find((*it2).substr(4));
			
			if(var_array != symbols.end()){
				std::cout << "Va a buscar en el array... " << std::endl << (*it2).substr(4) << ", la posicion " << (*itokens).substr(4) << std::endl;
				*it2 = arrayValue(symbols, (*it2).substr(4), (*itokens).substr(4)); // FUNCION POR DEFINIR
				//itokens = tokens.erase(itokens); // BORRAR LO QUE ESTABA MARCANDO it2
				itokens = tokens.erase(itokens);
				//std::cout << *it2 << std::endl ;
			}else {
				fprintf(stderr, "error: Undefined variable %s in this scope\n", (*it2).substr(4).c_str());
				ok_sofar = false;


			}		
		 } else if((*itokens) == TOK_SIZEOF_STR){
		 	std::list<std::string>::iterator it3 = std::next(itokens,1);

		 	if((*it3).substr(0,3) == TOK_EXP_STR){
		 		
		 		std::string resize = evaluateExpression((*it3).substr(4), symbols);
		 		(*itokens) = assignSize(resize);
		 		it3 = tokens.erase(it3);


		 	} else{
		 		fprintf(stderr, "error: Unexpected reference\n");
		 	}

		} else if((*itokens).substr(0, 3) == TOK_FNC_STR){
			std::map<std::string, wcontent_t>::iterator itFnc;
			itFnc = functions.find((*itokens).substr(4));
			if(itFnc != functions.end()){
				wcontent_t content = itFnc->second;

				std::list<std::string>::iterator itArgs = std::next(itokens, 1);
				while(*itArgs != TOK_EFC_STR){
					content.arguments.emplace_back(evaluateExpression((*itArgs).substr(4), symbols));
					itArgs++;
				}
				
				if(content.requiredArguments != content.arguments.size()){
					fprintf(stderr, "error: no matching funtion to call to %s. expects %d arguments, provided %d.\n", (*itokens).substr(4).c_str(), content.requiredArguments, content.arguments.size());
				} else {
					parse(itFnc->first, &content);
				}

				if(functions.at(itFnc->first).result != TOK_VOID_STR){
					tokens.insert(itokens, functions.at(itFnc->first).result);
					tokens.erase(itokens, std::next(itArgs, 1));
					itokens = tokens.begin();
				} else {
					fprintf(stderr, "error: Function %s does not return value\n", (*itokens).substr(4).c_str());
				}
				
			} else {
				fprintf(stderr, "error: Undefined function %s in this scope\n", (*itokens).substr(4).c_str());
				ok_sofar = false;
			}
		}
	}

	if(tokens.size() > 1 ){
		solveExpParenthesis(&tokens);
	}
	if(tokens.size() > 1){
		solveExp(&tokens);
	}
	result = tokens.front();

	return result;
}

void RNTourDialog::solveExpParenthesis(std::list<std::string>* tokens){

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
			if(*itTokens == "*" or *itTokens == "/" or *itTokens == "%" or *itTokens == TOK_AND_STR){
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
					if(op1.substr(0, 3) == TOK_NUM_STR){
						float res;
						if(oper == "*"){
							res = std::stof(op1.substr(4)) * std::stof(op2.substr(4));
						} else if(oper == "/"){
							res = std::stof(op1.substr(4)) / std::stof(op2.substr(4));
						} else if(oper == "%"){
							res = (float)(static_cast<int>(std::stof(op1.substr(4))) % static_cast<int>(std::stof(op2.substr(4))));
						} else if(oper == TOK_AND_STR){
							res = std::stof(op1.substr(4)) and std::stof(op2.substr(4));
						}
						result = TOK_NUM2PTS_STR + std::to_string(res);
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
	while(trm){
		trm = false;
		std::list<std::string>::iterator itTokens = tokens->begin();
		int idx_start = RN_NONE, idx_end = RN_NONE, idx = 0;
		std::string op1 = "", op2 = "", oper = "", result = "";
		while(itTokens != tokens->end()){
			if(*itTokens == "+" or *itTokens == "-" or *itTokens == TOK_OR_STR){
				trm = true;
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
		if(trm){
			if(op1 != "" and op2 != "" and oper != ""){
				if(op1.substr(0, 3) == op2.substr(0, 3)){
					if(op1.substr(0, 3) == TOK_NUM_STR){
						float res;
						if(oper == "+"){
							res = std::stof(op1.substr(4)) + std::stof(op2.substr(4));
						} else if(oper == "-"){
							res = std::stof(op1.substr(4)) - std::stof(op2.substr(4));
						} else if(oper == TOK_OR_STR){
							res = std::stof(op1.substr(4)) or std::stof(op2.substr(4));
						}
						result = TOK_NUM2PTS_STR + std::to_string(res);
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
	RNUtils::printList<std::string>(*tokens);
	while(se){
		se = false;
		std::list<std::string>::iterator itTokens = tokens->begin();
		int idx_start = RN_NONE, idx_end = RN_NONE, idx = 0;
		std::string op1 = "", op2 = "", oper = "", result = "";
		while(itTokens != tokens->end()){
			if(*itTokens == TOK_EQU_STR or *itTokens == TOK_NEQ_STR or *itTokens == TOK_LT_STR or *itTokens == TOK_LEQ_STR or *itTokens == TOK_GT_STR or *itTokens == TOK_GEQ_STR){
				se = true;
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
		if(se){
			if(op1 != "" and op2 != "" and oper != ""){
				if(op1.substr(0, 3) == op2.substr(0, 3)){
					int res;
					if(oper == TOK_EQU_STR){
						if(op1.substr(0, 3) == TOK_NUM_STR){
							res = std::stof(op1.substr(4)) == std::stof(op2.substr(4));
						} else {
							res = op1.substr(4) == op2.substr(4);
						}
					} else if(oper == TOK_NEQ_STR){
						if(op1.substr(0, 3) == TOK_NUM_STR){
							res = std::stof(op1.substr(4)) != std::stof(op2.substr(4));
						} else {
							res = op1.substr(4) != op2.substr(4);
						}
					} else if(oper == TOK_LT_STR){
						if(op1.substr(0, 3) == TOK_NUM_STR){
							res = std::stof(op1.substr(4)) < std::stof(op2.substr(4));
						} else {
							res = op1.substr(4) < op2.substr(4);
						}
					} else if(oper == TOK_LEQ_STR){
						if(op1.substr(0, 3) == TOK_NUM_STR){
							res = std::stof(op1.substr(4)) <= std::stof(op2.substr(4));
						} else {
							res = op1.substr(4) <= op2.substr(4);
						}
					} else if(oper == TOK_GT_STR){
						if(op1.substr(0, 3) == TOK_NUM_STR){
							res = std::stof(op1.substr(4)) > std::stof(op2.substr(4));
						} else {
							res = op1.substr(4) > op2.substr(4);
						}
					} else if(oper == TOK_GEQ_STR){
						if(op1.substr(0, 3) == TOK_NUM_STR){
							res = std::stof(op1.substr(4)) >= std::stof(op2.substr(4));
						} else {
							res = op1.substr(4) >= op2.substr(4);
						}
					}
					result = TOK_NUM2PTS_STR + std::to_string(res);
					
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
	std::cout << expr_cond << std::endl;
	std::list<std::string> tokens;
	std::string tok = "";
	std::string str = "";
	std::string cnd = "";
	int state = 0;
	int evalfunc = 0;
	int expr_started = 0;
	int cont_vect = 0;
	int val_size = 0;
	int eq_started = 0;
	
	while(it != expr_cond.end()){
		if(tok == RW_AND_STR and (*it == '(' or *it == ')' or *it == '\"' or *it == '#')){
			tokens.emplace_back(TOK_AND_STR);
			tok = "";
		} else if(tok == RW_OR_STR and (*it == '(' or *it == ')' or *it == '\"' or *it == '#')){
			tokens.emplace_back(TOK_OR_STR);
			tok = "";
		} else if(tok == RW_NOT_STR and (*it == '(' or *it == ')' or *it == '\"' or *it == '#')){
			tokens.emplace_back(TOK_NOT_STR);
			tok = "";
		} 

		if(expr_started == 1){
			
			if(eq_started == 0){
				tok += *it;
				if(*it == ' '){
					tok = " ";
				} else if(*it == ','){
					tokens.emplace_back(TOK_EXP2PTS_STR + cnd);
					cnd = "";
				} else if(*it == ')'){
					tokens.emplace_back(TOK_EXP2PTS_STR + cnd);
					expr_started = 0;
					if(evalfunc == 1){
						tokens.emplace_back(TOK_EFC_STR);
						evalfunc = 0;
					}
				}else if(*it == ']' and cont_vect > 0){ 
					
				
					cnd += tok;
					tok = "";
					cont_vect = cont_vect - 1;
					

					
				}else if( *it == ']' and cont_vect == 0){
						tokens.emplace_back("POS:" + cnd);
					    expr_started = 0;
						cnd = "";
						

					
				}else if(*it == '['){
					cnd += tok;
					tok = "";
					cont_vect += 1;



				}else {
					cnd += tok;
					tok = ""; //MIO 
				}
			}else{
				if(*it != ':' and *it != ')'){
					
					tok += *it;
					
				}else{
					
					tokens.emplace_back(TOK_EXP2PTS_STR + tok);
					expr_started = 0;
					eq_started = 0;
				}
			}
		} else {
			if(*it == ' '){
				if(state == 2){
					if(str != ""){
						tokens.emplace_back(TOK_NUM2PTS_STR + str);
					}
					str = "";
					state = 0;
					tok = "";
				} else if(tok != "" and (tok != RW_AND_STR and tok != RW_OR_STR and tok != RW_NOT_STR) and std::regex_match(tok, std::regex(REGEX_STR))){
					tokens.emplace_back(TOK_VAR2PTS_STR + tok);
				} else if(tok == RW_AND_STR){
					tokens.emplace_back(TOK_AND_STR);
				} else if(tok == RW_OR_STR){
					tokens.emplace_back(TOK_OR_STR);
				} else if(tok == RW_NOT_STR){
					tokens.emplace_back(TOK_NOT_STR);
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
						tokens.emplace_back(TOK_STR2PTS_STR + str);
					}
					str = "";
					state = 0;
				}
				tok = "";
			}/*else if(*it == '='){
				tokens.emplace_back("EQ");
				expr_started = 1;
				eq_started = 1;

			}*/ else if(*it == '('){
				if(tok != ""){
					if(tok != RW_SIZEOF_STR){
						tokens.emplace_back(TOK_FNC2PTS_STR + tok);
						expr_started = 1;
						evalfunc = 1;
					}else{
						tokens.emplace_back(TOK_SIZEOF_STR);
						expr_started = 1;
					}
				} else {
					tokens.emplace_back("(");
				}
				tok = "";
			
			} else if(*it == ')'){
				if(evalfunc == 1){
					tokens.emplace_back(TOK_EFC_STR);
					evalfunc = 0;
				} else {
					if(tok != "" and std::regex_match(tok, std::regex(REGEX_STR))){
						tokens.emplace_back(TOK_VAR2PTS_STR + tok);
					} else if(state == 2){
						if(str != ""){
							tokens.emplace_back(TOK_NUM2PTS_STR + str);
						}
						str = "";
						state = 0;
					}
					tokens.emplace_back(")");
				}
				tok = "";
			} else if(state == 0 and (*it == '+' or *it == '-' or *it == '*' or *it == '/' or *it == '%')){
				
				if(tok != "" or str != ""){
						if(state == 2){
							if(str != ""){
								tokens.emplace_back(TOK_NUM2PTS_STR + str);
							}
							str = "";
							state = 0;
						} else if(tok != "" and std::regex_match(tok, std::regex(REGEX_STR))){
							tokens.emplace_back(TOK_VAR2PTS_STR + tok);
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
							tokens.emplace_back(TOK_NUM2PTS_STR + str);
						}
						str = "";
						state = 0;
					} else if(tok != "" and std::regex_match(tok, std::regex(REGEX_STR))){
						tokens.emplace_back(TOK_VAR2PTS_STR + tok);
					}
					tok = "";
				}
				std::string::iterator it2 = std::next(it, 1);
				if(*it2 == '='){
					if(*it == '='){
						tokens.emplace_back(TOK_EQU_STR);
						tok = "";
					} else if(*it == '!'){
						tokens.emplace_back(TOK_NEQ_STR);
						tok = "";
					} else if(*it == '<'){
						tokens.emplace_back(TOK_LEQ_STR);
						tok = "";
					} else if(*it == '>'){
						tokens.emplace_back(TOK_GEQ_STR);
						tok = "";
					}
					it++;
				} else {
					if(*it == '='){
						
					} else if(*it == '!'){
						
					} else if(*it == '<'){
						tokens.emplace_back(TOK_LT_STR);
						tok = "";
					} else if(*it == '>'){
						tokens.emplace_back(TOK_GT_STR);
						tok = "";
					}
				}
			} else if(state > 0){ 
				str += *it;
				tok = "";
				

			}else if(*it == '[' and tok != ""){
				tokens.emplace_back(TOK_VAR2PTS_STR + tok);
				tok = "";						
				expr_started = 1;

			
			}else if(state == 0){
				if(*it == '0' or *it == '1' or *it == '2' or *it == '3' or *it == '4' or *it == '5' or *it == '6' or *it == '7' or *it == '8' or *it == '9' ){
				fprintf(stderr, "Error: Expected # symbol before number \n" );


				} else{
					tok += *it; 
				}
			}
		}
		it++;
	}	
	if(tok != "" or str != ""){
		if(tok != "" and std::regex_match(tok, std::regex(REGEX_STR))){
			tokens.emplace_back(TOK_VAR2PTS_STR + tok);
		} else if(state == 2){
			if(str != ""){
				tokens.emplace_back(TOK_NUM2PTS_STR + str);
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
		if(optsIt->first == RW_FACE_STR){
			op = globalSymbols.find(TOK_FACE2PTS_STR + optsIt->second);
			if(op != opts.end()){
				//head->setGesture(op->second.substr(3));
			}
		} else if(optsIt->first == RW_ATTENTION_STR){
			op = globalSymbols.find(TOK_ATTN2PTS_STR + optsIt->second);
			if(op != opts.end()){
				//head->setGesture(op->second.substr(3));
			}
		}
	}
}

std::string RNTourDialog::arrayValue(std::map<std::string, std::string> simbolos, std::string nombre, std::string position){

	std::string contenido;
	std::map<std::string, std::string>::iterator busca = simbolos.find(nombre);
	std::string valor = NULLPTR;
	std::string indice;
	std::vector<std::string> cont_indices;
	
	int res;

	indice = evaluateExpression(position, simbolos);
	if(indice.substr(0,3) == TOK_STR_STR){
		fprintf(stderr,"Posicion no valida\n");
	} else{
		res = std::stoi(indice.substr(4));
	}	


	if(busca != simbolos.end()){
		
		contenido = busca->second;
		cont_indices = RNUtils::split(contenido.substr(4), ",");
	} else {
		fprintf(stderr, "error: Undefined variable in this scope\n");
	}

	if(res < cont_indices.size()){
		valor = cont_indices[res];
	} else{
		fprintf(stderr, "error: index out of bounds\n");
	}
	return valor;
}

std::string RNTourDialog::assignArray(std::map<std::string, std::string> simbolo, std::string array, std::string pos, std::string exprs){

	std::string contenido;
	std::map<std::string, std::string>::iterator busca = simbolo.find(array);
	std::string valors;
	std::string indices;
	std::vector<std::string> cont_array;
	float res;
	int i = 0;
	float tam = 0;
	std::string asigna;
	std::string cad_total;
	asigna = evaluateExpression(exprs, simbolo);

	indices = evaluateExpression(pos, simbolo);
	if(indices.substr(0,3) == "STR"){
		fprintf(stderr,"Posicion no valida\n");
	}else{
		res = std::stof(indices.substr(4));
	}	

	if(busca != simbolo.end()){

			contenido = busca -> second;
			if(contenido != "UNK:nil"){
				cont_array = RNUtils::split(contenido, ",");
		   	
				tam = cont_array.size() -1;


			   	if(res <= tam){

				   	cont_array[res] = asigna;
				    
				   	for(i = 0; i <= tam; i++){
				   		if(i < tam){

				   		cad_total += cont_array[i] + ",";

				   		}else{

				   			cad_total += cont_array[i];
				   			
				   		}
				   		
				   	}
			  		
			  		std::cout << cad_total << '\n' ;
			   	
			  		return cad_total;
			  		
			   	}else{
				    
				   	for(i = 0; i <= tam; i++){				   		

				   		cad_total += cont_array[i] + ",";				   						   		
				   	}

				   	for(i = tam +1; i <= res; i++){
				   		if(i < res){
				   			cad_total += "NUM:0," ;
				   		}else{
				   			cad_total += asigna;
				   			std::cout << cad_total << std::endl ;
				   		}
				   	}
			   		
			   }
   			}else{
   				for(i = 0; i<= res; ++i){
   					if(i < res){
   					cad_total += "NUM:0,";
   					}else{
   						cad_total += asigna;

   					} 
   				}
   				std::cout << cad_total << '\n' ;

   				
   			}


   }else {
		fprintf(stderr, "error: Undefined variable in this scope\n");
	}

  
  
}

std::string RNTourDialog::assignSize(std::string word_size){
	std::string chain = word_size;
	std::vector<std::string> cont_size;
	cont_size = RNUtils::split(chain, ",");
	float len = 0;
	int size = 0;
	int i = 0;
	std::string val;
	len = cont_size.size() - 1;
	if(len == 0){
		if(chain.substr(0,3) == TOK_STR_STR){
			size = chain.substr(4).size();
			fprintf(stderr,"The length of the string is %i \n", size);

		} else if(chain.substr(0,3) == TOK_NUM_STR) {
			size = 1;
			fprintf(stderr,"The length of the string is %i \n", size);
		} else if(chain.substr(0, 3) == TOK_ARR_STR){
			size = 1;
		}
	}else{
		for(i = 0; i <=len ; i++){
			size++;
		}
		fprintf(stderr,"The length of the vector is %i \n", size);
	}
	val =  TOK_NUM2PTS_STR + std::to_string(size);
	std::cout << "-----> SIZEOF: " << word_size << "Size: " << val << std::endl;
	return val;
}