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

#include "utils.h"

const char Utils::separator = ';';
string Utils::training_files = "dataset.txt";
string Utils::root_path = "";

Utils::Utils()
{

}

Utils::~Utils()
{

}
vector<string> Utils::readFileList(string name_list_file) {
    ifstream in_file(name_list_file, ifstream::in);

    vector<string> name_list;
    if (!in_file) {
        fprintf(stderr, "name list can not opened!!\n");
        return name_list;
    }

    string line;
    while (getline(in_file, line)) {
        if (!line.empty()) {
            name_list.push_back(line);
        }
    }

    in_file.close();
    return name_list;
}

void Utils::readCVSfiles(vector<Mat> &images, vector<int> &labels) {
    training_files.insert(0, root_path);
    ifstream in_file(training_files, ifstream::in);

    if (!in_file) {
        fprintf(stderr, "cvs file can not opened!!\n");
        return;
    }
    string line, path, label;
    while (getline(in_file, line)) {
        stringstream liness(line);
        getline(liness, path, separator);
        getline(liness, label);

        if(!path.empty() && !label.empty()) {
            images.push_back(imread(path, CV_LOAD_IMAGE_GRAYSCALE));
            labels.push_back(atoi(label.c_str()));
        }
    }

    in_file.close();
}

void Utils::createCVSFile(string root_path, string sub_path) {
    string faces_path(root_path);
    faces_path.append(sub_path);
    DIR *dir0, *dir1;
    struct dirent *in0, *in1;
    int label = 0;
    ofstream cvs_out(format("%s/dataset.txt", root_path.c_str()), ostream::out);

    if (!cvs_out) {
        fprintf(stderr, "cvs file can not created!!\n");
        return;
    }

    if ((dir0 = opendir(faces_path.c_str())) != NULL) {
        while ((in0 = readdir(dir0)) != NULL) {
            if (in0->d_name[0] != '.') {
                string sample_path = faces_path;
                sample_path.append(in0->d_name);
                if ((dir1 = opendir(sample_path.c_str())) != NULL) {
                    while ((in1 = readdir(dir1)) != NULL) {
                        if (in1->d_name[0] != '.') {
                            string path(sample_path);
                            path.append(format("/%s%c%d", in1->d_name, separator, label));
                            cout << path << endl;
                            cvs_out << path.c_str() << endl;
                        }
                    }
                    free(dir1);
                    label++;
                }
            }
        }
        free(dir0);
    }

    cvs_out.close();

}

