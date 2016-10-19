#include "ExceptionHandler.h"
#include <eh.h> 
#include <signal.h>
#include <sstream>
#include "new.h"
#include "DbgHelp.h"
#include <shlobj.h>

#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "Dbghelp.lib")
using std::string;
using std::ostringstream;
//using namespace skt;

#ifndef _AddressOfReturnAddress

// Taken from: http://msdn.microsoft.com/en-us/library/s975zw7k(VS.71).aspx
#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

// _ReturnAddress and _AddressOfReturnAddress should be prototyped before use 
EXTERNC void * _AddressOfReturnAddress(void);
EXTERNC void * _ReturnAddress(void);

#endif 

void dumpException(EXCEPTION_POINTERS *pException)
{
	//OutputDebugString(L"lwl, wke.dd 走到dumpException\n");
	char MyDir[_MAX_PATH];
	SHGetSpecialFolderPathA(NULL, MyDir, CSIDL_APPDATA, 0);
	std::string logPath(MyDir);
	logPath += "\\2LV Files\\logs\\";

	SYSTEMTIME st = { 0 };
	::GetLocalTime(&st);
	char cBuf[50] = { 0 };
	sprintf_s(cBuf, _countof(cBuf), "%04d-%02d-%02d %02d%02d%02d",
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	char cBufName[50] = { 0 };
	sprintf_s(cBufName, "2LvReport_%s.dmp", cBuf);
	logPath += cBufName;

	//OutputDebugStringA(logPath.c_str());

	// 创建Dump文件  
	//  
	HANDLE hDumpFile = CreateFileA(logPath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hDumpFile == INVALID_HANDLE_VALUE)
	{
		//OutputDebugString(L"lwl, wke创建deump文件失败");
		return;
	}
	// Dump信息  
	//  
	MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
	dumpInfo.ExceptionPointers = pException;
	dumpInfo.ThreadId = GetCurrentThreadId();
	dumpInfo.ClientPointers = TRUE;

	// 写入Dump文件内容  
	//  
	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);

	CloseHandle(hDumpFile);
}


const char* opDescription(ULONG opcode)
{
	switch (opcode) 
	{
	case 0: 
		return "read";
	case 1: 
		return "write";
	case 8: 
		return "user-mode data execution prevention (DEP) violation";
	default: 
		return "unknown";
	}
}

