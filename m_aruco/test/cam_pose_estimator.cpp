#include "aruco.h"
#include "cvdrawingutils.h"
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <sys/shm.h>
#include <string.h>

using namespace std;
using namespace cv;
using namespace aruco;

MarkerDetector MDetector;
VideoCapture TheVideoCapturer;
vector<Marker> TheMarkers;
Mat TheInputImage,TheInputImageGrey, TheInputImageCopy;
CameraParameters TheCameraParameters;

int waitTime = 0;
int ref_id = 0;
bool isVideo=false;
class CmdLineParser{int argc;char** argv;public:CmdLineParser(int _argc, char** _argv): argc(_argc), argv(_argv){}   bool operator[](string param)    {int idx = -1;  for (int i = 0; i < argc && idx == -1; i++)if (string(argv[i]) == param)idx = i;return (idx != -1);}    string operator()(string param, string defvalue = "-1")    {int idx = -1;for (int i = 0; i < argc && idx == -1; i++)if (string(argv[i]) == param)idx = i;if (idx == -1)return defvalue;else return (argv[idx + 1]);}};
struct TimerAvrg{std::vector<double> times;size_t curr=0,n; std::chrono::high_resolution_clock::time_point begin,end;   TimerAvrg(int _n=30){n=_n;times.reserve(n);   }inline void start(){begin= std::chrono::high_resolution_clock::now();    }inline void stop(){end= std::chrono::high_resolution_clock::now();double duration=double(std::chrono::duration_cast<std::chrono::microseconds>(end-begin).count())*1e-6;if ( times.size()<n) times.push_back(duration);else{ times[curr]=duration; curr++;if (curr>=times.size()) curr=0;}}double getAvrg(){double sum=0;for(auto t:times) sum+=t;return sum/double(times.size());}};

TimerAvrg Fps;
char cam_pose[100];
char cam_vect[100];

void putText(cv::Mat &im,string text,cv::Point p,float size){
    float fact=float(im.cols)/float(640);
    if (fact<1) fact=1;
    cv::putText(im,text,p,FONT_HERSHEY_SIMPLEX, size,cv::Scalar(0,0,0),3*fact);
    cv::putText(im,text,p,FONT_HERSHEY_SIMPLEX, size,cv::Scalar(125,255,255),1*fact);
}

void printInfo(cv::Mat &im){
    float fs=float(im.cols)/float(1200);
    putText(im, "fps="+to_string(1./Fps.getAvrg()),cv::Point(10,fs*30),fs*1.0f);
    putText(im, cam_pose, cv::Point(10,fs*60),fs*1.0f);
    putText(im, cam_vect, cv::Point(10,fs*90),fs*1.0f);
}

