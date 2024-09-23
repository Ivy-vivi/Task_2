#include <opencv2/opencv.hpp>
#include <iostream>

void processImage(const cv::Mat& image) {
    if (image.empty()) {
        std::cout << "Image not loaded correctly" << std::endl;
        return;
    }
    
    // 缩小图像尺寸
    cv::Mat resized_image;
    double scale = 0.5; // 缩放比例，例如 0.5 表示缩小到原来的 50%
    cv::resize(image, resized_image, cv::Size(), scale, scale);
    
    // 转化为灰度图
    cv::Mat gray_image;
    cv::cvtColor(resized_image, gray_image, cv::COLOR_BGR2GRAY);

    // 转化为 HSV 图片
    cv::Mat hsv_image;
    cv::cvtColor(resized_image, hsv_image, cv::COLOR_BGR2HSV);

    // 应用均值滤波
    cv::Mat mean_blur;
    cv::blur(resized_image, mean_blur, cv::Size(5, 5));

    // 应用高斯滤波
    cv::Mat gaussian_blur;
    cv::GaussianBlur(resized_image, gaussian_blur, cv::Size(5, 5), 0);

    // 提取红色颜色区域
    cv::Mat lower_red_mask, upper_red_mask, red_mask;
    cv::inRange(hsv_image, cv::Scalar(0, 120, 70), cv::Scalar(10, 255, 255), lower_red_mask);
    cv::inRange(hsv_image, cv::Scalar(160, 120, 70), cv::Scalar(180, 255, 255), upper_red_mask);
    cv::add(lower_red_mask, upper_red_mask, red_mask);


    // 寻找红色的外轮廓
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(red_mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    //计算轮廓面积
    for (size_t i = 0; i < contours.size(); ++i) {
    double area = cv::contourArea(contours[i]);
    std::cout << "Contour " << i << " area: " << area << std::endl;
}

   
    
     // 提取高亮颜色区域并进行图形学处理
     // 二值化
    cv::Mat binary_image;
    cv::threshold(gray_image, binary_image, 128, 255, cv::THRESH_BINARY);

    // 膨胀
    cv::Mat dilated;
    cv::dilate(binary_image, dilated, cv::Mat(), cv::Point(-1, -1), 2);
    
    //腐蚀
    cv::Mat eroded;
    cv::erode(dilated, eroded, cv::Mat(), cv::Point(-1, -1), 2);
    
    // 漫水处理
    cv::Mat flood_filled = eroded.clone();
    cv::floodFill(flood_filled, cv::Point(0, 0), cv::Scalar(255));

    // 图像绘制
    //圆形方形文字
    cv::Mat drawing = resized_image.clone();
    cv::circle(drawing, cv::Point(50, 50), 25, cv::Scalar(255, 0, 0), 3);
    cv::rectangle(drawing, cv::Rect(100, 100, 50, 25), cv::Scalar(0, 255, 0), 3);
    cv::putText(drawing, "666", cv::Point(150, 150), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255), 2);
  
    //绘制红色外轮廓
    cv::Mat contour_image = resized_image.clone();
    for (size_t i = 0; i < contours.size(); ++i) {
    cv::drawContours(contour_image, contours, (int)i, cv::Scalar(0, 255, 0), 2);
    }

    //绘制bounding box
    for (size_t i = 0; i < contours.size(); ++i) {
    cv::Rect bounding_box = cv::boundingRect(contours[i]);
    cv::rectangle(contour_image, bounding_box, cv::Scalar(255, 0, 0), 2);
    }

   
    
    // 图像旋转 35 度
    cv::Mat rotated;
    cv::Point2f center(resized_image.cols / 2.0, resized_image.rows / 2.0);
    cv::Mat rot_mat = cv::getRotationMatrix2D(center, 35, 1.0);
    cv::warpAffine(resized_image, rotated, rot_mat, resized_image.size());

    // 图像裁剪为原图的左上角 1/4
    cv::Mat cropped = resized_image(cv::Rect(0, 0, resized_image.cols / 2, resized_image.rows / 2));

    //指定保存目录
    std::string saveDirectory = "/home/ros/opencv_project/resources/";



    // 保存处理后的图像
    cv::imwrite(saveDirectory + "resized_image.png", resized_image);
    cv::imwrite(saveDirectory + "gray_image.png", gray_image);
    cv::imwrite(saveDirectory + "hsv_image.png", hsv_image);
    cv::imwrite(saveDirectory + "mean_blur.png", mean_blur);
    cv::imwrite(saveDirectory + "gaussian_blur.png", gaussian_blur);
    cv::imwrite(saveDirectory + "red_mask.png", red_mask);
    cv::imwrite(saveDirectory + "dilated.png", dilated);
    cv::imwrite(saveDirectory + "eroded.png", eroded);
    cv::imwrite(saveDirectory + "flood_filled.png", flood_filled);
    cv::imwrite(saveDirectory + "drawing.png", drawing);
    cv::imwrite(saveDirectory + "contour_image.png", contour_image);
    cv::imwrite(saveDirectory + "rotated.png", rotated);
    cv::imwrite(saveDirectory + "cropped.png", cropped);




    
    
    // 显示结果
    cv::imshow("Resized Image", resized_image);
    cv::imshow("Gray Image", gray_image);
    cv::imshow("HSV Image", hsv_image);
    cv::imshow("Mean Blur", mean_blur);
    cv::imshow("Gaussian Blur", gaussian_blur);
    cv::imshow("Red Mask", red_mask);
    cv::imshow("Dilated ", dilated);
    cv::imshow("Eroded", eroded);
    cv::imshow("Flood Filled", flood_filled);
    cv::imshow("Drawing", drawing);
    cv::imshow("Contours", contour_image);
    cv::imshow("Rotated Image", rotated);
    cv::imshow("Cropped Image", cropped);

    cv::waitKey(0);
}

int main() {
    cv::Mat image = cv::imread("/home/ros/opencv_project/resources/test_image.png", cv::IMREAD_COLOR);
    processImage(image);
    return 0;
}

