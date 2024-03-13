#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <process.h>
#include <Windows.h>
int main(int argc, char* argv[])
{
	int is_par = 0;
	if (strcmp(argv[0], "lab9") == 0 || strcmp(argv[0], "lab9.exe") == 0)	//printf("\nis_par=%d\nstrcmp(%s,lab9)==0||strcmp(%s,lab9.exe)==0", is_par,argv[0],argv[0]);//komentarz techniczny
	{
		if (argc==1)
		{
			printf("Wystapil blad");
			return 1;
		}
		is_par = 1;
	}
	/*//komentarz techniczny
	for (int i = 0; i < argc; i++)
	{
		printf("\nargv[%d]: %s, Dlugosc: %d\n", i, argv[i],strlen(argv[i]));
	}*/
	if (argc <= 2 && (strlen(argv[is_par]) >= 1 && strlen(argv[is_par]) <= 25)) 
	{
		
		for (int i = 0; i < strlen(argv[is_par]);i++)
		{
			if (isdigit(argv[is_par][i]) == 0)
			{
				printf("Wystapil blad");
				return 1;
			}
		}
		if (strlen(argv[is_par]) == 1)
		{
			return argv[is_par][0] - '0';
		}
		else
		{
			//printf("\nDotarl do tworzenia procesu\n");//komentarz techniczny
			int dl1 = strlen(argv[is_par]) / 2;
			int dl2 = strlen(argv[is_par]) - dl1;
			char args1[dl1];
			char args2[dl2];
			strncpy(args1, argv[is_par], dl1);
			strncpy(args2, argv[is_par] + dl1, dl2);
			args1[dl1] = '\0';
			args2[dl2] = '\0';
			//printf("\ndl1: %s\n dl2: %s\n", args1, args2);//komentarz techniczny
			STARTUPINFO si;
			PROCESS_INFORMATION	pi[2];
			memset(&si, 0, sizeof(si));
			memset(&pi, 0, sizeof(pi));
			si.cb = sizeof(si);
			if (CreateProcessA("lab9.exe", args1, NULL, NULL, 0, 0, NULL, NULL, &si, pi) == 0
			 || CreateProcessA("lab9.exe", args2, NULL, NULL, 0, 0, NULL, NULL, &si, pi +1) == 0) 
			{
				printf("CreateProcess %s nieudaned %d.\n", argv[is_par], GetLastError());
				return 2;
			}

			HANDLE children[2] = {pi[0].hProcess,pi[1].hProcess};
			DWORD exitcodes[2];
			WaitForMultipleObjects(2, children, 1, INFINITE);
			GetExitCodeProcess(pi[0].hProcess, &exitcodes[0]);
			GetExitCodeProcess(pi[1].hProcess, &exitcodes[1]);
			CloseHandle(pi[0].hProcess);
			CloseHandle(pi[0].hThread);
			CloseHandle(pi[1].hProcess);
			CloseHandle(pi[1].hThread);
			fprintf(stdout, "\n%d\t%d\t\t\t%s\t%d\n",GetCurrentProcessId(), pi[0].dwProcessId, args1, exitcodes[0]);
			fprintf(stdout, "\n%d\t%d\t\t\t%s\t%d\n",GetCurrentProcessId(), pi[1].dwProcessId, args2, exitcodes[1]);
			return exitcodes[0] + exitcodes[1];
		}


	}
	else
	{
		fprintf(stderr, "Wystapil blad\n");
	}

}
