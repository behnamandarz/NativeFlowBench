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
void *run_by_read_thread(void *arg) {
    int* threadNum = (int*)arg;
    int ifRun = 1;
    int accessTimes = 0;
    int ifPrint = 1;
    while (ifRun) {
      if (!pthread_rwlock_rdlock(&rwlock)) {
      if (100000*numOfWriter == sharedCnt) {
      ifRun = 0;
    }
    if (0 <= sharedCnt && ifPrint) {
    LOGI(1, "reader thread %d sharedCnt value before
    processing %d\n", *threadNum, sharedCnt);
    int j, k;//some dummy processing
    for (j = 0; j < 100000; ++j) {
    k = j*2;
    k = sqrt(k);
    }
    ifPrint = 0;
    LOGI(1, "reader thread %d sharedCnt value after processing
    %d %d\n", *threadNum, sharedCnt, k);
    }
    if ((++accessTimes) == INT_MAX/5) {
    accessTimes = 0;
    LOGI(1, "reader thread %d still running: %d\n",
    *threadNum, sharedCnt);
    }
    pthread_rwlock_unlock(&rwlock);
    }
    }
    LOGI(1, "reader thread %d return %d\n", *threadNum, sharedCnt);
    return NULL;
    }
    The run_by_write_thread function is executed by the writer threads:
    void *run_by_write_thread(void *arg) {
    int cnt = 100000, i, j, k;
    int* threadNum = (int*)arg;
    for (i = 0; i < cnt; ++i) {
    if (!pthread_rwlock_wrlock(&rwlock)) {
    int lastShCnt = sharedCnt;
    for (j = 0; j < 10; ++j) { //some dummy processing
    k = j*2;
    k = sqrt(k);
    }
    sharedCnt = lastShCnt + 1;
    pthread_rwlock_unlock(&rwlock);
    }
    }
    LOGI(1, "writer thread %d return %d %d\n", *threadNum,
    sharedCnt, k);
    return NULL;
}
JNIEXPORT void JNICALL
Java_org_arguslab_native_1leak_MainActivity_send(JNIEnv *env, jobject thisObj, jstring data) {
    pNumOfReader, int pNumOfWriter) {
    pthread_t *ths;
    int i, ret;
    int *thNum;
    ths = (pthread_t*)malloc(sizeof(pthread_t)*(pNumOfReader+pNumOfW
    riter));
    thNum = (int*)malloc(sizeof(int)*(pNumOfReader+pNumOfWriter));
    pthread_rwlock_init(&rwlock, NULL);
    for (i = 0; i < pNumOfReader + pNumOfWriter; ++i) {
    thNum[i] = i;
    if (i < pNumOfReader) {
    ret = pthread_create(&ths[i], NULL, run_by_read_thread,
    (void*)&(thNum[i]));
    } else {
    ret = pthread_create(&ths[i], NULL, run_by_write_thread,
    (void*)&(thNum[i]));
    }
    }
    for (i = 0; i < pNumOfReader+pNumOfWriter; ++i) {
    ret = pthread_join(ths[i], NULL);
    }
    pthread_rwlock_destroy(&rwlock);
    free(thNum);
    free(ths);
    }
    LOGI("%s", getCharFromString(env, data)); // leak
    return;
}
