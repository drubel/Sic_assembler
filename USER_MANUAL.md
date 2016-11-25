===========================================
SIC Assembler & Simulator v1.0 (21/11/2016)
===========================================

.::USER MANUAL::.

.::CONTENTS::.
--------------
   
 * Introduction
 * Requirements
 * Data Structures
 * Functions
 * Additional Header files
 * Instruction set & source code writing
 * Usage (console)
 * Usage (GUI)
 * Troubleshooting & FAQ
 * Maintainers


INTRODUCTION
------------

The Simplified Instructional Computer (SIC) Assembler and Simulator v1.0 was developed as
part of our Systems Programming Lab in the 3rd year. 
The source code has been written using C++.
The assembler generates the object code and also user interactively simulates the results
on the console or the Graphical User Interface. Please note that the console is supported 
on Windows only whereas the GUI is exclusive to Linux.

REQUIREMENTS
------------

This module requires the following modules:

 * g++ 5.4.0 (C++ compiler)
 * Qt 5.5.1  along with QMake 3.0 (GUI purpose)
   https://www.qt.io/download/


DATA STRUCTURES
---------------

 STRUCT: 
  struct pseudo
  {
	string type;
	string value;
	string address;
  };

 Used to store information for symbols using assembler directives BYTE, WORD, RESB, RESW.
 The type stores the type of the directive, the value stores the actual value in case of
 BYTE and WORD and the size to be allocated in case of RESB and RESW. The address stores the
 first address location assigned to that symbol in the memory. 

 MAP:
  optab<string, string>;

 Used to store the opcode and its corresponding hexcode as strings.

 MAP:
  symtab<string, pseudo>

 Used to store the symbols from pass 1 and their corresponding information.

 ARRAY:
  string memory [5000];

 Used as the memory.

 VECTOR:
  vector<string>;
 
 Used to store a tokenized line of instruction.


FUNCTIONS
---------

