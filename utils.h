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

#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include "cfacedetection.h"
#include <dirent.h>

#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

class Utils
{
public:
    Utils();
    ~Utils();

    static const char separator;
    static string training_files;
    static string root_path;

    static vector<string> readFileList(string person_text);
    static void readCVSfiles(vector<Mat> &images, vector<int> &labels);
    static void createCVSFile(string root_path, string sub_path);
};

#endif // UTILS_H
