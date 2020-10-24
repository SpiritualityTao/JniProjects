#include <jni.h>
#include <string>
#include <android/log.h>
#include <pthread.h>

#define TAG "JniProjects"
// __VA_ARGS__ 代表 ...的可变参数
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG,  __VA_ARGS__);
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG,  __VA_ARGS__);
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG,  __VA_ARGS__);

const char *classPathName = "com/peter/study/jniprojects/MainActivity";

extern "C" JNIEXPORT jstring JNICALL
Java_com_peter_study_jniprojects_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT void JNICALL
Java_com_peter_study_jniprojects_MainActivity_nativeTransmit(JNIEnv *env, jobject thiz, jboolean b,
                                                             jbyte b1, jchar c, jshort s, jlong l,
                                                             jfloat f, jdouble d, jstring name,
                                                             jint age, jintArray i_,
                                                             jobjectArray strs, jobject person,
                                                             jbooleanArray bArray_) {
    // 1. 接受Java传递过来的boolean值
    unsigned char b_boolean = b;
    LOGD("boolean->%d", b_boolean);

    // 2. byte
    char b_byte = b1;
    LOGD("byte->%d", b_byte);

    // 3. char
    unsigned short c_char = c;
    LOGD("char->%d", c_char);

    // 4. short
    short s_short = s;
    LOGD("short->%d", s_short);

    // 5. long
    long l_long = l;
    LOGD("long->%d", l_long);

    // 6. float
    float f_float = f;
    LOGD("float->%f", f_float);

    // 7. double
    double d_double = d;
    LOGD("double->%f", d_double);

    // 8. String
    const char* str_name = env->GetStringUTFChars(name, 0);
    LOGD("string->%s", str_name);

    // 9. int
    int i_age = age;
    LOGD("int:%d", i_age);

    // 10. int[]
    jint *intArray = env->GetIntArrayElements(i_, NULL);
    jsize intArrayLength = env->GetArrayLength(i_);
    for (int i = 0; i < intArrayLength; ++i) {
        LOGD("intArray[%d]->%d", i, intArray[i]);
    }
    //释放数组
    env->ReleaseIntArrayElements(i_, intArray, 0);

    // 11. String[]
    jsize stringArrayLength = env->GetArrayLength(strs);
    for (int i = 0; i < stringArrayLength; ++i) {
        jobject jobject1 = env->GetObjectArrayElement(strs, i);
        // 强转JNI string
        jstring stringArrayData = static_cast<jstring>(jobject1);
        // 转C string
        const char* itemstr = env->GetStringUTFChars(stringArrayData, NULL);
        LOGD("String[%d]:%s", i, itemstr);
        env->ReleaseStringUTFChars(stringArrayData, itemstr);
    }

    // 12. Object
    // 12.1 获取字节码
    const char* person_class_str = "com/peter/study/jniprojects/Person";
    // 12.2 转jni class
    jclass person_class = env->FindClass(person_class_str);
    // 12.3 拿到方法签名 javap -a
    const char *sig = "()Ljava/lang/String;";
    jmethodID jmethodId = env->GetMethodID(person_class, "getName", "()Ljava/lang/String;");

    jobject obj_string = env->CallObjectMethod(person, jmethodId);
    jstring person_stirng = static_cast<jstring>(obj_string);
    const char* itemStr2 = env->GetStringUTFChars(person_stirng, NULL);
    LOGD("Person:%s",itemStr2);
    env->DeleteLocalRef(person_class);
    env->DeleteLocalRef(person);

    // 13. 打印 Java 传递过来的 booleanArray
    jsize bArrayLength = env->GetArrayLength(bArray_);
    jboolean *bArray = env->GetBooleanArrayElements(bArray_, NULL);
    for (int i = 0; i < bArrayLength; ++i) {
        bool b = bArray[i];
        jboolean b2 = bArray[i];
        LOGD("bool:%d", b)
        LOGD("jboolean:%d", b2);
    }
    env->ReleaseBooleanArrayElements(bArray_, bArray, 0);

}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_peter_study_jniprojects_MainActivity_getPerson(JNIEnv *env, jobject thiz) {
    // 1. Person 类全路径
    const char* person_class_str = "com/peter/study/jniprojects/Person";
    const char* method = "<init>";

    // 2. 拿到类和方法
    jclass person_class = env->FindClass(person_class_str);
    jmethodID person_construct = env->GetMethodID(person_class, method, "()V");

    // 3. 创建对象
    jobject person_obj = env->NewObject(person_class, person_construct);

    // 4. n拿到setName和setAge方法
    const char *nameSig = "(Ljava/lang/String;)V";
    jmethodID nameMethodId = env->GetMethodID(person_class, "setName", nameSig);

    const char *ageSig = "(I)V";
    jmethodID ageMethodId = env->GetMethodID(person_class, "setAge", ageSig);

    // 5. 调用方法
    const char *name = "PeterPeng";
    jstring newStringName = env->NewStringUTF(name);
    env->CallVoidMethod(person_obj, nameMethodId, newStringName);
    env->CallVoidMethod(person_obj, ageMethodId, 26);

    // 6. toString 方法
    const char *toStringSig = "()Ljava/lang/String;";
    jmethodID toStringMethodId = env->GetMethodID(person_class, "toString", toStringSig);
    jobject obj_String = env->CallObjectMethod(person_obj, toStringMethodId);
    jstring perStr = static_cast<jstring>(obj_String);
    const char *itemStr2 = env->GetStringUTFChars(perStr, NULL);
    LOGD("Native Person:%s",itemStr2);

    return person_obj;
}

