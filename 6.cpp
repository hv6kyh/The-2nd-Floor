#include <fmod.h>
#pragma comment(lib,"fmodex_vc.lib")

enum SOUNDKIND		// 메인배경음악
{
	SD_1 = 0,
	SD_2,
	SD_3,
	SD_4,
	SD_5,
	SD_6,
	SD_7,
	SD_8,
	SD_9,
	SD_10,
	SD_11,
	SD_END
};
enum EFFSOUNDKIND		// 효과음
{
	EFFSD_1 =0,
	EFFSD_2,
	EFFSD_3,
	EFFSD_4,
	EFFSD_5,
	EFFSD_6,
	EFFSD_7,
	EFFSD_8,
	EFFSD_9,
	EFFSD_10,
	EFFSD_11,
	EFFSD_END
};

#include <windows.h>
#include<tchar.h>
#include<MMSystem.h> //사운드함수 정의
#pragma comment(lib,"Winmm.lib")  //라이브러리함수 정의
#include "resource.h"
#include <time.h>

#define S_1 430
#define S_2 460
#define S_3 490
#define S_4 520
#define S_5 550
#define head 400

// FMOD 함수들 선언
static FMOD_SYSTEM* g_psystem;
static FMOD_SOUND* g_psound[SD_END];
static FMOD_CHANNEL* g_pchannel[SD_END];

static FMOD_SYSTEM* effg_psystem;
static FMOD_SOUND* effg_psound[EFFSD_END];
static FMOD_CHANNEL* effg_pchannel[EFFSD_END];

char sel_cursor[] = "☞";

struct stage {
	char qst[50];
	char str1[50];
	char str2[50];
	char str3[50];
	char str4[50];
	char str5[50];
};


HWND hWnd;

HINSTANCE g_hinstance = NULL;

// 함수 프로토타입

void soundsetup();
void playsound(SOUNDKIND esound);
void effsoundsetup();
void effplaysound(EFFSOUNDKIND esound);

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

LPCTSTR IpszClass = TEXT("The 2nd Floor");

// 전역 변수 선언
int nState;			// 현재 스테이지 상태 담는 변수
int oldState;		// 그 전에 어떤 스테이지였는지 담는 변수
int oldSelect;		// 그 전에 어떤 선택지를 선택했는지 담는 변수
int flag = 0;

// 타이머 변수들 선언
enum {

	T_Ending1,
	T_Ending2,
	T_Ending5,
	T_Toilet,
	T_GamePlay,
	T_In211,
	T_SelFinger_wrong,
	T_SelFinger_right,
	T_208_1,
	T_Happy

};

// 스테이지 변수 선언
enum {

