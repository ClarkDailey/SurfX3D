//	<><><><><><><><><><><><><>  PlotCalc.cpp  <><><><><><><><><><><><><><> 
//
// -----------------------------------------------------------------
// (C) Copyright Clark Dailey, Inc. 2006
//
// PlotCalc is a calculator class for user definable plotting functions
//
// -----------------------------------------------------------------

#include "stdafx.h"
#include "stdlib.h"
#include "plotcalc.h"

// --------------------------------------------------------------
// Supported Instructions
static const PcInstr g_Inst[] =
{
	// CAUTION! this order needs to match the order of PcOpCode enum
	{ PC_OP_CONSTANT,     PC_OP_OPERAND,      0,       0 }, // not used (padding for one based)
	{ PC_OP_OPEN_PAREN,   PC_OP_CONTROL,      "(",     0 },
	{ PC_OP_CLOSE_PAREN,  PC_OP_CONTROL,      ")",     0 },
	{ PC_OP_COMMA,        PC_OP_CONTROL,      ",",     0 },
	{ PC_OP_ADD,          PC_OP_OPERATOR,     "+",     0 },
	{ PC_OP_SUB,          PC_OP_OPERATOR,     "-",     0 },
	{ PC_OP_MULT,         PC_OP_OPERATOR,     "*",     0 },
	{ PC_OP_DIV,          PC_OP_OPERATOR,     "/",     0 },
	{ PC_OP_POW,          PC_OP_OPERATOR,     "^",     0 },
	{ PC_OP_POW2,         PC_OP_INTERNAL,     "^2",    0 }, // optimization
	{ PC_OP_POW3,         PC_OP_INTERNAL,     "^3",    0 }, // optimization
	{ PC_OP_POW4,         PC_OP_INTERNAL,     "^4",    0 }, // optimization
	{ PC_OP_NEG,          PC_OP_INTERNAL,     "neg",   0 },
	{ PC_OP_PI,           PC_OP_OPERAND,      "pi",    0 },
	{ PC_OP_E,            PC_OP_OPERAND,      "e",     0 },
    { PC_OP_X,            PC_OP_OPERAND,      "x",     0 },
    { PC_OP_Y,            PC_OP_OPERAND,      "y",     0 },
    { PC_OP_Z,            PC_OP_OPERAND,      "z",     0 },
    { PC_OP_U,            PC_OP_OPERAND,      "u",     0 }, // an alias for x
    { PC_OP_V,            PC_OP_OPERAND,      "v",     0 }, // an alias for y
    { PC_OP_C1,           PC_OP_OPERAND,      "c1",    0 },	// c1-c8 user definable constants
    { PC_OP_C2,           PC_OP_OPERAND,      "c2",    0 },
    { PC_OP_C3,           PC_OP_OPERAND,      "c3",    0 },
    { PC_OP_C4,           PC_OP_OPERAND,      "c4",    0 },
    { PC_OP_C5,           PC_OP_OPERAND,      "c5",    0 },
    { PC_OP_C6,           PC_OP_OPERAND,      "c6",    0 },
    { PC_OP_C7,           PC_OP_OPERAND,      "c7",    0 },
    { PC_OP_C8,           PC_OP_OPERAND,      "c8",    0 },
    { PC_OP_SETC1,        PC_OP_INTERNAL,     "c1=",   0 },	// set c1-c8 user constants
    { PC_OP_SETC2,        PC_OP_INTERNAL,     "c2=",   0 },
    { PC_OP_SETC3,        PC_OP_INTERNAL,     "c3=",   0 },
    { PC_OP_SETC4,        PC_OP_INTERNAL,     "c4=",   0 },
    { PC_OP_SETC5,        PC_OP_INTERNAL,     "c5=",   0 },
    { PC_OP_SETC6,        PC_OP_INTERNAL,     "c6=",   0 },
    { PC_OP_SETC7,        PC_OP_INTERNAL,     "c7=",   0 },
    { PC_OP_SETC8,        PC_OP_INTERNAL,     "c8=",   0 },
    { PC_OP_SQRT,         PC_OP_FUNCTION,     "sqrt",  1 },
    { PC_OP_ABS,          PC_OP_FUNCTION,     "abs",   1 },
    { PC_OP_SIN,          PC_OP_FUNCTION,     "sin",   1 },
    { PC_OP_COS,          PC_OP_FUNCTION,     "cos",   1 },
    { PC_OP_TAN,          PC_OP_FUNCTION,     "tan",   1 },
    { PC_OP_ASIN,         PC_OP_FUNCTION,     "asin",  1 },
    { PC_OP_ACOS,         PC_OP_FUNCTION,     "acos",  1 },
    { PC_OP_ATAN,         PC_OP_FUNCTION,     "atan",  1 },
    { PC_OP_SINH,         PC_OP_FUNCTION,     "sinh",  1 },
    { PC_OP_COSH,         PC_OP_FUNCTION,     "cosh",  1 },
    { PC_OP_TANH,         PC_OP_FUNCTION,     "tanh",  1 },
    { PC_OP_EXP,          PC_OP_FUNCTION,     "exp",   1 },
    { PC_OP_LN,           PC_OP_FUNCTION,     "ln",    1 },
    { PC_OP_LOG10,        PC_OP_FUNCTION,     "log10", 1 },
    { PC_OP_CEIL,         PC_OP_FUNCTION,     "ceil",  1 },
    { PC_OP_FLOOR,        PC_OP_FUNCTION,     "floor", 1 },
    { PC_OP_RAND,         PC_OP_FUNCTION,     "rand",  0 },
    { PC_OP_MIN,          PC_OP_FUNCTION,     "min",   2 },
    { PC_OP_MAX,          PC_OP_FUNCTION,     "max",   2 },
    { PC_OP_ATAN2,        PC_OP_FUNCTION,     "atan2", 2 },
    { PC_OP_HYPOT,        PC_OP_FUNCTION,     "hypot", 2 },
    { PC_OP_SIGN,         PC_OP_FUNCTION,     "sign",  1 },
};

