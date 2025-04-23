#include <iostream>
#include <unistd.h>
#include <sys/mman.h>
#include "Instructions.h"

const unsigned char PUSH_EBX = 0x53;
const unsigned char PUSH_ESI = 0x56;
const unsigned char PUSH_EDI = 0x57;
const unsigned char POP_EDI = 0x5F;
const unsigned char POP_ESI = 0x5E;
const unsigned char POP_EBX = 0x5B; 


// Some assembly like definitions for our machine code:
const unsigned char PUSH_EBP = 0x55;
const unsigned char MOV_EBP_ESP1 = 0x8B;
const unsigned char MOV_EBP_ESP2 = 0xEC;

const unsigned char POP_EBP = 0x5D;
const unsigned char NEAR_RET = 0xC3;

const unsigned char IMMEDIATE_TO_EAX = 0xB8;
const unsigned char PUSH_EAX = 0x50; 

const unsigned char SYS_CALL1 = 0x0F;
const unsigned char SYS_CALL2 = 0x05;
const unsigned char CALL = 0xE8; 
	// function Call within segment. Add 4 byte offset
const unsigned char POP_EAX = 0x58;
const unsigned char EAX_TO_MEM = 0xA3; 
	// Add 4 (or 8) byte address value in reverse order
const unsigned char BIT64 = 0x48;
	// BIT64 (0x48) makes IMMEDIATE_TO_EAX take 8 byte address,
	// 		or IMMEDIATE_TO_RAX
const unsigned char IMMEDIATE_TO_EDX = 0xBA;
const unsigned char IMMEDIATE_TO_ESI = 0xBE;
const unsigned char IMMEDIATE_TO_EDI = 0xBF;
const unsigned char CMP_EAX1 = 0x83; // followed by 1 byte
const unsigned char CMP_EAX2 = 0xF8;
const unsigned char MEM_TO_EAX = 0xA1; 
	// Add 4 (or 8) byte address value in reverse order
const unsigned char JNE = 0x75;
const unsigned char JGE = 0x7D;
const unsigned char NEG_EAX1 = 0xF7;
const unsigned char NEG_EAX2 = 0xD8;
const unsigned char IMMEDIATE_TO_ECX = 0xB9;
const unsigned char IMMEDIATE_TO_EBX = 0xBB;
const unsigned char ADD_ECX1 = 0x83; // followed by 1 byte to add.
const unsigned char ADD_ECX2 = 0xC1;
const unsigned char SUB_ECX1 = 0x83; // followed by 1 byte to add.
const unsigned char SUB_ECX2 = 0xE9;
const unsigned char DIV_EAX_EBX1 = 0xF7; 
const unsigned char DIV_EAX_EBX2 = 0xFB;        
	// Registers D and A divided by B. (clear D with CDQ first!)
	// .. Quotient->A and Remainder->D.
const unsigned char CDQ = 0x99;
const unsigned char ADD_EDX1 = 0x83; // followed by 1 byte
const unsigned char ADD_EDX2 = 0xC2;
const unsigned char POP_ECX = 0x59;
const unsigned char POP_EDX = 0x5A;
const unsigned char PUSH_ECX = 0x51;
const unsigned char PUSH_EDX = 0x52;
const unsigned char CMP_ECX1 = 0x83; // followed by 1 byte
const unsigned char CMP_ECX2 = 0xF9;

const unsigned char ADD_EAX_EBX1 = 0x03;
const unsigned char ADD_EAX_EBX2 = 0xC3;
const unsigned char SUB_EAX_EBX1 = 0x2B;
const unsigned char SUB_EAX_EBX2 = 0xC3;
const unsigned char MUL_EAX_EBX1 = 0xF7;
const unsigned char MUL_EAX_EBX2 = 0xEB;

const unsigned char CMP_EAX_EBX1 = 0x3B; // compares A and B registers.
const unsigned char CMP_EAX_EBX2 = 0xC3; // followed by 1 byte value
const unsigned char JL = 0x7C;

