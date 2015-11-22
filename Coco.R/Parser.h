

#if !defined(COCO_PARSER_H__)
#define COCO_PARSER_H__



#include "Scanner.h"



class Errors {
public:
	int count;			// number of errors detected

	Errors();
	void SynErr(int line, int col, int n);
	void Error(int line, int col, const wchar_t *s);
	void Warning(int line, int col, const wchar_t *s);
	void Warning(const wchar_t *s);
	void Exception(const wchar_t *s);

}; // Errors

class Parser {
private:
	enum {
		_EOF=0,
		_N=1,
		_LogOperator=2,
		_MathMultiOperator=3,
		_MathSumOperator=4,
		_Logic=5
	};
	int maxT;

	Token *dummyToken;
	int errDist;
	int minErrDist;

	void SynErr(int n);
	void Get();
	void Expect(int n);
	bool StartOf(int s);
	void ExpectWeak(int n, int follow);
	bool WeakSeparator(int n, int syFol, int repFol);

public:
	Scanner *scanner;
	Errors  *errors;

	Token *t;			// last recognized token
	Token *la;			// lookahead token



	Parser(Scanner *scanner);
	~Parser();
	void SemErr(const wchar_t* msg);

	void Programm();
	void Imports();
	void Variables();
	void Operators();
	void Library();
	void Variable();
	void Name();
	void Integer();
	void Type();
	void Operator();
	void Cin();
	void Cout();
	void Assignment();
	void Cycle();
	void Ramification();
	void Methods();
	void Expression();
	void SimpleExpression();
	void Term();
	void Factor();
	void Realistic();
	void WhileDo();
	void RepeatUntil();
	void For();

	void Parse();

}; // end Parser



#endif

