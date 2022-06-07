#include <windows.h>
#include <tchar.h>
#include "..\flogPr\resource.h"
#include <math.h>
#include<string.h>
#include<winres.h>
#include<process.h>
#include<time.h>
#include<stdio.h>
#include<stdlib.h>

#define BSIZE 20

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg,
	WPARAM wParam, LPARAM lParam);

void DrawStart(HDC hdc, int mode, BOOL startButton);

double LengthPts(int x1, int y1, int x2, int y2);		//모드 체크하기
BOOL InCircle(int x, int y, int mx, int my);
BOOL InRect(int x, int y, int mx, int my);


void DrawMap(int Rudder, int Blood, int Time, int Day, HDC hdc);
int CountTime(int Time);
int CountTimerest(int Time);
int DayCount(int Time);

void DrawEnding(HDC hdc, BOOL outcome);


HINSTANCE hInst;	//비트맵 출력하기

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpszCmdLine, int nCmdShow)
	// UNICODE 사용시 wWinMain() 형태 	
	// hPrevInstance 이전 인스턴스 항상 0값
	// lpszCmdLine > 외부에서 (내부로) 입력받는 변수
	// nCmdShow 윈도우 출력 형태에 관련한 값
{
	HWND     hwnd;
	MSG		 msg;
	WNDCLASS WndClass;
	hInst = hInstance;

	WndClass.style = CS_HREDRAW | CS_VREDRAW;	//height, vertical redraw
	WndClass.lpfnWndProc = WndProc;		// Proc 설정
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	// 형변환
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDC_FLOGMENU);
	// resource.h 파일 에서 메뉴이름 확인 109번
	WndClass.lpszClassName = _T("Window Class Name");
	RegisterClass(&WndClass);		// WndClass 등록
	hwnd = CreateWindow(
		_T("Window Class Name"),
		_T("SurvivleFlog"),		// 타이틀바, 학번이름 체크
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,		// 윈도우 스타일
		0, 0,					// 창출력좌표 x, y 
		1065, 670,					// 창크기 x, y축
		NULL,						// 부모 윈도우
		NULL,						// 메뉴바 핸들
		hInstance,					// 인스턴스
		NULL						// 여분, NULL
	);
	ShowWindow(hwnd, nCmdShow);		// 윈도우 출력, WM_PAINT 출력내용 가져옴
	UpdateWindow(hwnd);				// WM_PAINT 출력내용 발생해서 출력하도록
									// hwnd 핸들을 통해 보여주고 갱신

	//ShowWindow(hwnd, SW_SHOW);	// 위와 같음
	//UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))	// 메시지 큐의 메시지를 가져옴
	{
		TranslateMessage(&msg);		// 키입력에 반응하는 메시지 변환, WM_KEYDOWN (키가 눌릴때) WM_CHAR 메시지 발생
		DispatchMessage(&msg);		// WndProc() 함수 호출과 WndProc()으로 메세지 전달
	}								// 종료는 WM_QUIT 발생할때 FALSE 리턴하면서 종료
	return (int)msg.wParam;			// wParam, lParam 윈도우 크기가 어떻게 변했는지, 변경된 클라이언트, 키보드, 마우스 값

}

double LengthPts(int x1, int y1, int x2, int y2) {
	return(sqrt((float)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1))));
}

BOOL InCircle(int x, int y, int mx, int my) {
	if (LengthPts(x, y, mx, my) < BSIZE)return TRUE;
	else return FALSE;
}
BOOL InRect(int x, int y, int mx, int my) {
	if (LengthPts(x, y, mx, my) < 30)return TRUE;
	else return FALSE;
}
int CountTime(int Time) {
	int minute;
	minute = Time / 60;
	return minute;
}

int CountTimerest(int Time) {
	int second;
	second = Time % 60;
	return second;
}

int DayCount(int firstTime ,int Time) {
	if ((Time <= firstTime+1)&& (Time >= (firstTime * 2 / 3)))return 1;
	else if (Time <= (firstTime * 2 / 3)&& Time >= (firstTime * 1 / 3))return 2;
	else return 3;
}

