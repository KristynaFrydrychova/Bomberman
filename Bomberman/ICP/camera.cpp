// icp.cpp 
// Author: JJ

typedef struct s_globals {
	cv::VideoCapture capture;
} s_globals;

s_globals globals;

//=====================================================================================================

static void init(void)
{
	globals.capture = cv::VideoCapture(cv::CAP_DSHOW);

	if (!globals.capture.isOpened())
	{ 
		std::cerr << "no camera" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
	{
		std::cout << "Camera " << 
			": width=" << globals.capture.get(cv::CAP_PROP_FRAME_WIDTH) <<
			", height=" << globals.capture.get(cv::CAP_PROP_FRAME_HEIGHT) <<
			", FPS=" << globals.capture.get(cv::CAP_PROP_FPS) << std::endl;
	}

	if (!globals.capture.set(cv::CAP_PROP_FRAME_WIDTH, 640))
		std::cout << "Failed width." << std::endl;
	if (!globals.capture.set(cv::CAP_PROP_FRAME_HEIGHT, 480))
		std::cout << "Failed height." << std::endl;
	if (!globals.capture.set(cv::CAP_PROP_FPS, 30))
		std::cout << "Failed FPS." << std::endl;

	std::cout << "Camera changed:" <<
		": width=" << globals.capture.get(cv::CAP_PROP_FRAME_WIDTH) <<
		", height=" << globals.capture.get(cv::CAP_PROP_FRAME_HEIGHT) <<
		", FPS=" << globals.capture.get(cv::CAP_PROP_FPS) << std::endl;
}

int main(int argc, char * argv[])
{
	cv::Mat frame, scene;
	double h_low = 128.0;
	double s_low = 128.0;
	double v_low = 128.0;
	double h_hi = 255.0;
	double s_hi = 255.0;
	double v_hi = 255.0;

	init();

	while (1)
	{
		globals.capture.read(frame);
		if (frame.empty())
		{
			std::cerr << "Cam disconnected?" << std::endl;
			break;
		}

		cv::imshow("grabbed", frame);

		// analyze the image...
		// ...
		cv::Mat scene_hsv, scene_threshold;

		cv::cvtColor(frame, scene_hsv, cv::COLOR_BGR2HSV);

		cv::Scalar lower_threshold = cv::Scalar(h_low, s_low, v_low);
		cv::Scalar upper_threshold = cv::Scalar(h_hi, s_hi, v_hi);
		cv::inRange(scene_hsv, lower_threshold, upper_threshold, scene_threshold);

		cv::imshow("threshold", scene_threshold);

		if (cv::waitKey(1) == 27)
			break;
	}

	if (globals.capture.isOpened())
		globals.capture.release();

	exit(EXIT_SUCCESS);
}