const unsigned char JLE = 0x7E;
const unsigned char JG = 0x7F;
//const unsigned char JGE = 0x7D; // already declared
const unsigned char JE = 0x74;
//const unsigned char JNE = 0x75; // already declared

const unsigned char JUMP_ALWAYS = 0xEB;

const unsigned char JE_FAR1 = 0x0f; // 4 byte jump
const unsigned char JE_FAR2 = 0x84; // 4 byte jump
const unsigned char JUMP_ALWAYS_FAR = 0xE9; // 4 byte jump
static const unsigned char OP_HALT = 0xFF;



// Put one instruction at a time into mCode:
void InstructionsClass::Encode(unsigned char c){
	if(mCurrent + sizeof(unsigned char) < MAX_INSTRUCTIONS)
	{
		mCode[mCurrent] = c;
		mCurrent += sizeof(unsigned char);
	}
	else
	{
		std::cerr << "Error.  Used up all " << MAX_INSTRUCTIONS 
			<< " instructions." << std::endl;
		exit(1);
	}
}

InstructionsClass::InstructionsClass()
{
	// This command allows mCode data to be called as a function.
	mprotect((void *)((uintptr_t)mCode& ~(sysconf(_SC_PAGE_SIZE)-1)), 
		MAX_INSTRUCTIONS, PROT_READ|PROT_WRITE|PROT_EXEC);

	// Initialize all class variables:
	mCurrent = 0;
	mStartOfMain = 0;
	mPrintInteger = 0;
	mTempInteger = 0;
	mMinusString = '-';
	mSpaceString = ' ';

	// Record where the PrintIntegerLinux64 function starts:
	mStartOfPrint = mCurrent; 

	// Code one function PrintIntegerLinux64 that prints an integer. 
	// It will be called later many times.
	PrintIntegerLinux64(); // Write all the codes into mCode.

	// Now record where the main function will start:
	mStartOfMain = mCurrent;

	// All functions start this way. So does the main function.
	Encode(PUSH_EBP);
	Encode(MOV_EBP_ESP1);
	Encode(MOV_EBP_ESP2);
	// Make sure we save and restore all 5 Callee-Save registers.
	// That is, EBP, ESP, EBX, ESI, and EDI,
	Encode(PUSH_EBX);
	Encode(PUSH_ESI);
	Encode(PUSH_EDI);
}