	G_Main,
	G_Prologue1,
	G_Prologue2,
	G_Prologue3,
	G_Prologue4,
	G_Prologue5,
	G_GamePlay,
	G_Toilet,
	G_Ending1,
	G_Ending2,
	G_Ending3,
	G_Ending4,
	G_Ending5,
	G_InToilet,
	G_EmptyToilet,
	G_DollToilet,
	G_211_1,
	G_211_2,
	G_In211,
	G_Finger,
	G_SelFinger,
	G_GetKey,
	G_Password,
	G_208,
	G_208_1,
	G_208_2,
	G_Event,
	G_Gate

};
enum { ID_GamePlay, ID_Exit };

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
   HWND hWnd;
   MSG Message;
   WNDCLASS WndClass;
   g_hinstance = hInstance;

   WndClass.cbClsExtra = 0;
   WndClass.cbWndExtra = 0;
   WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
   WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
   //WndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
   WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
   WndClass.hInstance = hInstance;
   WndClass.lpfnWndProc = WndProc;
   WndClass.lpszClassName = IpszClass;
   WndClass.lpszMenuName = NULL;
   WndClass.style = CS_HREDRAW | CS_VREDRAW;
   RegisterClass(&WndClass);

   hWnd = CreateWindow(IpszClass, IpszClass, WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
      CW_USEDEFAULT, CW_USEDEFAULT, 1024, 785,
      NULL, (HMENU)NULL, hInstance, NULL);
   ShowWindow(hWnd, nCmdShow);

   while (GetMessage(&Message, NULL, 0, 0)) {
      TranslateMessage(&Message);
      DispatchMessage(&Message);
   }
   return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	HDC hdc, MemDC;
	PAINTSTRUCT ps;
	HFONT font, oldfont;
	static int fontSize = 20;

	srand((unsigned)time(NULL));

	static USHORT toiletRand;
	static USHORT toiletSelect;

	static char pass[50];
	static char src_208[] = "8271";
	static char src_211[] = "4542";

	// 비트맵 변수 선언
	static HBITMAP Main_Bit; //메인화면

	static HBITMAP Play_Bit; //게임화면

	static HBITMAP Prologue_Bit1;
	static HBITMAP Prologue_Bit2;
	static HBITMAP Prologue_Bit3;
	static HBITMAP Prologue_Bit4;
	static HBITMAP Prologue_Bit5;
	static HBITMAP InToilet_Bit;

	static HBITMAP Ending1_Bit;		// 고독 엔딩
	static HBITMAP Ending2_Bit;		// 겁쟁이 엔딩

	static HBITMAP Toilet_Bit;

	static HBITMAP EmptyToilet_Bit;
	static HBITMAP DollToilet_Bit;

	static HBITMAP B211_1_Bit;
	static HBITMAP B211_2_Bit;
	static HBITMAP In211_Bit;
	static HBITMAP Finger_Bit;
	static HBITMAP GetKey_Bit;

	static HBITMAP Password_Bit;
	static HBITMAP R208_Bit;
	static HBITMAP R208_2_Bit;

	static HBITMAP Event_Bit;

	static HBITMAP R208_back_Bit;
	static HBITMAP R208_init_Bit;
	static HBITMAP R208_nothing_Bit;
	static HBITMAP R211_dkdk_Bit;
	static HBITMAP Wellcome_Bit;
	static HBITMAP Ghost_Bit;
	static HBITMAP Iron_Bit;
	static HBITMAP GoHome_Bit;
	
	// 선택지가 있어야 하는 스테이지에 각각 구조체 선언
	static struct stage main;
	static struct stage toilet;
	static struct stage InToilet;
	static struct stage In211;
	static struct stage SelFinger;
	static struct stage In208;
	
	// 타이머
	static HANDLE myTimer;

	int len;

	// 선택지들에 텍스트 대입
	strcpy(main.qst, "어디로 갈까?");
	strcpy(main.str1, "계속 내려간다.");
	strcpy(main.str2, "화장실로 간다.");
    strcpy(main.str3, "208호로 간다.");
	strcpy(main.str4, "211호로 간다.");
	strcpy(main.str5, "철문으로 간다.");

	strcpy(toilet.qst, "몇번째 칸을 열까?");
	strcpy(toilet.str1, "첫번째 칸");
	strcpy(toilet.str2, "두번째 칸");
	strcpy(toilet.str3, "세번째 칸");

	strcpy(InToilet.qst, "정말 열까?");
	strcpy(InToilet.str1, "네");
	strcpy(InToilet.str2, "아니오");

	strcpy(In211.qst, "열어볼까?");
	strcpy(In211.str1,"네");
	strcpy(In211.str2, "아니오");

	strcpy(SelFinger.qst, "몇 번째 손을 자를까?");
	strcpy(SelFinger.str1, "첫 번째 손");
	strcpy(SelFinger.str2, "두 번째 손");
	strcpy(SelFinger.str3, "세 번째 손");

	strcpy(In208.qst, "어떻게 할까?");
	strcpy(In208.str1, "뒤를 살핀다.");
	strcpy(In208.str2, "돌아갈까?");

	static HWND BTN_GamePlay, BTN_Exit; //버튼

	// 선택지에서 손가락 모양의 좌표
	static int sel_x = 150;
	static int sel_y = 430;
	
	switch (uMsg) {

	case WM_CHAR :		// 208, 211호에서 비밀번호 입력

		if (nState == G_Password && (wParam != VK_RETURN) ) {

			effplaysound(EFFSD_4);
			len = strlen(pass);
			pass[len] = (TCHAR)wParam;
			pass[len+1] = 0;

		}

		return 0;
	
	case WM_CREATE:		// 윈도우 창이 만들어질 때 실행하는 부분
		
		Main_Bit = (HBITMAP)LoadBitmap(g_hinstance, MAKEINTRESOURCE(IDB_BITMAP16)); //비트맵 생성
		Play_Bit = (HBITMAP)LoadBitmap(g_hinstance, MAKEINTRESOURCE(IDB_BITMAP15)); //비트맵 생성

		// 선언했던 변수들에 실제 이미지를 씌움
		Prologue_Bit1 = (HBITMAP)LoadBitmap(g_hinstance, MAKEINTRESOURCE(IDB_BITMAP10));
		Prologue_Bit2 = (HBITMAP)LoadBitmap(g_hinstance, MAKEINTRESOURCE(IDB_BITMAP11));
		Prologue_Bit3 = (HBITMAP)LoadBitmap(g_hinstance, MAKEINTRESOURCE(IDB_BITMAP12));
		Prologue_Bit4 = (HBITMAP)LoadBitmap(g_hinstance, MAKEINTRESOURCE(IDB_BITMAP13));
		Prologue_Bit5 = (HBITMAP)LoadBitmap(g_hinstance, MAKEINTRESOURCE(IDB_BITMAP14));

		Toilet_Bit = (HBITMAP)LoadBitmap(g_hinstance, MAKEINTRESOURCE(IDB_BITMAP2)); //비트맵 생성
		InToilet_Bit = (HBITMAP)LoadBitmap(g_hinstance, MAKEINTRESOURCE(IDB_BITMAP2));
		EmptyToilet_Bit = (HBITMAP)LoadBitmap(g_hinstance, MAKEINTRESOURCE(IDB_BITMAP3)); //비트맵 생성
		DollToilet_Bit = (HBITMAP)LoadBitmap(g_hinstance, MAKEINTRESOURCE(IDB_BITMAP17)); //비트맵 생성
		
		Ending1_Bit = (HBITMAP)LoadBitmap(g_hinstance, MAKEINTRESOURCE(IDB_BITMAP5));			// 고독
		Ending2_Bit = (HBITMAP)LoadBitmap(g_hinstance, MAKEINTRESOURCE(IDB_BITMAP6));			// 겁쟁이

		B211_1_Bit = (HBITMAP)LoadBitmap(g_hinstance, MAKEINTRESOURCE(IDB_BITMAP1));
		B211_2_Bit = (HBITMAP)LoadBitmap(g_hinstance, MAKEINTRESOURCE(IDB_BITMAP1));

		In211_Bit = (HBITMAP)LoadBitmap(g_hinstance, MAKEINTRESOURCE(IDB_BITMAP9));
		Finger_Bit = (HBITMAP)LoadBitmap(g_hinstance, MAKEINTRESOURCE(IDB_BITMAP7));
		GetKey_Bit = (HBITMAP)LoadBitmap(g_hinstance, MAKEINTRESOURCE(IDB_BITMAP8));

		Password_Bit = (HBITMAP)LoadBitmap(g_hinstance, MAKEINTRESOURCE(IDB_BITMAP25));

		Event_Bit = (HBITMAP)LoadBitmap(g_hinstance, MAKEINTRESOURCE(IDB_BITMAP13));

		R208_back_Bit = (HBITMAP)LoadBitmap(g_hinstance, MAKEINTRESOURCE(IDB_BITMAP18));
		R208_init_Bit = (HBITMAP)LoadBitmap(g_hinstance, MAKEINTRESOURCE(IDB_BITMAP19));
		R208_nothing_Bit = (HBITMAP)LoadBitmap(g_hinstance, MAKEINTRESOURCE(IDB_BITMAP20));
		R211_dkdk_Bit = (HBITMAP)LoadBitmap(g_hinstance, MAKEINTRESOURCE(IDB_BITMAP21));
		Wellcome_Bit = (HBITMAP)LoadBitmap(g_hinstance, MAKEINTRESOURCE(IDB_BITMAP22));
		Ghost_Bit = (HBITMAP)LoadBitmap(g_hinstance, MAKEINTRESOURCE(IDB_BITMAP23));
		Iron_Bit = (HBITMAP)LoadBitmap(g_hinstance, MAKEINTRESOURCE(IDB_BITMAP24));
		GoHome_Bit = (HBITMAP)LoadBitmap(g_hinstance, MAKEINTRESOURCE(IDB_BITMAP26));

		BTN_GamePlay = CreateWindow(TEXT("button"), TEXT("게임 시작"), WS_CHILD | WS_VISIBLE | //버튼 생성
			BS_PUSHBUTTON, 450, 600, 100, 25, hwnd, (HMENU)ID_GamePlay, g_hinstance, NULL);
		BTN_Exit = CreateWindow(TEXT("button"), TEXT("종료"), WS_CHILD | WS_VISIBLE | //버튼 생성
			BS_PUSHBUTTON, 450, 650, 100, 25, hwnd, (HMENU)ID_Exit, g_hinstance, NULL);

		soundsetup();
		effsoundsetup();

		playsound(SD_1);			// 배경음 출력

		nState = G_Main;			// 현재 스테이지 상태

		toiletRand = rand()%3 + 1;	// 난수 생성

		return 0;
	
	case WM_COMMAND: //버튼 입력
		
		switch (LOWORD(wParam)) {
		
		case ID_GamePlay:
			
			ShowWindow(BTN_GamePlay, SW_HIDE);
			ShowWindow(BTN_Exit, SW_HIDE);
			nState = G_Prologue1;
			break;

		case ID_Exit:
			DestroyWindow(hwnd);
			break;
		
		}  
		
		InvalidateRect(hwnd, NULL, FALSE);
		return 0;
	
	case WM_KEYDOWN:
		
		switch (wParam) {

		// 스테이지를 전환하는 핵심 부분
		case VK_RETURN:		// 엔터키를 눌렀을 때
			
			// 프롤로그 화면 넘어가는 부분
			if (nState == G_Prologue1) {
				nState = G_Prologue2;
			}
			else if (nState == G_Prologue2) {
				nState = G_Prologue3;
			}
			else if (nState == G_Prologue3) {
				nState = G_Prologue4;
			}
			else if (nState == G_Prologue4) {
				nState = G_Prologue5;
			}
			else if (nState == G_Prologue5) {
				nState = G_GamePlay;
			}
			// 여기까지

			else if (nState == G_Gate) {
				nState = G_GamePlay;
			}

			else if (nState == G_211_1) {
				nState = G_211_2;
			}

			else if (nState == G_211_2) {
				nState = G_In211;
			}

			else if (nState == G_Finger) {
				nState = G_SelFinger;
			}

			else if (nState == G_EmptyToilet) {
				nState = G_Toilet;
			}

			else if (nState == G_DollToilet) {
				nState = G_GamePlay;
			}

			else if (nState == G_208_1) {
				nState = G_208_2;
			}

			else if (nState == G_208_2) {
				nState = G_GamePlay;
			}

			else if (nState == G_Ending1) {
				nState = G_GamePlay;
			}

			else if (nState == G_Ending2) {
				nState = G_GamePlay;
			}

			else if (nState == G_Ending3) {
				nState = G_GamePlay;
			}

			else if (nState == G_Ending5) {
				nState = G_GamePlay;
			}

			else if (nState == G_GetKey) {
				oldState = G_GetKey;
				nState = G_Event;

			}

			else if (nState == G_Password) {

				if (oldSelect == S_3) {

					if ( strcmp(src_208, pass) == 0 ) {
						effplaysound(EFFSD_5);
						nState = G_208;
					}
					else
						nState = G_GamePlay;

				}

				else if (oldSelect == S_4) {

					if ( strcmp(src_211, pass) == 0 ) {
						effplaysound(EFFSD_5);
						nState = G_211_1;
					}
					else
						nState = G_GamePlay;

				}

			}
			
			else if (nState == G_GamePlay) {
				
				if (sel_y == S_1) {
					sel_y = S_1;
					oldState = G_GamePlay;
					nState = G_Event;
				}
				
				else if (sel_y == S_2) {
					sel_y = S_1;
					nState = G_Toilet;
				}

				else if (sel_y == S_3) {
					sel_y = S_1;
					oldSelect = S_3;
					nState = G_Password;
				}

				else if (sel_y == S_4) {
					sel_y = S_1;
					oldSelect = S_4;
					nState = G_Password;
				}

				else if (sel_y == S_5) {
					sel_y = S_1;
					oldSelect = S_5;
					nState = G_Gate;

				}
			}
			
			else if (nState == G_Toilet) {
				
				if (sel_y == S_1) {
					sel_y = S_1;
					toiletSelect = 1;
					nState = G_InToilet;
				}
				
				else if (sel_y == S_2) {
					sel_y = S_1;
					toiletSelect = 2;
					nState = G_InToilet;
				}
				
				else if (sel_y == S_3) {
					sel_y = S_1;
					toiletSelect = 3;
					nState = G_InToilet;
				}
			
			}
			
			else if (nState == G_InToilet) {

				if (sel_y == S_1) {
					sel_y = S_1;
					oldSelect = S_1;
					oldState = G_Toilet;
					nState = G_Event;
				}
				else if (sel_y == S_2) {
					sel_y = S_1;
					oldSelect = S_2;
					oldState = G_Toilet;
					nState = G_Event;
				}
			}

			else if (nState == G_In211) {

				if (sel_y == S_1) {
					sel_y = S_1;
					oldSelect = S_1;
					oldState = G_In211;
					nState = G_Event;
				}

				else if (sel_y == S_2) {
					sel_y = S_1;
					oldSelect = S_2;
					oldState = G_In211;
					nState = G_Event;
				}

			}

			else if (nState == G_208) {

				if (sel_y == S_1) {
					sel_y = S_1;
					oldSelect = S_1;
					oldState = G_208;
					flag = 1;
					nState = G_Event;
				}

				else if (sel_y == S_2) {
					sel_y = S_1;
					oldSelect = S_2;
					oldState = G_208;
					nState = G_GamePlay;
				}

			}

			else if (nState == G_Event) {

				if (oldState == G_208) {

					nState = G_208;

				}

			}

			else if (nState == G_SelFinger) {

				if (sel_y == S_1 || sel_y == S_3) {
					oldSelect = S_1;
					oldState = G_SelFinger;
					sel_y = S_1;
					nState = G_Event;
				}

				else if (sel_y == S_2) {
					oldSelect = S_2;
					oldState = G_SelFinger;
					sel_y = S_1;
					nState = G_Event;
				}

			}

			break;

		// 방향키로 선택지 손가락 모양 위치 이동
		case VK_UP :
	
			if (sel_y - 30 < S_1)
				return 0;
			else {
				sel_y -= 30;
			}
			break;

		case VK_DOWN :
	
			if (sel_y + 30 > S_5)
				return 0;
			else {
				sel_y += 30;
			}
			break;

		}

		InvalidateRect(hwnd, NULL, TRUE);
		return 0;

	// 각 스테이지에 들어왔을때 알맞은 비트맵을 출력해주는 부분
	case WM_PAINT: //비트맵 출력
	
		hdc = BeginPaint(hwnd, &ps);
		MemDC = CreateCompatibleDC(hdc);
	
		font=CreateFont(fontSize,0,0,0,0,0,0,0,HANGEUL_CHARSET,0,0,0,0,"돋움");
		oldfont=(HFONT)SelectObject(hdc,font);
		SetTextColor(hdc, RGB(255, 255, 255));
		SetBkMode(hdc, TRANSPARENT);

		switch (nState) {
	
		case G_Main:
			SelectObject(MemDC, Main_Bit);
			BitBlt(hdc, 0, 0, 1024, 750, MemDC, 0, 0, SRCCOPY);
			DeleteObject(MemDC);
			break;

		case G_Toilet:
			SelectObject(MemDC, Toilet_Bit);
			BitBlt(hdc, 0, 0, 1024, 750, MemDC, 0, 0, SRCCOPY);
			DeleteObject(MemDC);

			playsound(SD_3);

			TextOut(hdc,sel_x,sel_y,sel_cursor,strlen(sel_cursor));
			TextOut(hdc, 200, head, toilet.qst, strlen(toilet.qst));
			TextOut(hdc,200,S_1,toilet.str1,strlen(toilet.str1));
			TextOut(hdc,200,S_2,toilet.str2,strlen(toilet.str2));
			TextOut(hdc,200,S_3,toilet.str3,strlen(toilet.str3));
		
			break;

		case G_InToilet :
			SelectObject(MemDC, Toilet_Bit);
			BitBlt(hdc, 0, 0, 1024, 750, MemDC, 0, 0, SRCCOPY);
			DeleteObject(MemDC);
		
			TextOut(hdc,sel_x,sel_y,sel_cursor,strlen(sel_cursor));
			TextOut(hdc, 200, head, InToilet.qst, strlen(InToilet.qst));
			TextOut(hdc,200,S_1,InToilet.str1,strlen(InToilet.str1));
			TextOut(hdc,200,S_2,InToilet.str2,strlen(InToilet.str2));
		
			break;

		case G_GamePlay:
			SelectObject(MemDC, Play_Bit);
			BitBlt(hdc, 0, 0, 1024, 750, MemDC, 0, 0, SRCCOPY);
			DeleteObject(MemDC);

			playsound(SD_2);

			TextOut(hdc,sel_x,sel_y,sel_cursor,strlen(sel_cursor));
			TextOut(hdc,200,head,main.qst,strlen(main.qst));
			TextOut(hdc,200,S_1,main.str1,strlen(main.str1));
			TextOut(hdc,200,S_2,main.str2,strlen(main.str2));
			TextOut(hdc,200,S_3,main.str3,strlen(main.str3));
			TextOut(hdc,200,S_4,main.str4,strlen(main.str4));
			TextOut(hdc,200,S_5,main.str5,strlen(main.str5));

			KillTimer(hwnd, T_GamePlay);

			strcpy(pass, "");

			break;

		case G_Gate :
			SelectObject(MemDC, Iron_Bit);
			BitBlt(hdc, 0, 0, 1024, 750, MemDC, 0, 0, SRCCOPY);
			DeleteObject(MemDC);

			TextOut(hdc, 200, head, "굳게 닫혀있다.", 14);
			TextOut(hdc, 200, head + 30, "열쇠가 필요하다.", 16);


			break;
	
		case G_Ending1 :
			SelectObject(MemDC, Ending1_Bit);
			BitBlt(hdc, 0, 0, 1024, 750, MemDC, 0, 0, SRCCOPY);
			DeleteObject(MemDC);
			KillTimer(hwnd, T_Ending1);
			break;
	
		case G_Ending2 :
			SelectObject(MemDC, Ending2_Bit);
			BitBlt(hdc, 0, 0, 1024, 750, MemDC, 0, 0, SRCCOPY);
			DeleteObject(MemDC);

			KillTimer(hwnd, T_Ending2);
			break;

		case G_Ending4 :
			SelectObject(MemDC, GoHome_Bit);
			BitBlt(hdc, 0, 0, 1024, 750, MemDC, 0, 0, SRCCOPY);
			DeleteObject(MemDC);
			playsound(SD_8);
			KillTimer(hwnd, T_Happy);
			break;

		case G_Ending5 :
			SelectObject(MemDC, Wellcome_Bit);
			BitBlt(hdc, 0, 0, 1024, 750, MemDC, 0, 0, SRCCOPY);
			DeleteObject(MemDC);
			KillTimer(hwnd, T_Ending5);
			break;

		case G_Prologue1:
			SelectObject(MemDC, Prologue_Bit1);
			BitBlt(hdc, 0, 0, 1024, 750, MemDC, 0, 0, SRCCOPY);		
			DeleteObject(MemDC);
			break;

	
		case G_Prologue2:
			SelectObject(MemDC, Prologue_Bit2);
			BitBlt(hdc, 0, 0, 1024, 750, MemDC, 0, 0, SRCCOPY);
			DeleteObject(MemDC);
			break;
	
		case G_Prologue3:
			SelectObject(MemDC, Prologue_Bit3);
			BitBlt(hdc, 0, 0, 1024, 750, MemDC, 0, 0, SRCCOPY);
			DeleteObject(MemDC);
			break;
	
		case G_Prologue4:
			SelectObject(MemDC, Prologue_Bit4);
			BitBlt(hdc, 0, 0, 1024, 750, MemDC, 0, 0, SRCCOPY);
			DeleteObject(MemDC);

			effplaysound(EFFSD_1);

			break;
	
		case G_Prologue5:
			SelectObject(MemDC, Prologue_Bit5);
			BitBlt(hdc, 0, 0, 1024, 750, MemDC, 0, 0, SRCCOPY);
			DeleteObject(MemDC);
			break;

		case G_Event :		// 이벤트
			SelectObject(MemDC, Event_Bit);	
			BitBlt(hdc, 0, 0, 1024, 750, MemDC, 0, 0, SRCCOPY);
			DeleteObject(MemDC);

			if (oldState == G_GamePlay) {
				playsound(SD_9);
				myTimer = (HANDLE)SetTimer(hwnd, T_Ending1, 2000, NULL);
			}

			else if (oldState == G_Toilet) {

				if (oldSelect == S_1) {
					effplaysound(EFFSD_3);
					myTimer = (HANDLE)SetTimer(hwnd, T_Toilet, 2000, NULL);
				}

				else if (oldSelect == S_2) {
					effplaysound(EFFSD_10);
					myTimer = (HANDLE)SetTimer(hwnd, T_Ending2, 4500, NULL);		// 겁쟁이엔딩
				}
			}

			else if (oldState == G_208) {

				if (oldSelect == S_1) {

					myTimer = (HANDLE)SetTimer(hwnd, T_208_1, 2000, NULL);

				}

			}

			else if (oldState == G_In211) {

				if (oldSelect == S_1) {
				
					effplaysound(EFFSD_7);
					myTimer = (HANDLE)SetTimer(hwnd, T_In211, 1000, NULL);

				}

				else if (oldSelect == S_2) {
					playsound(SD_10);
					myTimer = (HANDLE)SetTimer(hwnd, T_Ending5, 1000, NULL);

				}

			}

			else if (oldState == G_SelFinger) {

				effplaysound(EFFSD_9);

				if (oldSelect == S_1) {

				myTimer = (HANDLE)SetTimer(hwnd, T_SelFinger_wrong, 2000, NULL);

				}

				else if (oldSelect == S_2) {

					myTimer = (HANDLE)SetTimer(hwnd, T_SelFinger_right, 2000, NULL);

				}
			}

			else if (oldState == G_GetKey) {
				effplaysound(EFFSD_11);
				myTimer = (HANDLE)SetTimer(hwnd, T_Happy, 3000, NULL);

			}
			break;
	
		case G_EmptyToilet:
			SelectObject(MemDC, EmptyToilet_Bit);
			BitBlt(hdc, 0, 0, 1024, 750, MemDC, 0, 0, SRCCOPY);
			DeleteObject(MemDC);

			TextOut(hdc, 200, head, "비어있다. 돌아가자", 18);

			KillTimer(hwnd, T_Toilet);
			break;
	
		case G_DollToilet:
			SelectObject(MemDC, DollToilet_Bit);
			BitBlt(hdc, 0, 0, 1024, 750, MemDC, 0, 0, SRCCOPY);
			DeleteObject(MemDC);

			playsound(SD_4);

			KillTimer(hwnd, T_Toilet);
			break;
	
		case G_211_1 :
			SelectObject(MemDC, B211_1_Bit);
			BitBlt(hdc, 0, 0, 1024, 750, MemDC, 0, 0, SRCCOPY);
			DeleteObject(MemDC);

			playsound(SD_7);

			break;

		case G_211_2 :
			SelectObject(MemDC, B211_2_Bit);
			BitBlt(hdc, 0, 0, 1024, 750, MemDC, 0, 0, SRCCOPY);
			DeleteObject(MemDC);

			effplaysound(EFFSD_6);

			break;
		
		case G_In211 :
			SelectObject(MemDC, In211_Bit);
			BitBlt(hdc, 0, 0, 1024, 750, MemDC, 0, 0, SRCCOPY);
			DeleteObject(MemDC);

			TextOut(hdc, 200, head + 200, In211.qst, strlen(In211.qst));
			TextOut(hdc,sel_x,sel_y + 200,sel_cursor,strlen(sel_cursor));
			TextOut(hdc, 200, S_1+ 200, In211.str1, strlen(In211.str1));
			TextOut(hdc, 200, S_2+ 200, In211.str2, strlen(In211.str2));
			
			break;		

		case G_Finger :
			SelectObject(MemDC, Finger_Bit);
			BitBlt(hdc, 0, 0, 1024, 750, MemDC, 0, 0, SRCCOPY);
			DeleteObject(MemDC);

			KillTimer(hwnd, T_In211);

			break;

		case G_SelFinger :
			SelectObject(MemDC, Finger_Bit);
			BitBlt(hdc, 0, 0, 1024, 750, MemDC, 0, 0, SRCCOPY);
			DeleteObject(MemDC);

			effplaysound(EFFSD_8);
			
			TextOut(hdc, 200, head + 200, SelFinger.qst, strlen(SelFinger.qst));
			TextOut(hdc,sel_x,sel_y + 200,sel_cursor,strlen(sel_cursor));
			TextOut(hdc, 200, S_1 + 200, SelFinger.str1, strlen(SelFinger.str1));
			TextOut(hdc, 200, S_2 + 200, SelFinger.str2, strlen(SelFinger.str2));
			TextOut(hdc, 200, S_3 + 200, SelFinger.str3, strlen(SelFinger.str3));

			break;


		case G_GetKey :
			SelectObject(MemDC, GetKey_Bit);
			BitBlt(hdc, 0, 0, 1024, 750, MemDC, 0, 0, SRCCOPY);
			DeleteObject(MemDC);

			TextOut(hdc, 200, head + 200, "열쇠를 얻었다.", 14);

			KillTimer(hwnd, T_SelFinger_right);

			break;

		case G_Password :
			SelectObject(MemDC, Password_Bit);
			BitBlt(hdc, 0, 0, 1024, 750, MemDC, 0, 0, SRCCOPY);
			DeleteObject(MemDC);

			TextOut(hdc, 200, head, "input password", 14);
			TextOut(hdc, 200, 600, pass, strlen(pass));

			break;

		case G_208 :
			SelectObject(MemDC, R208_init_Bit);
			BitBlt(hdc, 0, 0, 1024, 750, MemDC, 0, 0, SRCCOPY);
			DeleteObject(MemDC);

			playsound(SD_5);
			
			TextOut(hdc, 200, head + 200, In208.qst, strlen(In208.qst));
			TextOut(hdc,sel_x,sel_y + 200,sel_cursor,strlen(sel_cursor));
			TextOut(hdc, 200, S_1 + 200, In208.str1, strlen(In208.str1));
			TextOut(hdc, 200, S_2 + 200, In208.str2, strlen(In208.str2));			

			break;

		case G_208_1 :
			SelectObject(MemDC, R208_nothing_Bit);
			BitBlt(hdc, 0, 0, 1024, 750, MemDC, 0, 0, SRCCOPY);
			DeleteObject(MemDC);

			TextOut(hdc, 200, head + 200, "아무것도 없는 것 같다. 돌아가자.", 32);

			KillTimer(hwnd, T_208_1);

			break;

		case G_208_2 :
			SelectObject(MemDC, R208_back_Bit);
			BitBlt(hdc, 0, 0, 1024, 750, MemDC, 0, 0, SRCCOPY);
			DeleteObject(MemDC);

			playsound(SD_6);

			TextOut(hdc, 200, head + 200, "갑자기 칠판에 글자가 나타났다.", 30);

			break;

		case G_Ending3 :
			SelectObject(MemDC, Ghost_Bit);
			BitBlt(hdc, 0, 0, 1024, 750, MemDC, 0, 0, SRCCOPY);
			DeleteObject(MemDC);

			KillTimer(hwnd, T_SelFinger_wrong);

			break;

			}
		
		SelectObject(hdc,oldfont);
		DeleteObject(font);
		
		EndPaint(hwnd, &ps);
		
		return 0;

	case WM_DESTROY: //닫기 버튼 누르면 실행

		for (int i = 0; i < SD_END; i++)
			FMOD_Sound_Release(g_psound[i]);

		for (int i = 0; i < EFFSD_END; i++)
			FMOD_Sound_Release(effg_psound[i]);

		FMOD_System_Close(g_psystem);
		FMOD_System_Release(g_psystem);

		FMOD_System_Close(effg_psystem);
		FMOD_System_Release(effg_psystem);

		PostQuitMessage(0); //윈도우창 종료;
		return 0;

	case WM_TIMER :

		switch (wParam) {

		case T_Ending1 :

			nState = G_Ending1;
			break;

		case T_Toilet :

			if (toiletSelect == toiletRand)
				nState = G_DollToilet;
			else
				nState = G_EmptyToilet;
			break;

		case T_Ending2 :

			nState = G_Ending2;
			break;

		case T_Ending5 :
			nState = G_Ending5;
			break;

		case T_GamePlay :

			nState = G_GamePlay;
			break;

		case T_In211 :

			nState = G_Finger;
			break;

		case T_SelFinger_right :

			nState = G_GetKey;
			break;

		case T_SelFinger_wrong :
			playsound(SD_11);
			nState = G_Ending3;
			break;

		case T_208_1 :

			nState = G_208_1;
			break;

		case T_Happy :

			nState = G_Ending4;
			break;

		}

		InvalidateRect(hwnd, NULL, TRUE);
		return 0;		

		}

