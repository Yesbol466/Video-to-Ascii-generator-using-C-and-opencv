#include <opencv2/opencv.hpp>
#include <chrono>
#include <iostream>
#include <thread>

using namespace std;
using namespace cv;

string pixelToASCII(int pixel_intensity) {
    const string ASCII_CHARS = " .'`^\",:;~-_+<>i!lI?/\\|()1{}[]rcvunxwzjftLCJUYXQZ0Omwqpdbkhao*#MW&8%B@$";
    return string(1, ASCII_CHARS[pixel_intensity * ASCII_CHARS.length() / 256]);
}

int main() {
    string video_path = "C:\\Users\\66985\\Downloads\\vid1.mp4";
    VideoCapture cap(video_path);

    if (!cap.isOpened()) {
        cerr << "Error opening video file." << endl;
        return -1;
    }

    double fps = cap.get(CAP_PROP_FPS);
    if (fps < 1e-5) {
        cerr << "Invalid FPS value." << endl;
        return -1;
    }

    int frame_duration_ms = static_cast<int>(1000.0 / fps);


    int width = 120;
    int height = static_cast<int>(width * cap.get(CAP_PROP_FRAME_HEIGHT) / cap.get(CAP_PROP_FRAME_WIDTH) * 0.5);

    Mat frame, gray_frame, resized_frame;

    while (true) {
        auto frame_start_time = chrono::system_clock::now();
        cap >> frame;
        if (frame.empty())
            break;

        cvtColor(frame, gray_frame, COLOR_BGR2GRAY);
        resize(gray_frame, resized_frame, Size(width, height), 0, 0, INTER_LINEAR);

        string ascii_frame;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                ascii_frame += pixelToASCII(resized_frame.at<uchar>(i, j));
            }
            ascii_frame += "\n";
        }

#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif

        cout << ascii_frame;

        auto frame_end_time = chrono::system_clock::now();
        auto frame_elapsed_time = chrono::duration_cast<chrono::milliseconds>(frame_end_time - frame_start_time);

        if (frame_duration_ms > frame_elapsed_time.count())
            this_thread::sleep_for(chrono::milliseconds(frame_duration_ms - frame_elapsed_time.count()));
    }

    return 0;
}
