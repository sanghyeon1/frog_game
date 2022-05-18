#include <windows.h>
#include <TCHAR.H>
#include "resource.h"

void move_ani(int xPos, int yPos, HDC hdc);

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
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	// 형변환
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
	static HBITMAP hBitmap46, hBitmap79;
	static int x = 100, y = 200;
	static bool flag[5];
	static char direct = 'n';
	static int xPos;

	static HBITMAP RunBit_L[3], RunBit_R[3];
	static int count;
	int  i;

	

	switch (iMsg) {
	case WM_CREATE:
		RunBit_L[0] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(MOVE_1));
		RunBit_L[1] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(MOVE_2));
		RunBit_L[2] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(MOVE_3));
		RunBit_R[0] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(MOVE_1R));
		RunBit_R[1] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(MOVE_2R));
		RunBit_R[2] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(MOVE_3R));

		hBitmap46 = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
			MAKEINTRESOURCE(LIFE46));
		hBitmap79 = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
			MAKEINTRESOURCE(LIFE79));
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
		SelectObject(memdc, hBitmap79);
		if (direct == 'L') {			
			SelectObject(memdc, RunBit_L[count]);
			StretchBlt(hdc, x, y, 600, 600, memdc, 0, 0, 1300, 1300, SRCCOPY); //dest, w/h, src
			DeleteDC(memdc);
			count++;
			count = count % 3;
			
		}			
		else if (direct == 'R') {
			SelectObject(memdc, RunBit_R[count]);
			StretchBlt(hdc, x, y, 600, 600, memdc, 0, 0, 1300, 1300, SRCCOPY); //dest, w/h, src
			DeleteDC(memdc);
			count++;
			count = count % 3;
		}
		StretchBlt(hdc, x, y, 600, 600, memdc, 0, 0, 1300, 1300, SRCCOPY); //dest, w/h, src
		DeleteDC(memdc);
		EndPaint(hwnd, &ps);
		break;
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_LEFT:
			direct = 'L';
			x -= 10;
			
			break;
		case VK_RIGHT:
			direct = 'R';
			x += 10;
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