#define	TOTAL_OP_CODES	(sizeof(g_Inst)/sizeof(g_Inst[0]))

#define	CONSTANT_TOLERANCE	(1.e-10) // tolerance allowed for constant equality

// add program instruction
// returns: 0=ok, 1=too many instructions
int PlotCalc::AddInst(PcOpCode inst,unsigned char operand)
{
	double c;
	if (m_nInst>=PC_MAX_INSTRUCTIONS)
		return(1);
	// toss any non-executable instructions
	if (inst==PC_OP_OPEN_PAREN || inst==PC_OP_CLOSE_PAREN || inst==PC_OP_COMMA) return(0);
	if (inst==PC_OP_POW)
	{
		// optimize for ^2, ^3, ^4
		if (m_nInst && m_opcodes[m_nInst-1]==PC_OP_CONSTANT)
		{
			c = m_dconstants[m_operands[m_nInst-1]];
			if (fabs(c-2.)<CONSTANT_TOLERANCE)
			{
				m_opcodes[ m_nInst-1] = (unsigned char)PC_OP_POW2;
				m_operands[m_nInst-1] = 0;
				return(0);
			}
			if (fabs(c-3.)<CONSTANT_TOLERANCE)
			{
				m_opcodes[ m_nInst-1] = (unsigned char)PC_OP_POW3;
				m_operands[m_nInst-1] = 0;
				return(0);
			}
			if (fabs(c-4.)<CONSTANT_TOLERANCE)
			{
				m_opcodes[ m_nInst-1] = (unsigned char)PC_OP_POW4;
				m_operands[m_nInst-1] = 0;
				return(0);
			}
		}
	}
	if (inst>=PC_OP_SETC1 && inst<=PC_OP_SETC8)
	{
		// optimize for constant user constant
		if (m_nInst && m_opcodes[m_nInst-1]==PC_OP_CONSTANT)
		{
			switch(inst)
			{
			case PC_OP_SETC1: m_c1 = m_dconstants[m_operands[m_nInst-1]]; break;
			case PC_OP_SETC2: m_c2 = m_dconstants[m_operands[m_nInst-1]]; break;
			case PC_OP_SETC3: m_c3 = m_dconstants[m_operands[m_nInst-1]]; break;
			case PC_OP_SETC4: m_c4 = m_dconstants[m_operands[m_nInst-1]]; break;
			case PC_OP_SETC5: m_c5 = m_dconstants[m_operands[m_nInst-1]]; break;
			case PC_OP_SETC6: m_c6 = m_dconstants[m_operands[m_nInst-1]]; break;
			case PC_OP_SETC7: m_c7 = m_dconstants[m_operands[m_nInst-1]]; break;
			case PC_OP_SETC8: m_c8 = m_dconstants[m_operands[m_nInst-1]]; break;
			}
			m_nInst--;
			return(0);
		}
	}
	m_opcodes[ m_nInst] = (unsigned char)inst;
	m_operands[m_nInst] = operand;
	m_nInst++;
	return(0);
}

// --------------------------------------------------------------
int OpIndex(PcOpCode opcode)
{
	unsigned i;
	for (i=0; i<TOTAL_OP_CODES; i++)
	{
		if (opcode == g_Inst[i].opcode) return(i);
	}
	return(0);
}

