#include <jni.h>
#include <string>
#include <android/log.h>

#define TAG "JniProjects"
// __VA_ARGS__ 代表 ...的可变参数
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG,  __VA_ARGS__);

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