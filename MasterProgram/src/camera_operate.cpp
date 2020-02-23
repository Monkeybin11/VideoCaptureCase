#include "camera_operate.h"
//#include "windows.h"
using namespace System;
using namespace cv;

#using <System.Windows.Forms.dll>
using namespace System::Windows::Forms;

// Detect devices connected to this computer, return the number of connected devices. 
int Camera::CameraOperate::DeviceDetect()
{
    rs2::context ctx;

    auto list = ctx.query_devices();

    return (list.size());
}
// Detect sensors connected to this computer, return the number of connected devices. 
int Camera::CameraOperate::SensorDetect()
{
    rs2::context ctx;

    auto list = ctx.query_all_sensors();

    return (list.size());
}
// 启动相机
void Camera::CameraOperate::CameraStart(int ImageWidth, int ImageHeight, int FPS)
{
    // rs2::config
    bool device_detect = DeviceDetect();
    bool sensor_detect = SensorDetect();
    if ((device_detect == false)||(sensor_detect == false))
    {
        MessageBox::Show("未检测到相机设备，请检查硬件连接是否正常！");
        return;
    }
    rs2::config config;
    rs2::pipeline pipe;
    rs2::pipeline_profile pipe_profile = NULL;
    // 根据要求设置不同数据流的使能情况
    config.enable_stream(RS2_STREAM_INFRARED, 1, ImageWidth, ImageHeight, RS2_FORMAT_Y8, FPS);
    config.enable_stream(RS2_STREAM_INFRARED, 2, ImageWidth, ImageHeight, RS2_FORMAT_Y8, FPS);
    // start pipeline with config
    pipe_profile = pipe.start(config);

    rs2::device selected_device = pipe_profile.get_device();    // select the started device
    auto depth_sensor = selected_device.first<rs2::depth_sensor>();

    while ((waitKey(1) < 0) && flag_camera_opened)
    {
        if (flag_image_display)
        {
            rs2::colorizer color_map;
            rs2::frameset frames = pipe.wait_for_frames(); // Wait for next set of frames from the camera
            rs2::frame frame_left = frames.get_infrared_frame(1);
            rs2::frame frame_right = frames.get_infrared_frame(2);
            //rs2::frame frame_depth = frames.get_depth_frame().apply_filter(color_map);
            //rs2::frame frame_depth = frames.get_depth_frame();
            //rs2::frame frame_color = frames.get_color_frame();

            CurrentFrameLeft = cv::Mat(Size(ImageWidth, ImageHeight), CV_8UC1, (void*)frame_left.get_data());
            CurrentFrameRight = cv::Mat(Size(ImageWidth, ImageHeight), CV_8UC1, (void*)frame_right.get_data());

            //_ptr_CurrentFrameLeft = &mat_left;
            const auto window_img_left = "WindowImageLeft";
            cv::imshow(window_img_left, (CurrentFrameLeft));
            //destroyWindow(window_img_left);
        }
        if (flag_camera_close)
        {
            pipe.stop();
            flag_camera_opened = false;
        }
    }

}

//Mat GetFrameMat(int num)
//{
//    rs2::colorizer color_map;
//    rs2::frameset frames = pipe.wait_for_frames(); // Wait for next set of frames from the camera
//    rs2::frame frame_left = frames.get_infrared_frame(1);
//    rs2::frame frame_right = frames.get_infrared_frame(2);
//    //rs2::frame frame_depth = frames.get_depth_frame().apply_filter(color_map);
//    //rs2::frame frame_depth = frames.get_depth_frame();
//    //rs2::frame frame_color = frames.get_color_frame();
//
//    CurrentFrameLeft = cv::Mat(Size(ImageWidth, ImageHeight), CV_8UC1, (void*)frame_left.get_data());
//    CurrentFrameRight = cv::Mat(Size(ImageWidth, ImageHeight), CV_8UC1, (void*)frame_right.get_data());
//}


