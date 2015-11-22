

#include <wchar.h>
#include "Parser.h"
#include "Scanner.h"




void Parser::SynErr(int n) {
	if (errDist >= minErrDist) errors->SynErr(la->line, la->col, n);
	errDist = 0;
}

void Parser::SemErr(const wchar_t* msg) {
	if (errDist >= minErrDist) errors->Error(t->line, t->col, msg);
	errDist = 0;
}

void Parser::Get() {
	for (;;) {
		t = la;
		la = scanner->Scan();
		if (la->kind <= maxT) { ++errDist; break; }

		if (dummyToken != t) {
			dummyToken->kind = t->kind;
			dummyToken->pos = t->pos;
			dummyToken->col = t->col;
			dummyToken->line = t->line;
			dummyToken->next = NULL;
			coco_string_delete(dummyToken->val);
			dummyToken->val = coco_string_create(t->val);
			t = dummyToken;
		}
		la = t;
	}
}

void Parser::Expect(int n) {
	if (la->kind==n) Get(); else { SynErr(n); }
}

void Parser::ExpectWeak(int n, int follow) {
	if (la->kind == n) Get();
	else {
		SynErr(n);
		while (!StartOf(follow)) Get();
	}
}

bool Parser::WeakSeparator(int n, int syFol, int repFol) {
	if (la->kind == n) {Get(); return true;}
	else if (StartOf(repFol)) {return false;}
	else {
		SynErr(n);
		while (!(StartOf(syFol) || StartOf(repFol) || StartOf(0))) {
			Get();
		}
		return StartOf(syFol);
	}
}

void Parser::Programm() {
		Expect(6 /* "module" */);
		Expect(7 /* "main;" */);
		Imports();
		Expect(8 /* "var" */);
		Variables();
		Expect(9 /* "begin" */);
		Operators();
		Expect(10 /* "end" */);
		Expect(11 /* "main" */);
}

void Parser::Imports() {
		Expect(12 /* "import" */);
		Library();
		Expect(13 /* ";" */);
}

void Parser::Variables() {
		Variable();
		while (StartOf(1)) {
			Variable();
		}
}

void Parser::Operators() {
		Operator();
		while (StartOf(1)) {
			Operator();
		}
}

void Parser::Library() {
		Expect(14 /* "in,stdlog" */);
		if (la->kind == 15 /* ",math" */) {
			Get();
		}
}

void Parser::Variable() {
		Name();
		Expect(16 /* ":" */);
		if (la->kind == 17 /* "array" */) {
			Get();
			Integer();
			Expect(18 /* "of" */);
		}
		Type();
		Expect(13 /* ";" */);
}

void Parser::Name() {
		Get();
}

void Parser::Integer() {
		Expect(_N);
		while (la->kind == _N) {
			Get();
		}
}

void Parser::Type() {
		if (la->kind == 19 /* "integer" */) {
			Get();
		} else if (la->kind == 20 /* "real" */) {
			Get();
		} else if (la->kind == 21 /* "boolean" */) {
			Get();
		} else SynErr(45);
}

void Parser::Operator() {
		if (la->kind == 22 /* "in." */) {
			Cin();
		} else if (la->kind == 26 /* "stdlog." */) {
			Cout();
		} else if (StartOf(1)) {
			Assignment();
		} else if (la->kind == 34 /* "while(" */ || la->kind == 36 /* "repeat" */ || la->kind == 38 /* "for" */) {
			Cycle();
		} else if (la->kind == 41 /* "if" */) {
			Ramification();
		} else SynErr(46);
		Expect(13 /* ";" */);
}

void Parser::Cin() {
		Expect(22 /* "in." */);
		if (la->kind == 23 /* "open" */) {
			Get();
		} else if (StartOf(2)) {
			Methods();
			Expect(24 /* "(" */);
			Name();
			Expect(25 /* ")" */);
		} else SynErr(47);
}

void Parser::Cout() {
		Expect(26 /* "stdlog." */);
		if (StartOf(2)) {
			Methods();
			Expect(24 /* "(" */);
			Name();
			Expect(25 /* ")" */);
		} else if (la->kind == 27 /* "ln" */) {
			Get();
		} else SynErr(48);
}

void Parser::Assignment() {
		Name();
		Expect(31 /* ":=" */);
		Expression();
}

void Parser::Cycle() {
		if (la->kind == 34 /* "while(" */) {
			WhileDo();
		} else if (la->kind == 36 /* "repeat" */) {
			RepeatUntil();
		} else if (la->kind == 38 /* "for" */) {
			For();
		} else SynErr(49);
}

void Parser::Ramification() {
		Expect(41 /* "if" */);
		Expression();
		Expect(42 /* "then" */);
		Operators();
		if (la->kind == 43 /* "else" */) {
			Get();
			Operators();
		}
		Expect(10 /* "end" */);
}

void Parser::Methods() {
		if (la->kind == 28 /* "int" */) {
			Get();
		} else if (la->kind == 20 /* "real" */) {
			Get();
		} else if (la->kind == 29 /* "bool" */) {
			Get();
		} else if (la->kind == 30 /* "string" */) {
			Get();
		} else SynErr(50);
}

