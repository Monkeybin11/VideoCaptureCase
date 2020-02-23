#include <iostream>
#include <thread>
#include "main.h"


#using "PresentationFramework.dll"
#using "PresentationCore.dll"
#using "WindowsBase.dll"
#using "System.dll"
using namespace std;
using namespace WpfUILibrary;

[System::STAThread]
int main()
{
	// 首先打开相机

	Camera::CameraOperate CameraPart;
	//std::thread thread_CameraPart(&Camera::CameraOperate::CameraInitialAndImshow, &CameraPart, 640, 360, 30);
	//thread_CameraPart.join();
	CameraPart.CameraInitialAndImshow(640, 360, 30);

	WpfInitial();
	//system("pause");
	return 0;
}

void WpfInitial()
{
	MainWindow^ win = gcnew MainWindow;
	win->ShowDialog();
}