// --------------------------------------------------------------
// Load program
// returns: 0=ok, else pointer to error in string
int PlotCalc::LoadProgram(char* label,const char* pgm,int allow_z,
		const char* pc1,const char* pc2,const char* pc3,const char* pc4,
		const char* pc5,const char* pc6,const char* pc7,const char* pc8,int use_uv)
{
	PcOpCode this_opcode, last_opcode;
	PcOpCode opcode2, subcode;
	int  i, len, prec, prec_tos, in_table;
	int  it, is_done, is_first;
	unsigned int  iexp, next_con=1; // constants counter
	char byte, token[100];
	char* pCode [PC_NO_USER_CONSTANTS+1];
	char* pCode0[PC_NO_USER_CONSTANTS+1];
	char errbuf[128];
	double dval;

	// assume no error
	sprintf(m_sErr,"Ok");

	// validate function arguments
	if (pgm==NULL ||
		pc1==NULL || pc2==NULL || pc3==NULL || pc4==NULL ||
		pc5==NULL || pc6==NULL || pc7==NULL || pc8==NULL)
	{
		strcpy(m_sErr,"LoadProgram() called with null pointer");
		return(1);
	}

	// if using u,v don't allow z coordinate
	if (use_uv)
		allow_z = 0;

	// clear program
	NewProgram();

	// default constants
	m_c1 = 1.;	m_c2 = 1.;	m_c3 = 1.;	m_c4 = 1.;
	m_c5 = 1.;	m_c6 = 1.;	m_c7 = 1.;	m_c8 = 1.;

	// setup pointers
	pCode[0] = pCode0[0] = (char*)pc1;
	pCode[1] = pCode0[1] = (char*)pc2;
	pCode[2] = pCode0[2] = (char*)pc3;
	pCode[3] = pCode0[3] = (char*)pc4;
	pCode[4] = pCode0[4] = (char*)pc5;
	pCode[5] = pCode0[5] = (char*)pc6;
	pCode[6] = pCode0[6] = (char*)pc7;
	pCode[7] = pCode0[7] = (char*)pc8;
	pCode[8] = pCode0[8] = (char*)pgm;

	// --------------------
	// parse program string
	// --------------------

	for (iexp=0; iexp<PC_NO_USER_CONSTANTS+1; iexp++)
	{
		// init values
		it = 0;
		is_done = 0;
		is_first = 1;
		this_opcode = PC_OP_CONSTANT,
		last_opcode = PC_OP_CONSTANT;
	
		while (!is_done)
		{
			// get next byte
			byte = *pCode[iexp];
	
			// skip white space
			if (IsWhiteSpace(byte))
			{
				pCode[iexp]++;
				continue;
			}
	
			// is it a token separator
			if (!IsTokenSeparator(byte,&this_opcode))
			{
				// save the byte and keep looking for separator
				pCode[iexp]++;
				token[it] = byte;
				it++;
				if ((it+2) <= sizeof(token)) continue;
				strcpy(errbuf,"Unrecognized operand; too many characters");
				goto abort_load;
			}
	
			if (it==0)
			{
				// process the separator
				pCode[iexp]++;
				switch(this_opcode)
				{
				case PC_OP_END_INPUT:
					is_done = 1;
					break;
				case PC_OP_OPEN_PAREN:
					PushOp(PC_OP_OPEN_PAREN,PC_OP_CONSTANT);
					break;
				case PC_OP_CLOSE_PAREN:
					for (;;)
					{
						if (PopOp(&opcode2,&subcode) != 0)
						{
				 			strcpy(errbuf,"Missing opening parenthesis");
							goto abort_load;
						}
						if (opcode2 == PC_OP_OPEN_PAREN) break;
						if (opcode2 == PC_OP_START_FUNC)
						{
							if (m_got_operand) m_cur_parms++;
							if (m_cur_parms != m_req_parms)
							{
								i = OpIndex(subcode);
					 			sprintf(errbuf,"Function %s( ) requires %d parameter(s)",
									g_Inst[i].mnemonic ,(int)m_req_parms);
								goto abort_load;
							}
							AddInst(subcode,0);
							PopFunc(&m_cur_parms,&m_req_parms,&m_got_operand);
							break;
						}
						AddInst(opcode2,(unsigned char)subcode);
					} // for
					break;
				case PC_OP_COMMA:
					m_cur_parms++;
					if (m_req_parms<2 || m_cur_parms>m_req_parms || m_got_operand==0)
					{
						if (m_req_parms)
			 				strcpy(errbuf,"Too many function parameters");
						else
			 				strcpy(errbuf,"Improper use of comma");
		 				goto abort_load;
					}
					for (;;)
					{
						if (PeekOp(&opcode2,&subcode) != 0) break;
						if (opcode2 == PC_OP_COMMA) break;
						PopOp(&opcode2,&subcode);
						AddInst(opcode2,(unsigned char)subcode);
					} // for
					m_got_operand = 0; // don't have an operand yet
					break;
				case PC_OP_SUB:
					// check for negation
					if (is_first ||
					   (last_opcode==PC_OP_OPEN_PAREN ||
					    last_opcode==PC_OP_START_FUNC ||
						last_opcode==PC_OP_COMMA))
							this_opcode = PC_OP_NEG; // use negation operator
					// fall through with other operators
				case PC_OP_ADD:
				case PC_OP_MULT:
				case PC_OP_DIV:
				case PC_OP_POW:
					if (IsOperator(last_opcode))
					{
		 				strcpy(errbuf,"Two adjacent operators");
		 				goto abort_load;
					}
					// process operator
					prec = OpPrecedence(this_opcode);
					for (;;)
					{
						if (PeekOp(&opcode2,&subcode) != 0) break;
						prec_tos = OpPrecedence(opcode2);
						if (prec_tos < prec) break;
						PopOp(&opcode2,&subcode);
						AddInst(opcode2,0);
						if (prec_tos <= prec) break;
					}
					PushOp(this_opcode,PC_OP_CONSTANT);
					break;
				} // switch
			}
			else // it != 0
			{
				// Note: be sure to update this_opcode
	
				// process the token
				token[it] = 0; // null terminate
				len = strlen(token);
		
		 		// check if token is in table
		 		for (i=1, in_table=0; i<TOTAL_OP_CODES; i++)
		 		{
					if (g_Inst[i].optype == PC_OP_INTERNAL) continue; // ignore internal commands
		 			if (stricmp(g_Inst[i].mnemonic,token)!=0) continue;
					if ((allow_z==0) && (g_Inst[i].opcode==PC_OP_Z)) continue;
					if (use_uv)
					{
						// don't allow x,y when using u,v
						if (g_Inst[i].opcode==PC_OP_X) continue;
						if (g_Inst[i].opcode==PC_OP_Y) continue;
					}
					else
					{
						// don't allow u,v when using x,y
						if (g_Inst[i].opcode==PC_OP_U) continue;
						if (g_Inst[i].opcode==PC_OP_V) continue;
					}

					if (g_Inst[i].opcode>=PC_OP_C1 && g_Inst[i].opcode<=PC_OP_C8)
					{
						unsigned constant_no = (g_Inst[i].opcode - PC_OP_C1);
						if (iexp <= constant_no)
						{
							sprintf(errbuf,"C%d cannot use C%d",iexp+1,constant_no+1);
							goto abort_load;
						}
					}
					in_table = 1;
					if (g_Inst[i].optype == PC_OP_FUNCTION)
					{
						if (this_opcode != PC_OP_OPEN_PAREN)
						{
				 			strcpy(errbuf,"Missing open parenthesis for function call");
			 				goto abort_load;
						}
						m_got_operand = 1; // got an operand now
	
						// push onto function stack
						PushFunc(m_cur_parms,m_req_parms,m_got_operand);
						m_cur_parms = 0; // no parameters yet
						m_req_parms = g_Inst[i].nParms; // require this many
		
						// push open function call onto stack
						this_opcode = PC_OP_START_FUNC;
						PushOp(this_opcode, g_Inst[i].opcode);
						PushOp(PC_OP_COMMA, PC_OP_CONSTANT);
						m_got_operand = 0; // no operand yet
						pCode[iexp]++; // skip over the open parenthesis
					}
					else
					{
						if (g_Inst[i].optype == PC_OP_OPERAND) m_got_operand = 1;
						this_opcode = g_Inst[i].opcode;
						AddInst(this_opcode,0);
					}
					break;
				} // for
	
				// was it found in the lookup table?
				if (!in_table)
				{
			 		// not in lookup table; check if it is a number (constant)
			 		if (sscanf(token,"%lf",&dval) == 1)
			 		{
			 			if (SetConstant(next_con, dval) != 0)
			 			{
			 				strcpy(errbuf,"Too many constants");
			 				goto abort_load;
			 			}
						m_got_operand = 1; // got an operand now
						this_opcode = PC_OP_CONSTANT;
			 			AddInst(this_opcode,(unsigned char)next_con);
			 			next_con++;
			 		}
			 		else
			 		{
			 			strcpy(errbuf,"Unrecognized operand");
			 			goto abort_load;
			 		}
				} // if !in_table
	
				// reset token
				it = 0;
				token[0] = 0;
			} // else it != 0
	
			// update for next time
			last_opcode = this_opcode;
			is_first = 0;
	
		} // while !is_done
	
		// -----------------------------------
		//        input is done
		// pop and enqueue remaining operators
		// -----------------------------------
		for (;;)
		{
			if (PopOp(&this_opcode,&subcode) != 0) break;
			if (this_opcode==PC_OP_OPEN_PAREN || this_opcode==PC_OP_START_FUNC)
			{
	 			strcpy(errbuf,"Missing closing parenthesis");
				goto abort_load;
			}
			AddInst(this_opcode,0);
		}

		// set user constant
		if (iexp<PC_NO_USER_CONSTANTS)
		{
			this_opcode = (PcOpCode)(PC_OP_SETC1 + iexp);
			AddInst(this_opcode,0);
		}
	} // for iexp

	// ------------------------------------------------
	// Optimization: Eliminate unused constants C1...C8
	// ------------------------------------------------
	int ic,ip,is_cused,is_cset,first_inst,last_inst;
	PcOpCode op_cused, op_cset;

	// CAUTION! eliminate constants in reverse order (C8,C7...C1)
	// check each constant in reverse order
	for (ic=PC_NO_USER_CONSTANTS; ic>0; ic--)
	{
		// first if constant is used, set, and last instruction number
		op_cused = (PcOpCode)(PC_OP_C1   +ic-1);
		op_cset  = (PcOpCode)(PC_OP_SETC1+ic-1);
		is_cused = is_cset = last_inst = 0;
		for (ip=0; ip<(int)m_nInst; ip++)
		{
			if (m_opcodes[ip] == op_cused) is_cused=1;
			if (m_opcodes[ip] != op_cset ) continue;
			is_cset = 1;
			last_inst = ip;
		} // for ip
		if (is_cused || !is_cset) continue;
		// find first instruction number
		// search backwards until constant set instruction
		first_inst = 0;
		for (ip=last_inst-1; ip>=0; ip--)
		{
			if (m_opcodes[ip] < PC_OP_SETC1 ||
			    m_opcodes[ip] > PC_OP_SETC8 ) continue;
			first_inst = ip+1;
			break;
		} // for ip
		// delete the instruction(s)
		DelInsts(first_inst,last_inst);
	} // for ic
	return(0);

abort_load:
	NewProgram(); // clear program
	// calculate offset into string
	i = pCode[iexp] - pCode0[iexp];
	if (i==0) i=1;
	if (iexp==PC_NO_USER_CONSTANTS)
		sprintf(m_sErr,"[%s @ Col:%d] %s", label,i,errbuf);
	else
		sprintf(m_sErr,"[C%d @ Col:%d] %s", iexp+1,i,errbuf);
	return(i);

} // LoadProgram

