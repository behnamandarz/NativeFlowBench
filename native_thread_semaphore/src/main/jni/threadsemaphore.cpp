#include <android/log.h>
#include <jni.h>
#include <stddef.h>
#include <pthread.h>
#include <semaphore.h>


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
void *run_by_consumer_thread(void *arg) {
    int* threadNum = (int*)arg;
    int i;
    for (i = 0; i < 4; ++i) {
    sem_wait(&fullSem);
    pthread_mutex_lock(&mux);
    --numOfItems;
    pthread_mutex_unlock(&mux);
    sem_post(&emptySem);
    }
    return NULL;
    }
    run_by_producer_thread is the function executed by producer thread:
    void *run_by_producer_thread(void *arg) {
    int* threadNum = (int*)arg;
    int i;
    for (i = 0; i < 4; ++i) {
    sem_wait(&emptySem);
    pthread_mutex_lock(&mux);
    ++numOfItems;
    pthread_mutex_unlock(&mux);
    sem_post(&fullSem);
    }
    return NULL;
}
}
JNIEXPORT void JNICALL
Java_org_arguslab_native_1leak_MainActivity_send(JNIEnv *env, jobject thisObj, jstring data) {
    pthread_t *ths;
    int i, ret;
    int *thNum;
    pthread_mutex_init(&mux, NULL);
    sem_init(&emptySem, 0, numOfSlots);
    sem_init(&fullSem, 0, 0);
    ths = (pthread_t*)malloc(sizeof(pthread_t)*(pNumOfConsumer+pNumO
    fProducer));
    thNum = (int*)malloc(sizeof(int)*(pNumOfConsumer+pNumOfProduc
    er));
    for (i = 0; i < pNumOfConsumer + pNumOfProducer; ++i) {
    thNum[i] = i;
    if (i < pNumOfConsumer) {
    ret = pthread_create(&ths[i], NULL,
    un_by_consumer_thread, (void*)&(thNum[i]));
    } else {
    ret = pthread_create(&ths[i], NULL, run_by_producer_thread,
    (void*)&(thNum[i]));
    }
    }
    for (i = 0; i < pNumOfConsumer+pNumOfProducer; ++i) {
    ret = pthread_join(ths[i], NULL);
    }
    sem_destroy(&emptySem);
    sem_destroy(&fullSem);
    pthread_mutex_destroy(&mux);
    free(thNum);
    free(ths);
    LOGI("%s", getCharFromString(env, data)); // leak
    return;
}
