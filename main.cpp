#include <opencv2/opencv.hpp>
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>
#include <mutex>
#include <complex>
#include <vector>

int main() {
    const int width = 800;
    const int height = 600;
    const int maxIter = 500;
    cv::Mat image(height, width, CV_8UC3);

    double centerX = -0.7;
    double centerY = 0.0;
    double scale = 3.0;
    const double zoomFactor = 0.8;

    cv::namedWindow("Mandelbrot", cv::WINDOW_AUTOSIZE);
    std::mutex mtx;

    auto render = [&]() {
        const int tileSize = 64;
        std::vector<cv::Rect> tiles;
        for (int y = 0; y < height; y += tileSize) {
            for (int x = 0; x < width; x += tileSize) {
                tiles.emplace_back(x, y,
                    std::min(tileSize, width - x),
                    std::min(tileSize, height - y));
            }
        }

        tbb::parallel_for(size_t(0), tiles.size(), [&](size_t i) {
            cv::Rect roi = tiles[i];
            for (int r = roi.y; r < roi.y + roi.height; ++r) {
                for (int c = roi.x; c < roi.x + roi.width; ++c) {
                    double zx = centerX + (c - width / 2.0) * scale / width;
                    double zy = centerY + (r - height / 2.0) * scale / width;
                    std::complex<double> z(0, 0);
                    std::complex<double> cplx(zx, zy);
                    int iter = 0;
                    while (iter < maxIter && std::abs(z) <= 2.0) {
                        z = z * z + cplx;
                        ++iter;
                    }
                    int color = static_cast<int>(255.0 * iter / maxIter);
                    image.at<cv::Vec3b>(r, c) =
                        iter == maxIter ? cv::Vec3b(0, 0, 0)
                                         : cv::Vec3b(0, color, 255 - color);
                }
            }
            {
                std::lock_guard<std::mutex> lock(mtx);
                cv::imshow("Mandelbrot", image);
                cv::waitKey(1);
            }
        });
    };

    render();
    while (true) {
        int key = cv::waitKey(0);
        if (key == 27 || key == 'q')
            break;
        if (key == 'w')
            centerY -= scale * 0.1;
        if (key == 's')
            centerY += scale * 0.1;
        if (key == 'a')
            centerX -= scale * 0.1;
        if (key == 'd')
            centerX += scale * 0.1;
        if (key == '+')
            scale *= zoomFactor;
        if (key == '-')
            scale /= zoomFactor;
        render();
    }
    return 0;
}