// --------------------------------------------------------------
// get program instruction
// ip - instruction number to get (zero based)
// buf - buffer to received instruction into (dimension to 100)
// returns: 0=ok, 1=invalid instruction number
int  PlotCalc::GetProgramInst(unsigned ip,char* buf)
{
	unsigned i;

	if (ip>=m_nInst) return(1);
 	i = (unsigned)m_opcodes[ip];
 	if (i>=TOTAL_OP_CODES)
		{ sprintf(buf,"%04u: **Invalid** (%u)",ip,i); }
 	else if ((PcOpCode)i == PC_OP_CONSTANT)
		{ sprintf(buf,"%04u: %lf%c",ip,m_dconstants[m_operands[ip]],PC_END_OF_INSTRUCTION); }
 	else
		{ sprintf(buf,"%04u: %s%c",ip,g_Inst[i].mnemonic,PC_END_OF_INSTRUCTION); }
	return(0);
}

// --------------------------------------------------------------
// constructor
PlotCalc::PlotCalc()
{
	NewProgram();
	memset(m_sErr,0,sizeof(m_sErr));
}

// --------------------------------------------------------------
// initialize new program
void PlotCalc::NewProgram()
{
	m_sp = 0;
	m_opsp = 0;
	m_fsp = 0;
	m_nInst = 0;
	m_err_flags = 0;
	m_cur_parms = 0;
	m_req_parms = 0;
	m_got_operand = 0;
	memset(m_dstack,    0,sizeof(m_dstack    ));
	memset(m_dconstants,0,sizeof(m_dconstants));
	memset(m_opcodes,   0,sizeof(m_opcodes   ));
	memset(m_operands,  0,sizeof(m_operands  ));
	memset(m_opstack,   0,sizeof(m_opstack   ));
	memset(m_fstack,    0,sizeof(m_fstack    ));
}

