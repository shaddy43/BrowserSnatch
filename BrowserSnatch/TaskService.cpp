#include "includes\TaskService.h"

BOOL CreateScheduledTask(std::wstring& TaskName, std::wstring& Path, std::wstring& Argument)
{
    HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (FAILED(hr)) {
        return false;
    }

    hr = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_PKT_PRIVACY,
        RPC_C_IMP_LEVEL_IMPERSONATE, NULL, 0, NULL);
    if (FAILED(hr)) {
        CoUninitialize();
        return false;
    }

    ITaskService* pService = NULL;
    hr = CoCreateInstance(CLSID_TaskScheduler, NULL, CLSCTX_INPROC_SERVER,
        IID_ITaskService, (void**)&pService);
    if (FAILED(hr)) {
        CoUninitialize();
        return false;
    }

    hr = pService->Connect(_variant_t(), _variant_t(), _variant_t(), _variant_t());
    if (FAILED(hr)) {
        pService->Release();
        CoUninitialize();
        return false;
    }

    ITaskDefinition* pTask = NULL;
    hr = pService->NewTask(0, &pTask);
    if (FAILED(hr)) {
        pService->Release();
        CoUninitialize();
        return false;
    }

    IRegistrationInfo* pRegInfo = NULL;
    pTask->get_RegistrationInfo(&pRegInfo);
    pRegInfo->put_Author(_bstr_t("shaddy43"));
    pRegInfo->Release();

    // Set principal to run as SYSTEM
    IPrincipal* pPrincipal = NULL;
    pTask->get_Principal(&pPrincipal);
    pPrincipal->put_Id(_bstr_t(L"System"));
    pPrincipal->put_LogonType(TASK_LOGON_SERVICE_ACCOUNT);
    pPrincipal->put_UserId(_bstr_t(L"SYSTEM"));
    pPrincipal->Release();

    // Define the action (Run the executable)
    IActionCollection* pActionCollection = NULL;
    pTask->get_Actions(&pActionCollection);

    IAction* pAction = NULL;
    hr = pActionCollection->Create(TASK_ACTION_EXEC, &pAction);
    if (FAILED(hr)) {
        pActionCollection->Release();
        pTask->Release();
        pService->Release();
        CoUninitialize();
        return false;
    }

    IExecAction* pExecAction = NULL;
    hr = pAction->QueryInterface(IID_IExecAction, (void**)&pExecAction);
    if (FAILED(hr)) {
        pAction->Release();
        pActionCollection->Release();
        pTask->Release();
        pService->Release();
        CoUninitialize();
        return false;
    }

    // Set the executable path and argument
    pExecAction->put_Path(_bstr_t(Path.c_str()));
    pExecAction->put_Arguments(_bstr_t(Argument.c_str()));

    // Clean up
    pExecAction->Release();
    pAction->Release();
    pActionCollection->Release();

    // Register the task
    ITaskFolder* pRootFolder = NULL;
    hr = pService->GetFolder(_bstr_t(L"\\"), &pRootFolder);
    if (FAILED(hr)) {
        pTask->Release();
        pService->Release();
        CoUninitialize();
        return false;
    }

    IRegisteredTask* pRegisteredTask = NULL;
    hr = pRootFolder->RegisterTaskDefinition(
        _bstr_t(TaskName.c_str()),
        pTask,
        TASK_CREATE_OR_UPDATE,
        _variant_t(L""),
        _variant_t(L""),
        TASK_LOGON_SERVICE_ACCOUNT,
        _variant_t(L""),
        &pRegisteredTask);

    if (FAILED(hr)) {
        pRootFolder->Release();
        pTask->Release();
        pService->Release();
        CoUninitialize();
        return false;
    }

    // Clean up
    pRegisteredTask->Release();
    pRootFolder->Release();
    pTask->Release();
    pService->Release();
    CoUninitialize();

    return true;
}

BOOL RunScheduledTask(std::wstring& TaskName)
{
    HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (FAILED(hr)) {
        return false;
    }

    // Initialize COM security
    hr = CoInitializeSecurity(
        NULL, -1, NULL, NULL,
        RPC_C_AUTHN_LEVEL_PKT_PRIVACY,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL, 0, NULL);

    if (FAILED(hr)) {
        CoUninitialize();
        return false;
    }

    ITaskService* pService = NULL;
    hr = CoCreateInstance(CLSID_TaskScheduler, NULL, CLSCTX_INPROC_SERVER, IID_ITaskService, (void**)&pService);
    if (FAILED(hr)) {
        CoUninitialize();
        return false;
    }

    hr = pService->Connect(_variant_t(), _variant_t(), _variant_t(), _variant_t());
    if (FAILED(hr)) {
        pService->Release();
        CoUninitialize();
        return false;
    }

    ITaskFolder* pRootFolder = NULL;
    hr = pService->GetFolder(_bstr_t(L"\\"), &pRootFolder);
    if (FAILED(hr)) {
        pService->Release();
        CoUninitialize();
        return false;
    }

    IRegisteredTask* pTask = NULL;
    hr = pRootFolder->GetTask(_bstr_t(TaskName.c_str()), &pTask);
    if (FAILED(hr)) {
        pRootFolder->Release();
        pService->Release();
        CoUninitialize();
        return false;
    }

    IRunningTask* pRunningTask = NULL;
    hr = pTask->Run(_variant_t(), &pRunningTask);
    if (SUCCEEDED(hr)) {
        pRunningTask->Release();
    }
    else {
        return false;
    }

    // Cleanup
    pTask->Release();
    pRootFolder->Release();
    pService->Release();
    CoUninitialize();
    return true;
}

BOOL DeleteScheduledTask(const std::wstring& taskName)
{
    HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (FAILED(hr)) {
        return false;
    }

    hr = CoInitializeSecurity(
        NULL, -1, NULL, NULL,
        RPC_C_AUTHN_LEVEL_PKT_PRIVACY,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL, 0, NULL);

    if (FAILED(hr)) {
        CoUninitialize();
        return false;
    }

    ITaskService* pService = NULL;
    hr = CoCreateInstance(CLSID_TaskScheduler, NULL, CLSCTX_INPROC_SERVER, IID_ITaskService, (void**)&pService);
    if (FAILED(hr)) {
        CoUninitialize();
        return false;
    }

    hr = pService->Connect(_variant_t(), _variant_t(), _variant_t(), _variant_t());
    if (FAILED(hr)) {
        pService->Release();
        CoUninitialize();
        return false;
    }

    ITaskFolder* pRootFolder = NULL;
    hr = pService->GetFolder(_bstr_t(L"\\"), &pRootFolder);
    if (FAILED(hr)) {
        pService->Release();
        CoUninitialize();
        return false;
    }

    // Delete the task
    hr = pRootFolder->DeleteTask(_bstr_t(taskName.c_str()), 0);
    if (SUCCEEDED(hr)) {
    }
    else {
        return false;
    }

    // Cleanup
    pRootFolder->Release();
    pService->Release();
    CoUninitialize();
    return true;
}