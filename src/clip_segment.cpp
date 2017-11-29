
/**
    Try to use opencv and boost to clip ssbd videos.
    Using  boost::property_tree::ptree to parse xml files.
    Tutorial at https://akrzemi1.wordpress.com/2011/07/13/parsing-xml-with-boost/.
  */
#include <opencv2/video.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>

using namespace std;
using namespace cv;
namespace fs = boost::filesystem;
using namespace boost::property_tree;

int main(int argc, char** argv)
{
    // IO operation
    const char* keys =
    {
        "{ i  inputVideo      | /home/beahacker/Public/data/video/ssbd | }"
        "{ t  time_segment    | /home/beahacker/Desktop/workspace/BoW_frameworks/dense_trajectories_BoW/ssbd-release/Annotations | }"
        "{ o  outputFile       | /home/beahacker/Public/data/video/ssbd_avi | }"
        "{ d  duration         |  | }"
        "{ h  new_height       | 0  | new height of images and flows}"
        "{ w  new_width        | 0  | new width of images and flows}"
        "{ m  max_num        | 200  | maximum number of extracted frames}"
    };

    CommandLineParser cmd(argc, argv, keys);
    string input = cmd.get<string>("inputVideo");
    string time_segment = cmd.get<string>("time_segment");
    string outputFile = cmd.get<string>("outputFile");
    int new_height = cmd.get<int>("new_height");
    int new_width = cmd.get<int>("new_width");
    string duration = cmd.get<string>("duration");
    int max_num_frames = cmd.get<int>("max_num");

    cout << "OpenCV version: " << CV_VERSION << endl;

    // reading filenames in a folder
    int t1, t2, dur;
    sscanf(time_segment.c_str(), "%d:%d", &t1, &t2);
    sscanf(duration.c_str(), "%ds", & dur);

    VideoCapture inputVideo(input.c_str());

    double numFrames = inputVideo.get(CV_CAP_PROP_FRAME_COUNT);
    float fps = numFrames/static_cast<float>(dur);      // calc fps because sometimes fps is not retrieved correctly
    int start = cvFloor(fps * (t1/100*60 + t1%100));
    int end = cvFloor(fps * (t2/100*60 + t2%100));

    // setting parameters for output video
    int ex = static_cast<int>(inputVideo.get(CAP_PROP_FOURCC));     // Get Codec Type- Int form
    new_height = (new_height > 0) ? new_height : inputVideo.get(CV_CAP_PROP_FRAME_HEIGHT);
    new_width = (new_width > 0) ? new_width : inputVideo.get(CV_CAP_PROP_FRAME_WIDTH);
    cv::Size new_size(new_width, new_height);
    VideoWriter outputVideo(outputFile.c_str(), ex, fps, new_size);
    if (!outputVideo.isOpened()) {
        cout << "Could not open the output video for write" << endl;
        return -1;
    }

    Mat img;
    int fr = 0;
    int num = 0;
    for (;;)
    {
        inputVideo >> img;
        if (img.empty()) break;
        fr++;
        if (fr < start || fr > end) continue;
        resize(img, img, new_size);
        outputVideo << img;
        num++;
        if (num >= max_num_frames)
            break;
    }
}