The two-pass assembler assigns addresses to instructions and symbols on the first pass,
generates the object code on the second pass and then separately executes and simulates
the input source program. It also includes functions for the basic arithmetic operations, 
comparison, subroutine jump and return, looping.

 FUNCTION: OPTAB();                                                          

  PROTOTYPE: void OPTAB();                                             
    RETURNS: void   
 
  Initialises the opcode table with the available instruction opcodes.


 FUNCTION: get_arg();                                                          

  PROTOTYPE: void get_arg(string command, vector<string> &args);                                             
    RETURNS: void   
 
  Parses a line of instruction and stores the tokens in the vector.


 FUNCTION: add_location();                                                          

  PROTOTYPE: string add_location(string A, string B, int op);                                             
    RETURNS: string
 
  Adds(op=1) or subtracts(op=2) two hexadecimal values A and B depending on op and returns
  the result.
  e.g. Let the Accumulator contain 00002A and BETA contain 000014.
       ADD B
       will result in Accumulator containing 00003E.
       SUB B
       will result in Accumulator containing 000016.

 
 FUNCTION: multiply_hex();                                                          

  PROTOTYPE: string multiply_hex(string A, string B, int op);                                             
    RETURNS: string   
 
  Multiplies(op=1) or divides (op=2) two hexadecimal values A by B depending on op and returns 
  the result.
  e.g. Let the Accumulator contain 00002A and BETA contain 000015.
       ADD B
       will result in Accumulator containing 000372.
       DIV B
       will result in Accumulator containing 000002.


 FUNCTION: get_string();                                                          

  PROTOTYPE: string get_string(int val);                                             
    RETURNS: string
 
  Returns the string value of the integer val. 


 FUNCTION: make_word();                                                          

  PROTOTYPE: string make_word(string value, int len);                                             
    RETURNS: string
 
  Forms the complete word of specified length for a value.  
   

 FUNCTION: load_memory();                                                          

  PROTOTYPE: void load_memory();                                             
    RETURNS: void   
 
  Clears the memory to 00.


 FUNCTION: pass_1();                                                          

  PROTOTYPE: string pass_1();                                             
    RETURNS: string  
 
  FUNCTION: pass_1();                                                          

  PROTOTYPE: string pass_1();                                             
    RETURNS: string  
 
  PASS 1 :
	� Reads the entire file which contains the SIC assembly language program.
	� Assigns addresses to all instructions (including assembler directives).
	� Address is assigned in the following way :
		Using a �LOC� variable :
			1. The first statement is read.
			2. The location counter is set to the starting address.
				eg :  START FILENAME  1000 (LOC = 1000)
			3. For each instruction, 3 bytes of memory are assigned by 
			   incrementing the �LOC� variable by 3.
			4. An entry is made for each label name in the Symbol Table which is
			   implemented using a Map.
				eg : LABEL OPCODE OPERAND

				KEY	        VALUE
			       
			       LABEL		1. OPCODE
						2. OPERAND
						3. ADDRESS (i.e. VALUE OF �LOC�)

			5. This method is followed until the instruction �END� is encountered.
			6. Assembler directives are now read.
			7. For each directive, an entry is made in the Symbol Table
			   as done in Step 4.
				eg : ONE RESW 1
				
				KEY	        VALUE
				
				ONE		1. TYPE : RESW
						2. VALUE : 1
						3. ADDRESS (i.e. VALUE OF �LOC�)

			8. While proceeding from one assembler directive to another, the �LOC� variable
			   is incremented in the following way:

			  	INSTRUCTION  FORMAT		MODIFICATION OF �LOC�
				
				WORD_NAME  WORD  VAL			LOC+=3
				WORD_NAME RESW VAL			LOC+=3*VAL
				BYTE_NAME BYTE VAL			LOC+=1
				BYTE_NAME RESB VAL			LOC+=1*VAL

			9. This process continues until the end of file is reached.


 FUNCTION: pass_2();                                                          

  PROTOTYPE: void pass_2();                                             
    RETURNS: void   
 
   PASS 2 :
	� The file containing the SIC assembly language code is opened for reading.	
	� The object file is created and opened for writing.	
	� For the first statement, the object file output is created as follows :
       		 SOURCE FILE STATEMENT :  START FILENAME 1000
       		 OBJECT FILE OUTPUT    :  H FILENAME 	1000
	� For all other instructions (excluding assembler directives), following method is followed :
		1. Take any number of instructions, say N, and generate the 6 bit object code for
		   each of them in the following way :	
 		INSTRUCTION :  LABEL OPCODE OPERAND
		OBJECT CODE :  B5 B4 B3 B2 B1 B0
                	       WHERE 
 			       B5 B4 = OPTAB[OPCODE] (provides the opcode from Opcode Table)
 			       B3 B2 B1 B0 = SYMTAB[OPERAND].ADDRESS (provides starting address of the operand)
		2. The corresponding object file output will be : 
		   T <ADDRESS OF FIRST INSTRUCTION> <SIZE OF THE N INSTRUCTIONS> <OBJECT CODES OF THOSE INSTRUCTIONS>  

		3. The 3 Byte object code is stored in the 3 memory locations allocated for each such instruction
		   in pass 1 in the following way :
 			    INSTRUCTION :  LABEL OPCODE OPERAND
			    OBJECT CODE  :  B5 B4 B3 B2 B1 B0
				Address=SYMTAB[LABEL].ADDRESS
				memory[Address]=B5 B4
				memory[Address+1]=B1 B0
				memory[Address+2]=B3 B2
		4. This process continues until the 'END' instruction is reached.
		5. Finally the object file output is : E <starting address>
		6. Both files are closed.


 FUNCTION: ADD();                                                          

  PROTOTYPE: void ADD(vector<string> &args, int op);                                             
    RETURNS: void   
 
  Adds(op=1) or subtracts(op=2) the value of the operand from the instruction to the 
  Accumulator depending on the value of OP and saves it in the Accumulator. 
  Uses add_location(). Sets the zero flag if the result is zero.


 FUNCTION: MUL();                                                          

  PROTOTYPE: void MUL();                                             
    RETURNS: void   
 
  Multiplies(op=1) or divides(op=2) the value of the Accumulator by the operand in the 
  instruction depending on the value of OP and saves it in the Accumulator. 
  Uses multiply_hex(). Sets the zero flag if the result is zero. 

        
 FUNCTION: LDA();                                                          

  PROTOTYPE: void LDA(vector<string> &args);                                             
    RETURNS: void  
  
  Locates the operand symbol in the instruction line in the SYMTAB, checks if it is a byte or
  a word and then correctly loads the Accumulator with the value of the operand.
  e.g. Let the Accumulator contain 00002A and BETA contain 000015.
       LDA BETA
       Now the Accumulator contains 000015.

 
 FUNCTION: LDX();                                                          

  PROTOTYPE: void LDX(vector<string> &args);                                             
    RETURNS: void  
  
  Locates the operand symbol in the instruction line in the SYMTAB, checks if it is a byte or
  a word and then correctly loads the X register with the value of the operand.
  e.g. Let the X register contain 00002A and BETA contain 000015.
       LDX BETA
       Now the X register contains 000015.


 FUNCTION: STA();                                                          

  PROTOTYPE: void STA(vector<string> &args);                                             
    RETURNS: void  

  Locates the operand symbol in the instruction line in the SYMTAB, checks if it is a byte or
  a word and then correctly loads the value of the Accumulator in the operand memory location.
  e.g. Let the Accumulator contain 00002A and BETA contain 000015.
       STA BETA
       Now BETA contains 00002A in the memory.

 FUNCTION: STX();                                                          

  PROTOTYPE: void LDA(vector<string> &args);
    RETURNS: void
  
  Locates the operand symbol in the instruction line in the SYMTAB, checks if it is a byte or
  a word and then correctly loads the value of the X register in the operand memory location.
  e.g. Let the X register contain 00002A and BETA contain 000015.
       STX BETA
       Now BETA contains 00002A in the memory.


 FUNCTION: show_memory();

  PROTOTYPE: void show_memory();
    RETURNS: void
 
  Iterates through the symbol table and displays the memory address and their contents.

  [NOTE]: The memory is displayed in different colors on the console on a Windows platform.
  For this purpose, concol.h is included.



 FUNCTION: COMP();

  PROTOTYPE: int COMP(vector<string> &args);
    RETURNS: -1; when value in A is less than value in operand
              1; when value in A is greater than value in operand
              0; when value in A is equal to value in operand
 
  Uses symbol table to get the address of �args[1]�. Obtain the hexadecimal value pertaining 
  to the 6 digit hexadecimal (taking 2 digits at a time) address got from the symbol table.
  Then it compares the hexadecimal value of the ACCUMULATOR and the value of the particular
  address and returns an integer corresponding to the result.
  e.g.	Let the Accumulator contain value 000012 and K300 contain 000022.
        COMP K300
        Now as the value in A is smaller than the value in K300, the function returns -1.


 FUNCTION: TIX();

  PROTOTYPE: int TIX(vector<string> &args);
    RETURNS: -1; when value in X is less than value in operand
              1; when value in X is greater than value in operand
              0; when value in X is equal to value in operand
 
  Uses symbol table to get the address of �args[1]�. Obtain the hexadecimal value pertaining 
  to the 6 digit hexadecimal (taking 2 digits at a time) address got from the symbol table.
  Then it increases the value of index register by 1. Then it compares the hexadecimal value
  of the X register and the value of the particular address and returns an integer corresponding 
  to the result.
  e.g.	Let the X register contain value 000022 and K300 contain 000022.
        TIX K300
        First X is incremented to 000023. Now as the value in A is greater than the value in 
        K300, the function returns 1.



  FUNCTION: MOV();

   PROTOTYPE: void MOV();
     RETURNS: void

   It is a basic function which is used to accomplish the task of moving data from one storage
   space to another. In simple language, it copies the data from a specified register or copies
   an immediate constant, to another register.
   Syntax of MOV instruction: MOV destination, source.
   It performs two basic functions:
   1. moving data from register-to-register [MOV register, register]
   2. moving data from immediate-to-register [MOV register, immediate]
   Function 1 performs Register Addressing. In this mode a register contains the operand.
   Function 2 performs Immediate Addressing. An immediate operand has a constant value or expression. 
   e.g. MOV X, A
        The above call will copy the data which is inside register X to the Accumulator. 
   


  FUNCTION: JSUB();

  PROTOTYPE: void JSUB(int line_number);
    RETURNS: void 

  Take the memory location corresponding to given line_number (current line number) stored in SYMTAB
  at the time of pass 1 and store it to L register for future reference . For instructions 'J' or 
 'JLT' or 'JGT' or 'JEQ' along with 'JSUB' , it will call this function. 
  e.g. JSUB Loop
       .
       .
       Loop LDA N

  So in the above example when JSUB instruction will be executed, memory location of this line will 
  be stored in L register and jump to line 'Loop LDA N' for next execution.


  FUNCTION: RSUB();

  PROTOTYPE: int RSUB();
    RETURNS: int

  Get the location stored in L register to jump to previous location (from here subroutine call was done) 
  to start execution from next line and set L register as '000000'
  e.g. JSUB Loop
       .
       .
       Loop LDA N
       .
       .
       RSUB

   In above example when RSUB instruction will be called , it jumps to location of the line 'JSUB Loop' and 
   start execution from next line of it.
 
     

  FUNCTION: execute();

  PROTOTYPE: void execute();
    RETURNS: void
 
  Reads the file "test_source.txt" for the input source program and executes the instructions
  line by line. It also asks the user if they want to show the memory and whether they want
  a step-by-step or a all-at-once execution. Uses the other functions for execution as required.

  [NOTE]: Different colors are used to display some of the messages on the console on a 
  Windows platform. For this purpose, concol.h is included.


  FUNCTION: main();

  PROTOTYPE: int execute();
    RETURNS: 1; successful termination of the program
             0;  error

  Initialises the console for colored display support by calling concolinit(). 
  Clears all registers and flags, calls load_memory(), OPTAB() and then calls pass_1()
  which returns the value of the final program counter.
  After pass_1() is completed, user is given the option to enter input value(s) to zero or more
  symbol(s) that he may have used in the program by first entering the name of the corresponding 
  symbol. If the symbol name is found by the assembler in SYMTAB, the value can be entered. Otherwise,
  an error message is shown.
  pass_2() is then called to generate the object file.
  execute() is then called to simulate the execution of the instructions.
  The final status of the registers are shown before the program terminates successfully.