void DrawStart(HDC hdc, int mode, BOOL startButton) {
	HDC memdc;
	HBITMAP hBit , RoomCheck[2], RoomCheckMask[2] ,oldBit, StartButton[2];
	

	memdc = CreateCompatibleDC(hdc);
	hBit = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_STARTBACKGROUND));

	oldBit = (HBITMAP)SelectObject(memdc, hBit);
	BitBlt(hdc, 0, 0, 1050, 618, memdc, 0, 0, SRCCOPY);

	RoomCheck[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ROOM));
	RoomCheck[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CHECK));
	
	RoomCheckMask[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ROOMMASK));
	RoomCheckMask[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CHECKMASK));

	if (mode == 0) { 
		SelectObject(memdc, RoomCheckMask[1]);
		StretchBlt(hdc, 350, 370, 40, 40, memdc, 0, 0, 48, 48, SRCAND);
		SelectObject(memdc, RoomCheck[1]);
		StretchBlt(hdc, 350, 370, 40, 40, memdc, 0, 0, 48, 48, SRCPAINT);
		SelectObject(memdc, RoomCheckMask[0]);
		StretchBlt(hdc, 480, 370, 40, 40, memdc, 0, 0, 48, 48, SRCAND);
		StretchBlt(hdc, 600, 370, 40, 40, memdc, 0, 0, 48, 48, SRCAND);
		SelectObject(memdc, RoomCheck[0]);
		StretchBlt(hdc, 480, 370, 40, 40, memdc, 0, 0, 48, 48, SRCPAINT);
		StretchBlt(hdc, 600, 370, 40, 40, memdc, 0, 0, 48, 48, SRCPAINT);
	}
	if (mode == 1) {
		SelectObject(memdc, RoomCheckMask[1]);
		StretchBlt(hdc, 480, 370, 40, 40, memdc, 0, 0, 48, 48, SRCAND);
		SelectObject(memdc, RoomCheck[1]);
		StretchBlt(hdc, 480, 370, 40, 40, memdc, 0, 0, 48, 48, SRCPAINT);
		SelectObject(memdc, RoomCheckMask[0]);
		StretchBlt(hdc, 350, 370, 40, 40, memdc, 0, 0, 48, 48, SRCAND);
		StretchBlt(hdc, 600, 370, 40, 40, memdc, 0, 0, 48, 48, SRCAND);
		SelectObject(memdc, RoomCheck[0]);
		StretchBlt(hdc, 350, 370, 40, 40, memdc, 0, 0, 48, 48, SRCPAINT);
		StretchBlt(hdc, 600, 370, 40, 40, memdc, 0, 0, 48, 48, SRCPAINT);
	}
	if (mode == 2) {
		SelectObject(memdc, RoomCheckMask[1]);
		StretchBlt(hdc, 600, 370, 40, 40, memdc, 0, 0, 48, 48, SRCAND);
		SelectObject(memdc, RoomCheck[1]);
		StretchBlt(hdc, 600, 370, 40, 40, memdc, 0, 0, 48, 48, SRCPAINT);
		SelectObject(memdc, RoomCheckMask[0]);
		StretchBlt(hdc, 350, 370, 40, 40, memdc, 0, 0, 48, 48, SRCAND);
		StretchBlt(hdc, 480, 370, 40, 40, memdc, 0, 0, 48, 48, SRCAND);
		SelectObject(memdc, RoomCheck[0]);
		StretchBlt(hdc, 350, 370, 40, 40, memdc, 0, 0, 48, 48, SRCPAINT);
		StretchBlt(hdc, 480, 370, 40, 40, memdc, 0, 0, 48, 48, SRCPAINT);
	}
	StartButton[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_STARTBUTTON));
	StartButton[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_STARTBUTTONACT));
	if (startButton == FALSE) {
		SelectObject(memdc, StartButton[0]);
		StretchBlt(hdc, 390, 413, 220, 60, memdc, 0, 0, 322, 87, SRCCOPY);
	}
	if (startButton == TRUE) {
		SelectObject(memdc, StartButton[1]);
		StretchBlt(hdc, 390, 413, 220, 60, memdc, 0, 0, 322, 87, SRCCOPY);
	}

	SelectObject(memdc, oldBit);
	for (int i = 0; i < 2; i++) {
		DeleteObject(RoomCheck[i]);
		DeleteObject(RoomCheckMask[i]);
		DeleteObject(StartButton[i]);
	}
	DeleteDC(memdc);
	DeleteObject(hBit);
	}