return DefWindowProc(hwnd, uMsg, wParam, lParam); //기본적인 것을 처리하고 사용자가 원하는 것을 실행해줌

}

void soundsetup()
{
	char str[128];

	FMOD_System_Create(&g_psystem);

	FMOD_System_Init(g_psystem, 1, FMOD_INIT_NORMAL, NULL);

	for (int i = 0; i < SD_END; i++)
	{
		wsprintf(str, "sound\\sound%d.wav", i + 1);
		FMOD_System_CreateStream(g_psystem, str, FMOD_LOOP_NORMAL, 0, &g_psound[i]);
	}
}

void playsound(SOUNDKIND esound)
{
	FMOD_System_PlaySound(g_psystem, FMOD_CHANNEL_FREE, g_psound[esound], 0, &g_pchannel[esound]);
}

void effsoundsetup()
{
	char str[128];

	FMOD_System_Create(&effg_psystem);

	FMOD_System_Init(effg_psystem, 1, FMOD_INIT_NORMAL, NULL);

	for (int i = 0; i < EFFSD_END; i++)
	{
		wsprintf(str, "sound\\effsound%d.wav", i + 1);
		FMOD_System_CreateSound(effg_psystem, str, FMOD_DEFAULT, 0, &effg_psound[i]);
	}
}

void effplaysound(EFFSOUNDKIND esound)
{
	FMOD_System_PlaySound(effg_psystem, FMOD_CHANNEL_FREE, effg_psound[esound], 0, &effg_pchannel[esound]);
}