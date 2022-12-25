//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @Sigma0014.												|
// @Sigma0014: https://github.com/Sigma0014												|
//-------------------------------------------------------------------------------------->

#include <ArtemisSpecific/Midnight.h>

using namespace Artemis;

static Logger* pLog = &Midnight::GetInst()->Log;

void ExceptionHandler(Exception* e) { pLog->LogWarning(e->GetFunctionName(), e->GetExceptionMessage()); }
void ParameterExceptionHandler(ParameterException* e) { pLog->LogWarning(e->GetFunctionName(), "Regarding parameter %s: %s", e->GetParameterName(), e->GetExceptionMessage()); }
void InstanceInvalidExceptionHandler(InstanceInvalidException* e) { pLog->LogWarning(e->GetFunctionName(), e->GetExceptionMessage()); }
void MemoryAccessViolationExceptionHandler(MemoryAccessViolationException* e) { pLog->LogWarning(e->GetFunctionName(), "%s Operation: %s, Location: 0x%llX, Size: %lu", e->GetExceptionMessage(), (e->GetOperationType() == OperationType::Read ? "Read" : "Write"), e->GetAddress(), e->GetSize()); }
void WindowsApiExceptionHandler(WindowsApiException* e) { pLog->LogWarning(e->GetFunctionName(), "%s failed with code %lu: %s", e->GetWindowsFunction(), e->GetWindowsCode(), e->GetWindowsMessage()); }
void ObjectNotFoundExceptionHandler(ObjectNotFoundException* e) { pLog->LogWarning(e->GetFunctionName(), "%s Regarded Object: %s, Object Type: %s", e->GetExceptionMessage(), e->GetObjectName(), e->GetObjectType()); }
void NotImplementedExceptionHandler(NotImplementedException* e) { pLog->LogWarning(e->GetFunctionName(), e->GetExceptionMessage()); }
void CompatibilityExceptionHandler(CompatibilityException* e) { pLog->LogWarning(e->GetFunctionName(), e->GetExceptionMessage()); }
void AttributeExceptionHandler(AttributeException* e) { pLog->LogWarning(e->GetFunctionName(), "%s Regarded attribute: %s", e->GetExceptionMessage(), e->GetAttributeName()); }
void IndexOutOfRangeExceptionHandler(IndexOutOfRangeException* e) { pLog->LogWarning(e->GetFunctionName(), "%s Index: %i, Max allowed index: %i", e->GetExceptionMessage(), e->GetIndex(), e->GetMaxIndex()); }
void HookExceptionHandler(HookException* e) { pLog->LogWarning(e->GetFunctionName(), e->GetExceptionMessage()); }

void RegisterEventHandlers() {
	ExceptionEventManager::GetExceptionEvent() += ExceptionHandler;
	pLog->LogSuccess(__FUNCTION__, "Registered the Exception handler.");

	ExceptionEventManager::GetParameterExceptionEvent() += ParameterExceptionHandler;
	pLog->LogSuccess(__FUNCTION__, "Registered the ParameterException handler.");

	ExceptionEventManager::GetInstanceInvalidExceptionEvent() += InstanceInvalidExceptionHandler;
	pLog->LogSuccess(__FUNCTION__, "Registered the InstanceInvalidException handler.");

	ExceptionEventManager::GetMemoryAccessViolationExceptionEvent() += MemoryAccessViolationExceptionHandler;
	pLog->LogSuccess(__FUNCTION__, "Registered the MemoryAccessViolationException handler.");

	ExceptionEventManager::GetWindowsApiExceptionEvent() += WindowsApiExceptionHandler;
	pLog->LogSuccess(__FUNCTION__, "Registered the WindowsApiException handler.");

	ExceptionEventManager::GetObjectNotFoundExceptionEvent() += ObjectNotFoundExceptionHandler;
	pLog->LogSuccess(__FUNCTION__, "Registered the ObjectNotFoundException handler.");

	ExceptionEventManager::GetNotImplementedExceptionEvent() += NotImplementedExceptionHandler;
	pLog->LogSuccess(__FUNCTION__, "Registered the NotImplementedException handler.");

	ExceptionEventManager::GetCompatibilityExceptionEvent() += CompatibilityExceptionHandler;
	pLog->LogSuccess(__FUNCTION__, "Registered the CompatibilityException handler.");

	ExceptionEventManager::GetAttributeExceptionEvent() += AttributeExceptionHandler;
	pLog->LogSuccess(__FUNCTION__, "Registered the AttributeException handler.");

	ExceptionEventManager::GetIndexOutOfRangeExceptionEvent() += IndexOutOfRangeExceptionHandler;
	pLog->LogSuccess(__FUNCTION__, "Registered the IndexOutOfRangeException handler.");

	ExceptionEventManager::GetHookExceptionEvent() += HookExceptionHandler;
	pLog->LogSuccess(__FUNCTION__, "Registered the HookException handler.");
}