void DrawMap(int Rudder ,int Blood, int Time, int Day, HDC hdc) {
	
	HDC memdc;
	HBITMAP hBit[3], oldBit[3],  RudderBit[4] , bloodBit[3], BuffBit;	//hBit[0-2}=배경 hBit[3]=타임
	HBITMAP RudderBitMask[2], bloodBitMask, BuffBitMask;
	HFONT hFont, OldFont;
	int i;
	
	static TCHAR Cminute[10] = { 0 }, Csecond[10] = { 0 };
	TCHAR colon[] = _T(":");
	int Mminute, Msecond;
	
	Mminute = CountTime(Time);
	Msecond = CountTimerest(Time);
	_stprintf_s(Cminute, _countof(Cminute), _T("%d"), Mminute);
	_stprintf_s(Csecond, _countof(Csecond), _T("%d"), Msecond);

	memdc = CreateCompatibleDC(hdc);		//호환
	


											//수를 받아서 비트맵을 달리 받는 방법
	hBit[0] = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BACKGROUNDDAY));
	hBit[1] = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BACKGROUNDDAWN));
	hBit[2] = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BACKGROUNDNIGHT));
	


	//배경
	if(Day==1)oldBit[0] = (HBITMAP)SelectObject(memdc, hBit[0]);
	else if (Day == 2)oldBit[1] = (HBITMAP)SelectObject(memdc, hBit[1]);
	else if (Day == 3)oldBit[2] = (HBITMAP)SelectObject(memdc, hBit[2]);

	BitBlt(hdc, 0, 0, 1050, 618, memdc, 0, 0, SRCCOPY);		//hdc


	RudderBit[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_LEFT));
	RudderBit[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_RIGHT));
	RudderBit[2] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_LEFTPUSH));
	RudderBit[3] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_RIGHTPUSH));

	RudderBitMask[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_LEFTMASK));
	RudderBitMask[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_RIGHTMASK));

	if (Rudder == 0) {
		SelectObject(memdc, RudderBitMask[0]);
		BitBlt(hdc, 950, 568, 1000, 618, memdc, 0, 0, SRCAND);		//
		SelectObject(memdc, RudderBitMask[1]);
		BitBlt(hdc, 1000, 568, 1050, 618, memdc, 0, 0, SRCAND);
		SelectObject(memdc, RudderBit[0]);
		BitBlt(hdc, 950, 568, 1000, 618, memdc, 0, 0, SRCPAINT);		
		SelectObject(memdc, RudderBit[1]);
		BitBlt(hdc, 1000, 568, 1050, 618, memdc, 0, 0, SRCPAINT);		
	}

	else if (Rudder == 1) {
		SelectObject(memdc, RudderBitMask[0]);
		BitBlt(hdc, 950, 568, 1000, 618, memdc, 0, 0, SRCAND);		//
		SelectObject(memdc, RudderBitMask[1]);
		BitBlt(hdc, 1000, 568, 1050, 618, memdc, 0, 0, SRCAND);
		SelectObject(memdc, RudderBit[0]);
		BitBlt(hdc, 950, 568, 1000, 618, memdc, 0, 0, SRCPAINT);
		SelectObject(memdc, RudderBit[3]);
		BitBlt(hdc, 1000, 568, 1050, 618, memdc, 0, 0, SRCPAINT);
	}

	else if (Rudder == 2) {
		SelectObject(memdc, RudderBitMask[0]);
		BitBlt(hdc, 950, 568, 1000, 618, memdc, 0, 0, SRCAND);		//
		SelectObject(memdc, RudderBitMask[1]);
		BitBlt(hdc, 1000, 568, 1050, 618, memdc, 0, 0, SRCAND);
		SelectObject(memdc, RudderBit[2]);
		BitBlt(hdc, 950, 568, 1000, 618, memdc, 0, 0, SRCPAINT);
		SelectObject(memdc, RudderBit[1]);
		BitBlt(hdc, 1000, 568, 1050, 618, memdc, 0, 0, SRCPAINT);
	}

	bloodBit[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BLOODEMPTY));
	bloodBit[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BLOODHALF));
	bloodBit[2] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BLOODFULL));

	bloodBitMask = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BLOODEMPTYMASK));
	

		if (Blood == 10) {
			SelectObject(memdc, bloodBitMask);
			BitBlt(hdc, 0, 0, 40, 49, memdc, 0, 0, SRCAND);
			SelectObject(memdc, bloodBitMask);
			BitBlt(hdc, 40, 0, 80, 49, memdc, 0, 0, SRCAND);
			SelectObject(memdc, bloodBitMask);
			BitBlt(hdc, 80, 0, 120, 49, memdc, 0, 0, SRCAND);
			SelectObject(memdc, bloodBitMask);
			BitBlt(hdc, 120, 0, 160, 49, memdc, 0, 0, SRCAND);
			SelectObject(memdc, bloodBitMask);
			BitBlt(hdc, 160, 0, 200, 49, memdc, 0, 0, SRCAND);

			SelectObject(memdc, bloodBit[2]);
			BitBlt(hdc, 0, 0, 40, 49, memdc, 0, 0, SRCPAINT);
			SelectObject(memdc, bloodBit[2]);
			BitBlt(hdc, 40, 0, 80, 49, memdc, 0, 0, SRCPAINT);
			SelectObject(memdc, bloodBit[2]);
			BitBlt(hdc, 80, 0, 120, 49, memdc, 0, 0, SRCPAINT);
			SelectObject(memdc, bloodBit[2]);
			BitBlt(hdc, 120, 0, 160, 49, memdc, 0, 0, SRCPAINT);
			SelectObject(memdc, bloodBit[2]);
			BitBlt(hdc, 160, 0, 200, 49, memdc, 0, 0, SRCPAINT);
		}
		else if (Blood == 9) {
			SelectObject(memdc, bloodBitMask);
			BitBlt(hdc, 0, 0, 40, 49, memdc, 0, 0, SRCAND);
			SelectObject(memdc, bloodBitMask);
			BitBlt(hdc, 40, 0, 80, 49, memdc, 0, 0, SRCAND);
			SelectObject(memdc, bloodBitMask);
			BitBlt(hdc, 80, 0, 120, 49, memdc, 0, 0, SRCAND);
			SelectObject(memdc, bloodBitMask);
			BitBlt(hdc, 120, 0, 160, 49, memdc, 0, 0, SRCAND);
			SelectObject(memdc, bloodBitMask);
			BitBlt(hdc, 160, 0, 200, 49, memdc, 0, 0, SRCAND);

			SelectObject(memdc, bloodBit[2]);
			BitBlt(hdc, 0, 0, 40, 49, memdc, 0, 0, SRCPAINT);
			SelectObject(memdc, bloodBit[2]);
			BitBlt(hdc, 40, 0, 80, 49, memdc, 0, 0, SRCPAINT);
			SelectObject(memdc, bloodBit[2]);
			BitBlt(hdc, 80, 0, 120, 49, memdc, 0, 0, SRCPAINT);
			SelectObject(memdc, bloodBit[2]);
			BitBlt(hdc, 120, 0, 160, 49, memdc, 0, 0, SRCPAINT);
			SelectObject(memdc, bloodBit[1]);
			BitBlt(hdc, 160, 0, 200, 49, memdc, 0, 0, SRCPAINT);
		}
		else if (Blood == 8) {
			SelectObject(memdc, bloodBitMask);
			BitBlt(hdc, 0, 0, 40, 49, memdc, 0, 0, SRCAND);
			SelectObject(memdc, bloodBitMask);
			BitBlt(hdc, 40, 0, 80, 49, memdc, 0, 0, SRCAND);
			SelectObject(memdc, bloodBitMask);
			BitBlt(hdc, 80, 0, 120, 49, memdc, 0, 0, SRCAND);
			SelectObject(memdc, bloodBitMask);
			BitBlt(hdc, 120, 0, 160, 49, memdc, 0, 0, SRCAND);
			SelectObject(memdc, bloodBitMask);
			BitBlt(hdc, 160, 0, 200, 49, memdc, 0, 0, SRCAND);

			SelectObject(memdc, bloodBit[2]);
			BitBlt(hdc, 0, 0, 40, 49, memdc, 0, 0, SRCPAINT);
			SelectObject(memdc, bloodBit[2]);
			BitBlt(hdc, 40, 0, 80, 49, memdc, 0, 0, SRCPAINT);
			SelectObject(memdc, bloodBit[2]);
			BitBlt(hdc, 80, 0, 120, 49, memdc, 0, 0, SRCPAINT);
			SelectObject(memdc, bloodBit[2]);
			BitBlt(hdc, 120, 0, 160, 49, memdc, 0, 0, SRCPAINT);
			SelectObject(memdc, bloodBit[0]);
			BitBlt(hdc, 160, 0, 200, 49, memdc, 0, 0, SRCPAINT);
		}
		else if (Blood == 7) {
			SelectObject(memdc, bloodBitMask);
			BitBlt(hdc, 0, 0, 40, 49, memdc, 0, 0, SRCAND);
			SelectObject(memdc, bloodBitMask);
			BitBlt(hdc, 40, 0, 80, 49, memdc, 0, 0, SRCAND);
			SelectObject(memdc, bloodBitMask);
			BitBlt(hdc, 80, 0, 120, 49, memdc, 0, 0, SRCAND);
			SelectObject(memdc, bloodBitMask);
			BitBlt(hdc, 120, 0, 160, 49, memdc, 0, 0, SRCAND);
			SelectObject(memdc, bloodBitMask);
			BitBlt(hdc, 160, 0, 200, 49, memdc, 0, 0, SRCAND);

			SelectObject(memdc, bloodBit[2]);
			BitBlt(hdc, 0, 0, 40, 49, memdc, 0, 0, SRCPAINT);
			SelectObject(memdc, bloodBit[2]);
			BitBlt(hdc, 40, 0, 80, 49, memdc, 0, 0, SRCPAINT);
			SelectObject(memdc, bloodBit[2]);
			BitBlt(hdc, 80, 0, 120, 49, memdc, 0, 0, SRCPAINT);
			SelectObject(memdc, bloodBit[1]);
			BitBlt(hdc, 120, 0, 160, 49, memdc, 0, 0, SRCPAINT);
			SelectObject(memdc, bloodBit[0]);
			BitBlt(hdc, 160, 0, 200, 49, memdc, 0, 0, SRCPAINT);
		}
		else if (Blood == 6) {
			SelectObject(memdc, bloodBitMask);
			BitBlt(hdc, 0, 0, 40, 49, memdc, 0, 0, SRCAND);
			SelectObject(memdc, bloodBitMask);
			BitBlt(hdc, 40, 0, 80, 49, memdc, 0, 0, SRCAND);
			SelectObject(memdc, bloodBitMask);
			BitBlt(hdc, 80, 0, 120, 49, memdc, 0, 0, SRCAND);
			SelectObject(memdc, bloodBitMask);
			BitBlt(hdc, 120, 0, 160, 49, memdc, 0, 0, SRCAND);
			SelectObject(memdc, bloodBitMask);
			BitBlt(hdc, 160, 0, 200, 49, memdc, 0, 0, SRCAND);

			SelectObject(memdc, bloodBit[2]);
			BitBlt(hdc, 0, 0, 40, 49, memdc, 0, 0, SRCPAINT);
			SelectObject(memdc, bloodBit[2]);
			BitBlt(hdc, 40, 0, 80, 49, memdc, 0, 0, SRCPAINT);
			SelectObject(memdc, bloodBit[2]);
			BitBlt(hdc, 80, 0, 120, 49, memdc, 0, 0, SRCPAINT);
			SelectObject(memdc, bloodBit[0]);
			BitBlt(hdc, 120, 0, 160, 49, memdc, 0, 0, SRCPAINT);
			SelectObject(memdc, bloodBit[0]);
			BitBlt(hdc, 160, 0, 200, 49, memdc, 0, 0, SRCPAINT);
		}
		else if (Blood == 5) {
		SelectObject(memdc, bloodBitMask);
		BitBlt(hdc, 0, 0, 40, 49, memdc, 0, 0, SRCAND);
		SelectObject(memdc, bloodBitMask);
		BitBlt(hdc, 40, 0, 80, 49, memdc, 0, 0, SRCAND);
		SelectObject(memdc, bloodBitMask);
		BitBlt(hdc, 80, 0, 120, 49, memdc, 0, 0, SRCAND);
		SelectObject(memdc, bloodBitMask);
		BitBlt(hdc, 120, 0, 160, 49, memdc, 0, 0, SRCAND);
		SelectObject(memdc, bloodBitMask);
		BitBlt(hdc, 160, 0, 200, 49, memdc, 0, 0, SRCAND);

			SelectObject(memdc, bloodBit[2]);
			BitBlt(hdc, 0, 0, 40, 49, memdc, 0, 0, SRCPAINT);
			SelectObject(memdc, bloodBit[2]);
			BitBlt(hdc, 40, 0, 80, 49, memdc, 0, 0, SRCPAINT);
			SelectObject(memdc, bloodBit[1]);
			BitBlt(hdc, 80, 0, 120, 49, memdc, 0, 0, SRCPAINT);
			SelectObject(memdc, bloodBit[0]);
			BitBlt(hdc, 120, 0, 160, 49, memdc, 0, 0, SRCPAINT);
			SelectObject(memdc, bloodBit[0]);
			BitBlt(hdc, 160, 0, 200, 49, memdc, 0, 0, SRCPAINT);
		}
		else if (Blood == 4) {
		SelectObject(memdc, bloodBitMask);
		BitBlt(hdc, 0, 0, 40, 49, memdc, 0, 0, SRCAND);
		SelectObject(memdc, bloodBitMask);
		BitBlt(hdc, 40, 0, 80, 49, memdc, 0, 0, SRCAND);
		SelectObject(memdc, bloodBitMask);
		BitBlt(hdc, 80, 0, 120, 49, memdc, 0, 0, SRCAND);
		SelectObject(memdc, bloodBitMask);
		BitBlt(hdc, 120, 0, 160, 49, memdc, 0, 0, SRCAND);
		SelectObject(memdc, bloodBitMask);
		BitBlt(hdc, 160, 0, 200, 49, memdc, 0, 0, SRCAND);

			SelectObject(memdc, bloodBit[2]);
			BitBlt(hdc, 0, 0, 40, 49, memdc, 0, 0, SRCPAINT);
			SelectObject(memdc, bloodBit[2]);
			BitBlt(hdc, 40, 0, 80, 49, memdc, 0, 0, SRCPAINT);
			SelectObject(memdc, bloodBit[0]);
			BitBlt(hdc, 80, 0, 120, 49, memdc, 0, 0, SRCPAINT);
			SelectObject(memdc, bloodBit[0]);
			BitBlt(hdc, 120, 0, 160, 49, memdc, 0, 0, SRCPAINT);
			SelectObject(memdc, bloodBit[0]);
			BitBlt(hdc, 160, 0, 200, 49, memdc, 0, 0, SRCPAINT);
		}
		else if (Blood == 3) {
		SelectObject(memdc, bloodBitMask);
		BitBlt(hdc, 0, 0, 40, 49, memdc, 0, 0, SRCAND);
		SelectObject(memdc, bloodBitMask);
		BitBlt(hdc, 40, 0, 80, 49, memdc, 0, 0, SRCAND);
		SelectObject(memdc, bloodBitMask);
		BitBlt(hdc, 80, 0, 120, 49, memdc, 0, 0, SRCAND);
		SelectObject(memdc, bloodBitMask);
		BitBlt(hdc, 120, 0, 160, 49, memdc, 0, 0, SRCAND);
		SelectObject(memdc, bloodBitMask);
		BitBlt(hdc, 160, 0, 200, 49, memdc, 0, 0, SRCAND);

			SelectObject(memdc, bloodBit[2]);
			BitBlt(hdc, 0, 0, 40, 49, memdc, 0, 0, SRCPAINT);
			SelectObject(memdc, bloodBit[1]);
			BitBlt(hdc, 40, 0, 80, 49, memdc, 0, 0, SRCPAINT);
			SelectObject(memdc, bloodBit[0]);
			BitBlt(hdc, 80, 0, 120, 49, memdc, 0, 0, SRCPAINT);
			SelectObject(memdc, bloodBit[0]);
			BitBlt(hdc, 120, 0, 160, 49, memdc, 0, 0, SRCPAINT);
			SelectObject(memdc, bloodBit[0]);
			BitBlt(hdc, 160, 0, 200, 49, memdc, 0, 0, SRCPAINT);
		}
		else if (Blood == 2) {
		SelectObject(memdc, bloodBitMask);
		BitBlt(hdc, 0, 0, 40, 49, memdc, 0, 0, SRCAND);
		SelectObject(memdc, bloodBitMask);
		BitBlt(hdc, 40, 0, 80, 49, memdc, 0, 0, SRCAND);
		SelectObject(memdc, bloodBitMask);
		BitBlt(hdc, 80, 0, 120, 49, memdc, 0, 0, SRCAND);
		SelectObject(memdc, bloodBitMask);
		BitBlt(hdc, 120, 0, 160, 49, memdc, 0, 0, SRCAND);
		SelectObject(memdc, bloodBitMask);
		BitBlt(hdc, 160, 0, 200, 49, memdc, 0, 0, SRCAND);

			SelectObject(memdc, bloodBit[2]);
			BitBlt(hdc, 0, 0, 40, 49, memdc, 0, 0, SRCPAINT);
			SelectObject(memdc, bloodBit[0]);
			BitBlt(hdc, 40, 0, 80, 49, memdc, 0, 0, SRCPAINT);
			SelectObject(memdc, bloodBit[0]);
			BitBlt(hdc, 80, 0, 120, 49, memdc, 0, 0, SRCPAINT);
			SelectObject(memdc, bloodBit[0]);
			BitBlt(hdc, 120, 0, 160, 49, memdc, 0, 0, SRCPAINT);
			SelectObject(memdc, bloodBit[0]);
			BitBlt(hdc, 160, 0, 200, 49, memdc, 0, 0, SRCPAINT);
		}
		else if (Blood == 1) {
		SelectObject(memdc, bloodBitMask);
		BitBlt(hdc, 0, 0, 40, 49, memdc, 0, 0, SRCAND);
		SelectObject(memdc, bloodBitMask);
		BitBlt(hdc, 40, 0, 80, 49, memdc, 0, 0, SRCAND);
		SelectObject(memdc, bloodBitMask);
		BitBlt(hdc, 80, 0, 120, 49, memdc, 0, 0, SRCAND);
		SelectObject(memdc, bloodBitMask);
		BitBlt(hdc, 120, 0, 160, 49, memdc, 0, 0, SRCAND);
		SelectObject(memdc, bloodBitMask);
		BitBlt(hdc, 160, 0, 200, 49, memdc, 0, 0, SRCAND);

		SelectObject(memdc, bloodBit[1]);
		BitBlt(hdc, 0, 0, 40, 49, memdc, 0, 0, SRCPAINT);
		SelectObject(memdc, bloodBit[0]);
		BitBlt(hdc, 40, 0, 80, 49, memdc, 0, 0, SRCPAINT);
		SelectObject(memdc, bloodBit[0]);
		BitBlt(hdc, 80, 0, 120, 49, memdc, 0, 0, SRCPAINT);
		SelectObject(memdc, bloodBit[0]);
		BitBlt(hdc, 120, 0, 160, 49, memdc, 0, 0, SRCPAINT);
		SelectObject(memdc, bloodBit[0]);
		BitBlt(hdc, 160, 0, 200, 49, memdc, 0, 0, SRCPAINT);
		}
		else if (Blood == 0) {
		SelectObject(memdc, bloodBitMask);
		BitBlt(hdc, 0, 0, 40, 49, memdc, 0, 0, SRCAND);
		SelectObject(memdc, bloodBitMask);
		BitBlt(hdc, 40, 0, 80, 49, memdc, 0, 0, SRCAND);
		SelectObject(memdc, bloodBitMask);
		BitBlt(hdc, 80, 0, 120, 49, memdc, 0, 0, SRCAND);
		SelectObject(memdc, bloodBitMask);
		BitBlt(hdc, 120, 0, 160, 49, memdc, 0, 0, SRCAND);
		SelectObject(memdc, bloodBitMask);
		BitBlt(hdc, 160, 0, 200, 49, memdc, 0, 0, SRCAND);

		SelectObject(memdc, bloodBit[0]);
		BitBlt(hdc, 0, 0, 40, 49, memdc, 0, 0, SRCPAINT);
		SelectObject(memdc, bloodBit[0]);
		BitBlt(hdc, 40, 0, 80, 49, memdc, 0, 0, SRCPAINT);
		SelectObject(memdc, bloodBit[0]);
		BitBlt(hdc, 80, 0, 120, 49, memdc, 0, 0, SRCPAINT);
		SelectObject(memdc, bloodBit[0]);
		BitBlt(hdc, 120, 0, 160, 49, memdc, 0, 0, SRCPAINT);
		SelectObject(memdc, bloodBit[0]);
		BitBlt(hdc, 160, 0, 200, 49, memdc, 0, 0, SRCPAINT);
		
		}

		//버프 칸
		BuffBit = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ROOM));
		BuffBitMask = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ROOMMASK));

		SelectObject(memdc, BuffBitMask);
		BitBlt(hdc, 0, 50, 50, 100, memdc, 0, 0, SRCAND);		//dna
		SelectObject(memdc, BuffBitMask);
		BitBlt(hdc, 50, 50, 100, 100, memdc, 0, 0, SRCAND);	//dopping
		SelectObject(memdc, BuffBitMask);
		BitBlt(hdc, 100, 50, 150, 100, memdc, 0, 0, SRCAND);	//slime
		SelectObject(memdc, BuffBitMask);
		BitBlt(hdc, 0, 100, 50, 150, memdc, 0, 0, SRCAND);		//alchol
		SelectObject(memdc, BuffBitMask);
		BitBlt(hdc, 50, 100, 100, 150, memdc, 0, 0, SRCAND);	//rock

		SelectObject(memdc, BuffBit);
		BitBlt(hdc, 0, 50, 50, 100, memdc, 0, 0, SRCPAINT);		//dna
		SelectObject(memdc, BuffBit);
		BitBlt(hdc, 50, 50, 100, 100, memdc, 0, 0, SRCPAINT);	//dopping
		SelectObject(memdc, BuffBit);
		BitBlt(hdc, 100, 50, 150, 100, memdc, 0, 0, SRCPAINT);	//slime
		SelectObject(memdc, BuffBit);
		BitBlt(hdc, 0, 100, 50, 150, memdc, 0, 0, SRCPAINT);		//alchol
		SelectObject(memdc, BuffBit);
		BitBlt(hdc, 50, 100, 100, 150, memdc, 0, 0, SRCPAINT);	//rock

		//타이머 출력
		SetBkMode(hdc, TRANSPARENT);
		hFont = CreateFont(50,0,0,0,0,0,0,0,HANGEUL_CHARSET,0,0,0,VARIABLE_PITCH|FF_ROMAN, TEXT("궁서"));
		OldFont = (HFONT)SelectObject(hdc, hFont);
		TextOut(hdc, 500, 50, Cminute,_tcslen(Cminute));
		TextOut(hdc, 515, 50, colon, _tcslen(colon));
		TextOut(hdc, 530, 50, Csecond, _tcslen(Csecond));
		

		if (Day == 1)SelectObject(hdc, oldBit[0]);
		else if(Day == 2)SelectObject(hdc, oldBit[1]);
		else if(Day == 3)SelectObject(hdc, oldBit[2]);
		for (i = 0; i < 4; i++)DeleteObject(RudderBit[i]);
		DeleteDC(hdc);
		for (i = 0; i < 3; i++)DeleteObject(hBit[i]);
		for (i = 0; i < 3; i++)DeleteObject(oldBit[i]);
		
		//if ((Blood == 0)&&(Time!=0)) { return 1; }			//패배
		//else if ((Time == 0)&&(Blood!=0)) { return 2; }		//승리
}

