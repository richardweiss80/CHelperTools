#include <Windows.h>
#include <stdio.h>
#include <tchar.h>

#define HASH_KEY 13


int _tmain(int argc, const TCHAR* argv[]) {
    if (argc != 2) {
        _tprintf(_TEXT("Usage:\tSimpleHashing <string>\n"));
        return 0;
    }

    // length conversion is essential
    HALF_PTR length = _tcslen(argv[argc - 1]) * sizeof(DWORD) / sizeof(TCHAR) ;
    DWORD_PTR valueBuffer = (DWORD_PTR)argv[argc - 1];
    DWORD_PTR result  = 0;

    printf("%i\n", valueBuffer);


    // taken from Stephen Fewer
    do
    {
        result = _rotr(result, HASH_KEY);
        // normalize to uppercase if the madule name is in lowercase
        if (*((BYTE*)valueBuffer) >= 'a')
            result += *((BYTE*)valueBuffer) - 0x20;
        else
            result += *((BYTE*)valueBuffer);
        valueBuffer++;
    } while (--length);

    // simple looping over rotr by 0x0D and adding the uppercase char value for the complete length

    _tprintf(_TEXT("Result:\n\tWord: %s\n\t\Value: 0x%X\n"), valueBuffer, result);
}


