
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
        "{ d  datasetFolder      | /home/beahacker/Public/data/video/ssbd | }"
        "{ a  annotationFolder   | /home/beahacker/Desktop/workspace/BoW_frameworks/dense_trajectories_BoW/ssbd-release/Annotations | }"
        "{ o  outputFolder       | /home/beahacker/Public/data/video/ssbd_avi | }"
    };

    CommandLineParser cmd(argc, argv, keys);
    string datasetFolder = cmd.get<string>("datasetFolder");
    string annotationFolder = cmd.get<string>("annotationFolder");
    string outputFolder = cmd.get<string>("outputFolder");

    cout << "OpenCV version: " << CV_VERSION << endl;

    // read action names of ssbd
    string str;
    vector<string> actions;
    fs::path filename(datasetFolder);
    filename.append("classes.txt");         // equivalent to fullfile in matlab
    ifstream infile(filename.c_str());
    if (infile.is_open())
        cout << "The file is open!" << endl;
    else
        exit(1);
    while (getline(infile, str))
    {
        actions.push_back(str);
    }
    infile.close();

    // reading filenames in a folder
    int cnt = 0, t1, t2, dur;
    char num[10];
    for (int ind = 0; ind < actions.size(); ind++)
    {
        fs::path actionFolder(datasetFolder);
        actionFolder.append(actions[ind]);

        int numFiles = 0;
        fs::directory_iterator end_itr;     // the end iterator
        for (fs::directory_iterator itr(actionFolder); itr != end_itr; itr++)
        {
            if (fs::is_regular_file(itr->status()))
                numFiles++;
        }
        cout << "total number of files in folder: " << numFiles << endl;

        // reading annotations;
        for (int i = 1; i <= numFiles; i++) {
            // xml file name
            sprintf(num, "_%02d.xml", i);
            string xml_file = "v_" + actions[ind];
            xml_file.append(num);
            fs::path xml_path(annotationFolder);
            xml_path.append(xml_file);      // final path
            //cout << xml_path.string() << endl;

            // read xml file
            ptree pt;
            read_xml(xml_path.string(), pt);

//            // traverse pt
//            BOOST_FOREACH(ptree::value_type const& v, pt.get_child("video.behaviours.behaviour"))
//                    if (v.first == "time") {
//                string time = v.second.data();
//                cout << "time of action: " << time << endl;
//            }

            // get first tag <behaviour> in <behaviours>, we don't need to traverse
            // if you understand it is a little efforts. Think and read before coding. Pls look at god and smile it.
            string time = pt.get<string>("video.behaviours.behaviour.time");
            sscanf(time.c_str(), "%d:%d", &t1, &t2);
            string duration = pt.get<string>("video.duration");
            sscanf(duration.c_str(), "%ds", &dur);
            cout << "time of action " << t1 << ":" << t2 << " in video of " << dur << "s" << endl;

            // im happy!!! happy coding hours
            // get information of a video.
            cnt++;
            fs::path vid_path(datasetFolder);
            vid_path.append(actions[ind]);
            vid_path.append(to_string(cnt) + ".avi");
            VideoCapture inputVideo(vid_path.c_str());

            double numFrames = inputVideo.get(CV_CAP_PROP_FRAME_COUNT);
            float fps = numFrames/static_cast<float>(dur);      // calc fps because sometimes fps is not calculated correctly
            int start = cvFloor(fps * (t1/100*60 + t1%100));
            int end = cvFloor(fps * (t2/100*60 + t2%100));

            // trimming videos
            fs::path outvid_path(outputFolder);
            outvid_path.append(actions[ind]);
            if (!fs::is_directory(outvid_path))
                fs::create_directory(outvid_path);
            string video_file = "v_" + actions[ind];
            sprintf(num, "_%02d.avi", i);
            video_file.append(num);
            outvid_path.append(video_file);

            // setting parameters for output video
            int ex = static_cast<int>(inputVideo.get(CAP_PROP_FOURCC));     // Get Codec Type- Int form
            Size S = Size(inputVideo.get(CV_CAP_PROP_FRAME_WIDTH), inputVideo.get(CV_CAP_PROP_FRAME_HEIGHT));
            VideoWriter outputVideo(outvid_path.c_str(), ex, fps, S);
            if (!outputVideo.isOpened()) {
                cout << "Could not open the output video for write" << endl;
                return -1;
            }

            Mat img;
            int fr = 0;
            for (;;)
            {
                inputVideo >> img;
                if (img.empty()) break;
                fr++;
                if (fr < start || fr > end) continue;
                outputVideo << img;
            }
        }
    }
}