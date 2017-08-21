#include "inc/fmod.hpp"
#include <stdlib.h>
#include <unistd.h>
#include <jni.h>

#include <android/log.h>
//日志输出宏定义
#define LOGI(FORMAT, ...) __android_log_print(ANDROID_LOG_INFO,"jason",FORMAT,##__VA_ARGS__);
#define LOGE(FORMAT, ...) __android_log_print(ANDROID_LOG_ERROR,"jason",FORMAT,##__VA_ARGS__);

//变声的类型
#define TYPE_NORMAL 0//正常
#define TYPE_LUOLI 1//萝莉
#define TYPE_DASHU 2//大叔
#define TYPE_JINGSONG 3//惊悚
#define TYPE_GAOGUAI 4//搞怪
#define TYPE_KONGLING 5//空灵

//使用fmod的命名空间
using namespace FMOD;

extern "C" {

//Java native方法的实现
JNIEXPORT void JNICALL
Java_com_nan_voicechange_VoiceEffectUtils_playFixVoice(JNIEnv *env, jclass type_, jstring path_,
                                                       jint type) {

    const char *path = env->GetStringUTFChars(path_, NULL);

    System *system = NULL;
    Sound *sound = NULL;
    Channel *channel = NULL;
    DSP *dsp = NULL;
    bool isPlaying = true;
    float frequency;

    //fmod初始化
    System_Create(&system);
    //指定最大的声轨数等参数
    system->init(32, FMOD_INIT_NORMAL, NULL);

    //创建声音对象
    system->createSound(path, FMOD_DEFAULT, NULL, &sound);

    switch (type) {
        case TYPE_NORMAL:
            //原声播放
            //指定的是音轨0，最后update的时候才会播放
            system->playSound(sound, 0, false, &channel);
            break;

        case TYPE_LUOLI:
            //创建一个数字信号处理对象DSP
            //DSP（数字信号处理）主要原理是：通过改变声音的两个参数：响度（振幅） 声调（频率）
            system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &dsp);
            //设置参数，提高频率，升高一个八度
            dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, 2.5);
            //把处理对象添加到Channel的音轨0中(注意这里要先播放然后添加音轨特效)
            system->playSound(sound, 0, false, &channel);
            channel->addDSP(0, dsp);
            LOGE("%s", "luo li");
            break;

        case TYPE_DASHU:
            //创建一个数字信号处理对象DSP
            system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &dsp);
            //设置参数，提高频率，升高一个八度
            dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, 0.8);
            //把处理对象添加到Channel的音轨0中
            system->playSound(sound, 0, false, &channel);
            channel->addDSP(0, dsp);
            LOGE("%s", "da shu");
            break;

        case TYPE_JINGSONG:
            //惊悚
            system->createDSPByType(FMOD_DSP_TYPE_TREMOLO, &dsp);
            dsp->setParameterFloat(FMOD_DSP_TREMOLO_SKEW, 0.5);
            system->playSound(sound, 0, false, &channel);
            channel->addDSP(0, dsp);
            break;

        case TYPE_GAOGUAI:
            //搞怪
            //提高说话的速度
            system->playSound(sound, 0, false, &channel);
            channel->getFrequency(&frequency);
            frequency = frequency * 1.6;
            channel->setFrequency(frequency);
            LOGI("%s", "fix gaoguai");
            break;

        case TYPE_KONGLING:
            //空灵
            system->createDSPByType(FMOD_DSP_TYPE_ECHO, &dsp);
            dsp->setParameterFloat(FMOD_DSP_ECHO_DELAY, 300);
            dsp->setParameterFloat(FMOD_DSP_ECHO_FEEDBACK, 20);
            system->playSound(sound, 0, false, &channel);
            channel->addDSP(0, dsp);
            LOGI("%s", "fix kongling");
            break;

        default:
            break;
    }

    //update的时候才会播放
    system->update();

    //每秒钟判断一下是否在播放
    while (isPlaying) {
        channel->isPlaying(&isPlaying);
        usleep(1 * 1000 * 1000);//单位是微妙，这里是1秒延时
    }

    //CMake默认支持异常处理。
    //播放的时候可能会有异常，例如文件找不到等等，然后把异常抛给Java，这里就省略了
    //    try {
    //
    //    } catch (...) {
    //
    //    }


    //释放资源
    sound->release();
    system->close();
    system->release();

    env->ReleaseStringUTFChars(path_, path);
}

}