//

void DrawEnding(int xPos, int yPos, HDC hdc, BOOL outcome) {
	HDC memdc;
	HBITMAP hBit, oldBit, frog[2], balloon[2] ,win[2],lose[2];
	
	memdc = CreateCompatibleDC(hdc);
	hBit = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ENDINGBACKGROUND));
	oldBit = (HBITMAP)SelectObject(memdc, hBit);
	StretchBlt(hdc, 0, 0, 1050, 618, memdc, 0, 0, 1532, 902, SRCCOPY);

	frog[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ENDINGFROGMASK));
	frog[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ENDINGFROG));

	balloon[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ENDINGBALLOONMASK));
	balloon[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ENDINGBALLOON));
	
	if (outcome==2) {
		win[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ENDINGVICTORYMASK));
		win[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ENDINGVICTORY));
		SelectObject(memdc, win[1]);
		StretchBlt(hdc, 420, 240, 220, 60, memdc, 0, 0, 449, 117, SRCAND);
		SelectObject(memdc, win[0]);
		StretchBlt(hdc, 420, 240, 220, 60, memdc, 0, 0, 453, 120 , SRCPAINT);

		
	}
	if (outcome == 1) {
		lose[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ENDINGLOSEMASK));
		lose[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ENDINGLOSE));
		SelectObject(memdc, lose[1]);
		StretchBlt(hdc, 420, 240, 130, 50, memdc, 0, 0, 258, 83, SRCAND);
		SelectObject(memdc, lose[0]);
		StretchBlt(hdc, 420, 240, 130, 50, memdc, 0, 0, 258, 83, SRCPAINT);
	}

	SelectObject(memdc, frog[1]);
	StretchBlt(hdc, xPos+20, yPos+120, 40, 40, memdc, 0, 0, 55, 55, SRCAND);
	SelectObject(memdc, frog[0]);
	StretchBlt(hdc, xPos+20, yPos+120, 40, 40, memdc, 0, 0, 56, 56, SRCPAINT);

	SelectObject(memdc, balloon[1]);
	StretchBlt(hdc, xPos, yPos, 102, 120, memdc, 0, 0, 204, 243, SRCAND);
	SelectObject(memdc, balloon[0]);
	StretchBlt(hdc, xPos, yPos, 102, 120, memdc, 0, 0, 205, 244, SRCPAINT);

	SelectObject(memdc, oldBit);
	for (int i = 0; i < 2; i++) {
		DeleteObject(frog[i]);
		DeleteObject(balloon[i]);
		DeleteObject(win[i]);
		DeleteObject(lose[i]);
	}
	DeleteDC(memdc);
	DeleteObject(hBit);

}		