// --------------------------------------------------------------
int PlotCalc::DelInsts(unsigned first_inst_no,unsigned last_inst_no)
{
	unsigned nmove;
	if (first_inst_no > last_inst_no) return(1);
	if (first_inst_no >= m_nInst) return(1);
	nmove = m_nInst - last_inst_no - 1;
	if (nmove)
	{
		memmove(&m_opcodes [first_inst_no],&m_opcodes [last_inst_no+1],nmove);
		memmove(&m_operands[first_inst_no],&m_operands[last_inst_no+1],nmove);
	}
	m_nInst -= (last_inst_no - first_inst_no + 1);
	return(0);
}

// --------------------------------------------------------------
// set a constant value
// number: 1-255, value=constant
// returns: 0=ok, 1=number out of range
int PlotCalc::SetConstant(unsigned number,double value)
{
	if (number<1 || number>PC_MAX_CONSTANTS)
		return(1);
	m_dconstants[number] = value;
	return(0);
}

// --------------------------------------------------------------
// returns: 0=ok, 1=stack overflow
int PlotCalc::PushOp(PcOpCode opcode, PcOpCode subcode)
{
	if (m_opsp>=PC_STACK_SIZE) return(1);
	m_opsp++;
	m_opstack[m_opsp][0] = opcode;
	m_opstack[m_opsp][1] = subcode;
	return(0);
}

