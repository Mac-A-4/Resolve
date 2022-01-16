#pragma once
#include <Windows.h>
#include <cstdint>
#include <string>

bool Resolve(HANDLE _Process, const std::string& _Expression, uintptr_t& _Return);

uintptr_t Resolve(HANDLE _Process, const std::string& _Expression);