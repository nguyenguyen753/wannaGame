#pragma once

#include <Windows.h>
#include <stdio.h>

#define MOD 0xffffffff

typedef struct api_table {
    DWORD hash_value;
    LPCSTR dll;
} api_table, papi_table;

DWORD calc_hash(LPCSTR name);
PDWORD api_resolver(api_table* element);