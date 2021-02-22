//	<><><><><><><><><><><><><>  PlotCalc.h  <><><><><><><><><><><><><><> 
//
// -----------------------------------------------------------------
// (C) Copyright Clark Dailey, Inc. 2006
//
// PlotCalc is a calculator class for user definable plotting functions
//
// Allows input of a mathematical equation via "Basic"-like syntax
// with x,y,z parameters.  The calculator can then be called with
// values for x,y,z.
// 
// A Reverse Polish Notation (RPN) calculator language is used
// similar to the FORTH stack-based computer language
//
// Examples:
//   arithetic expression              RPN postfix instruction list
//   --------------------              ----------------------------           
//   sin(x*x+y*y+z*z));                "x;x;*;y;y;*;z;z;*;+;+;sin;"          
//   atan(y/(x-1))-atan(y/(x+1));      "y;x;1;-;/;atan;y;x;1;+;/;atan;-;";   
//
// Notes:
//   1) white space (spaces,new-lines,tabs,etc) is ignored
//   2) case sensitivity is ignored
//
// -----------------------------------------------------------------

#ifndef	_PLOTCALC_H_	 	// avoid repeated including
#define	_PLOTCALC_H_	1	// has been included


#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "memory.h"
#include "time.h"
#include "math.h"

// --------------------------------------------------------------
#define	PC_STACK_SIZE			200		// stack size
#define PC_MAX_INSTRUCTIONS		1000	// maximum program size
#define	PC_MAX_CONSTANTS		255		// maximum constants
#define	PC_MAX_TOKEN_LEN		20		// maximum mnemonic length
#define	PC_END_OF_INSTRUCTION	';'		// instruction separator (must end with this)
#define	PC_NO_USER_CONSTANTS	8		// number of user constants


// --------------------------------------------------------------
// Calculator Instruction Op-Codes
enum PcOpCode
{
     PC_OP_CONSTANT=0,	// constant (special case)
     PC_OP_OPEN_PAREN=1,// open parenthesis
     PC_OP_CLOSE_PAREN,	// close parenthesis
     PC_OP_COMMA,		// comma
     PC_OP_ADD, 		// addition
     PC_OP_SUB,			// subtraction
     PC_OP_MULT,		// multiplication
     PC_OP_DIV,			// division
     PC_OP_POW,			// raised to a power
     PC_OP_POW2,  		// ^ 2 (optimization)
     PC_OP_POW3,  		// ^ 3 (optimization)
     PC_OP_POW4,  		// ^ 4 (optimization)
     PC_OP_NEG,  		// negation
     PC_OP_PI,			// PI constant (3.14159...)
     PC_OP_E,			// E constant  (2.71828...)
     PC_OP_X,			// put x value onto stack
     PC_OP_Y,			// put y value onto stack
     PC_OP_Z,			// put z value onto stack
     PC_OP_U,			// put u value onto stack (alias for x)
     PC_OP_V,			// put v value onto stack (alias for y)
	 PC_OP_C1,			// user contant #1
	 PC_OP_C2,			// user contant #2
	 PC_OP_C3,			// user contant #3
	 PC_OP_C4,			// user contant #4
	 PC_OP_C5,			// user contant #5
	 PC_OP_C6,			// user contant #6
	 PC_OP_C7,			// user contant #7
	 PC_OP_C8,			// user contant #8
	 PC_OP_SETC1,		// set user contant #1
	 PC_OP_SETC2,		// set user contant #2
	 PC_OP_SETC3,		// set user contant #3
	 PC_OP_SETC4,		// set user contant #4
	 PC_OP_SETC5,		// set user contant #5
	 PC_OP_SETC6,		// set user contant #6
	 PC_OP_SETC7,		// set user contant #7
	 PC_OP_SETC8,		// set user contant #8
     PC_OP_SQRT,		// square root
     PC_OP_ABS,			// absolute value
     PC_OP_SIN,			// trig sine
     PC_OP_COS,			// trig cosine
     PC_OP_TAN,			// trig tangent
     PC_OP_ASIN,		// arc sine
     PC_OP_ACOS,		// arc cosine
     PC_OP_ATAN,		// arc tangent
     PC_OP_SINH,		// hyperbolic sine
     PC_OP_COSH,  		// hyperbolic cosine
     PC_OP_TANH,		// hyperbolic tangent  
     PC_OP_EXP,			// exponent
     PC_OP_LN,			// natural logarithm (base E)
     PC_OP_LOG10,		// common logarithm (base 10)
     PC_OP_CEIL,		// ceiling
     PC_OP_FLOOR,		// floor
     PC_OP_RAND,		// random number
	 PC_OP_MIN,			// minimum
	 PC_OP_MAX,			// maximum
	 PC_OP_ATAN2,		// atan2
	 PC_OP_HYPOT,		// hypotenus
	 PC_OP_SIGN,		// sign

     PC_OP_END_INPUT,	// end input (special case)
     PC_OP_START_FUNC,	// start function (special case)
};


// --------------------------------------------------------------
// Error flags
#define	PC_ERR_NONE				0x0000
#define	PC_ERR_STACK_OVERFLOW   0x0001
#define	PC_ERR_STACK_UNDERFLOW  0x0002
#define PC_ERR_DIVIDE_BY_ZERO   0x0004
#define PC_ERR_NEGATIVE_SQRT	0x0008
#define PC_ERR_OUT_OF_RANGE		0x0010


