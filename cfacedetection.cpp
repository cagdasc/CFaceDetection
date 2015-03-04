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

#include "cfacedetection.h"

string CFaceDetection::cascade_file = "classifier_files/haarcascade_frontalface_alt2.xml";
const int CFaceDetection::face_width = 92;
const int CFaceDetection::face_height = 112;

CFaceDetection::CFaceDetection(string root_path)
{
    this->input_face = input_face;
    this->root_path = root_path;
    cascade_file.insert(0, root_path);
    classifier.load(cascade_file);
    this->face_recognizer = createEigenFaceRecognizer();
}

CFaceDetection::~CFaceDetection()
{

}

void CFaceDetection::setFaceRecognizer(int which) {
    if (which == 0) {
        this->face_recognizer = createEigenFaceRecognizer();
    } else if (which == 1) {
        this->face_recognizer = createFisherFaceRecognizer();
    } else if (which == 2) {
        this->face_recognizer = createLBPHFaceRecognizer();
    } else {
        this->face_recognizer = createEigenFaceRecognizer();
    }
}

string CFaceDetection::getRootPath() {
    return root_path;
}

void CFaceDetection::prepareDetectedFace(vector<string> name_list, int which) {

    string path(root_path);
    path.append(format("/faces/dataset%d/", which));
    mkdir(path.c_str(), S_IRWXU);

    for (int i = 0; i < name_list.size(); i++) {
        cout << "Raw file name: " << name_list.at(i) << endl;
        Mat raw_image = imread(name_list.at(i), CV_LOAD_IMAGE_GRAYSCALE);
        vector< Rect_<int> > raw_rect;
        classifier.detectMultiScale(raw_image, raw_rect);

        Mat face_resized;
        for(int j = 0; j < raw_rect.size(); j++) {
            Mat face = raw_image(raw_rect.at(j));

            cv::resize(face, face_resized, Size(face_width, face_height), 1.0, 1.0, INTER_CUBIC);
            rectangle(raw_image, raw_rect.at(j), CV_RGB(0, 255,0), 1);

            imwrite(format("%sresized%d-%d.pgm", path.c_str(), i, j), face_resized);
            face.release();
            face_resized.release();
        }
        //imwrite(format("detected%d-%d.jpg", which, i), raw_image);
        raw_image.release();
    }

}

void CFaceDetection::prepareDetectedFace(string name, int which) {

    string path(root_path);
    path.append(format("/faces/dataset%d/", which));
    mkdir(path.c_str(), S_IRWXU);


    cout << "Raw file name: " << name << endl;
    Mat raw_image = imread(name, CV_LOAD_IMAGE_GRAYSCALE);
    vector< Rect_<int> > raw_rect;
    classifier.detectMultiScale(raw_image, raw_rect);

    Mat face_resized;
    for(int j = 0; j < raw_rect.size(); j++) {
        Mat face = raw_image(raw_rect.at(j));

        cv::resize(face, face_resized, Size(face_width, face_height), 1.0, 1.0, INTER_CUBIC);
        rectangle(raw_image, raw_rect.at(j), CV_RGB(0, 255,0), 1);

        imwrite(format("%sresized%d-%d.pgm", path.c_str(), which, j), face_resized);
        face.release();
        face_resized.release();
    }
    //imwrite(format("detected%d-%d.jpg", which, i), raw_image);
    raw_image.release();


}

void CFaceDetection::predictFace(string which_face) {
    vector<Mat> images;
    vector<int> labels;

    //cout << "Training..." << endl;
    Utils::readCVSfiles(images, labels);
    face_recognizer->train(images, labels);

    Mat raw_image = imread(which_face, CV_LOAD_IMAGE_GRAYSCALE);
    vector< Rect_<int> > raw_rect;
    classifier.detectMultiScale(raw_image, raw_rect);

    Mat face_resized;
    for(int j = 0; j < raw_rect.size(); j++) {
        Mat face = raw_image(raw_rect.at(j));

        cv::resize(face, face_resized, Size(face_width, face_height), 1.0, 1.0, INTER_CUBIC);
        rectangle(raw_image, raw_rect.at(j), CV_RGB(0, 255,0), 1);
        int prediction = face_recognizer->predict(face_resized);
        cout << "Prediction: " << prediction << endl;

        face.release();
        face_resized.release();
    }

    raw_image.release();

}

void CFaceDetection::liveDetection(int deviceId) {
    vector<Mat> images;
    vector<int> labels;

    Utils::readCVSfiles(images, labels);
    face_recognizer->train(images, labels);

    VideoCapture cap(deviceId);
    if(!cap.isOpened()) {
        fprintf(stderr, "camera can not opened!!\n");
        return;
    }

    Mat frame;
    for (;;) {
        cap >> frame;
        Mat original;
        flip(frame.clone(), original, 1);
        frame.release();
        Mat gray;
        cvtColor(original, gray, CV_BGR2GRAY);

        vector< Rect_<int> > raw_rect;
        classifier.detectMultiScale(gray, raw_rect);

        Mat face_resized;
        for(int j = 0; j < raw_rect.size(); j++) {
            Mat face = gray(raw_rect.at(j));

            cv::resize(face, face_resized, Size(face_width, face_height), 1.0, 1.0, INTER_CUBIC);
            int prediction = face_recognizer->predict(face_resized);
            cout << "Prediction: " << prediction << endl;

            rectangle(original, raw_rect.at(j), CV_RGB(0, 255,0), 1);
            string box_text = format("Prediction = %d", prediction);
            int pos_x = std::max(raw_rect.at(j).tl().x - 10, 0);
            int pos_y = std::max(raw_rect.at(j).tl().y - 10, 0);
            putText(original, box_text, Point(pos_x, pos_y), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0);

            face.release();
            face_resized.release();
        }

        imshow("Face Detector", original);

        char key = (char) waitKey(10);
        if(key == 27)
            break;

        gray.release();
    }

    cap.release();

}
