#include <android/log.h>
#include <jni.h>
#include <stddef.h>
#include <pthread.h>


#define LOG_TAG    "leak"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

extern "C" {
JNIEXPORT void JNICALL
Java_org_arguslab_native_1leak_MainActivity_send(JNIEnv *env, jobject thisObj, jstring data);

}

const char *getCharFromString(JNIEnv *env, jstring string) {
    if (string == NULL)
        return NULL;

    return env->GetStringUTFChars(string, 0);
}
int cnt = 0;
int THR = 10, THR2 = 5;
void *run_by_thread1(void *arg) {
    int* threadNum = (int*)arg;
    pthread_mutex_lock(&mux);
    while (cnt != THR2) {
    LOGI(1, "thread %d: about to wait", *threadNum);
    pthread_cond_wait(&cond, &mux);
    }
    ++cnt;
    LOGI(1, "thread %d: cnt = %d", *threadNum, cnt);
    pthread_mutex_unlock(&mux);
}

void *run_by_thread2(void *arg) {
    int* threadNum = (int*)arg;
    while (cnt < THR) {
    pthread_mutex_lock(&mux);
    if (cnt == THR2) {
    pthread_cond_signal(&cond);
    } else {
    ++cnt;
    LOGI(1, "thread %d: cnt = %d", *threadNum, cnt);
    }
    pthread_mutex_unlock(&mux);
    sleep(1);
    }
}
JNIEXPORT void JNICALL
Java_org_arguslab_native_1leak_MainActivity_send(JNIEnv *env, jobject thisObj, jstring data) {
    pthread_mutex_t mux;
    pthread_cond_t cond;
    pthread_t th1, th2;
    int threadNum1 = 1, threadNum2 = 2;
    int ret;
    pthread_mutex_init(&mux, NULL);
    pthread_cond_init(&cond, NULL);
    ret = pthread_create(&th1, NULL, run_by_thread1,
    void*)&threadNum1);
    LOGI(1, "thread 1 started");
    ret = pthread_create(&th2, NULL, run_by_thread2,
    void*)&threadNum2);
    LOGI(1, "thread 2 started");
    ret = pthread_join(th1, NULL);
    LOGI(1, "thread 1 end %d", ret);
    ret = pthread_join(th2, NULL);
    LOGI(1, "thread 2 end %d", ret);
    pthread_mutex_destroy(&mux);
    pthread_cond_destroy(&cond);
    LOGI("%s", getCharFromString(env, data)); // leak
    return;
}