// --------------------------------------------------------------
enum PcOpType
{
	PC_OP_CONTROL = 1,
	PC_OP_OPERATOR,
	PC_OP_OPERAND,
	PC_OP_FUNCTION,
	PC_OP_INTERNAL
};

// --------------------------------------------------------------
// Calculator Instruction
typedef struct PcInstr_s
{
	PcOpCode	opcode;		// op-code number
	PcOpType	optype;		// op-type number
	char*		mnemonic;	// human readable string
	unsigned char nParms; 	// number of parameters (for function only)
} PcInstr;

// --------------------------------------------------------------
class PlotCalc
{
public:
	// constructor
	PlotCalc();

	// initialize new program
	void NewProgram();

	// destructor
	~PlotCalc() { }

	// Load program
	// returns: 0=ok, 1=invalid pointer, 2=invalid program
	int LoadProgram(char* label,const char* pgm,int allow_z,
		 const char* pc1,const char* pc2,const char* pc3,const char* pc4,
		 const char* pc5,const char* pc6,const char* pc7,const char* pc8,int use_uv=0);

	// calculate
	// if using u,v;  u=x,v=y
	double Calc(double x,double y,double z=0.0);

	// retrieve error flags
	unsigned GetErrFlags() const { return(m_err_flags); }

	// get pointer to error string
	char* GetErrString() { return(m_sErr); }

	// get program instruction
	// no - instruction number to get (zero based)
	// buf - buffer to received instruction into (dimension to 100)
	// returns: 0=ok, 1=invalid instruction number
	int GetProgramInst(unsigned no,char* buf);

protected:
	// add program instruction
	// returns: 0=ok, 1=too many instructions
	int AddInst(PcOpCode inst,unsigned char operand);

	// delete one or more program instructions
	// returns: 0=ok, 1=invalid ranges
	// zero based instruction numbers; first and last are included in deletion
	int DelInsts(unsigned first_inst_no,unsigned last_inst_no);

	// set a constant value
	// number: 1-255, value=constant
	// returns: 0=ok, 1=number out of range
	int SetConstant(unsigned number,double value);

	// increment stack pointer
	void IncSp()	// inline for speed
	{
		if (m_sp>=PC_STACK_SIZE)
			m_err_flags |= PC_ERR_STACK_OVERFLOW;
		else
			m_sp++;
	}

	// decrement stack pointer
	void DecSp()	// inline for speed
	{
		if (m_sp<1)
			m_err_flags |= PC_ERR_STACK_UNDERFLOW;
		else
			m_sp--;
	}

	// returns: 0=ok, 1=stack overflow
	int PushOp(PcOpCode opcode, PcOpCode subcode);

	// returns: 0=ok, 1=stack underflow
	int PopOp(PcOpCode* opcode, PcOpCode* subcode);

	// returns: 0=ok, 1=stack underflow
	int PeekOp(PcOpCode* opcode, PcOpCode* subcode);

	// returns: 0=ok, 1=stack overflow
	int PushFunc(unsigned char CurParms, unsigned char ReqParms, unsigned char GotOperand);

	// returns: 0=ok, 1=stack underflow
	int PopFunc(unsigned char* CurParms, unsigned char* ReqParms, unsigned char* GotOperand);

	// determine operator precedence
	int OpPrecedence(PcOpCode opcode);

	// returns: 0=no, 1=it is an operator
	int IsOperator(PcOpCode opcode);

	// determine if byte is a token separator
	// returns: 0=no, 1=yes (and opcode is valid)
	int IsTokenSeparator(char byte,PcOpCode* pCode);

	// determine if byte is white space
	// returns: 0=no, 1=yes
	int IsWhiteSpace(char byte);

protected:	// data
	double        m_c1,m_c2,m_c3,m_c4,m_c5,m_c6,m_c7,m_c8;	// user definable constants
	int			  m_sp;			// data stack pointer
	int			  m_opsp;		// operator stack pointer
	int			  m_fsp;		// function stack pointer
	unsigned char m_cur_parms;	// current number of function parameters
	unsigned char m_req_parms;	// required number of parameters
	unsigned char m_got_operand;// 0=no, 1=got an operand now
	unsigned	  m_err_flags;	// error flags
	unsigned	  m_nInst;		// number of program instructions
	double		  m_dstack[PC_STACK_SIZE+1];		// data stack (one based)
	double		  m_dconstants[PC_MAX_CONSTANTS+1];	// data constants (one based)
	unsigned char m_opcodes[PC_MAX_INSTRUCTIONS+1];	// program instructions
	unsigned char m_operands[PC_MAX_INSTRUCTIONS+1];// instruction operand
	PcOpCode      m_opstack[PC_STACK_SIZE+1][2];	// operator stack (one based)
	unsigned char m_fstack[PC_STACK_SIZE+1][3];		// function stack (one based)
	char		  m_sErr[128];						// error string
};

#endif	/* if _PLOTCALC_H_ included */

/*	<><><><><><><><><><><><><>  PlotCalc.h  <><><><><><><><><><><><><><> */
