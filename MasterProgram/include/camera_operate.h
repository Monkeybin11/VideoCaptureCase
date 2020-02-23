#pragma once

using namespace System;
using namespace System::Collections::Generic;
#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include <opencv2/opencv.hpp>   // Include OpenCV API

using namespace cv;

namespace Camera {
	public class CameraOperate
	{
	private:
		
	public:
		const int NUM_FRAME_LEFT = 1;
		const int NUM_FRAME_RIGHT = 2;
		const int NUM_FRAME_DEPTH = 3;
		const int NUM_FRAME_COLOR = 4;
		// ������Ĭ�ϲ���
		int Width = 640;
		int Height = 360;
		int FPS = 30;
		bool Emmiter = true;

		// ��¼����Ƿ��Ѿ��ɹ������ı�־
		bool flag_camera_opened;
		bool flag_image_display;
		bool flag_camera_close;

		// ��ǰ֡�Ĵ������
		cv::Mat CurrentFrameLeft;
		cv::Mat CurrentFrameRight;
		cv::Mat CurrentFrameDepth;
		cv::Mat CurrentFrameColor;

		CameraOperate()
		{
			flag_camera_opened = false;
			flag_image_display = true;
			flag_camera_close = !flag_camera_opened;
		}
		~CameraOperate()
		{
		}

		// Detect devices connected to this computer, return the number of connected devices. 
		int DeviceDetect();
		// Detect sensors connected to this computer, return the number of connected devices. 
		int SensorDetect();
		// �����ʼ��
		void CameraStart(int ImageWidth, int ImageHeight, int FPS);
		// �����ʼ��������ʼ�����չʾͼ��
		bool CameraInitialAndImshow(int ImageWidth, int ImageHeight, int FPS);
		uchar* GetFrameData(int num);
		int GetFrameStep(int num);

		//Mat GetFrameMat(int num);

	};
}