ADDITIONAL HEADERS
------------------

*concol.h
http://www.cplusplus.com/articles/Eyhv0pDG/

The header file is used to color text and background in Windows console applications.
Global variables - textcol, backcol, deftextcol, defbackcol, colorprotect.


*unistd.h
Used to create delays in the GUI interface.


INSTRUCTION SET & SOURCE CODE WRITING
-------------------------------------

 * Supported instructions are 
   ADD
   SUB
   MUL
   DIV
   LDA
   LDX
   STA
   COMP
   TIX
   RSUB
   JSUB
   J
   JEQ
   JLT
   JGT
 
 * The assembler directives WORD, BYTE, RESW, RESB are supported.

 * Start with the line
   START <programname> <starting memory location>

 * Mark the end of the main program with the line
   END
   
 * RESW/RESB/BYTE/WORD are to be declared after END as
   <name> RESW <size>
   <name> RESB <size>
   <name> WORD <value>
   <name> BYTE <value>

 * Use a single space to separate the tokens. e.g.
   LDA ARRAY X
   STA STOREVAL
  
 * Any Labels associated with a loop statement should start with "Loop" e.g.
   Loop1 LDA ZERO
   ...
   JLT Loop1

 * Do not leave any extra line after the last line of the program.

   
USAGE (CONSOLE)
------------

 * Write down the assembly language program in "test_source.txt".

 * Compile the sic_assembler_console.cpp and run it.

 * You may enter inputs for your reserved words/bytes by accepting the option. Enter the
   correct name of the identifier to perform input on it. By default, all memory locations
   are cleared.

 * You can choose to execute instructions step by step or all at once.

 * The simulation is performed interactively on the console and the object code is 
   generated in OBJECTFILE.txt.


