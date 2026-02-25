#pragma once

class GraphicsEngine;
extern HWND			g_hWnd ;				//ウィンドウハンドル。

//ゲームの初期化。
void InitGame(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow, const TCHAR* appName);
//ウィンドウメッセージをディスパッチ。falseが返ってきたら、ゲーム終了。
bool DispatchWindowMessage();


// ゲームループ用クラス。
class GameLoop
{
public:
    bool isLoop_ = true; // ゲームループ中かどうか。

};

// ゲームループのインスタンス。
extern GameLoop g_gameLoop_;