void Parser::Expression() {
		SimpleExpression();
		if (la->kind == _LogOperator) {
			Get();
			SimpleExpression();
		}
}

void Parser::SimpleExpression() {
		if (la->kind == _MathSumOperator) {
			Get();
		}
		Term();
		if (la->kind == _MathSumOperator) {
			Get();
			Term();
		}
}

void Parser::Term() {
		Factor();
		if (la->kind == _MathMultiOperator) {
			Get();
			Factor();
		}
}

void Parser::Factor() {
		if (la->kind == 24 /* "(" */) {
			Get();
			Expression();
			Expect(25 /* ")" */);
		} else if (StartOf(1)) {
			Name();
		} else if (la->kind == _N) {
			Integer();
		} else if (la->kind == _Logic) {
			Get();
		} else if (la->kind == _N) {
			Realistic();
		} else if (la->kind == 32 /* "~" */) {
			Get();
			Factor();
		} else SynErr(51);
}

void Parser::Realistic() {
		Integer();
		if (la->kind == 33 /* "." */) {
			Get();
			Integer();
		}
}

void Parser::WhileDo() {
		Expect(34 /* "while(" */);
		Expression();
		Expect(35 /* ")do" */);
		Operators();
		Expect(10 /* "end" */);
}

void Parser::RepeatUntil() {
		Expect(36 /* "repeat" */);
		Operators();
		Expect(37 /* "until(" */);
		Expression();
		Expect(25 /* ")" */);
}

void Parser::For() {
		Expect(38 /* "for" */);
		Assignment();
		Expect(39 /* "to" */);
		Integer();
		Expect(40 /* "do" */);
		Operators();
		Expect(10 /* "end" */);
}




// If the user declared a method Init and a mehtod Destroy they should
// be called in the contructur and the destructor respctively.
//
// The following templates are used to recognize if the user declared
// the methods Init and Destroy.

template<typename T>
struct ParserInitExistsRecognizer {
	template<typename U, void (U::*)() = &U::Init>
	struct ExistsIfInitIsDefinedMarker{};

	struct InitIsMissingType {
		char dummy1;
	};
	
	struct InitExistsType {
		char dummy1; char dummy2;
	};

	// exists always
	template<typename U>
	static InitIsMissingType is_here(...);

	// exist only if ExistsIfInitIsDefinedMarker is defined
	template<typename U>
	static InitExistsType is_here(ExistsIfInitIsDefinedMarker<U>*);

	enum { InitExists = (sizeof(is_here<T>(NULL)) == sizeof(InitExistsType)) };
};

template<typename T>
struct ParserDestroyExistsRecognizer {
	template<typename U, void (U::*)() = &U::Destroy>
	struct ExistsIfDestroyIsDefinedMarker{};

	struct DestroyIsMissingType {
		char dummy1;
	};
	
	struct DestroyExistsType {
		char dummy1; char dummy2;
	};

	// exists always
	template<typename U>
	static DestroyIsMissingType is_here(...);

	// exist only if ExistsIfDestroyIsDefinedMarker is defined
	template<typename U>
	static DestroyExistsType is_here(ExistsIfDestroyIsDefinedMarker<U>*);

	enum { DestroyExists = (sizeof(is_here<T>(NULL)) == sizeof(DestroyExistsType)) };
};

// The folloing templates are used to call the Init and Destroy methods if they exist.

// Generic case of the ParserInitCaller, gets used if the Init method is missing
template<typename T, bool = ParserInitExistsRecognizer<T>::InitExists>
struct ParserInitCaller {
	static void CallInit(T *t) {
		// nothing to do
	}
};

// True case of the ParserInitCaller, gets used if the Init method exists
template<typename T>
struct ParserInitCaller<T, true> {
	static void CallInit(T *t) {
		t->Init();
	}
};

// Generic case of the ParserDestroyCaller, gets used if the Destroy method is missing
template<typename T, bool = ParserDestroyExistsRecognizer<T>::DestroyExists>
struct ParserDestroyCaller {
	static void CallDestroy(T *t) {
		// nothing to do
	}
};

// True case of the ParserDestroyCaller, gets used if the Destroy method exists
template<typename T>
struct ParserDestroyCaller<T, true> {
	static void CallDestroy(T *t) {
		t->Destroy();
	}
};

void Parser::Parse() {
	t = NULL;
	la = dummyToken = new Token();
	la->val = coco_string_create(L"Dummy Token");
	Get();
	Programm();
	Expect(0);
}

Parser::Parser(Scanner *scanner) {
	maxT = 44;

	ParserInitCaller<Parser>::CallInit(this);
	dummyToken = NULL;
	t = la = NULL;
	minErrDist = 2;
	errDist = minErrDist;
	this->scanner = scanner;
	errors = new Errors();
}

