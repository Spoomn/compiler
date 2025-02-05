#include "Token.h"
#include "Scanner.h"
#include "StateMachine.h"
#include <iostream>

int main(){
	ScannerClass scanner("code.txt");
	TokenType tt;

	do {
		TokenClass tc = scanner.GetNextToken();
		std::cout << "Line " << scanner.GetLineNumber() << ":\n";
		std::cout << tc << std::endl;
		tt = tc.GetTokenType();
	} while (tt != ENDFILE_TOKEN);
	std::cout << "Total lines: " << scanner.GetLineNumber();

}
