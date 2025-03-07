#ifndef TASKSERVICE_H
#define TASKSERVICE_H

#include "includes\Imports.h"

BOOL CreateScheduledTask(std::wstring& TaskName, std::wstring& Path, std::wstring& Argument);
BOOL RunScheduledTask(std::wstring& TaskName);
BOOL DeleteScheduledTask(const std::wstring& TaskName);

#endif /* TASKSERVICE_H */#pragma once
