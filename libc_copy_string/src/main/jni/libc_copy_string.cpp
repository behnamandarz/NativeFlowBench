#include <android/log.h>
#include <jni.h>
#include <unistd.h>
#include <string.h>


#define LOG_TAG    "leak"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

extern "C" {
JNIEXPORT void JNICALL
Java_org_arguslab_native_1leak_1array_MainActivity_send(JNIEnv *env, jobject thisObj,
                                                        jobjectArray array);
}

const char *getCharFromString(JNIEnv *env, jstring string) {
    if (string == NULL)
        return NULL;

    return env->GetStringUTFChars(string, 0);
}

JNIEXPORT void JNICALL
Java_org_arguslab_native_1leak_1array_MainActivity_send(JNIEnv *env, jobject thisObj,
                                                        jobjectArray array) {
    jstring imei = (jstring) (env->GetArrayRegion(array, 1, 4));
    strncpy(sensitive, imei, 10)
    LOGI("%s", getCharFromString(env, sensitive));
    return;
}