// --------------------------------------------------------------
// returns: 0=ok, 1=stack underflow
int PlotCalc::PopOp(PcOpCode* opcode, PcOpCode* subcode)
{
	if (m_opsp<1) return(1);
	*opcode  = m_opstack[m_opsp][0];
	*subcode = m_opstack[m_opsp][1];
	m_opsp--;
	return(0);
}

// --------------------------------------------------------------
// returns: 0=ok, 1=stack underflow
int PlotCalc::PeekOp(PcOpCode* opcode, PcOpCode* subcode)
{
	if (m_opsp<1) return(1);
	*opcode  = m_opstack[m_opsp][0];
	*subcode = m_opstack[m_opsp][1];
	return(0);
}

// --------------------------------------------------------------
// returns: 0=ok, 1=stack overflow
int PlotCalc::PushFunc(unsigned char CurParms, unsigned char ReqParms, unsigned char GotOperand)
{
	if (m_fsp>=PC_STACK_SIZE) return(1);
	m_fsp++;
	m_fstack[m_fsp][0] = CurParms;
	m_fstack[m_fsp][1] = ReqParms;
	m_fstack[m_fsp][2] = GotOperand;
	return(0);
}

// --------------------------------------------------------------
// returns: 0=ok, 1=stack underflow
int PlotCalc::PopFunc(unsigned char* CurParms, unsigned char* ReqParms, unsigned char* GotOperand)
{
	if (m_fsp<1) return(1);
	*CurParms   = m_fstack[m_fsp][0];
	*ReqParms   = m_fstack[m_fsp][1];
	*GotOperand = m_fstack[m_fsp][2];
	m_fsp--;
	return(0);
}

// --------------------------------------------------------------
// determine operator precedence
int PlotCalc::OpPrecedence(PcOpCode opcode)
{
	int p=0;
	switch (opcode)
	{
	case PC_OP_START_FUNC:	p=1; break;
	case PC_OP_OPEN_PAREN:	p=1; break;
	case PC_OP_CLOSE_PAREN:	p=1; break;
	case PC_OP_COMMA	:	p=1; break;
	case PC_OP_ADD:			p=2; break;
	case PC_OP_SUB:			p=2; break;
	case PC_OP_MULT:		p=3; break;
	case PC_OP_DIV:			p=3; break;
	case PC_OP_POW:			p=4; break;
	case PC_OP_NEG:			p=5; break;
	} // switch
	return(p);
}

// --------------------------------------------------------------
// returns: 0=no, 1=it is an operator
int PlotCalc::IsOperator(PcOpCode opcode)
{
	if (opcode==PC_OP_ADD  ||
		opcode==PC_OP_SUB  ||
		opcode==PC_OP_MULT ||
		opcode==PC_OP_DIV  ||
		opcode==PC_OP_POW  ||
		opcode==PC_OP_NEG ) return(1);
	return(0);
}

// --------------------------------------------------------------
// determine if byte is a token separator
// returns: 0=no, 1=yes (and opcode is valid)
int PlotCalc::IsTokenSeparator(char byte,PcOpCode* pCode)
{
	PcOpCode op = PC_OP_CONSTANT;
	int is = 0;
	switch(byte)
	{
	case 0:   op = PC_OP_END_INPUT;	  is=1; break;	// end of input
	case '(': op = PC_OP_OPEN_PAREN;  is=1; break;	// control
	case ')': op = PC_OP_CLOSE_PAREN; is=1; break;
	case ',': op = PC_OP_COMMA;       is=1; break;
	case '+': op = PC_OP_ADD;		  is=1; break;	// operators
	case '-': op = PC_OP_SUB;		  is=1; break;
	case '*': op = PC_OP_MULT;		  is=1; break;
	case '/': op = PC_OP_DIV;		  is=1; break;
	case '^': op = PC_OP_POW;		  is=1; break;
	} // switch
	if (is && pCode) *pCode = op;
	return(is);
}

