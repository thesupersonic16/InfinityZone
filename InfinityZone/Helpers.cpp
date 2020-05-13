#include "pch.h"
#include "Helpers.h"

intptr_t GetAddressFromJump(intptr_t jmp)
{
	int r = *(int*)(jmp + 1);
	int abs = jmp + r + 5;
	return (intptr_t)abs;
}


void* LoadFile_ptr = (void*)(baseAddress + 0x001C53C0);
int LoadFile(char* filename, fileinfo* info, void* unknown)
{
	int result;
	__asm
	{
		mov ecx, info
		push unknown
		push filename
		call LoadFile_ptr
		mov result, eax
	}
	return result;
}

void* LoadAndReadFile(const char* filename, int* length)
{
	char buf[MAX_PATH];
	strcpy_s(buf, sizeof buf, filename);
	fileinfo fi;
	memset(&fi, 0, sizeof(fileinfo));
	if (LoadFile(buf, &fi, buf))
	{
		int size = fi.FileSize;
		if (length)
			*length = size;
		auto buffer = malloc(size);
		if (buffer)
		{
			memset(buffer, 0, size);
			ReadBytesFromFile(&fi, buffer, size);
			return buffer;
		}
	}
	return nullptr;
}

int ReadBytesFromFile(fileinfo* file, void* buffer, int bytes)
{
	int bytesRead = (*(decltype(fread)**)(baseAddress + 0x00243474))(buffer, 1, bytes, file->File);
	if (file->IsEncrypted)
		DecryptBytes(file, buffer, bytes);
	return bytesRead;
}

void* DecryptBytes_ptr = (void*)(baseAddress + 0x001C5690);
int DecryptBytes(fileinfo* file, void* buffer, int bufferSize)
{
	int result;
	__asm
	{
		push bufferSize
		push buffer
		mov ecx, file
		call DecryptBytes_ptr
		mov result, eax
	}
	return result;
}

void ReplaceString(string& str, string& from, string& to)
{
	size_t start_pos = str.find(from);
	if (start_pos == string::npos)
		return;
	str.replace(start_pos, from.length(), to);
}