bool Parser::StartOf(int s) {
	const bool T = true;
	const bool x = false;

	static bool set[3][46] = {
		{T,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x},
		{x,T,T,T, T,T,T,T, T,T,T,T, T,T,T,T, T,T,T,T, T,T,T,T, T,T,T,T, T,T,T,T, T,T,T,T, T,T,T,T, T,T,T,T, T,x},
		{x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, T,x,x,x, x,x,x,x, T,T,T,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x}
	};



	return set[s][la->kind];
}

Parser::~Parser() {
	ParserDestroyCaller<Parser>::CallDestroy(this);
	delete errors;
	delete dummyToken;
}

Errors::Errors() {
	count = 0;
}

void Errors::SynErr(int line, int col, int n) {
	wchar_t* s;
	switch (n) {
			case 0: s = coco_string_create(L"EOF expected"); break;
			case 1: s = coco_string_create(L"N expected"); break;
			case 2: s = coco_string_create(L"LogOperator expected"); break;
			case 3: s = coco_string_create(L"MathMultiOperator expected"); break;
			case 4: s = coco_string_create(L"MathSumOperator expected"); break;
			case 5: s = coco_string_create(L"Logic expected"); break;
			case 6: s = coco_string_create(L"\"module\" expected"); break;
			case 7: s = coco_string_create(L"\"main;\" expected"); break;
			case 8: s = coco_string_create(L"\"var\" expected"); break;
			case 9: s = coco_string_create(L"\"begin\" expected"); break;
			case 10: s = coco_string_create(L"\"end\" expected"); break;
			case 11: s = coco_string_create(L"\"main\" expected"); break;
			case 12: s = coco_string_create(L"\"import\" expected"); break;
			case 13: s = coco_string_create(L"\";\" expected"); break;
			case 14: s = coco_string_create(L"\"in,stdlog\" expected"); break;
			case 15: s = coco_string_create(L"\",math\" expected"); break;
			case 16: s = coco_string_create(L"\":\" expected"); break;
			case 17: s = coco_string_create(L"\"array\" expected"); break;
			case 18: s = coco_string_create(L"\"of\" expected"); break;
			case 19: s = coco_string_create(L"\"integer\" expected"); break;
			case 20: s = coco_string_create(L"\"real\" expected"); break;
			case 21: s = coco_string_create(L"\"boolean\" expected"); break;
			case 22: s = coco_string_create(L"\"in.\" expected"); break;
			case 23: s = coco_string_create(L"\"open\" expected"); break;
			case 24: s = coco_string_create(L"\"(\" expected"); break;
			case 25: s = coco_string_create(L"\")\" expected"); break;
			case 26: s = coco_string_create(L"\"stdlog.\" expected"); break;
			case 27: s = coco_string_create(L"\"ln\" expected"); break;
			case 28: s = coco_string_create(L"\"int\" expected"); break;
			case 29: s = coco_string_create(L"\"bool\" expected"); break;
			case 30: s = coco_string_create(L"\"string\" expected"); break;
			case 31: s = coco_string_create(L"\":=\" expected"); break;
			case 32: s = coco_string_create(L"\"~\" expected"); break;
			case 33: s = coco_string_create(L"\".\" expected"); break;
			case 34: s = coco_string_create(L"\"while(\" expected"); break;
			case 35: s = coco_string_create(L"\")do\" expected"); break;
			case 36: s = coco_string_create(L"\"repeat\" expected"); break;
			case 37: s = coco_string_create(L"\"until(\" expected"); break;
			case 38: s = coco_string_create(L"\"for\" expected"); break;
			case 39: s = coco_string_create(L"\"to\" expected"); break;
			case 40: s = coco_string_create(L"\"do\" expected"); break;
			case 41: s = coco_string_create(L"\"if\" expected"); break;
			case 42: s = coco_string_create(L"\"then\" expected"); break;
			case 43: s = coco_string_create(L"\"else\" expected"); break;
			case 44: s = coco_string_create(L"??? expected"); break;
			case 45: s = coco_string_create(L"invalid Type"); break;
			case 46: s = coco_string_create(L"invalid Operator"); break;
			case 47: s = coco_string_create(L"invalid Cin"); break;
			case 48: s = coco_string_create(L"invalid Cout"); break;
			case 49: s = coco_string_create(L"invalid Cycle"); break;
			case 50: s = coco_string_create(L"invalid Methods"); break;
			case 51: s = coco_string_create(L"invalid Factor"); break;

		default:
		{
			wchar_t format[20];
			coco_swprintf(format, 20, L"error %d", n);
			s = coco_string_create(format);
		}
		break;
	}
	wprintf(L"-- line %d col %d: %ls\n", line, col, s);
	coco_string_delete(s);
	count++;
}

void Errors::Error(int line, int col, const wchar_t *s) {
	wprintf(L"-- line %d col %d: %ls\n", line, col, s);
	count++;
}

void Errors::Warning(int line, int col, const wchar_t *s) {
	wprintf(L"-- line %d col %d: %ls\n", line, col, s);
}

void Errors::Warning(const wchar_t *s) {
	wprintf(L"%ls\n", s);
}

void Errors::Exception(const wchar_t* s) {
	wprintf(L"%ls", s); 
	exit(1);
}


