#pragma once

class TESObjectREFR;
class Script;
class ScriptLocals;
class SCRIPT_LINE;
class ScriptCompileData;
class SCRIPT_PARAMETER;
class SCRIPT_FUNCTION;
struct PluginInfo;

typedef bool (*Cmd_Execute)(SCRIPT_PARAMETER* apParameters, char* apCompiledParams, TESObjectREFR* apRef, TESObjectREFR* apContainer, Script* apScript, ScriptLocals* apScriptLocals, double* adResult, uint32_t* auiOffset);
typedef bool (*Cmd_Parse)(uint32_t auiParamCount, SCRIPT_PARAMETER* apParameters, SCRIPT_LINE* apScriptLine, ScriptCompileData* apCompileData);
typedef bool (*Cmd_Cond)(TESObjectREFR* apRef, void* apParam1, void* apParam2, double* adResult);

enum CommandReturnType : uint8_t
{
	kRetnType_Default,
	kRetnType_Form,
	kRetnType_String,
	kRetnType_Array,
	kRetnType_ArrayIndex,
	kRetnType_Ambiguous,

	kRetnType_Max
};

#define USE_EXTRACT_ARGS_EX NVSE_CORE

#define COMMAND_ARGS		SCRIPT_PARAMETER* apParameters, char* apCompiledParams, TESObjectREFR* apRef, TESObjectREFR* apContainer, Script* apScript, ScriptLocals* apScriptLocals, double* adResult, uint32_t* auiOffset
#define COMMAND_ARGS_EX		SCRIPT_PARAMETER* apParameters, char* apCompiledParams, uint32_t* auiOffset, Script* apScript, ScriptLocals* apScriptLocals
#define PASS_COMMAND_ARGS	apParameters, apCompiledParams, apRef, apContainer, apScript, apScriptLocals, adResult, auiOffset
#define COMMAND_ARGS_EVAL	TESObjectREFR* apRef, void* apParam1, void* apParam2, double* adResult
#define PASS_CMD_ARGS_EVAL	apRef, apParam1, apParam2, adResult
#define EXTRACT_ARGS_EX		apParameters, apCompiledParams, auiOffset, apScript, apScriptLocals
#define PASS_FMTSTR_ARGS	apParameters, apCompiledParams, auiOffset, apScript, apScriptLocals
#if USE_EXTRACT_ARGS_EX
#define EXTRACT_ARGS		EXTRACT_ARGS_EX
#else
#define EXTRACT_ARGS		apParameters, apCompiledParams, auiOffset, apRef, apContainer, apScript, apScriptLocals
#endif

//Macro to make SCRIPT_FUNCTION definitions a bit less tedious