const char* seDescription(uint32_t code)
{
	switch (code) 
	{
	case EXCEPTION_ACCESS_VIOLATION:
		return "EXCEPTION_ACCESS_VIOLATION : "
			"The thread tried to read from or write to a virtual address"
			" for which it does not have the appropriate access.";
	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
		return "EXCEPTION_ARRAY_BOUNDS_EXCEEDED : "
			"The thread tried to access an array element that is out of bounds"
			" and the underlying hardware supports bounds checking.";
	case EXCEPTION_BREAKPOINT:
		return "EXCEPTION_BREAKPOINT : A breakpoint was encountered.";
	case EXCEPTION_DATATYPE_MISALIGNMENT:
		return "EXCEPTION_DATATYPE_MISALIGNMENT : "
			"The thread tried to read or write data that is misaligned on hardware that does not provide alignment."
			" For example, 16-bit values must be aligned on 2-byte boundaries; 32-bit values on 4-byte boundaries, and so on.";
	case EXCEPTION_FLT_DENORMAL_OPERAND:
		return "EXCEPTION_FLT_DENORMAL_OPERAND : "
			"One of the operands in a floating-point operation is denormal."
			" A denormal value is one that is too small to represent as a standard floating-point value.";
	case EXCEPTION_FLT_DIVIDE_BY_ZERO:
		return "EXCEPTION_FLT_DIVIDE_BY_ZERO : The thread tried to divide a floating-point value by a floating-point divisor of zero.";
	case EXCEPTION_FLT_INEXACT_RESULT:
		return "EXCEPTION_FLT_INEXACT_RESULT : The result of a floating-point operation cannot be represented exactly as a decimal fraction.";
	case EXCEPTION_FLT_INVALID_OPERATION:
		return "EXCEPTION_FLT_INVALID_OPERATION : This exception represents any floating-point exception not included in this list.";
	case EXCEPTION_FLT_OVERFLOW:
		return "EXCEPTION_FLT_OVERFLOW : The exponent of a floating-point operation is greater than the magnitude allowed by the corresponding type.";
	case EXCEPTION_FLT_STACK_CHECK:
		return "EXCEPTION_FLT_STACK_CHECK : The stack overflowed or underflowed as the result of a floating-point operation.";
	case EXCEPTION_FLT_UNDERFLOW:
		return "EXCEPTION_FLT_UNDERFLOW : The exponent of a floating-point operation is less than the magnitude allowed by the corresponding type.";
	case EXCEPTION_ILLEGAL_INSTRUCTION:
		return "EXCEPTION_ILLEGAL_INSTRUCTION : The thread tried to execute an invalid instruction.";
	case EXCEPTION_IN_PAGE_ERROR:
		return "EXCEPTION_IN_PAGE_ERROR : The thread tried to access a page that was not present, and the system was unable to load the page."
			" For example, this exception might occur if a network connection is lost while running a program over the network.";
	case EXCEPTION_INT_DIVIDE_BY_ZERO:
		return "EXCEPTION_INT_DIVIDE_BY_ZERO : The thread tried to divide an integer value by an integer divisor of zero.";
	case EXCEPTION_INT_OVERFLOW:
		return "EXCEPTION_INT_OVERFLOW : The result of an integer operation caused a carry out of the most significant bit of the result.";
	case EXCEPTION_INVALID_DISPOSITION:
		return "EXCEPTION_INVALID_DISPOSITION : "
			"An exception handler returned an invalid disposition to the exception dispatcher."
			" Programmers using a high-level language such as C should never encounter this exception.";
	case EXCEPTION_NONCONTINUABLE_EXCEPTION:
		return "EXCEPTION_NONCONTINUABLE_EXCEPTION : The thread tried to continue execution after a noncontinuable exception occurred.";
	case EXCEPTION_PRIV_INSTRUCTION:
		return "EXCEPTION_PRIV_INSTRUCTION : The thread tried to execute an instruction whose operation is not allowed in the current machine mode.";
	case EXCEPTION_SINGLE_STEP:
		return "EXCEPTION_SINGLE_STEP : A trace trap or other single-instruction mechanism signaled that one instruction has been executed.";
	case EXCEPTION_STACK_OVERFLOW:
		return "EXCEPTION_STACK_OVERFLOW : The thread used up its stack.";
	default:
		return "Unknown Exception.";
	}
}

std::string exInformation(const _EXCEPTION_POINTERS* ep)
{
	uint32_t code = ep->ExceptionRecord->ExceptionCode;

	std::ostringstream oss;
	oss << "exception detail : " << std::endl
		<< seDescription(code);

	if (code == EXCEPTION_ACCESS_VIOLATION || code == EXCEPTION_IN_PAGE_ERROR) {
		oss << std::endl;
		oss << "Invalid operation: " << opDescription(ep->ExceptionRecord->ExceptionInformation[0]) 
			<< " at address 0x" << std::hex << ep->ExceptionRecord->ExceptionInformation[1] << std::dec << "\n";
	}

	if (code == EXCEPTION_IN_PAGE_ERROR) {
		oss << std::endl;
		oss << "Underlying NTSTATUS code that resulted in the exception " << ep->ExceptionRecord->ExceptionInformation[2] << "\n";
	}

	return oss.str();
}

LPTOP_LEVEL_EXCEPTION_FILTER WINAPI MyDummySetUnhandledExceptionFilter(LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter)
{
	return NULL;
}

