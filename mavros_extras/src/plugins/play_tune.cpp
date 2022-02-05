/**
 * @file play_tune.cpp
 * @author LauZanMo (LauZanMo@whu.edu.cn)
 * @author Vladimir Ermakov <vooon341@gmail.com>
 * @brief This file is from mavros open source respository, thanks for their contribution.
 * @version 1.0
 * @date 2022-02-05
 *
 * @copyright Copyright (c) 2022 acfly
 * @copyright Copyright 2014,2015,2016,2017 Vladimir Ermakov.
 * For commercial use, please contact acfly: https://www.acfly.cn
 *
 */

#include <mavros/mavros_plugin.h>
#include <mavros_msgs/PlayTuneV2.h>

#include <std_msgs/UInt8.h>

namespace mavros {
namespace extra_plugins {
/**
 * @brief Play tune plugin
 * @brief 用于使飞控播放音乐的插件
 */
class PlayTunePlugin : public plugin::PluginBase {
public:
    PlayTunePlugin() : PluginBase(), pt_nh("~play_tune") {}

    void initialize(UAS &uas_) override {
        PluginBase::initialize(uas_);
        playtune_raw_sub = pt_nh.subscribe("raw", 1, &PlayTunePlugin::playtune_raw_cb, this);
        acfly_default_tune_sub =
            pt_nh.subscribe("acfly_default", 1, &PlayTunePlugin::playtune_raw_cb, this);
    }

    Subscriptions get_subscriptions() override {
        return {/* 禁用接收 */};
    }

private:
    ros::NodeHandle pt_nh;
    ros::Subscriber playtune_raw_sub;
    ros::Subscriber acfly_default_tune_sub;

    void playtune_raw_cb(const mavros_msgs::PlayTuneV2::ConstPtr &tune) {
        auto msg = mavlink::common::msg::PLAY_TUNE_V2{};
        m_uas->msg_set_target(msg);
        msg.format = tune->format;
        mavlink::set_string_z(msg.tune, tune->tune);
        UAS_FCU(m_uas)->send_message_ignore_drop(msg);
    }

    void acfly_default_tune_cb(const std_msgs::UInt8::ConstPtr &ind) {
        // TODO: 添加acfly默认音乐
        switch (ind->data) {
        case 0:
            break;

        default:
            break;
        }
    }
};
} // namespace extra_plugins
} // namespace mavros

#include <pluginlib/class_list_macros.h>
PLUGINLIB_EXPORT_CLASS(mavros::extra_plugins::PlayTunePlugin, mavros::plugin::PluginBase)