// 
bool Camera::CameraOperate::CameraInitialAndImshow(int ImageWidth, int ImageHeight, int FPS)
{
    // rs2::config
    rs2::config config;
    // 
    config.enable_stream(RS2_STREAM_INFRARED, 1, ImageWidth, ImageHeight, RS2_FORMAT_Y8, FPS);
    config.enable_stream(RS2_STREAM_INFRARED, 2, ImageWidth, ImageHeight, RS2_FORMAT_Y8, FPS);
    config.enable_stream(RS2_STREAM_DEPTH, ImageWidth, ImageHeight, RS2_FORMAT_Z16, FPS);
    config.enable_stream(RS2_STREAM_COLOR, ImageWidth, ImageHeight, RS2_FORMAT_RGB8, FPS);
    // start pipeline with config
    rs2::pipeline pipe;
    rs2::pipeline_profile pipe_line_profile = NULL;
    pipe_line_profile = pipe.start(config);

    rs2::device selected_device = pipe_line_profile.get_device();    // select the started device
    auto depth_sensor = selected_device.first<rs2::depth_sensor>();
    // 
    if (depth_sensor.supports(RS2_OPTION_EMITTER_ENABLED))
    {
        depth_sensor.set_option(RS2_OPTION_EMITTER_ENABLED, 0); // Disable emitter
    }

    if (pipe_line_profile == NULL)
    {
        // 
        MessageBox::Show("设备启动失败");
        flag_camera_opened = false;
        flag_camera_opened = !flag_camera_opened;
        Width = 0;
        Height = 0;
        FPS = 0;
    }
    else
    {
        //
        MessageBox::Show("启动成功");
        flag_camera_opened = true;
        flag_camera_close = !flag_camera_opened;
        Width = ImageWidth;
        Height = ImageHeight;
        FPS = FPS;

        // 
        while ((waitKey(1) < 0) && flag_camera_opened)
        {
            if (flag_image_display)
            {
                rs2::colorizer color_map;
                rs2::frameset frames = pipe.wait_for_frames(); // Wait for next set of frames from the camera
                rs2::frame frame_left = frames.get_infrared_frame(1);
                rs2::frame frame_right = frames.get_infrared_frame(2);
                //rs2::frame frame_depth = frames.get_depth_frame().apply_filter(color_map);
                //rs2::frame frame_depth = frames.get_depth_frame();
                //rs2::frame frame_color = frames.get_color_frame();

                CurrentFrameLeft = cv::Mat(Size(ImageWidth, ImageHeight), CV_8UC1, (void*)frame_left.get_data());
                CurrentFrameRight = cv::Mat(Size(ImageWidth, ImageHeight), CV_8UC1, (void*)frame_right.get_data());

                //_ptr_CurrentFrameLeft = &mat_left;
                const auto window_img_left = "WindowImageLeft";
                cv::imshow(window_img_left, (CurrentFrameLeft));
                //destroyWindow(window_img_left);
            }
            if (flag_camera_close)
            {
                pipe.stop();
                flag_camera_opened = false;
            }
        }
    }
    return flag_camera_opened;
}

uchar* Camera::CameraOperate::GetFrameData(int num)
{
    //IntPtr _Mat_data = (IntPtr) _ptr_CurrentFrameLeft->data;
    if (num == 1)
        return CurrentFrameLeft.data;
    else if (num == 2)
        return CurrentFrameRight.data;
    else if (num == 3)
        return CurrentFrameDepth.data;
    else if (num == 4)
        return CurrentFrameColor.data;
    else
        return CurrentFrameLeft.data;
}
int Camera::CameraOperate::GetFrameStep(int num)
{
    if (num == 1)
        return CurrentFrameLeft.step;
    else if (num == 2)
        return CurrentFrameRight.step;
    else if (num == 3)
        return CurrentFrameDepth.step;
    else if (num == 4)
        return CurrentFrameColor.step;
    else
        return 0;
}

