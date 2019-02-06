#include "jniUtils.h"
#ifdef __ANDROID__
#include <jni.h>
#endif
#include <SDL.h>

void jniCallVoidMethodNoArgs(const std::string& methodName)
{
    #ifdef __ANDROID__
    JNIEnv* env = (JNIEnv*)SDL_AndroidGetJNIEnv();

    // retrieve the Java instance of the SDLActivity
    jobject activity = (jobject)SDL_AndroidGetActivity();

    // find the Java class of the activity. It should be SDLActivity or a subclass of it.
    jclass clazz(env->GetObjectClass(activity));

    // find the identifier of the method to call
    jmethodID method_id = env->GetMethodID(clazz, methodName.c_str(), "()V");

    // effectively call the Java method
    env->CallVoidMethod(activity, method_id);

    // clean up the local references.
    env->DeleteLocalRef(activity);
    env->DeleteLocalRef(clazz);
    #else
    SDL_Log("attempt to call android specific function, callVoidMethodNoArgs");
    #endif
}
