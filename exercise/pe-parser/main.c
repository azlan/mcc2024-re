#include <Windows.h>
#include <winnt.h>
#include <stdio.h>

int wmain(int argc, wchar_t* argv[]) {
	if (argc != 2){
		printf("usage: %ls filename", argv[0]);
		return 1;
	}

	wchar_t* filename = argv[1];
	wprintf(L"File: %ls\n", filename);

	HANDLE hFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == INVALID_HANDLE_VALUE) {
		printf("Couldn't open file with CreateFile()\n");
		return 1;
	}

	HANDLE hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
	if (hFileMapping == 0) {
		CloseHandle(hFile);
		printf("Couldn't open file mapping with CreateFileMapping()\n");
		return 1;
	}

	LPVOID lpFileBase = MapViewOfFile(hFileMapping, FILE_MAP_READ, 0, 0, 0);
	if (lpFileBase == 0) {
		CloseHandle(hFileMapping);
		CloseHandle(hFile);
		printf("Couldn't map view of file with MapViewOfFile()\n");
		return 1;
	}

	PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)lpFileBase;

#ifdef _WIN64
	PIMAGE_NT_HEADERS pNTHeader = (PIMAGE_NT_HEADERS)((DWORD64)(dosHeader)+(DWORD64)(dosHeader->e_lfanew));
#else
	PIMAGE_NT_HEADERS pNTHeader = (PIMAGE_NT_HEADERS)((DWORD)(dosHeader)+(DWORD)(dosHeader->e_lfanew));
#endif // _WIN64

	if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE || pNTHeader->Signature != 0x00004550) {
		printf("This is not a PE file\n");
		return 1;
	}

	PIMAGE_FILE_HEADER pImgFileHdr = (PIMAGE_FILE_HEADER)&pNTHeader->FileHeader;
	PIMAGE_OPTIONAL_HEADER optionalHeader = (PIMAGE_OPTIONAL_HEADER)&pNTHeader->OptionalHeader;

	DWORD file_size = GetFileSize(hFile, NULL);
	printf("FileSize %d\n", file_size);

	if (optionalHeader->Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
		// PE32
		PIMAGE_OPTIONAL_HEADER32 optionalHeader32 = (PIMAGE_OPTIONAL_HEADER32)&pNTHeader->OptionalHeader;
		printf("PE Format: PE32\n");
		printf("ImageBase: 0x%X\n",optionalHeader32->ImageBase);

	} else if (optionalHeader->Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC) {
		// PE32+
		PIMAGE_OPTIONAL_HEADER64 optionalHeader64 = (PIMAGE_OPTIONAL_HEADER64)&pNTHeader->OptionalHeader;
		printf("PE Format: PE32+\n");
		printf("ImageBase 0x%llX\n", optionalHeader64->ImageBase);
	} else {
		printf("Bad PE File\n");
	}

	UnmapViewOfFile(lpFileBase);
	CloseHandle(hFileMapping);
	CloseHandle(hFile);

	return 0;
}