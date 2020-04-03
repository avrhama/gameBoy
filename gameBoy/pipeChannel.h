#include <windows.h> 
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
class pipeChannel {
public:
	const char* pipeName;
	char pipeType;
	int nTimeOut;
	HANDLE hPipe;
	DWORD dwRead;
	DWORD dwWritten;
	byte* rBuffer;
	byte* wBuffer;
	int rBufferSize;
	int wBufferSize;
	//type 0 for server 1 for client
	bool  createPipe(char  pipeType,const char * pipeName, int rBufferSize, int wBufferSize, int nTimeOut = PIPE_WAIT) {
		this->nTimeOut = nTimeOut;
		this->pipeType = pipeType;
		this->pipeName = pipeName;
		this->rBufferSize = rBufferSize;
		this->wBufferSize = wBufferSize;
		rBuffer = (byte*)calloc(rBufferSize+1, 1);
		wBuffer = (byte*)calloc(wBufferSize+1, 1);
		
		if (pipeType == 0)
			return createServer();
		else
			return createClient();
	}
	std::wstring s2ws(const std::string& s)
	{
		int len;
		int slength = (int)s.length() + 1;
		len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
		wchar_t* buf = new wchar_t[len];
		MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
		std::wstring r(buf);
		delete[] buf;
		return r;
	}
	bool createServer() {
		char fileName[50];
		snprintf(fileName, 50, "\\\\.\\pipe\\%s", pipeName);
		string filename = fileName;
		std::wstring stemp = s2ws(filename);
		LPCWSTR result = stemp.c_str();
		hPipe = CreateNamedPipe(result,
			PIPE_ACCESS_DUPLEX,
			PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | nTimeOut,   // FILE_FLAG_FIRST_PIPE_INSTANCE is not needed but forces CreateNamedPipe(..) to fail if the pipe already exists...
			1,
			wBufferSize,
			rBufferSize,
			NMPWAIT_USE_DEFAULT_WAIT,
			NULL);
		return false;
	}
	bool createClient() {
		char fileName[50];
		snprintf(fileName, 50, "\\\\.\\pipe\\%s", pipeName);
		string filename = fileName;
		std::wstring stemp = s2ws(filename);
		LPCWSTR result = stemp.c_str();
		hPipe = CreateFile(result,
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);
		return hPipe != INVALID_HANDLE_VALUE;
	}
	//waits for connection
	bool wait() {
		char timeout = 60;
		if (pipeType == 0)
			return (ConnectNamedPipe(hPipe, NULL) != FALSE);// wait for someone to connect to the pipe
		else {
			do {
				createClient();
				Sleep(1000);
				timeout--;
			} while (hPipe == INVALID_HANDLE_VALUE && (timeout > 0 || nTimeOut == 0xffffffff));
			return hPipe != INVALID_HANDLE_VALUE;
		}
	}
	void close() {
		if (pipeType == 0)
			DisconnectNamedPipe(hPipe);
		else
			CloseHandle(hPipe);
		free(wBuffer);
		free(rBuffer);
	}
	bool read(int readLen = -1) {
		readLen = readLen > 0 ? readLen : rBufferSize;
		return ReadFile(hPipe, rBuffer, readLen, &dwRead, NULL) != FALSE;
	}
	bool write(int writeLen = -1) {
		writeLen = writeLen > 0 ? writeLen : wBufferSize;
		return WriteFile(hPipe,
			wBuffer,
			writeLen,   // = length of string + terminating '\0' !!!
			&dwWritten,
			NULL)!=FALSE;
	}
};