BOOL PreventSetUnhandledExceptionFilter()
{
	HMODULE hKernel32 = LoadLibrary(L"kernel32.dll");
	if (hKernel32 == NULL)
		return FALSE;


	void *pOrgEntry = GetProcAddress(hKernel32, "SetUnhandledExceptionFilter");
	if (pOrgEntry == NULL)
		return FALSE;


	unsigned char newJump[100];
	DWORD dwOrgEntryAddr = (DWORD)pOrgEntry;
	dwOrgEntryAddr += 5; // add 5 for 5 op-codes for jmp far


	void *pNewFunc = &MyDummySetUnhandledExceptionFilter;
	DWORD dwNewEntryAddr = (DWORD)pNewFunc;
	DWORD dwRelativeAddr = dwNewEntryAddr - dwOrgEntryAddr;


	newJump[0] = 0xE9;  // JMP absolute
	memcpy(&newJump[1], &dwRelativeAddr, sizeof(pNewFunc));
	SIZE_T bytesWritten;
	BOOL bRet = WriteProcessMemory(GetCurrentProcess(), pOrgEntry, newJump, sizeof(pNewFunc) + 1, &bytesWritten);
	return bRet;
}

void ExceptionHandler::SetProcessExceptionHandlers()
{
	_set_se_translator(StructuredExceptionHandler);

	// Catch pure virtual function calls.
	// Because there is one _purecall_handler for the whole process, 
	// calling this function immediately impacts all threads. The last 
	// caller on any thread sets the handler. 
	// http://msdn.microsoft.com/en-us/library/t296ys27.aspx
	_set_purecall_handler(PureCallHandler);

	// Catch new operator memory allocation exceptions
	_set_new_handler(NewHandler);

	// Catch invalid parameter exceptions.
	_set_invalid_parameter_handler(InvalidParameterHandler);

	// Set up C++ signal handlers

	_set_abort_behavior(_CALL_REPORTFAULT, _CALL_REPORTFAULT);

	// Catch an abnormal program termination
	signal(SIGABRT, SigabrtHandler);

	// Catch illegal instruction handler
	signal(SIGINT, SigintHandler);

	// Catch a termination request
	signal(SIGTERM, SigtermHandler);

	// Install top-level SEH handler
	SetUnhandledExceptionFilter(SehHandler);

	PreventSetUnhandledExceptionFilter();
}

void ExceptionHandler::SetThreadExceptionHandlers()
{
	// Catch terminate() calls. 
	// In a multithreaded environment, terminate functions are maintained 
	// separately for each thread. Each new thread needs to install its own 
	// terminate function. Thus, each thread is in charge of its own termination handling.
	// http://msdn.microsoft.com/en-us/library/t6fk7h29.aspx
	set_terminate(TerminateHandler);

	// Catch unexpected() calls.
	// In a multithreaded environment, unexpected functions are maintained 
	// separately for each thread. Each new thread needs to install its own 
	// unexpected function. Thus, each thread is in charge of its own unexpected handling.
	// http://msdn.microsoft.com/en-us/library/h46t5b69.aspx  
	set_unexpected(UnexpectedHandler);

	// Catch a floating point error
	typedef void(*sigh)(int);
	signal(SIGFPE, (sigh)SigfpeHandler);

	// Catch an illegal instruction
	signal(SIGILL, SigillHandler);

	// Catch illegal storage access errors
	signal(SIGSEGV, SigsegvHandler);

}

