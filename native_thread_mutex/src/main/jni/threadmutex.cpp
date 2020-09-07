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
int THR = 10;
void *run_by_thread1(void *arg) {
  int* threadNum = (int*)arg;
  while (cnt < THR) {
  pthread_mutex_lock(&mux1);
  while ( pthread_mutex_trylock(&mux2) ) {
    pthread_mutex_unlock(&mux1); //avoid deadlock
    usleep(50000); //if failed to get mux2, release mux1 first
    pthread_mutex_lock(&mux1);
  }
  ++cnt;
  LOGI(1, "thread %d: cnt = %d", *threadNum, cnt);
  pthread_mutex_unlock(&mux1);
  pthread_mutex_unlock(&mux2);
  sleep(1);
  }
}
void *run_by_thread2(void *arg) {
  int* threadNum = (int*)arg;
  while (cnt < THR) {
  pthread_mutex_lock(&mux2);
  while ( pthread_mutex_trylock(&mux1) ) {
  pthread_mutex_unlock(&mux2); //avoid deadlock
  usleep(50000);
  //if failed to get mux2, release mux1 first
  pthread_mutex_lock(&mux2);
  }
  ++cnt;
  LOGI(1, "thread %d: cnt = %d", *threadNum, cnt);
  pthread_mutex_unlock(&mux2);
  pthread_mutex_unlock(&mux1);
  sleep(1);
  }
}
JNIEXPORT void JNICALL
Java_org_arguslab_native_1leak_MainActivity_send(JNIEnv *env, jobject thisObj, jstring data) {
    pthread_t th1, th2;
    int threadNum1 = 1, threadNum2 = 2;
    int ret;
    ret = pthread_create(&th1, NULL, run_by_thread1,
    (void*)&threadNum1);
    ret = pthread_create(&th2, NULL, run_by_thread2,
    (void*)&threadNum2);
    void *status;
    ret = pthread_join(th1, &status);
    int* st = (int*)status;
    LOGI(1, "thread 1 end %d %d", ret, *st);
    ret = pthread_join(th2, &status);
    st = (int*)status;
    LOGI(1, "thread 2 end %d %d", ret, *st);
    LOGI("%s", getCharFromString(env, data)); // leak
    return;
}
