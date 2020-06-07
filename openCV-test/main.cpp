//
//  main.cpp
//  openCV-test
//
//  Created by Daksh Gupta on 07/06/20.
//  Copyright © 2020 Daksh Gupta. All rights reserved.
//
#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#ifdef HAVE_OPENCV_XFEATURES2D
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"

using namespace std;

using namespace cv;
using namespace xfeatures2d;
int main(int argc, const char * argv[]) {
    // insert code here...
    Mat img1=imread("/Users/daksh_mac/Desktop/Dev/openCV-test/openCV-test/objects.jpeg");
    Mat img2=imread("/Users/daksh_mac/Desktop/Dev/openCV-test/openCV-test/objectscopy.jpeg");
    if(img1.empty() ||!img2.data )
    { printf(" --(!) Error reading images \n"); return -1; }
    //converting to gray scale
    cvtColor(img1, img1, COLOR_BGR2GRAY);
    cvtColor(img2, img2, COLOR_BGR2GRAY);
    int minHessian = 400;
    Ptr<SURF> detector = SURF::create( minHessian );
    vector<KeyPoint> keypoints1, keypoints2;
    Mat descriptors1, descriptors2;
    detector->detectAndCompute( img1, noArray(), keypoints1, descriptors1 );
    detector->detectAndCompute( img2, noArray(), keypoints2, descriptors2 );
    
    //-- Step 2: Matching descriptor vectors with a FLANN based matcher
    // Since SURF is a floating-point descriptor NORM_L2 is used
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
    vector< std::vector<DMatch> > knn_matches;
    matcher->knnMatch( descriptors1, descriptors2, knn_matches, 2 );
    
    //-- Filter matches using the Lowe's ratio test
    const float ratio_thresh = 0.7f;
    vector<DMatch> good_matches;
    for (size_t i = 0; i < knn_matches.size(); i++)
    {
        if (knn_matches[i][0].distance < ratio_thresh * knn_matches[i][1].distance)
        {
            good_matches.push_back(knn_matches[i][0]);
        }
    }
    
    //-- Draw matches
    Mat img_matches;
    drawMatches( img1, keypoints1, img2, keypoints2, good_matches, img_matches, Scalar::all(-1),
                Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
    //-- Show detected matches
    imshow("Good Matches", img_matches );
    waitKey(50000);
    return 0;
}
#endif
