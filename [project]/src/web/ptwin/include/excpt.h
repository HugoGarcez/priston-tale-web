// excpt.h - web build: the Windows SDK headers include this MSVC header; nothing
// in it is needed in WebAssembly (no structured exception handling).
#pragma once
#define EXCEPTION_EXECUTE_HANDLER		1
#define EXCEPTION_CONTINUE_SEARCH		0
#define EXCEPTION_CONTINUE_EXECUTION	(-1)
typedef enum _EXCEPTION_DISPOSITION {
	ExceptionContinueExecution, ExceptionContinueSearch, ExceptionNestedException, ExceptionCollidedUnwind
} EXCEPTION_DISPOSITION;
