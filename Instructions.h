const int MAX_INSTRUCTIONS = 5000;
const int MAX_DATA = 5000;
class InstructionsClass
{
public:
InstructionsClass(); 
	void Finish(); 
	void Execute(); 
	void PushValue(int value);
	void PopAndWrite();
	unsigned char * GetAddress();
	void PushVariable(int index);
	void PopAndStore(int index);
	void PopPopDivPush();
	void PopPopAddPush();
	void PopPopSubPush();
	void PopPopMulPush();

	void PopPopLessPush();
	void PopPopLessEqualPush();
	void PopPopGreaterPush();
	void PopPopGreaterEqualPush();
	void PopPopEqualPush();
	void PopPopNotEqualPush();

	void PopPopAndPush();
	void PopPopOrPush();

	unsigned char * SkipIfZeroStack();
	unsigned char * Jump();
	void SetOffset(unsigned char * codeAddress, int offset);
	void PrintAllMachineCodes();


private:
	unsigned char mCode[MAX_INSTRUCTIONS]; 
	int mCurrent;
	int mTempInteger; 
    int mPrintInteger;
	int mStartOfPrint;
	int mStartOfMain; 
	char mMinusString; // Holds '-'
	char mSpaceString; // Holds ' '
    int mData[MAX_DATA];

    void Encode(unsigned char c);
    void Encode(int x);
    void Encode(long long x);
    void Encode(void * p);
    int * GetMem(int index);
    void PrintIntegerLinux64();
    void WriteMinusLinux64();
    void WriteSpaceLinux64();
    void Call(void *function_address);
    void PopPopComparePush(unsigned char relational_operator);


};