// The following code gets exception pointers using a workaround found in CRT code.
void ExceptionHandler::GetExceptionPointers(DWORD dwExceptionCode,
	EXCEPTION_POINTERS** ppExceptionPointers)
{
	// The following code was taken from VC++ 8.0 CRT (invarg.c: line 104)

	EXCEPTION_RECORD ExceptionRecord;
	CONTEXT ContextRecord;
	memset(&ContextRecord, 0, sizeof(CONTEXT));

#ifdef _X86_

	__asm {
		mov dword ptr[ContextRecord.Eax], eax
			mov dword ptr[ContextRecord.Ecx], ecx
			mov dword ptr[ContextRecord.Edx], edx
			mov dword ptr[ContextRecord.Ebx], ebx
			mov dword ptr[ContextRecord.Esi], esi
			mov dword ptr[ContextRecord.Edi], edi
			mov word ptr[ContextRecord.SegSs], ss
			mov word ptr[ContextRecord.SegCs], cs
			mov word ptr[ContextRecord.SegDs], ds
			mov word ptr[ContextRecord.SegEs], es
			mov word ptr[ContextRecord.SegFs], fs
			mov word ptr[ContextRecord.SegGs], gs
			pushfd
			pop[ContextRecord.EFlags]
	}

	ContextRecord.ContextFlags = CONTEXT_CONTROL;
#pragma warning(push)
#pragma warning(disable:4311)
	ContextRecord.Eip = (ULONG)_ReturnAddress();
	ContextRecord.Esp = (ULONG)_AddressOfReturnAddress();
#pragma warning(pop)
	ContextRecord.Ebp = *((ULONG *)_AddressOfReturnAddress() - 1);


#elif defined (_IA64_) || defined (_AMD64_)

	/* Need to fill up the Context in IA64 and AMD64. */
	RtlCaptureContext(&ContextRecord);

#else  /* defined (_IA64_) || defined (_AMD64_) */

	ZeroMemory(&ContextRecord, sizeof(ContextRecord));

#endif  /* defined (_IA64_) || defined (_AMD64_) */

	ZeroMemory(&ExceptionRecord, sizeof(EXCEPTION_RECORD));

	ExceptionRecord.ExceptionCode = dwExceptionCode;
	ExceptionRecord.ExceptionAddress = _ReturnAddress();

	///

	EXCEPTION_RECORD* pExceptionRecord = new EXCEPTION_RECORD;
	memcpy(pExceptionRecord, &ExceptionRecord, sizeof(EXCEPTION_RECORD));
	CONTEXT* pContextRecord = new CONTEXT;
	memcpy(pContextRecord, &ContextRecord, sizeof(CONTEXT));

	*ppExceptionPointers = new EXCEPTION_POINTERS;
	(*ppExceptionPointers)->ExceptionRecord = pExceptionRecord;
	(*ppExceptionPointers)->ContextRecord = pContextRecord;
}

// Structured exception handler
LONG WINAPI ExceptionHandler::SehHandler(PEXCEPTION_POINTERS pExceptionPtrs)
{
	dumpException(pExceptionPtrs);
	//throw EngineException("Unhandled structured exception." + exInformation(pExceptionPtrs));

	// Unreacheable code  
	return EXCEPTION_EXECUTE_HANDLER;
}

void ExceptionHandler::StructuredExceptionHandler(unsigned int u, _EXCEPTION_POINTERS* pExp)
{
	dumpException(pExp);

	//throw EngineException("Structured exception." + exInformation(pExp));
}

// CRT terminate() call handler
void __cdecl ExceptionHandler::TerminateHandler()
{
	// Abnormal program termination (terminate() function was called)

	// Retrieve exception information
	EXCEPTION_POINTERS* pExceptionPtrs = NULL;
	GetExceptionPointers(0, &pExceptionPtrs);

	dumpException(pExceptionPtrs);

	//throw EngineException("CRT terminate() call." + exInformation(pExceptionPtrs));
}

// CRT unexpected() call handler
void __cdecl ExceptionHandler::UnexpectedHandler()
{
	// Unexpected error (unexpected() function was called)

	// Retrieve exception information
	EXCEPTION_POINTERS* pExceptionPtrs = NULL;
	GetExceptionPointers(0, &pExceptionPtrs);

	dumpException(pExceptionPtrs);

	//throw EngineException("CRT unexpected() call." + exInformation(pExceptionPtrs));
}