// --------------------------------------------------------------
// determine if byte is white space
// returns: 0=no, 1=yes
int PlotCalc::IsWhiteSpace(char byte)
{
	if (byte==' ' || byte=='\n' || byte=='\r' || byte=='\t') return(1);
	return(0);
}

// --------------------------------------------------------------
// calculate
// this routine needs to be efficient
double PlotCalc::Calc(double x,double y,double z)
{
	double dtemp;
	unsigned ip; // instruction pointer

	m_sp = 0;	// clear stack pointer
	m_err_flags = 0;	// clear error flags
	for (ip=0; ip<m_nInst; ip++)
	{
		switch ((unsigned)m_opcodes[ip])
		{
	     case PC_OP_CONSTANT:IncSp(); m_dstack[m_sp] = m_dconstants[m_operands[ip]];           break;
	     case PC_OP_ADD:   DecSp(); m_dstack[m_sp] = m_dstack[m_sp] + m_dstack[m_sp+1];        break;
	     case PC_OP_SUB:   DecSp(); m_dstack[m_sp] = m_dstack[m_sp] - m_dstack[m_sp+1];	       break;
	     case PC_OP_MULT:  DecSp(); m_dstack[m_sp] = m_dstack[m_sp] * m_dstack[m_sp+1];	       break;
		 case PC_OP_DIV:   DecSp();
						   if (m_dstack[m_sp+1]==0.0)
						   {
						  	 m_dstack[m_sp] = 1.e10;
						  	 m_err_flags |= PC_ERR_DIVIDE_BY_ZERO;
						  	 break;
						   }
						   m_dstack[m_sp] = m_dstack[m_sp]/m_dstack[m_sp+1];
						   break;
	     case PC_OP_POW:   DecSp(); m_dstack[m_sp] = pow(m_dstack[m_sp],m_dstack[m_sp+1]);     break;
	     case PC_OP_POW2:  m_dstack[m_sp] = m_dstack[m_sp]*m_dstack[m_sp];                     break; // optimization
	     case PC_OP_POW3:  m_dstack[m_sp] = m_dstack[m_sp]*m_dstack[m_sp]*m_dstack[m_sp];      break; // optimization
	     case PC_OP_POW4:  dtemp = m_dstack[m_sp]*m_dstack[m_sp]; m_dstack[m_sp]=dtemp*dtemp;  break; // optimization
	     case PC_OP_NEG:   m_dstack[m_sp] = -m_dstack[m_sp];                                   break; // optimization
	     case PC_OP_PI:	   IncSp(); m_dstack[m_sp] = 3.141592653589793238462643;               break;
	     case PC_OP_E:	   IncSp(); m_dstack[m_sp] = 2.718281828459045235360287;		       break;
		 case PC_OP_U:	// use X code
		 case PC_OP_X:	   IncSp(); m_dstack[m_sp] = x;                                        break;
		 case PC_OP_V:  // use Y code
		 case PC_OP_Y:	   IncSp(); m_dstack[m_sp] = y;                                        break;
	     case PC_OP_Z:     IncSp(); m_dstack[m_sp] = z;                                        break;
		 case PC_OP_C1:    IncSp(); m_dstack[m_sp] = m_c1;									   break;
		 case PC_OP_C2:    IncSp(); m_dstack[m_sp] = m_c2;									   break;
		 case PC_OP_C3:    IncSp(); m_dstack[m_sp] = m_c3;									   break;
		 case PC_OP_C4:    IncSp(); m_dstack[m_sp] = m_c4;									   break;
		 case PC_OP_C5:    IncSp(); m_dstack[m_sp] = m_c5;									   break;
		 case PC_OP_C6:    IncSp(); m_dstack[m_sp] = m_c6;									   break;
		 case PC_OP_C7:    IncSp(); m_dstack[m_sp] = m_c7;									   break;
		 case PC_OP_C8:    IncSp(); m_dstack[m_sp] = m_c8;									   break;
		 case PC_OP_SETC1: DecSp(); m_c1 = m_dstack[m_sp+1];								   break;
		 case PC_OP_SETC2: DecSp(); m_c2 = m_dstack[m_sp+1];								   break;
		 case PC_OP_SETC3: DecSp(); m_c3 = m_dstack[m_sp+1];								   break;
		 case PC_OP_SETC4: DecSp(); m_c4 = m_dstack[m_sp+1];								   break;
		 case PC_OP_SETC5: DecSp(); m_c5 = m_dstack[m_sp+1];								   break;
		 case PC_OP_SETC6: DecSp(); m_c6 = m_dstack[m_sp+1];								   break;
		 case PC_OP_SETC7: DecSp(); m_c7 = m_dstack[m_sp+1];								   break;
		 case PC_OP_SETC8: DecSp(); m_c8 = m_dstack[m_sp+1];								   break;
		 case PC_OP_SQRT:  if (m_dstack[m_sp]<0.)
						   {
							   m_dstack[m_sp] = 0.;
							   m_err_flags |= PC_ERR_NEGATIVE_SQRT;
							   break;
						   }
						   m_dstack[m_sp] = sqrt(m_dstack[m_sp]);
						   break;
	     case PC_OP_ABS:   m_dstack[m_sp] = fabs(m_dstack[m_sp]);                              break;
	     case PC_OP_SIN:   m_dstack[m_sp] =  sin(m_dstack[m_sp]);                              break;
	     case PC_OP_COS:   m_dstack[m_sp] =  cos(m_dstack[m_sp]); 							   break;
	     case PC_OP_TAN:   m_dstack[m_sp] =  tan(m_dstack[m_sp]); 							   break;
	     case PC_OP_ASIN:  dtemp = m_dstack[m_sp];
						   if (dtemp<-1.)
							   { dtemp=-1.; m_err_flags |= PC_ERR_OUT_OF_RANGE; }
						   if (dtemp> 1.)
							   { dtemp= 1.; m_err_flags |= PC_ERR_OUT_OF_RANGE; }
						   m_dstack[m_sp] = asin(dtemp);
						   break;
	     case PC_OP_ACOS:  dtemp = m_dstack[m_sp];
						   if (dtemp<-1.)
							   { dtemp=-1.; m_err_flags |= PC_ERR_OUT_OF_RANGE; }
						   if (dtemp> 1.)
							   { dtemp= 1.; m_err_flags |= PC_ERR_OUT_OF_RANGE; }
						   m_dstack[m_sp] = acos(dtemp);
						   break;
	     case PC_OP_ATAN:  m_dstack[m_sp] = atan(m_dstack[m_sp]); 							   break;
	     case PC_OP_SINH:  m_dstack[m_sp] = sinh(m_dstack[m_sp]); 							   break;
	     case PC_OP_COSH:  m_dstack[m_sp] = cosh(m_dstack[m_sp]); 							   break;
	     case PC_OP_TANH:  m_dstack[m_sp] = tanh(m_dstack[m_sp]); 							   break;
	     case PC_OP_EXP:   m_dstack[m_sp] = exp(m_dstack[m_sp]); 							   break;
	     case PC_OP_LN:    if (m_dstack[m_sp]<=0.)
						   {
						   	   m_dstack[m_sp] = 0.;
						   	   m_err_flags |= PC_ERR_OUT_OF_RANGE;
							   break;
						   }
						   m_dstack[m_sp] = log(m_dstack[m_sp]);
						   break;
	     case PC_OP_LOG10: if (m_dstack[m_sp]<=0.)
						   {
						   	   m_dstack[m_sp] = 0.;
						   	   m_err_flags |= PC_ERR_OUT_OF_RANGE;
							   break;
						   }
						   m_dstack[m_sp] = log10(m_dstack[m_sp]);
						   break;
	     case PC_OP_CEIL:  m_dstack[m_sp] = ceil(m_dstack[m_sp]); 							   break;
	     case PC_OP_FLOOR: m_dstack[m_sp] = floor(m_dstack[m_sp]); 							   break;
		 case PC_OP_RAND:  IncSp(); m_dstack[m_sp] = rand();								   break;
		 case PC_OP_MIN:   DecSp(); m_dstack[m_sp] = __min(m_dstack[m_sp],m_dstack[m_sp+1]);   break;
		 case PC_OP_MAX:   DecSp(); m_dstack[m_sp] = __max(m_dstack[m_sp],m_dstack[m_sp+1]);   break;
		 case PC_OP_ATAN2: DecSp(); m_dstack[m_sp] = atan2(m_dstack[m_sp],m_dstack[m_sp+1]);   break;
		 case PC_OP_HYPOT: DecSp(); m_dstack[m_sp] = hypot(m_dstack[m_sp],m_dstack[m_sp+1]);   break;
		 case PC_OP_SIGN:  m_dstack[m_sp] = (m_dstack[m_sp]>=0.) ? 1.0 : -1.0;                 break;

		} // switch
	} // for
	return(m_dstack[1]);
}

/*	<><><><><><><><><><><><><>  PlotCalc.cpp  <><><><><><><><><><><><><><> */