int main(int argc, char** argv)
{
    try
    {
        int shmid;
        int keys;
        float *ppp;
        float Tvecbuf[3] = {0};
        keys = ftok("/home/ni/share_memory.txt",'a');
        shmid = shmget(keys,64,IPC_CREAT | 0777);
        printf("creat share memory sucess shmid=%d\n",shmid);
        ppp = (float *)shmat(shmid,NULL,0);//映射到用户空间
        printf("p=%p\n",ppp);
        CmdLineParser cml(argc, argv);
        if (argc < 2 || cml["-h"])
        {
            cerr << "Invalid number of arguments" << endl;
            cerr << "Usage: (in.avi|live[:camera_index(e.g 0 or 1)]) [-c camera_params.yml] [-s  marker_size_in_meters] [-d dictionaty:"
                    "ALL_DICTS by default] [-ref_id reference_marker's_id for estimating pose of camera] [-e use EnclosedMarker or not] [-h]" << endl;
            return false;
        }

        ///  PARSE ARGUMENTS
        string TheInputVideo = argv[1];
        ref_id = std::stoi(cml("-ref_id"));
        if(-1 == ref_id){cout<<"You need indicate a reference_marker by use the parameter [-ref_id].\n"<<endl;return false;}
	
        // read camera parameters if passed
        float TheMarkerSize = std::stof(cml("-s", "-1"));
		if (cml["-c"]) TheCameraParameters.readFromXMLFile(cml("-c"));
		MDetector.setDictionary(cml("-d", "ALL_DICTS") );  // sets the dictionary to be employed (ARUCO,APRILTAGS,ARTOOLKIT,etc)
		MDetector.getParameters().detectEnclosedMarkers(std::stoi(cml("-e", "0"))); //if use enclosed markers, set -e 1(true), or set -e 0(false). Default value is 0.
		std::map<uint32_t, MarkerPoseTracker> MTracker;  // use a map so that for each id, we use a different pose tracker
		
        ///  OPEN VIDEO
        // read from camera or from  file
        if (TheInputVideo.find("live") != string::npos)
        {
            int vIdx = 0;
            // check if the :idx is here
            char cad[100];
            if (TheInputVideo.find(":") != string::npos)
            {
                std::replace(TheInputVideo.begin(), TheInputVideo.end(), ':', ' ');
                sscanf(TheInputVideo.c_str(), "%s %d", cad, &vIdx);
            }
            cout << "Opening camera index " << vIdx << endl;
            TheVideoCapturer.open(vIdx);
            waitTime = 10;
            isVideo=true;
        }
        else{
            TheVideoCapturer.open(TheInputVideo);
            if ( TheVideoCapturer.get(CAP_PROP_FRAME_COUNT)>=2) isVideo=true;
        }
        // check video is open
        if (!TheVideoCapturer.isOpened()) throw std::runtime_error("Could not open video");
		cv::namedWindow("in",cv::WINDOW_AUTOSIZE);
        // CONFIGURE DATA
        // read first image to get the dimensions
        TheVideoCapturer >> TheInputImage;
        if (TheCameraParameters.isValid()) TheCameraParameters.resize(TheInputImage.size());

        char key = 0;
        int index = 0,indexSave=0;
        // capture until press ESC or until the end of the video
         do
        {
            TheVideoCapturer.retrieve(TheInputImage);
		    TheInputImage.copyTo(TheInputImageCopy);

            Fps.start();
            // TheMarkers = MDetector.detect(TheInputImage, TheCameraParameters, TheMarkerSize);
            TheMarkers = MDetector.detect(TheInputImage);
		    for (auto& marker : TheMarkers)  // for each marker
                MTracker[marker.id].estimatePose(marker, TheCameraParameters, TheMarkerSize);  // call its tracker and estimate the pose
            Fps.stop();

            for (unsigned int i = 0; i < TheMarkers.size(); i++)
			{
				if(ref_id == TheMarkers[i].id)
				{
                    Mat camPosMatrix, camVecMatrix;
                    Mat vect = (Mat_<float>(3,1)<<0,0,1);
                    // R、t矩阵法
                    Mat rMatrix, tMatrix;
                    Rodrigues(TheMarkers[i].Rvec, rMatrix);
                    transpose(TheMarkers[i].Tvec, tMatrix);
                    camPosMatrix = rMatrix.inv()*(-tMatrix);
                    camVecMatrix = rMatrix.inv()*vect;
                    cout << "Camara Position: " << camPosMatrix.t() << "\nCamera Direction: " << camVecMatrix.t() << endl;
                    // 齐次矩阵法
                    Mat RTinv = MTracker[ref_id].getRTMatrix().inv();
                    camPosMatrix = RTinv(Rect(3,0,1,3)).clone();
                    camVecMatrix = RTinv(Range(0,3),Range(0,3))*vect;
                        
                    sprintf(cam_pose,"Camera Position: px=%f, py=%f, pz=%f", camPosMatrix.at<float>(0,0), 
                    camPosMatrix.at<float>(1,0), camPosMatrix.at<float>(2,0));
                    sprintf(cam_vect,"Camera Direction: dx=%f, dy=%f, dz=%f", camVecMatrix.at<float>(0,0), 
                    camVecMatrix.at<float>(1,0), camVecMatrix.at<float>(2,0));
                    cout << TheMarkers[i].dict_info << " " <<  TheMarkers[i].id << endl;
                    cout << "Rvec = " << TheMarkers[i].Rvec << endl << "Tvec = " << TheMarkers[i].Tvec << endl;
                    cout << TheMarkers[i][0] << TheMarkers[i][1] << TheMarkers[i][2] << TheMarkers[i][3] << endl << endl;
                    CvDrawingUtils::draw3dAxis(TheInputImageCopy, TheMarkers[i], TheCameraParameters);
                    for(int j = 0; j < 3; j++)
                        {
                            Tvecbuf[j] = TheMarkers[i].Tvec.at<float>(0,j);
                            memcpy(ppp,Tvecbuf,sizeof(Tvecbuf));
                            printf("ppp = %f ",*(ppp+j));
                            printf("\n");
                        }
                    printf("\n");
				}
				TheMarkers[i].draw(TheInputImageCopy, Scalar(0, 0, 255), 2, true);
				// CvDrawingUtils::draw3dCube(TheInputImageCopy, TheMarkers[i], TheCameraParameters);
			}

            // show input with augmented information and  the thresholded image
            printInfo(TheInputImageCopy);
            cv::imshow("in", TheInputImageCopy);

            key = cv::waitKey(waitTime);  // wait for key to be pressed
            if (key == 's') waitTime = waitTime == 0 ? 10 : 0;
            index++;  // number of images captured

            if (isVideo) if ( TheVideoCapturer.grab()==false) key=27;
        } while (key != 27 );
    }
    catch (std::exception& ex) {cout << "Exception :" << ex.what() << endl; }
}

