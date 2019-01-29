#include <iostream>
#include <vector>
#include <opencv/cv.hpp>

#define ESCAPE 27

using namespace cv;
using namespace std;

int main()
{
    Mat img,hist;
    VideoCapture webcam = VideoCapture(0);
    bool success = false;
    char keypressed = 0;

    namedWindow("Image", CV_WINDOW_AUTOSIZE);
    namedWindow("Histogram", CV_WINDOW_AUTOSIZE);

    moveWindow("Image" ,600,100); //
    moveWindow("Histogram" ,1240,100);

    if(!webcam.isOpened())
    {
        cout << "Error opening the camera" << endl;
        return 1;
    }

    //We capture a sample image to check and to obtain dimensions automatically in order to resize components

    success = webcam.read(img);
    if(success == false)
    {
        cout << "Unable to capture frame" << endl;
        return 1;
    }

    //We start the loop (we are not checking if failure, first approach
    int histSize = 256;
    float range[] = {0,256};
    const float* histRange = {range};
    //Now we create the Mat to show on screen and adapt the histograms
    int hist_w = 512;
    int hist_he = 400;
    int bin_w = cvRound((double) hist_w/histSize); //value of the scale(aprox)
    Mat histImage = Mat(hist_he,hist_w, CV_8UC3, Scalar(40,0,50));

    while(keypressed != ESCAPE)
    {

        webcam.read(img);
        cvtColor(img,img,CV_BGR2GRAY);

        //We calculate the histogram for each component
        calcHist(&img, 1, 0, Mat(),hist, 1, &histSize,&histRange,true,false);

        //We normalize the values of the histogram
        normalize(hist,hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

        //We eliminate the previous histogram plotted
        histImage.setTo(Scalar(40,0,50));

        //We draw the histogram into thr histImage Mat
        for(int j = 1; j < histSize; j++)
        {
            line(histImage,Point(bin_w*(j-1),hist_he - cvRound(hist.at<float>(j-1))),
                 Point(bin_w*j,hist_he - cvRound(hist.at<float>(j))),
                 Scalar(200,200,0),2,8,0);
        }

        imshow("Image", img);
        imshow("Histogram", histImage);

        keypressed = waitKey(3); //waiting 3ms to key
    }

    //Free memory
    webcam.release();
    destroyAllWindows();

    return 0;
}
