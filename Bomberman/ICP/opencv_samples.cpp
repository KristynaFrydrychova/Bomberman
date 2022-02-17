// shorter code (slightly slower)
#include <numeric>

...

	std::vector<cv::Point> whitePixels;
	cv::findNonZero(scene, whitePixels);
	size_t cnt_white = whitePixels.size();
    cv::Point acc_white = std::accumulate(whitePixels.begin(), whitePixels.end(), cv::Point(0.0,0.0));

    if (cnt_white > 0)
  		average = acc_white / cnt_white;
  	else
  		average = { 0,0 };

...