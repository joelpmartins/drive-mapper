#include <iostream>
#include <conio.h>
#include <windows.h>

#define DRIVE_1_LABEL "(G:)"
#define DRIVE_2_LABEL "(H:)"
#define DRIVE_1_LETTER "g"						// letra de referência para o endereço 1
#define DRIVE_2_LETTER "h"						// letra de referência para o endereço 2

#define ADRESS_FOLDER_1 "192.168.2.1\\teste"
#define ADRESS_FOLDER_2 "192.168.2.2\\teste"

struct drive_1{
	HWND checkbox;
	bool isChecked;
}drive_1;

struct drive_2{
	HWND checkbox;
	bool isChecked;
}drive_2;

struct user{
	char username[64];
	char password[64];
	HWND username_textfield;
	HWND password_textfield;
}user;

HWND input_button;

int checkConnection();
int driveMap(int option, int drive);

LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message){
		case WM_DESTROY:{
			PostQuitMessage(0);
			break;
		}
		case WM_COMMAND:{
            switch(HIWORD(wParam)){
		        case BN_CLICKED:{
		            if((HWND)lParam == input_button){
		            	int response;
				        GetWindowText(user.username_textfield, user.username, 64);
				        GetWindowText(user.password_textfield, user.password, 64);
				        SetWindowText(user.username_textfield, "");
				        SetWindowText(user.password_textfield, "");
				        if(drive_1.isChecked == true){
				        	response = driveMap(1, 1);
				        	if(response == 0){
				        		driveMap(2, 1);
							}
							response = driveMap(3, 1);
				        	if(response == 0){
				        		MessageBox(NULL, "A unidade foi mapeada com sucesso!", DRIVE_1_LABEL, MB_ICONASTERISK | MB_OK);
							}else{
								response = checkConnection();
								if(response == 0){
									MessageBox(NULL, "Falha ao mapear unidade de rede!", DRIVE_1_LABEL, MB_ICONERROR | MB_OK);
								}else{
									MessageBox(NULL, "Você está com problema de conexão com a internet!", DRIVE_1_LABEL, MB_ICONERROR | MB_OK);
								}
							}
						}
						if(drive_2.isChecked == true){
							response = driveMap(1, 2);
							if(response == 0){
				        		driveMap(2, 2);
							}
							response = driveMap(3, 2);
				        	if(response == 0){
				        		MessageBox(NULL, "A unidade foi mapeada com sucesso!", DRIVE_2_LABEL, MB_ICONASTERISK | MB_OK);
							}else{
								response = checkConnection();
								if(response == 0){
									MessageBox(NULL, "Falha ao mapear unidade de rede!", DRIVE_2_LABEL, MB_ICONERROR | MB_OK);
								}else{
									MessageBox(NULL, "Você está com problema de conexão com a internet!", DRIVE_2_LABEL, MB_ICONERROR | MB_OK);
								}
							}
						}
					}else if((HWND)lParam == drive_1.checkbox){
						if(drive_1.isChecked == true){
							drive_1.isChecked = false;
						}else{
							drive_1.isChecked = true;
						}
				    }else if((HWND)lParam == drive_2.checkbox){
				    	if(drive_2.isChecked == true){
							drive_2.isChecked = false;
						}else{
							drive_2.isChecked = true;
						}
					}
					break;
				}
            }
            break;
	    }
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc;
	HWND hwnd;
	HWND consolehide;
	HINSTANCE g_inst;
	MSG msg; 
	memset(&wc,0,sizeof(wc));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+0);
	wc.lpszClassName = "WindowClass";
	wc.hIcon = LoadIcon(NULL, IDI_ASTERISK);
	wc.hIconSm = LoadIcon(NULL, IDI_ASTERISK);

	if(!RegisterClassEx(&wc)){
		MessageBox(NULL, "Falha no registro da janela!", "Erro!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass", "Unidades de rede", WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 300, 300, NULL, NULL, hInstance, NULL);
	user.username_textfield = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", " ", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_RIGHT, 70, 30, 150, 30, hwnd, NULL, g_inst, NULL);
	user.password_textfield = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", " ", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_RIGHT | ES_PASSWORD, 70, 70, 150, 30, hwnd, NULL, g_inst, NULL);
	drive_1.checkbox = CreateWindow("Button", DRIVE_1_LABEL, BS_AUTOCHECKBOX | WS_CHILD | WS_VISIBLE, 70, 150, 50, 28, hwnd, (HMENU)1, hInstance, NULL);
	drive_2.checkbox = CreateWindow("Button", DRIVE_2_LABEL, BS_AUTOCHECKBOX | WS_CHILD | WS_VISIBLE, 175, 150, 50, 28, hwnd, (HMENU)1, hInstance, NULL);
	input_button = CreateWindowEx(0, "BUTTON", "Entrar", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 85, 110, 120, 20, hwnd, (HMENU)1, 0, 0);

    AllocConsole();
    consolehide = FindWindowA("ConsoleWindowClass", 0);
    ShowWindow(consolehide, 0);

	LONG gwl=GetWindowLong(hwnd, GWL_STYLE);
	gwl&=(~WS_MAXIMIZEBOX);
	SetWindowLong(hwnd, GWL_STYLE, gwl);

	if(hwnd == NULL) {
		MessageBox(NULL, "Falha na criação da janela!", "Erro!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	while(GetMessage(&msg, NULL, 0, 0) > 0){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

int driveMap(int option, int drive){
	char strcmd[64];
	switch(option){
		case 1:{
			if(drive == 1){
				sprintf(strcmd,"net use %s:", DRIVE_1_LETTER);
			}else{
				sprintf(strcmd,"net use %s:", DRIVE_2_LETTER);
			}
			printf("%s", strcmd);
			system(strcmd);
			break;
		}
		case 2:{
			if(drive == 1){
				sprintf(strcmd,"net use %s: /delete /yes", DRIVE_1_LETTER);
			}else{
				sprintf(strcmd,"net use %s: /delete /yes", DRIVE_2_LETTER);
			}
			printf("%s", strcmd);
			system(strcmd);
			break;
		}
		case 3:{
			if(drive == 1){
				sprintf(strcmd,"net use %s: \\\\%s /USER:local\\%s %s", DRIVE_1_LETTER, ADRESS_FOLDER_1, user.username, user.password);
			}else{
				sprintf(strcmd,"net use %s: \\\\%s /USER:local\\%s %s", DRIVE_2_LETTER, ADRESS_FOLDER_2, user.username, user.password);
			}
			printf("%s", strcmd);
			system(strcmd);
			break;
		}
	}	
}

int checkConnection(){
	int response = system("ping www.google.com.br");
	if(response == 0){
		response = 0;
	}else{
		response = 1;
	}
	return response;
}