// Modified from https://baptiste-wicht.com/posts/2011/11/print-strings-integers-intel-assembly.html
// This is made into a function, so it can be called per cout instead of rewritten every time.
// The integer to print should be previously stored in mPrintInteger.
void InstructionsClass::PrintIntegerLinux64()
{
	// All functions start this way. So does PrintInteger.
	Encode(PUSH_EBP);
	Encode(MOV_EBP_ESP1);
	Encode(MOV_EBP_ESP2);
	// Make sure we save and restore all 5 Callee-Save registers.
	// That is, EBP, ESP, EBX, ESI, and EDI,
	Encode(PUSH_EBX);
	Encode(PUSH_ESI);
	Encode(PUSH_EDI);

	// Get the integer to print from mPrintInteger:
	Encode(MEM_TO_EAX);
	Encode(&mPrintInteger);

	// Check if the number to print is negative:
	Encode(CMP_EAX1);
	Encode(CMP_EAX2);
	Encode((unsigned char)0);

	// Jump if the integer is Not negative
	Encode(JGE);
	unsigned char DistanceToJump = 0; // fill in later
	int fillInAddress = mCurrent;
	Encode(DistanceToJump); // fill in this distance later
	unsigned char * jumpFrom = GetAddress();

	// Negate negative integers:
	Encode(NEG_EAX1);
	Encode(NEG_EAX2);

	// Print the minus sign, first saving register EAX.
	Encode(PUSH_EAX);
	WriteMinusLinux64();
	Encode(POP_EAX);

	// Fill in how far to jump from before:
	unsigned char * beginningOfPrintPositiveInteger = GetAddress();
	mCode[fillInAddress]= (unsigned char)(beginningOfPrintPositiveInteger -jumpFrom);

	// Jump to here!
	// Beginning of Write Positive Integer:

	// ECX is counter for how many decimal bytes are in the integer
	Encode(IMMEDIATE_TO_ECX);
	Encode((int)0);

	// Beginning of loop1.
	// It puts all the ascii characters of the integer on the stack.
		unsigned char *divide_loop= GetAddress(); 

		// increment the counter
		Encode(ADD_ECX1);
		Encode(ADD_ECX2);
		Encode((unsigned char)1);

		// Clear out high bytes before division
		Encode(CDQ);    // Necessary to clear the D register.

		// edx = eax % 10
		Encode(IMMEDIATE_TO_EBX);
		Encode((int)10); // Divide by 10
		Encode(DIV_EAX_EBX1); // divide eax by ebx
		Encode(DIV_EAX_EBX2); // remainder result in edx

		// convert decimal remainder to ascii by adding 48.
		Encode(ADD_EDX1);
		Encode(ADD_EDX2);
		Encode((unsigned char)48);

		// ascii remainder onto stack
		Encode(PUSH_EDX);

		// we stop loop1 when quotient (eax) is zero
		Encode(CMP_EAX1);
		Encode(CMP_EAX2);
		Encode((unsigned char)0);

		// repeat loop1 while quotient != 0. Encode the jump.
		unsigned char *end_divide_loop = GetAddress() + 2;
		Encode(JNE);
		Encode((unsigned char)(divide_loop-end_divide_loop));
	// End of loop1.

	// Beginning of loop2. 
	// It pops and prints all ascii characters.
		unsigned char *print_loop = GetAddress();

		// Code to print a character using syscall:
		{
		Encode(POP_EAX);
		Encode(EAX_TO_MEM);
		Encode(&mPrintInteger);

		Encode(BIT64);
		Encode(IMMEDIATE_TO_ESI); // move into ESI
		Encode(&mPrintInteger); // address of bytes to print

		Encode(IMMEDIATE_TO_EAX);
		Encode((int)1); // 1 for print syscall

		Encode(IMMEDIATE_TO_EDI);
		Encode((int)1); // 1 for stdout port

		Encode(IMMEDIATE_TO_EDX);
		Encode((int)1); // length of bytes to print

		// Engage 64 bit syscall with special codes 0x0F, 0x05
		// The EAX register indicates which syscall (print)
		// The EDI tells which port to print to (stdout)
		// The ESI contains the address to print
		// The EDX says how many bytes to print
		Encode(PUSH_ECX); // First save important registers
		Encode((unsigned char)SYS_CALL1);
		Encode((unsigned char)SYS_CALL2);
		Encode(POP_ECX); // Restore important register
		}

		// Decrement the count of characters left to print
		Encode(SUB_ECX1);
		Encode(SUB_ECX2);
		Encode((unsigned char)1);

		// Are there no characters left to print?
		Encode(CMP_ECX1);
		Encode(CMP_ECX2);
		Encode((unsigned char)0);

		// Repeat loop if there are more characters to print
		unsigned char *end_print_loop = GetAddress() + 2;
		Encode(JNE);
		Encode((unsigned char)(print_loop-end_print_loop));
	// End of loop2.

	// Separate multiple printed items with a space.
	WriteSpaceLinux64();

	// Restore Callee-Saved registers:
	Encode(POP_EDI);
	Encode(POP_ESI);
	Encode(POP_EBX);
	// All functions end this way:
	Encode(POP_EBP);
	Encode(NEAR_RET);
}

// 64 bit print syscall Taken from:
// https://stackoverflow.com/questions/22503944/using-interrupt-0x80-on-64-bit-linux
void InstructionsClass::WriteMinusLinux64()
{
	Encode(IMMEDIATE_TO_EAX);
	Encode((int)1); // 1 for print syscall

	Encode(IMMEDIATE_TO_EDI);
	Encode((int)1); // 1 for stdout port

	Encode(BIT64);
	Encode(IMMEDIATE_TO_ESI); 
	Encode(&mMinusString); // address of bytes to print

	Encode(IMMEDIATE_TO_EDX);
	Encode((int)1); // length of bytes to print

	// 64 bit syscall:
	Encode((unsigned char)SYS_CALL1);
	Encode((unsigned char)SYS_CALL2);
}