// CRT Pure virtual method call handler
void __cdecl ExceptionHandler::PureCallHandler()
{
	// Pure virtual function call

	// Retrieve exception information
	EXCEPTION_POINTERS* pExceptionPtrs = NULL;
	GetExceptionPointers(0, &pExceptionPtrs);

	dumpException(pExceptionPtrs);

	//throw EngineException("CRT Pure virtual method call." + exInformation(pExceptionPtrs));

}


// CRT invalid parameter handler
void __cdecl ExceptionHandler::InvalidParameterHandler(
	const wchar_t* expression,
	const wchar_t* function,
	const wchar_t* file,
	unsigned int line,
	uintptr_t pReserved)
{
	pReserved;

	// Invalid parameter exception

	// Retrieve exception information
	EXCEPTION_POINTERS* pExceptionPtrs = NULL;
	GetExceptionPointers(0, &pExceptionPtrs);

	dumpException(pExceptionPtrs);

	//throw EngineException("CRT invalid parameter." + exInformation(pExceptionPtrs));

}

// CRT new operator fault handler
int __cdecl ExceptionHandler::NewHandler(size_t)
{
	// 'new' operator memory allocation exception

	// Retrieve exception information
	EXCEPTION_POINTERS* pExceptionPtrs = NULL;
	GetExceptionPointers(0, &pExceptionPtrs);

	dumpException(pExceptionPtrs);

	//throw EngineException("CRT new operator fault." + exInformation(pExceptionPtrs));

	// Unreacheable code
	return 0;
}

// CRT SIGABRT signal handler
void ExceptionHandler::SigabrtHandler(int)
{
	// Caught SIGABRT C++ signal

	// Retrieve exception information
	EXCEPTION_POINTERS* pExceptionPtrs = NULL;
	GetExceptionPointers(0, &pExceptionPtrs);

	dumpException(pExceptionPtrs);

	//throw EngineException("CRT SIGABRT signal." + exInformation(pExceptionPtrs));

}

// CRT SIGFPE signal handler
void ExceptionHandler::SigfpeHandler(int /*code*/, int subcode)
{
	// Floating point exception (SIGFPE)

	EXCEPTION_POINTERS* pExceptionPtrs = (PEXCEPTION_POINTERS)_pxcptinfoptrs;

	dumpException(pExceptionPtrs);

	//throw EngineException("CRT SIGFPE signal." + exInformation(pExceptionPtrs));

}

// CRT sigill signal handler
void ExceptionHandler::SigillHandler(int)
{
	// Illegal instruction (SIGILL)

	// Retrieve exception information
	EXCEPTION_POINTERS* pExceptionPtrs = NULL;
	GetExceptionPointers(0, &pExceptionPtrs);

	dumpException(pExceptionPtrs);

	//throw EngineException("CRT sigill signal." + exInformation(pExceptionPtrs));

}

// CRT sigint signal handler
void ExceptionHandler::SigintHandler(int)
{
	// Interruption (SIGINT)

	// Retrieve exception information
	EXCEPTION_POINTERS* pExceptionPtrs = NULL;
	GetExceptionPointers(0, &pExceptionPtrs);

	dumpException(pExceptionPtrs);

	//throw EngineException("CRT sigint signal." + exInformation(pExceptionPtrs));

}

// CRT SIGSEGV signal handler
void ExceptionHandler::SigsegvHandler(int)
{
	// Invalid storage access (SIGSEGV)

	PEXCEPTION_POINTERS pExceptionPtrs = (PEXCEPTION_POINTERS)_pxcptinfoptrs;

	dumpException(pExceptionPtrs);

	//throw EngineException("CRT SIGSEGV signal." + exInformation(pExceptionPtrs));

}

// CRT SIGTERM signal handler
void ExceptionHandler::SigtermHandler(int)
{
	// Termination request (SIGTERM)

	// Retrieve exception information
	EXCEPTION_POINTERS* pExceptionPtrs = NULL;
	GetExceptionPointers(0, &pExceptionPtrs);

	dumpException(pExceptionPtrs);

	//throw EngineException("CRT SIGTERM signal." + exInformation(pExceptionPtrs));
}


