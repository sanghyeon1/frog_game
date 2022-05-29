#include <windows.h>
#include <TCHAR.H>
#include "resource.h"


LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

HINSTANCE hInst;

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
	WndClass.style = CS_HREDRAW | CS_VREDRAW;	//height, vertical redraw
	WndClass.lpfnWndProc = WndProc;		// Proc 설정
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);	// 형변환
	WndClass.lpszMenuName = MAKEINTRESOURCE(109);
	// resource.h 파일 에서 메뉴이름 확인 109번
	WndClass.lpszClassName = _T("Window Class Name");
	RegisterClass(&WndClass);		// WndClass 등록
	hwnd = CreateWindow(_T("Window Class Name"),
		_T("2018253004_박상현"),	// 타이틀바, 학번이름 체크
		WS_OVERLAPPEDWINDOW,		// 윈도우 스타일
		600, 400,					// 창출력좌표 x, y 
		600, 400,					// 창크기 x, y축
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

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg,
	WPARAM wParam, LPARAM lParam)
	//	WinDef.h 에서 정의
	//	wPram > unsigned ptr, lParam > long ptr 
{
	HDC hdc, memdc;
	PAINTSTRUCT ps;
	static HBITMAP hBitmap13, hBitmap46, hBitmap79, hBitmap1012, hBitmap_TONGUE_L1_L, hBitmap_TONGUE_L1_R,
		hBitmap_GameOver, hBitmap13B, hBitmap46B, hBitmap79B, hBitmap1012B, hBitmap_TONGUE_L1_LB,
		hBitmap_TONGUE_L1_RB, hBitmap_GameOverB;
	BITMAP bit;
	static HBITMAP old_bitmap, life_bitmap;
	static int x = 100, y = 200;
	static bool flag[5];
	static char direct = 'n', space = 'n', old_drect = 'n';
	static int xPos;

	static HBITMAP RunBit_L[3], RunBit_R[3], RunBit_LB[3], RunBit_RB[3];
	static int count, life = 1;
	int  i;
	int bx, by;




	switch (iMsg) {
	case WM_CREATE:
		RunBit_L[0] = (HBITMAP)LoadImage(NULL, TEXT("C:\\Users\\psh99\\source\\repos\\frog_movement_temp\\frog_movement_temp\\x64\\MOVE_1_BLACK2.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		RunBit_L[1] = (HBITMAP)LoadImage(NULL, TEXT("C:\\Users\\psh99\\source\\repos\\frog_movement_temp\\frog_movement_temp\\x64\\MOVE_2_BLACK2.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		RunBit_L[2] = (HBITMAP)LoadImage(NULL, TEXT("C:\\Users\\psh99\\source\\repos\\frog_movement_temp\\frog_movement_temp\\x64\\MOVE_3_BLACK2.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		RunBit_R[0] = (HBITMAP)LoadImage(NULL, TEXT("C:\\Users\\psh99\\source\\repos\\frog_movement_temp\\frog_movement_temp\\x64\\MOVE_1R_BLACK2.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		RunBit_R[1] = (HBITMAP)LoadImage(NULL, TEXT("C:\\Users\\psh99\\source\\repos\\frog_movement_temp\\frog_movement_temp\\x64\\MOVE_2R_BLACK2.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		RunBit_R[2] = (HBITMAP)LoadImage(NULL, TEXT("C:\\Users\\psh99\\source\\repos\\frog_movement_temp\\frog_movement_temp\\x64\\MOVE_3R_BLACK2.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

		RunBit_LB[0] = (HBITMAP)LoadImage(NULL, TEXT("C:\\Users\\psh99\\source\\repos\\frog_movement_temp\\frog_movement_temp\\x64\\MOVE_1_BLACK.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		RunBit_LB[1] = (HBITMAP)LoadImage(NULL, TEXT("C:\\Users\\psh99\\source\\repos\\frog_movement_temp\\frog_movement_temp\\x64\\MOVE_2_BLACK.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		RunBit_LB[2] = (HBITMAP)LoadImage(NULL, TEXT("C:\\Users\\psh99\\source\\repos\\frog_movement_temp\\frog_movement_temp\\x64\\MOVE_3_BLACK.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		RunBit_RB[0] = (HBITMAP)LoadImage(NULL, TEXT("C:\\Users\\psh99\\source\\repos\\frog_movement_temp\\frog_movement_temp\\x64\\MOVE_1R_BLACK.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		RunBit_RB[1] = (HBITMAP)LoadImage(NULL, TEXT("C:\\Users\\psh99\\source\\repos\\frog_movement_temp\\frog_movement_temp\\x64\\MOVE_2R_BLACK.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		RunBit_RB[2] = (HBITMAP)LoadImage(NULL, TEXT("C:\\Users\\psh99\\source\\repos\\frog_movement_temp\\frog_movement_temp\\x64\\MOVE_3R_BLACK.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

		hBitmap13 = (HBITMAP)LoadImage(NULL, TEXT("C:\\Users\\psh99\\source\\repos\\frog_movement_temp\\frog_movement_temp\\x64\\LIFE13_BLACK2.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		hBitmap46 = (HBITMAP)LoadImage(NULL, TEXT("C:\\Users\\psh99\\source\\repos\\frog_movement_temp\\frog_movement_temp\\x64\\LIFE46_BLACK2.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		hBitmap79 = (HBITMAP)LoadImage(NULL, TEXT("C:\\Users\\psh99\\source\\repos\\frog_movement_temp\\frog_movement_temp\\x64\\LIFE79_BLACK2.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		hBitmap1012 = (HBITMAP)LoadImage(NULL, TEXT("C:\\Users\\psh99\\source\\repos\\frog_movement_temp\\frog_movement_temp\\x64\\LIFE1012_BLACK2.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		hBitmap_TONGUE_L1_R = (HBITMAP)LoadImage(NULL, TEXT("C:\\Users\\psh99\\source\\repos\\frog_movement_temp\\frog_movement_temp\\x64\\TONGUE_L1_R_BLACK2.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		hBitmap_TONGUE_L1_L = (HBITMAP)LoadImage(NULL, TEXT("C:\\Users\\psh99\\source\\repos\\frog_movement_temp\\frog_movement_temp\\x64\\TONGUE_L1_L_BLACK2.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		hBitmap_GameOver = (HBITMAP)LoadImage(NULL, TEXT("C:\\Users\\psh99\\source\\repos\\frog_movement_temp\\frog_movement_temp\\x64\\GAMEOVER_BLACK2.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

		hBitmap13B = (HBITMAP)LoadImage(NULL, TEXT("C:\\Users\\psh99\\source\\repos\\frog_movement_temp\\frog_movement_temp\\x64\\LIFE13_BLACK.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		hBitmap46B = (HBITMAP)LoadImage(NULL, TEXT("C:\\Users\\psh99\\source\\repos\\frog_movement_temp\\frog_movement_temp\\x64\\LIFE46_BLACK.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		hBitmap79B = (HBITMAP)LoadImage(NULL, TEXT("C:\\Users\\psh99\\source\\repos\\frog_movement_temp\\frog_movement_temp\\x64\\LIFE79_BLACK.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		hBitmap1012B = (HBITMAP)LoadImage(NULL, TEXT("C:\\Users\\psh99\\source\\repos\\frog_movement_temp\\frog_movement_temp\\x64\\LIFE1012_BLACK.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		hBitmap_TONGUE_L1_RB = (HBITMAP)LoadImage(NULL, TEXT("C:\\Users\\psh99\\source\\repos\\frog_movement_temp\\frog_movement_temp\\x64\\TONGUE_L1_R_BLACK.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		hBitmap_TONGUE_L1_LB = (HBITMAP)LoadImage(NULL, TEXT("C:\\Users\\psh99\\source\\repos\\frog_movement_temp\\frog_movement_temp\\x64\\TONGUE_L1_L_BLACK.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		hBitmap_GameOverB = (HBITMAP)LoadImage(NULL, TEXT("C:\\Users\\psh99\\source\\repos\\frog_movement_temp\\frog_movement_temp\\x64\\GAMEOVER_BLACK.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		break;
	case WM_TIMER:
		if (direct == 'L') {
			x += 10;
		}
		InvalidateRgn(hwnd, NULL, true);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		memdc = CreateCompatibleDC(hdc);

		// 생명력에 따라 다른 비트맵 이미지 출력.
		if (life >= 1 && life <= 3)
		{
			SelectObject(memdc, hBitmap13);
		}
		else if (life >= 4 && life <= 6)
			SelectObject(memdc, hBitmap46);
		else if (life >= 7 && life <= 9)
			SelectObject(memdc, hBitmap79);
		else if (life >= 10 && life <= 12)
			SelectObject(memdc, hBitmap1012);
		else
			SelectObject(memdc, hBitmap_GameOver);

		// 방향키 좌측일 때와 우측일 때 비트맵 출력 두 조건 모두 실행될 필요가 있어서
		// if로만 구현.
		if (direct == 'L') {
			old_drect = 'L';


			GetObject(RunBit_L[count], sizeof(BITMAP), &bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;
			SelectObject(memdc, RunBit_L[count]);
			StretchBlt(hdc, x, y, 55, 55, memdc, 0, 0, bx, by, SRCAND);
			SelectObject(memdc, RunBit_LB[count]);
			StretchBlt(hdc, x, y, 55, 55, memdc, 0, 0, bx, by, SRCPAINT);
			//dest, w/h, src
			DeleteDC(memdc);
			count++;
			count = count % 3;
		}
		if (direct == 'R') {
			old_drect = 'R';
			GetObject(RunBit_R[count], sizeof(BITMAP), &bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;
			SelectObject(memdc, RunBit_R[count]);
			StretchBlt(hdc, x, y, 55, 55, memdc, 0, 0, bx, by, SRCAND);
			SelectObject(memdc, RunBit_RB[count]);
			StretchBlt(hdc, x, y, 55, 55, memdc, 0, 0, bx, by, SRCPAINT);
			//dest, w/h, src
			DeleteDC(memdc);
			count++;
			count = count % 3;
		}

		// 스페이스바가 눌렸을 때 좌, 우측에 따른 비트맵 출력.
		if (space == 'O') {
			if (old_drect == 'L') {
				GetObject(hBitmap_TONGUE_L1_L, sizeof(BITMAP), &bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;
				SelectObject(memdc, hBitmap_TONGUE_L1_L);
				StretchBlt(hdc, x - 80, y, 140, 55, memdc, 0, 0, bx, by, SRCAND);
				SelectObject(memdc, hBitmap_TONGUE_L1_LB);
				StretchBlt(hdc, x - 80, y, 140, 55, memdc, 0, 0, bx, by, SRCPAINT);
				//dest, w/h, src
				DeleteDC(memdc);
			}
			else if (old_drect == 'R') {
				GetObject(hBitmap_TONGUE_L1_R, sizeof(BITMAP), &bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;
				SelectObject(memdc, hBitmap_TONGUE_L1_R);
				StretchBlt(hdc, x, y, 140, 55, memdc, 0, 0, bx, by, SRCAND);
				SelectObject(memdc, hBitmap_TONGUE_L1_RB);
				StretchBlt(hdc, x, y, 140, 55, memdc, 0, 0, bx, by, SRCPAINT);
				//dest, w/h, src
				DeleteDC(memdc);
			}
		}
		GetObject(hBitmap13B, sizeof(BITMAP), &bit);
		bx = bit.bmWidth;
		by = bit.bmHeight;
		SelectObject(memdc, hBitmap13);
		StretchBlt(hdc, x, y, 55, 55, memdc, 0, 0, bx, by, SRCAND);
		SelectObject(memdc, hBitmap13B);
		StretchBlt(hdc, x, y, 55, 55, memdc, 0, 0, bx, by, SRCPAINT);
		DeleteDC(memdc);
		EndPaint(hwnd, &ps);
		break;
	case WM_KEYDOWN:	// 키를 눌렀을 때.
		switch (wParam) {
		case VK_LEFT:
			direct = 'L';
			space = 'x';
			x -= 10;
			break;

		case VK_RIGHT:
			direct = 'R';
			space = 'x';
			x += 10;
			break;

		case VK_SPACE:
			space = 'O';

			
			break;
		}
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_KEYUP:		//키를 떼었을 때.
		switch (wParam) {
		case VK_SPACE:
			space = 'X';
			break;
		case VK_LEFT:
			direct = 'n';
			break;
		case VK_RIGHT:
			direct = 'n';
			break;
		}
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
