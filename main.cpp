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

#include <iostream>
#include <string.h>
#include "cfacedetection.h"

using namespace std;

void usage();
int main(int argc, const char *argv[])
{

    string root_path, which, sub_path, label;
    if (argc < 4) {
        usage();
        return 0;
    } else {
        string ctrl(argv[1]);
        if (strcmp(ctrl.c_str(), "-i") == 0) {
            if (argc < 5) {
                usage();
                return 0;
            }
            root_path = string(argv[2]);
            ctrl = string(argv[3]);

            if (strcmp(ctrl.c_str(), "-d") == 0) {
                ctrl = string(argv[4]);
                label = string(argv[5]);
                CFaceDetection *cface = new CFaceDetection(root_path);
                Utils::root_path = root_path;
                cface->detectFace(ctrl, atoi(label.c_str()));
            } else if (strcmp(ctrl.c_str(), "-df") == 0) {
                ctrl = string(argv[4]);
                CFaceDetection *cface = new CFaceDetection(root_path);
                Utils::root_path = root_path;
                if (strcmp(ctrl.c_str(), "-l") == 0) {
                    which = string(argv[5]);
                    label = string(argv[6]);
                    vector<string> name_list = Utils::readFileList(which);
                    cface->prepareSampleFace(name_list, atoi(label.c_str()));

                } else {
                    which = string(argv[4]);
                    label = string(argv[5]);
                    cface->prepareSampleFace(which, atoi(label.c_str()));
                }
                delete cface;
            } else if (strcmp(ctrl.c_str(), "-pf") == 0) {
                which = string(argv[4]);
                CFaceDetection *cface = new CFaceDetection(root_path);
                Utils::root_path = root_path;
                cface->predictFace(which);
                delete cface;

            } else if (strcmp(ctrl.c_str(), "-lf") == 0) {
                label = string(argv[4]);
                CFaceDetection *cface = new CFaceDetection(root_path);
                Utils::root_path = root_path;
                cface->liveDetection(atoi(label.c_str())); // set device id in here
                delete cface;
            } else {
                usage();
            }

        } else if (strcmp(ctrl.c_str(), "-t") == 0) {
            root_path = string(argv[2]);
            sub_path = string(argv[3]);
            Utils::createCVSFile(root_path, sub_path);
        } else if (strcmp(ctrl.c_str(), "-h") == 0) {
            usage();
        }
    }

    return 0;
}

void usage() {
    cout << "Usage: CIP -[i | t]\n"
         "-i = Image Processing Tool\n\t"
         "-d = Detect Face\n\t"
         "-df = Detect Face for create dataset\n\t\t"
         "-l = Images File List\n\t"
         "-pd = Predict Face\n\t"
         "-lf = Live Detection From Camera\n"
         "-t = CVS File Creating\n"
         "[rootpath/] = All data files must be in this(cascade files, faces, dataset list, imageslist)\n"
         "[imagefoldersrootpath] = Faces root path.\n\n"
         "Example File Structure:\n"
         "--data\n\t"
         "--faces\n\t\t"
         "--dataset0\n\t\t\t"
         "--resized0-0.pgm\n\t\t\t"
         "--resized1-0.pgm\n\t\t"
         "--dataset1\n\t\t\t"
         "--resized0-0.pgm\n\t\t\t"
         "--resized1-0.pgm\n\t"
         "--dataset.txt\n\t"
         "--classifier_files\n\t\t"
         "--*.xml\n\t"
         "--data.txt\n\n"
         "Example Usage:\n"
         "./CIP -i [rootpath/] -d [imagefile.*] [label] Detect given face and save it with label.\n"
         "./CIP -i [rootpath/] -df [imagefile.*] [label] Detect given face and save it with label in \"faces\" folder for create dataset.\n"
         "./CIP -i [rootpath/] -df -l [imageslist.txt] [label] Detect given faces and save them with label in \"faces\" folder.\n"
         "./CIP -i [rootpath/] -pf [imagefile.*] Predict face using cvs file which name is \"dataset.txt\"\n"
         "./CIP -i [rootpath/] -lf [deciveId] Open camera for live detection\n"
         "./CIP -i data/ -lf -1 Start live detection\n\n"
         "./CIP -t [rootpath/] [imagefoldersrootpath]" << endl;
}