void InstructionsClass::WriteSpaceLinux64()
{
	Encode(IMMEDIATE_TO_EAX);
	Encode((int)1); // 1 for print syscall

	Encode(IMMEDIATE_TO_EDI);
	Encode((int)1); // 1 for stdout port

	Encode(BIT64);
	Encode(IMMEDIATE_TO_ESI); 
	Encode(&mSpaceString); // address of bytes to print

	Encode(IMMEDIATE_TO_EDX);
	Encode((int)1); // length of bytes to print

	// 64 bit syscall:
	Encode((unsigned char)SYS_CALL1);
	Encode((unsigned char)SYS_CALL2);
}

void InstructionsClass::PopAndWrite()
{
	// Move the integer to be printed from stack to mPrintInteger:
	Encode(POP_EAX);
	Encode(EAX_TO_MEM);
	Encode(&mPrintInteger);

	// Call previously coded function that prints mPrintInteger
	Call( (void*) &(mCode[mStartOfPrint])); 
		// &(mCode[mStartOfPrint])is where PrintIntegerLinux64 is.
}

void InstructionsClass::PopAndStore(int index)
{
    // Remove the integer from the stack and move it to the EAX register
    Encode(POP_EAX);

    // Move the value from the EAX register to the correct RAM address
    Encode(EAX_TO_MEM);
    Encode(GetMem(index)); // Get the RAM address using GetMem and encode it
}

void InstructionsClass::PopAndStoreTemp()
{
	Encode(POP_EAX);
	Encode(EAX_TO_MEM);
	Encode(&mTempInteger);
}

void InstructionsClass::Call(void * function_address)
{
	unsigned char * a1 = (unsigned char*)function_address;
	unsigned char * a2 = (unsigned char*)(&mCode[mCurrent+5]);
	int offset = (int)(a1 - a2);
	Encode(CALL);
	Encode(offset);
}

unsigned char * InstructionsClass::GetAddress()
{
	return &(mCode[mCurrent]);
}


void InstructionsClass::Finish(){
    // Retore Callee-Saved registers:
	Encode(POP_EDI);
	Encode(POP_ESI);
	Encode(POP_EBX);

	// All functions end this way:
	Encode(POP_EBP);
	Encode(NEAR_RET);


	// std::cout << "Finished creating " << mCurrent << 
	// 	" bytes of machine code" << std::endl;
}

void InstructionsClass::Execute(){
	// Jump into the main function of what we just coded, 
	//	found at mCode[mStartOfMain]
	// std::cout << "About to Execute the machine code..." << std::endl;
    try{
        void * ptr = &(mCode[mStartOfMain]);
        // std::cout << "ptr: " << ptr << std::endl;
        try{
            void (*f)(void);
            f = (void (*)(void)) ptr ;
            f();
        }
        catch(std::bad_alloc &e){
            std::cerr << "Error.  Could not execute the machine code." << std::endl;
            exit(1);
        }
        // Did everything work?
        std::cout << "\nThere and back again!" << std::endl; 
    }catch (std::bad_alloc &e){
        std::cerr << "Error.  Could not allocate memory for the machine code." << std::endl;
        std::cerr << "Error.  Could not execute the machine code." << std::endl;
        exit(1);
    } 


}

void InstructionsClass::Encode(int x){

    if(mCurrent + sizeof(unsigned char) < MAX_INSTRUCTIONS)
	{
        *((int *) (&mCode[mCurrent])) = x;
        mCurrent += sizeof(int);
	}
	else
	{
		std::cerr << "Error.  Used up all " << MAX_INSTRUCTIONS 
			<< " instructions." << std::endl;
		exit(1);
	}
}

void InstructionsClass::Encode(long long x) {
    if (mCurrent + sizeof(long long) < MAX_INSTRUCTIONS) {
        *((long long *)(&mCode[mCurrent])) = x;
        mCurrent += sizeof(long long);
    } else {
        std::cerr << "Error.  Used up all " << MAX_INSTRUCTIONS 
                  << " instructions." << std::endl;
        exit(1);
    }
}