USAGE (GUI INTERFACE)
---------------------

 * open assembler.pro in qt and run it to start GUI .

 * After opening GUI, user will be guided by stepwise messages.
 
 * 'Source Code' TextBox only for source code input as well other user inputs whereas 'Diplay Output' TextBrowser widget for displaying output  
    (current status of using Register , allocating memory in stepwise manner) and also for showing source code.

 * Seven button widget are used for different purposes:
     
      RESET -      To reset or clear 'Source Code' text box
      SUBMIT -     To submit the source code only 
      INPUT -      To submit other reqiured inputs
      EXECUTE -    To start execution of the program
      MEMORY -     To show current memory status
      REGISTER -   To show current status of all registers as well as program counter
      SOURCE CODE- To show given source code

 * Steps ,dicussed as below, should be strictly follwed .
 
    - Enter source code and click SUBMIT button
    - Enter inputs (one input in one line) and click INPUT button (input must start from leftmost point of corresponding line).
    - Click EXECUTE button.

 * Skip all guiding messages (displaying on the screen) by clicking OK .

 * GUI must be separately run for every execution.



TROUBLESHOOTING & FAQ
---------------------

Q: Why does the assembler not allow declaration of RESW/RESB/WORD/BYTE before END statement?

A: Ours is a simple two pass assembler where we generate the symbol table in pass 1 and
   assign memory locations. Forward references are thus possible.
   It also organizes the main code and the data.

Q: Why does the assembler does not support console interaction on Linux and GUI on windows?

A: The color codes for console output were written first according to Windows API whereas 
   the GUI was written to be compatible with Linux by separate developers.

Q: When I make a choice during execution, why does the assembler not register it correctly?

A: All choices are case-sensitive. Please be careful while entering your choice.

Q: Why should stepwise guiding instruction be strictly followed?

A: Wrong button clicking may cause premature program termination or program crashing. 

Q: Why must inputs start from leftmost point of corresponding line?

A: Qt tool can not recognize any input after certain number of columns (depends on Qt version) in every line.

Q: Why must GUI run separately for every execution?

A: Execution more than once in one GUI generates overhead which may cause program crashing. So it is recommended to run separately for
   separate execution.



MAINTAINERS
-----------

 * Debanjan Das - debanjandas94rusho@gmail.com
 * Antariksha Ray - theantariksharay@gmail.com
 * Rubel Das - drubel116@gmail.com
 * Bibaswan Basu - bibaswan1000@gmail.com
 * Debasmita Barua - debasmita.barua95@gmail.com
# Sic_assembler
