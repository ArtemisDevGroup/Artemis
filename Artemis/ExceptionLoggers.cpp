#include <ArtemisSpecific/Midnight.h>

using namespace Artemis;

static Logger* pLog = &Midnight::GetInst()->Log;

void ExceptionHandler(Exception* e) { pLog->LogError(e->GetFunctionName(), e->GetExceptionMessage()); }
void ParameterExceptionHandler(ParameterException* e) { pLog->LogError(e->GetFunctionName(), "Regarding parameter %s: %s", e->GetParameterName(), e->GetExceptionMessage()); }
void InstanceInvalidExceptionHandler(InstanceInvalidException* e) { pLog->LogError(e->GetFunctionName(), e->GetExceptionMessage()); }
void MemoryAccessViolationExceptionHandler(MemoryAccessViolationException* e) { pLog->LogError(e->GetFunctionName(), "%s Operation: %s, Location: 0xllX, Size: %lu", e->GetExceptionMessage(), (e->GetOperationType() == OperationType::Read ? "Read" : "Write"), e->GetAddress(), e->GetSize()); }
void WindowsApiExceptionHandler(WindowsApiException* e) { pLog->LogError(e->GetFunctionName(), "%s failed with code %lu: %s", e->GetWindowsFunction(), e->GetWindowsCode(), e->GetWindowsMessage()); }
void ObjectNotFoundExceptionHandler(ObjectNotFoundException* e) { pLog->LogError(e->GetFunctionName(), "%s Regarded Object: %s, Object Type: %s", e->GetExceptionMessage(), e->GetObjectName(), e->GetObjectType()); }
void NotImplementedExceptionHandler(NotImplementedException* e) { pLog->LogError(e->GetFunctionName(), e->GetExceptionMessage()); }
void CompatibilityExceptionHandler(CompatibilityException* e) { pLog->LogError(e->GetFunctionName(), e->GetExceptionMessage()); }
void AttributeExceptionHandler(AttributeException* e) { pLog->LogError(e->GetFunctionName(), "%s Regarded attribute: %s", e->GetExceptionMessage(), e->GetAttributeName()); }
void IndexOutOfRangeExceptionHandler(IndexOutOfRangeException* e) { pLog->LogError(e->GetFunctionName(), "%s Index: %i, Max allowed index: %i", e->GetExceptionMessage(), e->GetIndex(), e->GetMaxIndex()); }
void HookExceptionHandler(HookException* e) { pLog->LogError(e->GetFunctionName(), e->GetExceptionMessage()); }

void RegisterEventHandlers() {
	ExceptionEventManager::GetExceptionEvent() += ExceptionHandler;
	ExceptionEventManager::GetParameterExceptionEvent() += ParameterExceptionHandler;
	ExceptionEventManager::GetInstanceInvalidExceptionEvent() += InstanceInvalidExceptionHandler;
	ExceptionEventManager::GetMemoryAccessViolationExceptionEvent() += MemoryAccessViolationExceptionHandler;
	ExceptionEventManager::GetWindowsApiExceptionEvent() += WindowsApiExceptionHandler;
	ExceptionEventManager::GetObjectNotFoundExceptionEvent() += ObjectNotFoundExceptionHandler;
	ExceptionEventManager::GetNotImplementedExceptionEvent() += NotImplementedExceptionHandler;
	ExceptionEventManager::GetCompatibilityExceptionEvent() += CompatibilityExceptionHandler;
	ExceptionEventManager::GetAttributeExceptionEvent() += AttributeExceptionHandler;
	ExceptionEventManager::GetIndexOutOfRangeExceptionEvent() += IndexOutOfRangeExceptionHandler;
	ExceptionEventManager::GetHookExceptionEvent() += HookExceptionHandler;
}