void InstructionsClass::Encode(void * p){
        int pointerSize = sizeof(p);

        if (pointerSize==4)
        {
                Encode((int)(long long)p);
        }
        else if(sizeof(p)==8)
        {
                Encode((long long)p);
        }
}

void InstructionsClass::PushValue(int value){
    Encode(IMMEDIATE_TO_EAX);
    Encode(value);
    Encode(PUSH_EAX);
}

void InstructionsClass::PushVariable(int p){
    Encode(MEM_TO_EAX);
    Encode(GetMem(p));
    Encode(PUSH_EAX);
}

int *InstructionsClass::GetMem(int index){
    if (index < 0 || index >= MAX_DATA) {
        std::cerr << "Error.  Index out of bounds." << std::endl;
        exit(1);
    }
    return &(mData[index]);
}

void InstructionsClass::PopPopDivPush()
{
    Encode(POP_EBX);
    Encode(POP_EAX);
    Encode(CDQ); // Necessary to clear the EDX before the divide.
    Encode(DIV_EAX_EBX1); // Divide EAX by EBX. Result in EAX.
    Encode(DIV_EAX_EBX2);
    Encode(PUSH_EAX);
}

void InstructionsClass::PopPopAddPush()
{
    Encode(POP_EBX);
    Encode(POP_EAX);
    Encode(ADD_EAX_EBX1);
    Encode(ADD_EAX_EBX2);
    Encode(PUSH_EAX);
}

void InstructionsClass::PopPopMulPush()
{
    Encode(POP_EBX);
    Encode(POP_EAX);
    Encode(MUL_EAX_EBX1);
    Encode(MUL_EAX_EBX2);
    Encode(PUSH_EAX);
}

void InstructionsClass::PopPopModPush()
{
	Encode(POP_EBX);
	Encode(POP_EAX);
	Encode(CDQ);
	Encode(DIV_EAX_EBX1);
	Encode(DIV_EAX_EBX2);
	Encode(PUSH_EDX);
}

void InstructionsClass::PopPopSubPush()
{
    Encode(POP_EBX);
    Encode(POP_EAX);
    Encode(SUB_EAX_EBX1);
    Encode(SUB_EAX_EBX2);
    Encode(PUSH_EAX);
}

void InstructionsClass::PopPopComparePush(unsigned char
    relational_operator)
{
    Encode(POP_EBX);
    Encode(POP_EAX);
    Encode(CMP_EAX_EBX1);
    Encode(CMP_EAX_EBX2); // The FLAG register is now set.
    Encode(IMMEDIATE_TO_EAX); // load A register with 1
    Encode(1); // assume the result of compare is 1, or TRUE.
    Encode(relational_operator); 
    // Depending on the FLAG register and this 
    // particular relational_operator,
    // possibly skip around setting A register
    // to zero, or FALSE, leaving it at TRUE.
    Encode((unsigned char)5);
    Encode(IMMEDIATE_TO_EAX); // load A register with 0
    Encode(0);
    Encode(PUSH_EAX); // push 1 or 0
}

void InstructionsClass::PopPopLessPush()
{
        PopPopComparePush(JL);
}

void InstructionsClass::PopPopLessEqualPush()
{
        PopPopComparePush(JLE);
}
void InstructionsClass::PopPopGreaterPush()
{
        PopPopComparePush(JG);
}
void InstructionsClass::PopPopGreaterEqualPush()
{
        PopPopComparePush(JGE);
}
void InstructionsClass::PopPopEqualPush()
{
        PopPopComparePush(JE);
}
void InstructionsClass::PopPopNotEqualPush()
{
        PopPopComparePush(JNE);
}