#define DEFINE_CMD_FULL(name, altName, description, refRequired, numParams, paramInfo, parser) \
	extern bool Cmd_ ## name ## _Execute(COMMAND_ARGS); \
	static SCRIPT_FUNCTION (kCommandInfo_ ## name) = { \
	#name, \
	#altName, \
	0, \
	#description, \
	refRequired, \
	numParams, \
	paramInfo, \
	HANDLER(Cmd_ ## name ## _Execute), \
	parser, \
	NULL, \
	0 \
	};

// Deprecated, use DEFINE_CMD_ALIAS instead.
#define DEFINE_CMD_ALT(name, altName, description, refRequired, numParams, paramInfo) \
	DEFINE_CMD_FULL(name, altName, description, refRequired, numParams, paramInfo, Cmd_Default_Parse)	

#define DEFINE_CMD_ALIAS(name, altName, description, refRequired, paramInfo) \
	DEFINE_CMD_FULL(name, altName, description, refRequired, (paramInfo) ? (sizeof(paramInfo) / sizeof(SCRIPT_PARAMETER)) : 0, paramInfo, Cmd_Default_Parse)	

#define DEFINE_CMD_ALT_EXP(name, altName, description, refRequired, paramInfo) \
	DEFINE_CMD_FULL(name, altName, description, refRequired, (paramInfo) ? (sizeof(paramInfo) / sizeof(SCRIPT_PARAMETER)) : 0, paramInfo, Cmd_Expression_Parse)	

// Deprecated, use DEFINE_CMD instead.
#define DEFINE_COMMAND(name, description, refRequired, numParams, paramInfo) \
	DEFINE_CMD_FULL(name, , description, refRequired, numParams, paramInfo, Cmd_Default_Parse)	

#define DEFINE_CMD(name, description, refRequired, paramInfo) \
	DEFINE_CMD_FULL(name, , description, refRequired, (paramInfo) ? (sizeof(paramInfo) / sizeof(SCRIPT_PARAMETER)) : 0, paramInfo, Cmd_Default_Parse)

#define DEFINE_COMMAND_EXP(name, description, refRequired, paramInfo) \
	DEFINE_CMD_ALT_EXP(name, , description, refRequired, paramInfo)

#define DEFINE_COMMAND_PLUGIN(name, description, refRequired, paramInfo) \
	DEFINE_CMD_FULL(name, , description, refRequired, (paramInfo) ? (sizeof(paramInfo) / sizeof(SCRIPT_PARAMETER)) : 0, paramInfo, NULL)

#define DEFINE_COMMAND_ALT_PLUGIN(name, altName, description, refRequired, paramInfo) \
	DEFINE_CMD_FULL(name, altName, description, refRequired, (paramInfo) ? (sizeof(paramInfo) / sizeof(SCRIPT_PARAMETER)) : 0, paramInfo, NULL)

#define DEFINE_COMMAND_PLUGIN_EXP(name, description, refRequired, paramInfo) \
	DEFINE_CMD_FULL(name, , description, refRequired, (paramInfo) ? (sizeof(paramInfo) / sizeof(SCRIPT_PARAMETER)) : 0, paramInfo, Cmd_Expression_Plugin_Parse)

#define DEFINE_COMMAND_ALT_PLUGIN_EXP(name, altName, description, refRequired, paramInfo) \
	DEFINE_CMD_FULL(name, altName, description, refRequired, (paramInfo) ? (sizeof(paramInfo) / sizeof(SCRIPT_PARAMETER)) : 0, paramInfo, Cmd_Expression_Plugin_Parse)

// for commands which can be used as conditionals
#define DEFINE_CMD_ALT_COND_ANY(name, altName, description, refRequired, paramInfo, parser) \
	extern bool Cmd_ ## name ## _Execute(COMMAND_ARGS); \
	extern bool Cmd_ ## name ## _Eval(COMMAND_ARGS_EVAL); \
	static SCRIPT_FUNCTION (kCommandInfo_ ## name) = { \
	#name,	\
	#altName,		\
	0,		\
	#description,	\
	refRequired,	\
	(paramInfo) ? (sizeof(paramInfo) / sizeof(SCRIPT_PARAMETER)) : 0,	\
	paramInfo,	\
	HANDLER(Cmd_ ## name ## _Execute),	\
	parser,	\
	HANDLER_EVAL(Cmd_ ## name ## _Eval),	\
	1	\
	};

#define DEFINE_CMD_ALT_COND(name, altName, description, refRequired, paramInfo) \
	DEFINE_CMD_ALT_COND_ANY(name, altName, description, refRequired, paramInfo, Cmd_Default_Parse)

#define DEFINE_CMD_ALT_COND_PLUGIN(name, altName, description, refRequired, paramInfo) \
	DEFINE_CMD_ALT_COND_ANY(name, altName, description, refRequired, paramInfo, NULL)

#define DEFINE_CMD_COND(name, description, refRequired, paramInfo) \
	DEFINE_CMD_ALT_COND(name, , description, refRequired, paramInfo)

#define DEFINE_CMD_COND_PLUGIN(name, description, refRequired, paramInfo) \
	DEFINE_CMD_ALT_COND_ANY(name, , description, refRequired, paramInfo, NULL)


bool Cmd_Default_Execute(COMMAND_ARGS);

bool Cmd_Default_Parse(uint32_t numParams, SCRIPT_PARAMETER * paramInfo, SCRIPT_LINE * lineBuf, ScriptCompileData * scriptBuf);
const Cmd_Parse Cmd_Expression_Plugin_Parse = (Cmd_Parse)0x08000000;

bool Cmd_Default_Eval(COMMAND_ARGS_EVAL);


#ifdef RUNTIME
#define HANDLER(x)	x
#define HANDLER_EVAL(x)	x
#else
#define HANDLER(x)	Cmd_Default_Execute
#define HANDLER_EVAL(x)	Cmd_Default_Eval
#endif