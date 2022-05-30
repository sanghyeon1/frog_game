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

int life = 5;

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
	static int count;
	int  i;
	int bx, by;




	switch (iMsg) {
	case WM_CREATE:
		RunBit_L[0] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_MOVE_1L_B));
		RunBit_L[1] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_MOVE_2L_B));
		RunBit_L[2] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_MOVE_3L_B));
		RunBit_R[0] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_MOVE_1R_B));
		RunBit_R[1] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_MOVE_2R_B));;
		RunBit_R[2] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_MOVE_3R_B));

		RunBit_LB[0] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_MOVE_1L));
		RunBit_LB[1] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_MOVE_2L));
		RunBit_LB[2] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_MOVE_3L));
		RunBit_RB[0] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_MOVE_1R));
		RunBit_RB[1] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_MOVE_2R));
		RunBit_RB[2] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_MOVE_3R));

		hBitmap13 = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_LIFE13_B));
		hBitmap46 = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_LIFE46_B));
		hBitmap79 = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_LIFE79_B));
		hBitmap1012 = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_LIFE1012_B));
		hBitmap_TONGUE_L1_R = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_TONGUE_R1_B));
		hBitmap_TONGUE_L1_L = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_TONGUE_L1_B));
		hBitmap_GameOver = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_GameOver));

		hBitmap13B = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_LIFE13));
		hBitmap46B = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_LIFE46));
		hBitmap79B = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_LIFE79));
		hBitmap1012B = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_LIFE1012));
		hBitmap_TONGUE_L1_RB = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_TONGUE_R1));
		hBitmap_TONGUE_L1_LB = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_TONGUE_L1));
		hBitmap_GameOverB = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_GameOver_B));
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
		if (life >= 1 && life <= 3) {
			GetObject(hBitmap13, sizeof(BITMAP), &bit);
			SelectObject(memdc, hBitmap13);
		}			
		else if (life >= 4 && life <= 6) {
			GetObject(hBitmap46, sizeof(BITMAP), &bit);
			SelectObject(memdc, hBitmap46);
		}
		else if (life >= 7 && life <= 9) {
			GetObject(hBitmap79, sizeof(BITMAP), &bit);
			SelectObject(memdc, hBitmap79);
		}			
		else if (life >= 10 && life <= 12) {
			GetObject(hBitmap1012, sizeof(BITMAP), &bit);
			SelectObject(memdc, hBitmap1012);
		}			
		else {
			GetObject(hBitmap_GameOver, sizeof(BITMAP), &bit);
			SelectObject(memdc, hBitmap_GameOver);
		}
			

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
		
		bx = bit.bmWidth;
		by = bit.bmHeight;
		
		if (life >= 1 && life <= 3) {
			StretchBlt(hdc, x, y, 55, 55, memdc, 0, 0, bx, by, SRCAND);
			GetObject(hBitmap13B, sizeof(BITMAP), &bit);
			SelectObject(memdc, hBitmap13B);
			StretchBlt(hdc, x, y, 55, 55, memdc, 0, 0, bx, by, SRCPAINT);
		}

		else if (life >= 4 && life <= 6) {
			StretchBlt(hdc, x, y, 55, 55, memdc, 0, 0, bx, by, SRCAND);
			GetObject(hBitmap46B, sizeof(BITMAP), &bit);
			SelectObject(memdc, hBitmap46B);
			StretchBlt(hdc, x, y, 55, 55, memdc, 0, 0, bx, by, SRCPAINT);
		}
		else if (life >= 7 && life <= 9) {
			StretchBlt(hdc, x, y, 60, 55, memdc, 0, 0, bx, by, SRCAND);
			GetObject(hBitmap79B, sizeof(BITMAP), &bit);
			SelectObject(memdc, hBitmap79B);
			StretchBlt(hdc, x, y, 60, 55, memdc, 0, 0, bx, by, SRCPAINT);
		}

		else if (life >= 10 && life <= 12) {
			StretchBlt(hdc, x, y, 65, 60, memdc, 0, 0, bx, by, SRCAND);
			GetObject(hBitmap1012B, sizeof(BITMAP), &bit);
			SelectObject(memdc, hBitmap1012B);
			StretchBlt(hdc, x, y, 65, 60, memdc, 0, 0, bx, by, SRCPAINT);
		}
		else {
			StretchBlt(hdc, x, y, 80, 80, memdc, 0, 0, bx, by, SRCAND);
			GetObject(hBitmap_GameOverB, sizeof(BITMAP), &bit);
			SelectObject(memdc, hBitmap_GameOverB);
			StretchBlt(hdc, x, y, 80, 80, memdc, 0, 0, bx, by, SRCPAINT);
		}
		
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
			life += 1;
			
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

void collision_check(int item_index) {
	switch (item_index) {

	case 0:
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		break;
	case 6:
		break;
	case 7:
		break;
	case 8:
		break;
	case 9:
		break;
	}
}