void InstructionsClass::PopPopAndPush()
{
        Encode(IMMEDIATE_TO_EAX); // load A register with 0
        Encode(0);
        Encode(POP_EBX); // load B register with stack item 2
        Encode(CMP_EAX_EBX1);
        Encode(CMP_EAX_EBX2);
        Encode(POP_EBX); // load B register with stack item 1
        Encode(JE); // if stack item 2 is zero, jump to FALSE code
        Encode((unsigned char)11);
        Encode(CMP_EAX_EBX1);
        Encode(CMP_EAX_EBX2);
        Encode(JE); // if stack item 1 is zero, jump to FALSE code
        Encode((unsigned char)7);
        // TRUE code:
        Encode(IMMEDIATE_TO_EAX); // load A register with 1
        Encode(1);
        Encode(JUMP_ALWAYS); // Jump around FALSE code
        Encode((unsigned char)5);
        // FALSE code:
        Encode(IMMEDIATE_TO_EAX); // load A register with 0
        Encode(0);
        // Save A to the stack
        Encode(PUSH_EAX); // push 1 or 0
}

void InstructionsClass::PopPopOrPush()
{
        Encode(IMMEDIATE_TO_EAX); // load A register with 0
        Encode(0);
        Encode(POP_EBX); // load B register with stack item 2
        Encode(CMP_EAX_EBX1);
        Encode(CMP_EAX_EBX2);
        Encode(POP_EBX); // load B register with stack item 1
        Encode(JNE); // if stack item 2 is not zero, jump to TRUE code
        Encode((unsigned char)11);
        Encode(CMP_EAX_EBX1);
        Encode(CMP_EAX_EBX2);
        Encode(JNE); // if stack item 1 is not zero, jump to TRUE code
        Encode((unsigned char)7);
        // FALSE code:
        Encode(IMMEDIATE_TO_EAX); // load A register with 0
        Encode(0);
        Encode(JUMP_ALWAYS); // Jump around TRUE code
        Encode((unsigned char)5);
        // TRUE code:
        Encode(IMMEDIATE_TO_EAX); // load A register with 1
        Encode(1);
        // Save A to the stack
        Encode(PUSH_EAX); // push 1 or 0
}

void InstructionsClass::PushTemp()
{
	Encode(MEM_TO_EAX);
	Encode(&mTempInteger);
	Encode(PUSH_EAX);
}

unsigned char *InstructionsClass::SkipIfZeroStack()
{
        Encode(POP_EBX);
        Encode(IMMEDIATE_TO_EAX); // load A register with 0
        Encode(0);
        Encode(CMP_EAX_EBX1);
        Encode(CMP_EAX_EBX2);
        Encode(JE_FAR1); // If stack had zero, do a jump
        Encode(JE_FAR2);
        unsigned char * addressToFillInLater = GetAddress();
        Encode(0); // the exact number of bytes to skip gets set later,
                   // when we know it!  Call SetOffset() to do that.
        return addressToFillInLater;
}

unsigned char *  InstructionsClass::Jump()
{
        Encode(JUMP_ALWAYS_FAR);
        unsigned char * addressToFillInLater = GetAddress();
        Encode(0); // the exact number of bytes to jump gets set later,
                   // when we know it!  Call SetOffset() to do that.
        return addressToFillInLater;
}

void InstructionsClass::SetOffset(unsigned char * codeAddress, int offset)
{
        *((int*)codeAddress) = offset;
}

void InstructionsClass::PrintAllMachineCodes()
{
	for (int i=0; i<mCurrent; i++)
	{
	printf("HEX: %2x  Decimal: %3i\n", (int)mCode[i], (int)mCode[i]);
	}
}

int InstructionsClass::AllocateSlot()
{
    return NextFreeSlot++;
}

void InstructionsClass::RegisterVariableSlot(int slot)
{
	if (slot+1 > NextFreeSlot)
	{
		NextFreeSlot = slot + 1;
	}
}

void InstructionsClass::WriteEndLinux64()
{
	Encode(IMMEDIATE_TO_EAX);
	Encode((int)1);

	Encode(IMMEDIATE_TO_EDI);
	Encode((int)1);

	Encode(BIT64);
	Encode(IMMEDIATE_TO_ESI);
	Encode(&mEndlString);

    Encode(IMMEDIATE_TO_EDX);
    Encode((int)1);

    Encode((unsigned char)SYS_CALL1);
    Encode((unsigned char)SYS_CALL2);
}
