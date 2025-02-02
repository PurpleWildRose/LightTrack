//
// Created by xiongzhuang on 2021/10/8.
//

#ifndef LIGHTTRACK_LIGHTTRACK_H
#define LIGHTTRACK_LIGHTTRACK_H

#include <vector>
#include <map>
#include <opencv2/opencv.hpp>
#include "rknn_api.h"

#define PI 3.1415926f

using namespace cv;

class LightTrack {
public:
    LightTrack(const std::string& model_init, const std::string& model_backbone, const std::string& model_neck_head);
    ~LightTrack();
    void init(const cv::Mat& img, cv::Point target_pos_, cv::Point2f target_sz_);
    void update(const cv::Mat &x_crop, float scale_z);
    void track(const cv::Mat& im);
    static unsigned char *load_model(const char *filename, int *model_size);

    cv::Point target_pos = {0, 0};
    cv::Point2f target_sz = {0.f, 0.f};

private:
    void grids();
    cv::Mat get_subwindow_tracking(const cv::Mat& im, cv::Point2f pos, int model_sz, int original_sz);

    int stride=16;
    int even=0;
    int exemplar_size = 127;
    int instance_size = 288;
    float lr = 0.616; //给的越大，标定框飘离得越快
    float ratio = 1;
    float penalty_tk = 0.007;
    float context_amount = 0.5;
    float window_influence = 0.225;
    int score_size;
    int total_stride = 16;

    std::vector<float> window;
    std::vector<float> grid_to_search_x;
    std::vector<float> grid_to_search_y;

    //可以设置成模型文件路径或者模型的二进制数据（采用的2）
    unsigned char *init_model_data, *backbone_model_data, *neck_head_model_data;

    //三个模型的指针init.rknn  backbone.rknn net_neck.rknn
    rknn_context net_init, net_backbone, net_neck_head;

    //查询输入/出端口数量
    rknn_input_output_num init_io_num;
    rknn_input_output_num backbone_io_num;
    rknn_input_output_num neck_head_io_num;

    //查询模型输入/出属性信息
    rknn_tensor_attr init_input_attrs[1];
    rknn_tensor_attr init_output_attrs[1];
    rknn_tensor_attr backbone_input_attrs[1];
    rknn_tensor_attr backbone_output_attrs[1];
    rknn_tensor_attr neck_head_input_attrs[2];
    rknn_tensor_attr neck_head_output_attrs[2];

    //结构体输出数据
    rknn_output zf[1];
    rknn_output tfInit[1];
    rknn_output tfbackbone[1];
    //数据内存暂时存储器
    rknn_tensor_mem *input_mem[2], *output_mem[2];

    //输出内存结构体
    rknn_tensor_mem* init_output_mem[1];
    rknn_tensor_mem* backbone_output_mem[1];
};




#endif //LIGHTTRACK_LIGHTTRACK_H
