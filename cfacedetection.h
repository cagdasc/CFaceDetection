/*
 * Copyright 2015 Cagdas Caglak
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CFACEDETECTION_H
#define CFACEDETECTION_H

#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include "utils.h"

using namespace cv;
using namespace std;

class CFaceDetection
{
public:
    CFaceDetection(string root_path);
    ~CFaceDetection();

    void train_algorithm();
    void setFaceRecognizer(int which);
    void prepareDetectedFace(vector<string> name_list, int which);
    void prepareDetectedFace(string name, int which);
    void predictFace(string which_face);
    void liveDetection(int deviceId);
    string getRootPath();

    static string cascade_file;

private:
    string input_face;
    Ptr<FaceRecognizer> face_recognizer;
    CascadeClassifier classifier;
    string root_path;

    static const int face_width;
    static const int face_height;
};

#endif // CFACEDETECTION_H