extern "C"
JNIEXPORT void JNICALL
native_dynamicRegister(JNIEnv *env, jobject thiz, jstring name) {
    const char *jname = env->GetStringUTFChars(name, NULL);
    LOGD("动态注册：%s", jname);
    env->ReleaseStringUTFChars(name, jname);
}

extern "C"
JNIEXPORT void JNICALL
native_handleException(JNIEnv *env, jobject thiz, jstring name) {
    const char *jname = env->GetStringUTFChars(name, NULL);
    LOGD("动态注册：%s", jname);

    jclass clazz = env->GetObjectClass(thiz);
    //执行 Java 测试抛出异常的代码
    jmethodID mid = env->GetMethodID(clazz, "testException", "()V");
    // 执行会抛出一个异常
    env->CallVoidMethod(thiz, mid);
    // 执行会抛出一个异常
    jthrowable exc = env->ExceptionOccurred();
    if(exc) { // 发生
        // 打印异常
        env->ExceptionDescribe();
        // 清楚异常
        env->ExceptionClear();
        jclass newExcCls = env->FindClass("java/lang/IllegalArgumentException");
        //返回一个新的异常到Java
        env->ThrowNew(newExcCls, "Jni抛出一个异常");
    }

    env->ReleaseStringUTFChars(name, jname);
}

extern "C"
JNIEXPORT void JNICALL
native_count(JNIEnv *env, jobject thiz) {
    jclass cls = env->GetObjectClass(thiz);
    jfieldID fieldID = env->GetFieldID(cls, "count", "I");

    if (env->MonitorEnter(thiz) != JNI_OK) {
        LOGE("%s: MonitorEnter() failed", __FUNCTION__);
    }

    /* synchronized block */
    int val = env->GetIntField(thiz, fieldID);
    val++;
    LOGI("Native count=%d", val);
    env->SetIntField(thiz, fieldID, val);

    if (env->ExceptionOccurred()) {
        LOGE("ExceptionOccurred()...");
        if (env->MonitorExit(thiz) != JNI_OK) {
            LOGE("%s: MonitorExit() failed", __FUNCTION__);
        };
    }

    if (env->MonitorExit(thiz) != JNI_OK) {
        LOGE("%s: MonitorExit() failed", __FUNCTION__);
    };
}


JavaVM * jvm;
jobject instance;
void * customThread(void * pVoid) {
    // 调用的话，一定需要JNIEnv *env
    // JNIEnv *env 无法跨越线程，只有JavaVM才能跨越线程

    JNIEnv * env = NULL; // 全新的env
    int result = jvm->AttachCurrentThread(&env, 0); // 把native的线程，附加到JVM
    if (result != 0) {
        return 0;
    }

    jclass mainActivityClass = env->GetObjectClass(instance);

    // 拿到MainActivity的updateUI
    const char * sig = "()V";
    jmethodID updateUI = env->GetMethodID(mainActivityClass, "updateUI", sig);

    env->CallVoidMethod(instance, updateUI);

    // 解除 附加 到 JVM 的native线程
    jvm->DetachCurrentThread();

    return 0;
}

extern "C"  //支持 C 语言
JNIEXPORT void JNICALL //告诉虚拟机，这是jni函数
native_testOriginalThread(JNIEnv *env, jobject thiz) {
    instance = env->NewGlobalRef(thiz); // 全局的，就不会被释放，所以可以在线程里面用
    // 如果是非全局的，函数一结束，就被释放了
    pthread_t pthreadID;
    pthread_create(&pthreadID, 0, customThread, instance);
    pthread_join(pthreadID, 0);

}

extern "C"  //支持 C 语言
JNIEXPORT void JNICALL //告诉虚拟机，这是jni函数
native_unThread(JNIEnv *env, jobject thiz) {

    if (NULL != instance) {
        env->DeleteGlobalRef(instance);
        instance = NULL;
    }

}

/* 源码结构体
 * typedef struct {
    const char* name;
    const char* signature;
    void*       fnPtr;
    } JNINativeMethod;
 */
static const JNINativeMethod jniNativeMethod[] = {
        {"dynamicRegister", "(Ljava/lang/String;)V", (void *) (native_dynamicRegister)},
        {"handleException", "(Ljava/lang/String;)V", (void *) (native_handleException)},
        {"nativeCount", "()V", (void *) (native_count)},
        {"unThread", "()V", (void *) (native_unThread)},
        {"testOriginalThread", "()V", (void *) (native_testOriginalThread)}
};

/**
 * jni.h 文件中，System.loadLibrary会调用此方法
 * @param vm
 * @param unused
 * @return
 */
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *unused) {

    jvm = vm;
    // 通过虚拟机创建新的env
    JNIEnv *jniEnv = nullptr;
    jint result = vm->GetEnv(reinterpret_cast<void **>(&jniEnv), JNI_VERSION_1_6);
    if (result != JNI_OK) {
       return JNI_ERR;
    }
    jclass mainActivityClass = jniEnv->FindClass(classPathName);
    jniEnv->RegisterNatives(mainActivityClass, jniNativeMethod, sizeof(jniNativeMethod) / sizeof(JNINativeMethod));

    return JNI_VERSION_1_6;

}