//


LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg,
	WPARAM wParam, LPARAM lParam)
	//	WinDef.h 에서 정의
	//	wPram > unsigned ptr, lParam > long ptr 
{
	HDC hdc;		//최종적으로 hdc에 그림을 출력 memdc에는 버퍼 역할
	PAINTSTRUCT ps;
	static int keyboard ,blood , day, firstTime ;
	static int time, mode, drawMapNum;
	
	
	static int x[4], y[4];	//1-3mode 4 start button
	int mx, my;			//마우스 포인트
	static BOOL Selection[3], startButton, start;
	static BOOL startMap;//시작 페이지에만 있는 사각형만들기

	static int outcome ,xPos, yPos;
	
	switch (iMsg)
	{
	case WM_CREATE:
		//초기화
		keyboard = 0;
		startMap = TRUE;
		mode = 0;
		blood = 7;			//blood 설정하기 
		time = 0;

		if (startMap == TRUE) {
			x[0] = 370; y[0] = 390;
			x[1] = 500; y[1] = 390;
			x[2] = 620; y[2] = 390;
			x[3] = 500; y[3] = 443;
			Selection[0] = FALSE;
			Selection[1] = FALSE;
			Selection[2] = FALSE;
			startButton = FALSE;
			
		}

		outcome = 0;

		break;

	case WM_TIMER:
		switch (wParam) {
		case 1://버프 및 게임 시간 타이머
			if ((time > 0) && (start == TRUE)) {
				time--;
				day = DayCount(firstTime, time);
				if ((time <= 0) && (blood > 0)) {
					start = FALSE;
					outcome = 2;
					xPos = 0;
					yPos = 600;
					
					InvalidateRgn(hwnd, NULL, TRUE);
					SetTimer(hwnd, 2, 1000, NULL);
				}

				else if ((time >= 0) && (blood <= 0)) {
					start = FALSE;
					outcome = 1;
					xPos = 0;
					yPos = 0;
					InvalidateRgn(hwnd, NULL, TRUE);
					SetTimer(hwnd, 2, 1000, NULL);
				}
				InvalidateRgn(hwnd, NULL, TRUE);
			}

			

			break;
		case 2:

			if (outcome == 2)
			{
				xPos += 10;
				yPos -= 10;
				InvalidateRgn(hwnd, NULL, TRUE);
			}
			else if (outcome == 1)
			{
				xPos += 10;
				yPos += 10;
				InvalidateRgn(hwnd, NULL, TRUE);
			}
				break;
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		
		if (startMap == TRUE) {
			if (Selection[0]) { 
				Rectangle(hdc, x[0] - BSIZE, y[0] - BSIZE, x[0] + BSIZE, y[0] + BSIZE);
			}
			if (Selection[1]) {
				Rectangle(hdc, x[1] - BSIZE, y[1] - BSIZE, x[1] + BSIZE, y[1] + BSIZE);
			}
			if (Selection[2]) {
				Rectangle(hdc, x[2] - BSIZE, y[2] - BSIZE, x[2] + BSIZE, y[2] + BSIZE);
			}
			if (startButton) {
				Rectangle(hdc, x[3]-110, y[3]-30, x[3]+110, y[3]+30);
			}
			}//확인하기 필요없음
	
		if (startMap == TRUE) { DrawStart(hdc, mode, startButton);}		//START맵 구현

		if ((start == TRUE)&&(startMap == FALSE)) {DrawMap(keyboard, blood, time, day, hdc);}		//맵 구현
		
		if ((start == FALSE) && (startMap == FALSE) && ((outcome == 1) || (outcome == 2))){DrawEnding(xPos, yPos, hdc, outcome);}		//엔딩 맵 구현
		
		
		EndPaint(hwnd, &ps);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDM_CHARINFO:
			break;
		}

	case WM_LBUTTONDOWN://왼쪽 마우스 클릭했을때
		mx = LOWORD(lParam);
		my = HIWORD(lParam);

		if (InCircle(x[0], y[0], mx, my) && (InCircle(x[1], y[1], mx, my)==FALSE) && (InCircle(x[2], y[2], mx, my)==FALSE)) {
			Selection[0] = TRUE;
			Selection[1] = FALSE;
			Selection[2] = FALSE;
			mode = 0;

			
			InvalidateRgn(hwnd, NULL, TRUE);
		}
		else if (InCircle(x[1], y[1], mx, my) && (InCircle(x[0], y[0], mx, my) == FALSE) && (InCircle(x[2], y[2], mx, my) == FALSE)) {
			Selection[0] = FALSE;
			Selection[1] = TRUE;
			Selection[2] = FALSE;
			mode = 1;
			
			InvalidateRgn(hwnd, NULL, TRUE);
		}
		else if (InCircle(x[2], y[2], mx, my) && (InCircle(x[0], y[0], mx, my) == FALSE) && (InCircle(x[1], y[1], mx, my) == FALSE)){
			Selection[0] = FALSE;
			Selection[1] = FALSE;
			Selection[2] = TRUE;
			mode = 2;
			
			InvalidateRgn(hwnd, NULL, TRUE);
		}
		
		if (InRect(x[3], y[3], mx, my)==TRUE && Selection[0] == TRUE) {
			startButton = TRUE;
		}
		else if (InRect(x[3], y[3], mx, my)==TRUE && Selection[1] == TRUE) {
			startButton = TRUE;
		}
		else if (InRect(x[3], y[3], mx, my)==TRUE && Selection[2] == TRUE) {
			startButton = TRUE;
		}


		break;

	case WM_KEYUP:
		keyboard = 0;
		InvalidateRgn(hwnd, NULL, TRUE);		//방향키 입력
		break;

	case WM_KEYDOWN:
		if (wParam == VK_RIGHT)keyboard = 1;
		if (wParam == VK_LEFT)keyboard = 2;

		if ((wParam == VK_RETURN) && (Selection[0] == TRUE) && (startButton==TRUE)) { 
			start = TRUE; 

			startButton == FALSE;
			Selection[0] == FALSE;
			startMap = FALSE;
			time = 100;			//시간 초기화
			firstTime = time;
			SetTimer(hwnd, 1, 1000, NULL);//1초마다
		InvalidateRgn(hwnd, NULL, TRUE);
		}
		else if ((wParam == VK_RETURN) && (Selection[1] == TRUE) && (startButton == TRUE)) {
			start = TRUE; 

			startButton == FALSE;
			Selection[1] == FALSE;
			startMap = FALSE;
			time = 100;
			firstTime = time;
			SetTimer(hwnd, 1, 1000, NULL);//1초마다
		InvalidateRgn(hwnd, NULL, TRUE);
		}
		else if ((wParam == VK_RETURN) && (Selection[2] == TRUE) && (startButton == TRUE)) {
			start = TRUE; 
			
			startButton == FALSE;
			Selection[2] == FALSE;
			startMap = FALSE;
			time = 100;
			firstTime = time;
			SetTimer(hwnd, 1, 1000, NULL);//1초마다
		InvalidateRgn(hwnd, NULL, TRUE);
		}
		
	

		break;

	case WM_DESTROY:
		
		KillTimer(hwnd, 1);
		KillTimer(hwnd, 2);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}