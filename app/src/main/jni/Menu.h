#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wexceptions"
#include <sstream>
#include <list>
#include <vector>
#include <string.h>
#include <pthread.h>
#include <cstring>
#include <jni.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <dlfcn.h>
#include <regex>
#include <jni.h>
#include <android/log.h>
#include <android/api-level.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;


bool titleValid, headingValid, iconValid, settingsValid, isLeeched;

int __system_property_get(const char* __name, char* __value);
int atoi(const char* __s) __attribute_pure__;

int api_level() {
    char value[92] = { 0 };
    if (__system_property_get("ro.build.version.sdk", value) < 1) return -1;
    int api_level = atoi(value);
    return (api_level > 0) ? api_level : -1;
}

void setText(JNIEnv *env, jobject obj, const char* text){
    jclass html = (env)->FindClass(OBFUSCATE("android/text/Html"));
    jmethodID fromHtml = (env)->GetStaticMethodID(html, OBFUSCATE("fromHtml"), OBFUSCATE("(Ljava/lang/String;)Landroid/text/Spanned;"));
    jclass textView = (env)->FindClass(OBFUSCATE("android/widget/TextView"));
    jmethodID setText = (env)->GetMethodID(textView, OBFUSCATE("setText"), OBFUSCATE("(Ljava/lang/CharSequence;)V"));

    jstring jstr = (env)->NewStringUTF(text);
    env->CallVoidMethod(obj, setText,  (*env).CallStaticObjectMethod(html, fromHtml, jstr));
}

void setTitleText(JNIEnv *env, jobject thiz, jobject obj) {
    setText(env, obj, OBFUSCATE("<b>Modded by (yourname)</b>"));
    titleValid = true;
}

void setHeadingText(JNIEnv *env, jobject thiz, jobject obj) {
    setText(env, obj, OBFUSCATE("<b><marquee><p style=\"font-size:30\"><p style=\"color:green;\">Modded by LGL</p> | https://github.com/LGLTeam | Lorem Ipsum is simply dummy text of the printing and typesetting</p></marquee></b>"));
    headingValid = true;
}

jstring Icon(JNIEnv *env, jobject thiz) {
    iconValid = true;

    return env->NewStringUTF(
            OBFUSCATE("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"));
}

jstring IconWebViewData(JNIEnv *env, jobject thiz) {
    iconValid = true;
   return NULL;
}

jobjectArray settingsList(JNIEnv *env, jobject activityObject) {
    jobjectArray ret;

    const char *features[] = {
            OBFUSCATE("Category_Settings"),
            OBFUSCATE("-1_Toggle_Save feature preferences"), //-1 is checked on Preferences.java
            OBFUSCATE("-3_Toggle_Auto size vertically"),
            OBFUSCATE("Category_Logcat"),
            OBFUSCATE("RichTextView_Save logcat if a bug occured and sent it to the modder. Clear logcat and reproduce bug again if the log file is too large"),
            OBFUSCATE("RichTextView_<small>Saving logcat does not need file permission. Logcat location:"
                            "<br/>Android 11: /storage/emulated/0/Documents/"
                            "<br/>Android 10 and below: /storage/emulated/0/Android/data/(package name)/files/Mod Menu</small>"),
            OBFUSCATE("-4_Button_Save logcat to file"),
            OBFUSCATE("-5_Button_Clear logcat"),
            OBFUSCATE("Category_Menu"),
            OBFUSCATE("-6_Button_<font color='red'>Close settings</font>"),
    };

    int Total_Feature = (sizeof features /
                         sizeof features[0]);
    ret = (jobjectArray)
            env->NewObjectArray(Total_Feature, env->FindClass(OBFUSCATE("java/lang/String")),
                                env->NewStringUTF(""));
    int i;
    for (i = 0; i < Total_Feature; i++)
        env->SetObjectArrayElement(ret, i, env->NewStringUTF(features[i]));

    settingsValid = true;

    return (ret);
}

void setDialog(jobject ctx, JNIEnv *env, const char *title, const char *msg){
    jclass Alert = env->FindClass(OBFUSCATE("android/app/AlertDialog$Builder"));
    jmethodID AlertCons = env->GetMethodID(Alert, OBFUSCATE("<init>"), OBFUSCATE("(Landroid/content/Context;)V"));

    jobject MainAlert = env->NewObject(Alert, AlertCons, ctx);

    jmethodID setTitle = env->GetMethodID(Alert, OBFUSCATE("setTitle"), OBFUSCATE("(Ljava/lang/CharSequence;)Landroid/app/AlertDialog$Builder;"));
    env->CallObjectMethod(MainAlert, setTitle, env->NewStringUTF(title));

    jmethodID setMsg = env->GetMethodID(Alert, OBFUSCATE("setMessage"), OBFUSCATE("(Ljava/lang/CharSequence;)Landroid/app/AlertDialog$Builder;"));
    env->CallObjectMethod(MainAlert, setMsg, env->NewStringUTF(msg));

    jmethodID setCa = env->GetMethodID(Alert, OBFUSCATE("setCancelable"), OBFUSCATE("(Z)Landroid/app/AlertDialog$Builder;"));
    env->CallObjectMethod(MainAlert, setCa, false);

    jmethodID setPB = env->GetMethodID(Alert, OBFUSCATE("setPositiveButton"), OBFUSCATE("(Ljava/lang/CharSequence;Landroid/content/DialogInterface$OnClickListener;)Landroid/app/AlertDialog$Builder;"));
    env->CallObjectMethod(MainAlert, setPB, env->NewStringUTF("Ok"), static_cast<jobject>(NULL));

    jmethodID create = env->GetMethodID(Alert, OBFUSCATE("create"), OBFUSCATE("()Landroid/app/AlertDialog;"));
    jobject creaetob = env->CallObjectMethod(MainAlert, create);

    jclass AlertN = env->FindClass(OBFUSCATE("android/app/AlertDialog"));

    jmethodID show = env->GetMethodID(AlertN, OBFUSCATE("show"), OBFUSCATE("()V"));
    env->CallVoidMethod(creaetob, show);
}

void Toastx(JNIEnv *env, jobject thiz, const char *text, int length) {
    jstring jstr = env->NewStringUTF(text);
    jclass toast = env->FindClass(OBFUSCATE("android/widget/Toast"));
    jmethodID methodMakeText =env->GetStaticMethodID(toast,OBFUSCATE("makeText"),OBFUSCATE("(Landroid/content/Context;Ljava/lang/CharSequence;I)Landroid/widget/Toast;"));
    jobject toastobj = env->CallStaticObjectMethod(toast, methodMakeText,thiz, jstr, length);
    jmethodID methodShow = env->GetMethodID(toast, OBFUSCATE("show"), OBFUSCATE("()V"));
    env->CallVoidMethod(toastobj, methodShow);
}

void startActivityPermisson(JNIEnv *env, jobject ctx){
    jclass native_context = env->GetObjectClass(ctx);
    jmethodID startActivity = env->GetMethodID(native_context, OBFUSCATE("startActivity"),OBFUSCATE("(Landroid/content/Intent;)V"));

    jmethodID pack = env->GetMethodID(native_context, OBFUSCATE("getPackageName"),OBFUSCATE("()Ljava/lang/String;"));
    jstring packageName = static_cast<jstring>(env->CallObjectMethod(ctx, pack));

    const char *pkg = env->GetStringUTFChars(packageName, 0);

    std::stringstream pkgg;
    pkgg << OBFUSCATE("package:");
    pkgg << pkg;
    std::string pakg = pkgg.str();

    jclass Uri = env->FindClass(OBFUSCATE("android/net/Uri"));
    jmethodID Parce = env->GetStaticMethodID(Uri, OBFUSCATE("parse"), OBFUSCATE("(Ljava/lang/String;)Landroid/net/Uri;"));
    jobject UriMethod = env->CallStaticObjectMethod(Uri, Parce, env->NewStringUTF(pakg.c_str()));

    jclass intentclass = env->FindClass(OBFUSCATE("android/content/Intent"));
    jmethodID newIntent = env->GetMethodID(intentclass, OBFUSCATE("<init>"), OBFUSCATE("(Ljava/lang/String;Landroid/net/Uri;)V"));
    jobject intent = env->NewObject(intentclass,newIntent,env->NewStringUTF(OBFUSCATE("android.settings.action.MANAGE_OVERLAY_PERMISSION")), UriMethod);

    env->CallVoidMethod(ctx, startActivity, intent);
}

void startService(JNIEnv *env, jobject ctx){
    jclass native_context = env->GetObjectClass(ctx);
    jclass intentClass = env->FindClass(OBFUSCATE("android/content/Intent"));
    jclass actionString = env->FindClass(OBFUSCATE("com/android/support/Menu"));
    jmethodID newIntent = env->GetMethodID(intentClass, OBFUSCATE("<init>"), OBFUSCATE("(Landroid/content/Context;Ljava/lang/Class;)V"));
    jobject intent = env->NewObject(intentClass,newIntent,ctx,actionString);
    jmethodID startActivityMethodId = env->GetMethodID(native_context, OBFUSCATE("startService"), OBFUSCATE("(Landroid/content/Intent;)Landroid/content/ComponentName;"));
    env->CallObjectMethod(ctx, startActivityMethodId, intent);
}

void startActivity(JNIEnv *env, jobject ctx){
    jclass native_context = env->GetObjectClass(ctx);
    jclass MainActivity = env->FindClass("com/unity3d/player/UnityPlayerActivity");
    jmethodID startActivityMethod = env->GetMethodID(native_context, "startActivity", "(Landroid/content/Intent;)V");
    jclass intentClass = env->FindClass("android/content/Intent");
    jmethodID intentConstructor = env->GetMethodID(intentClass, "<init>", "(Landroid/content/Context;Ljava/lang/Class;)V");
    jfieldID contextField = env->GetFieldID(native_context, "mContext", "Landroid/content/Context;");
    jobject context = env->GetObjectField(ctx, contextField);
    jobject intentObject = env->NewObject(intentClass, intentConstructor, context, MainActivity);
    env->CallVoidMethod(ctx, startActivityMethod, intentObject);
}

jboolean isNotInGame(JNIEnv *env, jobject context) {
    jclass contextClass = env->GetObjectClass(context);
    jclass activityManagerClass = env->FindClass("android/app/ActivityManager");
    jmethodID getSystemServiceMethod = env->GetMethodID(contextClass, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
    jobject activityManager = env->CallObjectMethod(context, getSystemServiceMethod, env->NewStringUTF("activity"));
    jmethodID getRunningAppProcessesMethod = env->GetMethodID(activityManagerClass, "getRunningAppProcesses", "()Ljava/util/List;");
    jobject runningAppProcesses = env->CallObjectMethod(activityManager, getRunningAppProcessesMethod);
    jclass listClass = env->FindClass("java/util/List");
    jmethodID sizeMethod = env->GetMethodID(listClass, "size", "()I");
    jint size = env->CallIntMethod(runningAppProcesses, sizeMethod);
    jmethodID getMethod = env->GetMethodID(listClass, "get", "(I)Ljava/lang/Object;");

    jint maxImportance = 0;
    jint myPid = getpid();

    for (int i = 0; i < size; i++) {
        jobject runningAppProcessInfo = env->CallObjectMethod( runningAppProcesses, getMethod, i);
        jclass runningAppProcessInfoClass = env->GetObjectClass( runningAppProcessInfo);
        jfieldID importanceField = env->GetFieldID( runningAppProcessInfoClass, "importance", "I");
        jint importance = env->GetIntField( runningAppProcessInfo, importanceField);
        jfieldID pidField = env->GetFieldID( runningAppProcessInfoClass, "pid", "I");
        jint pid = (env)->GetIntField( runningAppProcessInfo, pidField);

        if (importance > maxImportance) {
            maxImportance = importance;
        }

        if (pid == myPid) {
            return maxImportance != 100;
        }
    }

    return JNI_TRUE;
}

jint dp(JNIEnv* env, jobject obj, jint i) {
    jclass typedValueClass = env->FindClass("android/util/TypedValue");
    jmethodID applyDimensionMethod = env->GetStaticMethodID(typedValueClass, "applyDimension","(IFLandroid/util/DisplayMetrics;)F");

    jclass mainActivityClass = env->GetObjectClass(obj);
    jmethodID getResourcesMethod = env->GetMethodID(mainActivityClass, "getResources", "()Landroid/content/res/Resources;");
    jobject resources = env->CallObjectMethod(obj, getResourcesMethod);

    jclass resourcesClass = env->GetObjectClass(resources);
    jmethodID getDisplayMetricsMethod = env->GetMethodID(resourcesClass, "getDisplayMetrics", "()Landroid/util/DisplayMetrics;");
    jobject displayMetrics = env->CallObjectMethod(resources, getDisplayMetricsMethod);

    jfloat result = env->CallStaticFloatMethod(typedValueClass, applyDimensionMethod, 1, (jfloat) i, displayMetrics);

    return (jint) result;
}

jint convertDipToPixels(JNIEnv* env, jobject obj, jint i) {
    jclass cls = env->GetObjectClass(obj);

    jclass PreferencesClass = env->FindClass("com/android/support/Preferences");
    jfieldID context_ok = env->GetStaticFieldID(PreferencesClass, "context","Landroid/content/Context;");

    jobject context = env->GetStaticObjectField(PreferencesClass,context_ok);

    cls = env->FindClass("android/content/Context");
    jmethodID getResourcesMethod = env->GetMethodID(cls, "getResources", "()Landroid/content/res/Resources;");
    jobject resources = env->CallObjectMethod(context, getResourcesMethod);

    cls = env->FindClass("android/content/res/Resources");
    jmethodID getDisplayMetricsMethod = env->GetMethodID(cls, "getDisplayMetrics", "()Landroid/util/DisplayMetrics;");
    jobject displayMetrics = env->CallObjectMethod(resources, getDisplayMetricsMethod);

    cls = env->FindClass("android/util/DisplayMetrics");
    jfieldID densityField = env->GetFieldID(cls, "density", "F");
    jfloat density = env->GetFloatField(displayMetrics, densityField);

    jint pixels = (jint) ((((float) i) * density) + 0.5f);
    return pixels;
}

jobject RichWebView(JNIEnv* env, jobject obj, jstring text) {
    jclass webViewClass = env->FindClass("android/webkit/WebView");
    jmethodID webViewConstructor = env->GetMethodID(webViewClass, "<init>", "(Landroid/content/Context;)V");
    jobject wView = env->NewObject(webViewClass, webViewConstructor, obj);

    jmethodID loadDataMethod = env->GetMethodID(webViewClass, "loadData", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    env->CallVoidMethod(wView, loadDataMethod, text, env->NewStringUTF("text/html"), env->NewStringUTF("utf-8"));

    jmethodID setBackgroundColorMethod = env->GetMethodID(webViewClass, "setBackgroundColor", "(I)V");
    env->CallVoidMethod(wView, setBackgroundColorMethod, 0x00000000);

    jmethodID setPaddingMethod = env->GetMethodID(webViewClass, "setPadding", "(IIII)V");
    env->CallVoidMethod(wView, setPaddingMethod, 0, 5, 0, 5);

    jmethodID getSettingsMethod = env->GetMethodID(webViewClass, "getSettings", "()Landroid/webkit/WebSettings;");
    jobject webSettings = env->CallObjectMethod(wView, getSettingsMethod);

    return wView;
}

jobject RichTextView(JNIEnv* env, jobject obj, jstring text) {
    jclass textViewClass = env->FindClass("android/widget/TextView");
    jmethodID textViewConstructor = env->GetMethodID(textViewClass, "<init>", "(Landroid/content/Context;)V");
    jobject textView = env->NewObject(textViewClass, textViewConstructor, obj);

    jclass htmlClass = env->FindClass("android/text/Html");
    jmethodID fromHtmlMethod = env->GetStaticMethodID(htmlClass, "fromHtml", "(Ljava/lang/String;)Landroid/text/Spanned;");
    jobject spanned = env->CallStaticObjectMethod(htmlClass, fromHtmlMethod, text);

    jmethodID setTextMethod = env->GetMethodID(textViewClass, "setText", "(Ljava/lang/CharSequence;)V");
    env->CallVoidMethod(textView, setTextMethod, spanned);

    jclass colorClass = env->FindClass("android/graphics/Color");
    jfieldID whiteField = env->GetStaticFieldID(colorClass, "WHITE", "I");
    jint white = env->GetStaticIntField(colorClass, whiteField);

    jmethodID setTextColorMethod = env->GetMethodID(textViewClass, "setTextColor", "(I)V");
    env->CallVoidMethod(textView, setTextColorMethod, white);

    jmethodID setPaddingMethod = env->GetMethodID(textViewClass, "setPadding", "(IIII)V");
    env->CallVoidMethod(textView, setPaddingMethod, 10, 5, 10, 5);

    return textView;
}

jobject Category(JNIEnv* env, jobject obj, jstring text) {
    jclass textViewClass = env->FindClass("android/widget/TextView");
    jmethodID textViewConstructor = env->GetMethodID(textViewClass, "<init>", "(Landroid/content/Context;)V");
    jobject textView = env->NewObject(textViewClass, textViewConstructor, obj);

    jmethodID setBackgroundColorMethod = env->GetMethodID(textViewClass, "setBackgroundColor", "(I)V");
    jint bgColor = env->CallStaticIntMethod(env->FindClass("android/graphics/Color"), env->GetStaticMethodID(env->FindClass("android/graphics/Color"), "parseColor", "(Ljava/lang/String;)I"), env->NewStringUTF("#2F3D4C"));
    env->CallVoidMethod(textView, setBackgroundColorMethod, bgColor);

    jmethodID setTextMethod = env->GetMethodID(textViewClass, "setText", "(Ljava/lang/CharSequence;)V");
    jobject html = env->CallStaticObjectMethod(env->FindClass("android/text/Html"), env->GetStaticMethodID(env->FindClass("android/text/Html"), "fromHtml", "(Ljava/lang/String;)Landroid/text/Spanned;"), text);
    env->CallVoidMethod(textView, setTextMethod, html);

    jmethodID setGravityMethod = env->GetMethodID(textViewClass, "setGravity", "(I)V");
    env->CallVoidMethod(textView, setGravityMethod, 17);

    jmethodID setTextColorMethod = env->GetMethodID(textViewClass, "setTextColor", "(I)V");
    jint textColor = env->CallStaticIntMethod(env->FindClass("android/graphics/Color"), env->GetStaticMethodID(env->FindClass("android/graphics/Color"), "parseColor", "(Ljava/lang/String;)I"), env->NewStringUTF("#FFFFFF"));
    env->CallVoidMethod(textView, setTextColorMethod, textColor);

    jmethodID setTypefaceMethod = env->GetMethodID(textViewClass, "setTypeface", "(Landroid/graphics/Typeface;I)V");
    jobject typeface = env->CallStaticObjectMethod(env->FindClass("android/graphics/Typeface"), env->GetStaticMethodID(env->FindClass("android/graphics/Typeface"), "defaultFromStyle", "(I)Landroid/graphics/Typeface;"), 0);
    env->CallVoidMethod(textView, setTypefaceMethod, typeface, 1);

    jmethodID setPaddingMethod = env->GetMethodID(textViewClass, "setPadding", "(IIII)V");
    env->CallVoidMethod(textView, setPaddingMethod, 0, 5, 0, 5);

    return textView;
}

jint Color_parseColor(JNIEnv *env, jobject obj, const char *text){
    jclass colorClass = env->FindClass("android/graphics/Color");
    jmethodID parseColorMethod = env->GetStaticMethodID(colorClass, "parseColor", "(Ljava/lang/String;)I");
    jint white = env->CallStaticIntMethod(colorClass, parseColorMethod, env->NewStringUTF(text));

    return white;
}

extern "C" JNIEXPORT void Java_com_android_support_LGLClickListener_OnClick(JNIEnv *env,jclass clazz,jlong nativePeer) {
    auto f = reinterpret_cast<std::function<void(void)>*>(nativePeer);
    (*f)();
}

extern "C" JNIEXPORT void Java_com_android_support_LGLClickListener_Release(JNIEnv *env,jclass clazz,jlong nativePeer) {
    auto f = reinterpret_cast<std::function<void(void)>*>(nativePeer);
    delete f;
}

jobject ButtonLink(JNIEnv* env, jobject obj, jstring featName, jstring url) {

    jclass textViewClass = env->FindClass("android/widget/Button");
    jmethodID textViewConstructor = env->GetMethodID(textViewClass, "<init>", "(Landroid/content/Context;)V");
    jobject button = env->NewObject(textViewClass, textViewConstructor, obj);

    jclass ViewGroup = env->FindClass("android/view/ViewGroup$LayoutParams");

    jclass layoutParamsClass = env->FindClass("android/widget/LinearLayout$LayoutParams");

    jfieldID matchParentField = env->GetStaticFieldID(ViewGroup, "MATCH_PARENT", "I");
    jint matchParent = env->GetStaticIntField(ViewGroup, matchParentField);
    jmethodID layoutParamsConstructor = env->GetMethodID(layoutParamsClass, "<init>", "(II)V");
    jobject layoutParams = env->NewObject(layoutParamsClass, layoutParamsConstructor, matchParent, matchParent);

    jfieldID marginField = env->GetFieldID(layoutParamsClass, "leftMargin", "I");
    env->SetIntField(layoutParams, marginField, 7);
    marginField = env->GetFieldID(layoutParamsClass, "topMargin", "I");
    env->SetIntField(layoutParams, marginField, 5);
    marginField = env->GetFieldID(layoutParamsClass, "rightMargin", "I");
    env->SetIntField(layoutParams, marginField, 7);
    marginField = env->GetFieldID(layoutParamsClass, "bottomMargin", "I");
    env->SetIntField(layoutParams, marginField, 5);

    jmethodID setLayoutParamsMethod = env->GetMethodID(textViewClass, "setLayoutParams", "(Landroid/view/ViewGroup$LayoutParams;)V");
    env->CallVoidMethod(button, setLayoutParamsMethod, layoutParams);

    jmethodID setAllCapsMethod = env->GetMethodID(textViewClass, "setAllCaps", "(Z)V");
    env->CallVoidMethod(button, setAllCapsMethod, false);

    jclass colorClass = env->FindClass("android/graphics/Color");
    jmethodID parseColorMethod = env->GetStaticMethodID(colorClass, "parseColor", "(Ljava/lang/String;)I");
    jint white = env->CallStaticIntMethod(colorClass, parseColorMethod, env->NewStringUTF("#FFFFFF"));
    jmethodID setTextColorMethod = env->GetMethodID(textViewClass, "setTextColor", "(I)V");
    env->CallVoidMethod(button, setTextColorMethod, white);

    jclass htmlClass = env->FindClass("android/text/Html");
    jmethodID fromHtmlMethod = env->GetStaticMethodID(htmlClass, "fromHtml", "(Ljava/lang/String;)Landroid/text/Spanned;");
    jstring htmlString = (jstring) env->CallStaticObjectMethod(htmlClass, fromHtmlMethod, featName);
    jmethodID setTextMethod = env->GetMethodID(textViewClass, "setText", "(Ljava/lang/CharSequence;)V");
    env->CallVoidMethod(button,setTextMethod, htmlString);

    jint whiteColor = env->CallStaticIntMethod(colorClass, parseColorMethod, env->NewStringUTF("#FFFFFF"));
    env->CallVoidMethod(button, setTextColorMethod, whiteColor);

    jint buttonBgColor = env->CallStaticIntMethod(colorClass, parseColorMethod, env->NewStringUTF("#1C262D"));
    jmethodID setBackgroundColorMethod = env->GetMethodID(textViewClass, "setBackgroundColor", "(I)V");
    env->CallVoidMethod(button, setBackgroundColorMethod, buttonBgColor);

    jobject globalObj = env->NewGlobalRef(obj);
    const char* urlChars = env->GetStringUTFChars(url, nullptr);
    auto callback = new std::function<void(void)>([env, globalObj, urlChars] {
        jclass uriClass = env->FindClass("android/net/Uri");
        jmethodID parseMethod = env->GetStaticMethodID(uriClass, "parse", "(Ljava/lang/String;)Landroid/net/Uri;");
        jobject uri = env->CallStaticObjectMethod(uriClass, parseMethod,env->NewStringUTF(urlChars));

        jclass intentClass = env->FindClass("android/content/Intent");
        jmethodID intentConstructor = env->GetMethodID(intentClass, "<init>", "(Ljava/lang/String;)V");
        jfieldID actionViewField = env->GetStaticFieldID(intentClass, "ACTION_VIEW", "Ljava/lang/String;");
        jstring actionViewString = (jstring) env->GetStaticObjectField(intentClass, actionViewField);
        jobject intent = env->NewObject(intentClass, intentConstructor, actionViewString);

        jfieldID flagsField = env->GetStaticFieldID(intentClass, "FLAG_ACTIVITY_NEW_TASK", "I");
        jint flags = env->GetStaticIntField(intentClass, flagsField);
        jmethodID addFlagsMethod = env->GetMethodID(intentClass, "addFlags", "(I)Landroid/content/Intent;");
        env->CallObjectMethod(intent, addFlagsMethod, flags);

        jmethodID setDataMethod = env->GetMethodID(intentClass, "setData", "(Landroid/net/Uri;)Landroid/content/Intent;");
        env->CallObjectMethod(intent, setDataMethod, uri);

        jclass contextClass = env->GetObjectClass(globalObj);

        jmethodID startActivityMethod = env->GetMethodID(contextClass, "startActivity", "(Landroid/content/Intent;)V");
        env->CallVoidMethod(globalObj, startActivityMethod, intent);
        env->DeleteGlobalRef(globalObj);

    });
 jclass listener_clazz = env->FindClass("com/android/support/LGLClickListener");
    jmethodID new_listener = env->GetMethodID(listener_clazz, "<init>", "(J)V");
    jobject listener = env->NewObject(listener_clazz, new_listener, (jlong)callback);
    jmethodID set_onclicklistener = env->GetMethodID(textViewClass, "setOnClickListener", "(Landroid/view/View$OnClickListener;)V");
    env->CallVoidMethod(button, set_onclicklistener, listener);
    return button;
}

jint onStartCommand(JNIEnv* env, jobject obj, jobject intent, jint i, jint i2) {
    jclass serviceClass = env->FindClass("android/app/Service");
    jfieldID startNotStickyField = env->GetStaticFieldID(serviceClass, "START_NOT_STICKY", "I");
    jint startNotSticky = env->GetStaticIntField(serviceClass, startNotStickyField);

    return startNotSticky;
}

jboolean isViewCollapsed(JNIEnv* env, jobject obj) {
    jclass Menu = env->GetObjectClass(obj);

    jfieldID rootFrame_field = env->GetFieldID(Menu, "rootFrame", "Landroid/widget/FrameLayout;");
    jobject rootFrame_obj = env->GetObjectField(obj, rootFrame_field);

    jclass frameLayoutClass =  env->GetObjectClass(rootFrame_obj);

    jfieldID mCollapsed_field = env->GetFieldID(env->GetObjectClass(obj), "mCollapsed", "Landroid/widget/RelativeLayout;");

    jobject mCollapsed_obj = env->GetObjectField(obj, mCollapsed_field);

    jclass relativeLayoutClass =  env->GetObjectClass(mCollapsed_obj);

    jfieldID visibilityField = env->GetStaticFieldID(relativeLayoutClass, "VISIBLE", "I");

    jclass viewClass = env->FindClass("android/view/View");
    jint visibilityValue = env->GetStaticIntField(viewClass, visibilityField);

    jmethodID getVisibilityMethod = env->GetMethodID(relativeLayoutClass, "getVisibility", "()I");
    jint mCollapsedVisibility = env->CallIntMethod(mCollapsed_obj, getVisibilityMethod);

    jmethodID getIdMethod = env->GetMethodID(frameLayoutClass, "getId", "()I");
    jint rootFrameId = env->CallIntMethod(rootFrame_obj, getIdMethod);

    jboolean result = (rootFrameId == 0 || mCollapsedVisibility == visibilityValue);
    return result;
}

jint Get_Color(JNIEnv* env, jobject obj,const char * color)
{
    jclass colorClass = env->FindClass("android/graphics/Color");
    jfieldID whiteField = env->GetStaticFieldID(colorClass, color, "I");
    jint white = env->GetStaticIntField(colorClass, whiteField);
    return white;
}

jboolean InitializeField(JNIEnv* env, jobject obj) {

    jclass Menu = env->GetObjectClass(obj);

    jfieldID myFieldId = env->GetStaticFieldID(Menu, "MENU_FEATURE_BG_COLOR", "I");
    env->SetStaticIntField(Menu, myFieldId, Color_parseColor(env,obj,"#DD141C22"));

    jfieldID ToggleON = env->GetStaticFieldID(Menu, "ToggleON", "I");
    env->SetStaticIntField(Menu, ToggleON, Get_Color(env,obj,"GREEN"));

    jfieldID BTN_COLOR = env->GetStaticFieldID(Menu, "BTN_COLOR", "I");
    env->SetStaticIntField(Menu, BTN_COLOR, Color_parseColor(env,obj,"#1C262D"));

    jfieldID BtnON = env->GetStaticFieldID(Menu, "BtnON", "I");
    env->SetStaticIntField(Menu, BtnON, Color_parseColor(env,obj,"#1b5e20"));

    jfieldID BtnOFF = env->GetStaticFieldID(Menu, "BtnOFF", "I");
    env->SetStaticIntField(Menu, BtnOFF, Color_parseColor(env,obj,"#7f0000"));

    jfieldID SeekBarColor = env->GetStaticFieldID(Menu, "SeekBarColor", "I");
    env->SetStaticIntField(Menu, SeekBarColor, Color_parseColor(env,obj,"#80CBC4"));

    jfieldID SeekBarProgressColor = env->GetStaticFieldID(Menu, "SeekBarProgressColor", "I");
    env->SetStaticIntField(Menu, SeekBarProgressColor, Color_parseColor(env,obj,"#80CBC4"));

    jfieldID CheckBoxColor = env->GetStaticFieldID(Menu, "CheckBoxColor", "I");
    env->SetStaticIntField(Menu, CheckBoxColor, Color_parseColor(env,obj,"#80CBC4"));

    jfieldID RadioColor = env->GetStaticFieldID(Menu, "RadioColor", "I");
    env->SetStaticIntField(Menu, RadioColor, Color_parseColor(env,obj,"#FFFFFF"));

    jfieldID NumberTxtColor = env->GetStaticFieldID(Menu, "NumberTxtColor", "Ljava/lang/String;");
    env->SetStaticObjectField(Menu, NumberTxtColor, env->NewStringUTF("#FFFFFF"));

    jfieldID ICON_ALPHA = env->GetStaticFieldID(Menu, "ICON_ALPHA", "F");
    env->SetStaticFloatField(Menu, ICON_ALPHA, 0.7f);

    jclass FrameLayout_class = env->FindClass("android/widget/FrameLayout");
    jmethodID FrameLayout_constructor = env->GetMethodID(FrameLayout_class, "<init>", "(Landroid/content/Context;)V");
    jobject context_obj = env->CallObjectMethod(obj, env->GetMethodID(env->GetObjectClass(obj), "getBaseContext", "()Landroid/content/Context;"));
    jobject rootFrame_obj = env->NewObject(FrameLayout_class, FrameLayout_constructor, context_obj);
    jfieldID rootFrame_field = env->GetFieldID(Menu, "rootFrame", "Landroid/widget/FrameLayout;");
    env->SetObjectField(obj, rootFrame_field, rootFrame_obj);


    jclass RelativeLayout_class = env->FindClass("android/widget/RelativeLayout");
    jmethodID RelativeLayout_constructor = env->GetMethodID(RelativeLayout_class, "<init>", "(Landroid/content/Context;)V");
    jobject mRootContainer_obj = env->NewObject(RelativeLayout_class, RelativeLayout_constructor, context_obj);
    jfieldID mRootContainer_field = env->GetFieldID(env->GetObjectClass(obj), "mRootContainer", "Landroid/widget/RelativeLayout;");
    env->SetObjectField(obj, mRootContainer_field, mRootContainer_obj);

    jobject mCollapsed_obj = env->NewObject(RelativeLayout_class, RelativeLayout_constructor, context_obj);
    jfieldID mCollapsed_field = env->GetFieldID(env->GetObjectClass(obj), "mCollapsed", "Landroid/widget/RelativeLayout;");
    env->SetObjectField(obj, mCollapsed_field, mCollapsed_obj);
    jclass View_class = env->FindClass("android/view/View");
    jfieldID View_VISIBLE_field = env->GetStaticFieldID(View_class, "VISIBLE", "I");
    jint VISIBLE_value = env->GetStaticIntField(View_class, View_VISIBLE_field);
    jmethodID RelativeLayout_setVisibility = env->GetMethodID(RelativeLayout_class, "setVisibility", "(I)V");
    env->CallVoidMethod(mCollapsed_obj, RelativeLayout_setVisibility, VISIBLE_value);
    jfieldID ICON_ALPHA_field = env->GetStaticFieldID(env->GetObjectClass(obj), "ICON_ALPHA", "F");
    jfloat ICON_ALPHA_value = env->GetStaticFloatField(env->GetObjectClass(obj), ICON_ALPHA_field);
    jmethodID RelativeLayout_setAlpha = env->GetMethodID(RelativeLayout_class, "setAlpha", "(F)V");
    env->CallVoidMethod(mCollapsed_obj, RelativeLayout_setAlpha, ICON_ALPHA_value);

    jclass ImageViewClass = env->FindClass("android/widget/ImageView");
    jmethodID ImageViewConstructor = env->GetMethodID(ImageViewClass, "<init>", "(Landroid/content/Context;)V");
    jobject startimage = env->NewObject(ImageViewClass, ImageViewConstructor, context_obj);
    jfieldID startimage_field = env->GetFieldID(Menu, "startimage", "Landroid/widget/ImageView;");
    env->SetObjectField(obj, startimage_field, startimage);

    jclass LinearLayoutClass = env->FindClass("android/widget/LinearLayout");
    jmethodID LinearLayoutConstructor = env->GetMethodID(LinearLayoutClass, "<init>", "(Landroid/content/Context;)V");
    jobject mExpanded = env->NewObject(LinearLayoutClass, LinearLayoutConstructor, context_obj);
    jobject patches = env->NewObject(LinearLayoutClass, LinearLayoutConstructor, context_obj);

    jfieldID mExpanded_field = env->GetFieldID(Menu, "mExpanded", "Landroid/widget/LinearLayout;");
    env->SetObjectField(obj, mExpanded_field, mExpanded);

    jfieldID patches_field = env->GetStaticFieldID(Menu, "patches", "Landroid/widget/LinearLayout;");
    env->SetStaticObjectField(Menu, patches_field, patches);


    jclass WebViewClass = env->FindClass("android/webkit/WebView");
    jmethodID WebViewConstructor = env->GetMethodID(WebViewClass, "<init>", "(Landroid/content/Context;)V");
    jobject wView = env->NewObject(WebViewClass, WebViewConstructor, context_obj);

    jfieldID wView_field = env->GetFieldID(Menu, "wView", "Landroid/webkit/WebView;");
    env->SetObjectField(obj, wView_field, wView);

    jclass TextViewClass = env->FindClass("android/widget/TextView");
    jmethodID TextViewConstructor = env->GetMethodID(TextViewClass, "<init>", "(Landroid/content/Context;)V");
    jobject settings = env->NewObject(TextViewClass, TextViewConstructor, context_obj);
    jobject title = env->NewObject(TextViewClass, TextViewConstructor, context_obj);
    jobject heading = env->NewObject(TextViewClass, TextViewConstructor, context_obj);

    jfieldID settings_field = env->GetFieldID(Menu, "settings", "Landroid/widget/TextView;");
    jfieldID title_field = env->GetFieldID(Menu, "title", "Landroid/widget/TextView;");
    jfieldID heading_field = env->GetFieldID(Menu, "heading", "Landroid/widget/TextView;");

    env->SetObjectField(obj, settings_field, settings);
    env->SetObjectField(obj, title_field, title);
    env->SetObjectField(obj, heading_field, heading);


    jclass RelativeLayoutClass = env->FindClass("android/widget/RelativeLayout");
    jmethodID RelativeLayoutConstructor = env->GetMethodID(RelativeLayoutClass, "<init>", "(Landroid/content/Context;)V");
    jobject titleText = env->NewObject(RelativeLayoutClass, RelativeLayoutConstructor, context_obj);
    jobject relativeLayout = env->NewObject(RelativeLayoutClass, RelativeLayoutConstructor, context_obj);

    jfieldID titleText_field = env->GetFieldID(Menu, "titleText", "Landroid/widget/RelativeLayout;");
    env->SetObjectField(obj, titleText_field, titleText);

    jfieldID relativeLayout_field = env->GetFieldID(Menu, "rLayout", "Landroid/widget/RelativeLayout;");
    env->SetObjectField(obj, relativeLayout_field, relativeLayout);


    jclass ScrollViewClass = env->FindClass("android/widget/ScrollView");
    jmethodID ScrollViewConstructor = env->GetMethodID(ScrollViewClass, "<init>", "(Landroid/content/Context;)V");
    jobject scrollView = env->NewObject(ScrollViewClass, ScrollViewConstructor, context_obj);

    jfieldID scrollView_field = env->GetStaticFieldID(Menu, "scrollView", "Landroid/widget/ScrollView;");
    env->SetStaticObjectField(Menu, scrollView_field, scrollView);

    jclass ButtonClass = env->FindClass("android/widget/Button");
    jmethodID ButtonConstructor = env->GetMethodID(ButtonClass, "<init>", "(Landroid/content/Context;)V");
    jobject hideBtn = env->NewObject(ButtonClass, ButtonConstructor, context_obj);
    jobject closeBtn = env->NewObject(ButtonClass, ButtonConstructor, context_obj);


    jfieldID hideBtn_field = env->GetFieldID(Menu, "hideBtn", "Landroid/widget/Button;");
    env->SetObjectField(obj, hideBtn_field, hideBtn);

    jfieldID closeBtn_field = env->GetFieldID(Menu, "closeBtn", "Landroid/widget/Button;");
    env->SetObjectField(obj, closeBtn_field, closeBtn);

    jobject mSettings = env->NewObject(LinearLayoutClass, LinearLayoutConstructor, context_obj);
    jclass linear_layout_class = env->FindClass("android/widget/LinearLayout");
    jmethodID set_orientation_method = env->GetMethodID(linear_layout_class, "setOrientation", "(I)V");

    jint orientation_vertical = 1;

    env->CallVoidMethod(mSettings, set_orientation_method, orientation_vertical);
    jfieldID mSettingsFieldID = env->GetStaticFieldID(Menu, "mSettings", "Landroid/widget/LinearLayout;");
    env->SetStaticObjectField(Menu, mSettingsFieldID, mSettings);


    jclass context_class = env->GetObjectClass(context_obj);

    return true;
}

void AddViews(JNIEnv* env, jobject obj)
{
    jclass Menu = env->GetObjectClass(obj);
    jfieldID rootFrame_field = env->GetFieldID(Menu, "rootFrame", "Landroid/widget/FrameLayout;");
    jobject rootFrame_obj = env->GetObjectField(obj, rootFrame_field);

    jfieldID mRootContainer_field = env->GetFieldID(env->GetObjectClass(obj), "mRootContainer", "Landroid/widget/RelativeLayout;");
    jobject mRootContainer_obj = env->GetObjectField(obj, mRootContainer_field);
    jclass RelativeLayout_class = env->GetObjectClass(mRootContainer_obj);

    jclass frame_layout_class = env->GetObjectClass(rootFrame_obj);
    jmethodID addView_method = env->GetMethodID(frame_layout_class, "addView", "(Landroid/view/View;)V");

    env->CallVoidMethod(rootFrame_obj, addView_method, mRootContainer_obj);

    jmethodID addView_RelativeLayout = env->GetMethodID(RelativeLayout_class, "addView", "(Landroid/view/View;)V");

    jfieldID mCollapsed_field = env->GetFieldID(env->GetObjectClass(obj), "mCollapsed", "Landroid/widget/RelativeLayout;");

    jobject mCollapsed_obj = env->GetObjectField(obj, mCollapsed_field);


    jfieldID mExpanded_field = env->GetFieldID(Menu, "mExpanded", "Landroid/widget/LinearLayout;");
    jobject mExpanded_obj = env->GetObjectField(obj, mExpanded_field);
    jclass LinearLayout_class = env->GetObjectClass(mExpanded_obj);
    jmethodID addView_LinearLayout = env->GetMethodID(LinearLayout_class, "addView", "(Landroid/view/View;)V");


    env->CallVoidMethod(mRootContainer_obj, addView_RelativeLayout, mCollapsed_obj);
    env->CallVoidMethod(mRootContainer_obj, addView_RelativeLayout, mExpanded_obj);

    jfieldID title_field = env->GetFieldID(Menu, "title", "Landroid/widget/TextView;");
    jobject title_obj = env->GetObjectField(obj, title_field);

    jfieldID settings_field = env->GetFieldID(Menu, "settings", "Landroid/widget/TextView;");
    jobject settings_obj = env->GetObjectField(obj, settings_field);

    jfieldID titleText_field = env->GetFieldID(Menu, "titleText", "Landroid/widget/RelativeLayout;");
    jobject titleText_obj = env->GetObjectField(obj, titleText_field);

    env->CallVoidMethod(titleText_obj, addView_RelativeLayout, title_obj);
    env->CallVoidMethod(titleText_obj, addView_RelativeLayout, settings_obj);


    env->CallVoidMethod(mExpanded_obj, addView_LinearLayout, titleText_obj);

    jfieldID heading_field = env->GetFieldID(Menu, "heading", "Landroid/widget/TextView;");
    jobject heading_obj = env->GetObjectField(obj, heading_field);

    env->CallVoidMethod(mExpanded_obj, addView_LinearLayout, heading_obj);

    jfieldID scrollView_field = env->GetStaticFieldID(Menu, "scrollView", "Landroid/widget/ScrollView;");
    jobject scrollView_obj = env->GetStaticObjectField(Menu, scrollView_field);
    jclass ScrollView_class = env->GetObjectClass(scrollView_obj);
    jmethodID addView_ScrollView = env->GetMethodID(ScrollView_class, "addView", "(Landroid/view/View;)V");

    jfieldID patches_field = env->GetStaticFieldID(Menu, "patches", "Landroid/widget/LinearLayout;");
    jobject patches_obj = env->GetStaticObjectField(Menu, patches_field);

    env->CallVoidMethod(scrollView_obj, addView_ScrollView, patches_obj);
    env->CallVoidMethod(mExpanded_obj, addView_LinearLayout, scrollView_obj);

    jfieldID hideBtn_field = env->GetFieldID(Menu, "hideBtn", "Landroid/widget/Button;");
    jobject hideBtn_obj = env->GetObjectField(obj, hideBtn_field);

    jfieldID closeBtn_field = env->GetFieldID(Menu, "closeBtn", "Landroid/widget/Button;");
    jobject closeBtn_obj = env->GetObjectField(obj, closeBtn_field);

    jfieldID relativeLayout_field = env->GetFieldID(Menu, "rLayout", "Landroid/widget/RelativeLayout;");
    jobject relativeLayout_obj = env->GetObjectField(obj, relativeLayout_field);

    jfieldID wView_field = env->GetFieldID(Menu, "wView", "Landroid/webkit/WebView;");
    jobject wView_obj = env->GetObjectField(obj, wView_field);

    jfieldID startimage_field = env->GetFieldID(Menu, "startimage", "Landroid/widget/ImageView;");
    jobject startimage_obj = env->GetObjectField(obj, startimage_field);

    if (IconWebViewData(env,obj) != NULL) {
        env->CallVoidMethod(mCollapsed_obj, addView_RelativeLayout, wView_obj);
    } else {
        env->CallVoidMethod(mCollapsed_obj, addView_RelativeLayout, startimage_obj);
    }

    env->CallVoidMethod(relativeLayout_obj, addView_RelativeLayout, hideBtn_obj);
    env->CallVoidMethod(relativeLayout_obj, addView_RelativeLayout, closeBtn_obj);
    env->CallVoidMethod(mExpanded_obj, addView_LinearLayout, relativeLayout_obj);


    jfieldID params_field = env->GetFieldID(Menu, "params", "Landroid/view/WindowManager$LayoutParams;");

    jclass view_group_layout_params_class = env->FindClass("android/view/ViewGroup$LayoutParams");
    jfieldID wrap_content_field = env->GetStaticFieldID(view_group_layout_params_class, "WRAP_CONTENT", "I");
    jint wrap_content_value = env->GetStaticIntField(view_group_layout_params_class, wrap_content_field);

    jint iparams = env->GetStaticIntField(env->FindClass("android/os/Build$VERSION"), env->GetStaticFieldID(env->FindClass(
            "android/os/Build$VERSION"), "SDK_INT", "I")) >= 26 ? 2038 : 2002;

    jobject params_obj = env->NewObject(env->FindClass("android/view/WindowManager$LayoutParams"), env->GetMethodID(env->FindClass("android/view/WindowManager$LayoutParams"), "<init>", "(IIIII)V"), wrap_content_value, wrap_content_value, iparams, 8, -3);
    jclass layoutParams_class = env->GetObjectClass(params_obj);
    jfieldID gravity_field = env->GetFieldID(layoutParams_class, "gravity", "I");
    env->SetIntField(params_obj, gravity_field, 51);
    jfieldID x_field = env->GetFieldID(layoutParams_class, "x", "I");
    env->SetIntField(params_obj, x_field, 0);
    jfieldID y_field = env->GetFieldID(layoutParams_class, "y", "I");
    env->SetIntField(params_obj, y_field, 100);
    env->SetObjectField(obj, params_field, params_obj);

    jfieldID mWindowManager_field = env->GetFieldID(Menu, "mWindowManager", "Landroid/view/WindowManager;");
    jobject mWindowManager_obj = env->GetObjectField(obj, mWindowManager_field);
    if (mWindowManager_obj == NULL) {
        return;
    }
    jclass mWindowManagerclass = env->GetObjectClass(mWindowManager_obj);

    jmethodID addViewWindowManager = env->GetMethodID(mWindowManagerclass, "addView", "(Landroid/view/View;Landroid/view/ViewGroup$LayoutParams;)V");
    env->CallVoidMethod(mWindowManager_obj, addViewWindowManager, rootFrame_obj,params_obj);



}

void MakeWindowx(JNIEnv* env, jobject obj) {
    jclass Menu = env->GetObjectClass(obj);

    jfieldID mExpanded_field = env->GetFieldID(Menu, "mExpanded", "Landroid/widget/LinearLayout;");
    jobject mExpanded = env->GetObjectField(obj, mExpanded_field);

    jclass cls = env->GetObjectClass(mExpanded);

    jmethodID setVisibilityMethodID = env->GetMethodID(cls, "setVisibility", "(I)V");

    env->CallVoidMethod(mExpanded, setVisibilityMethodID, 0);

    jmethodID setBackgroundColorMethodID = env->GetMethodID(cls, "setBackgroundColor", "(I)V");

    jint color = 0xEE1C2A35;
    env->CallVoidMethod(mExpanded, setBackgroundColorMethodID, color);

    jmethodID setOrientationMethodID = env->GetMethodID(cls, "setOrientation", "(I)V");

    jint orientation = env->GetStaticIntField(cls, env->GetStaticFieldID(cls, "VERTICAL", "I"));
    env->CallVoidMethod(mExpanded, setOrientationMethodID, orientation);

    jclass layoutParamsCls = env->FindClass("android/widget/LinearLayout$LayoutParams");

    jmethodID layoutParamsConstructorMethodID = env->GetMethodID(layoutParamsCls, "<init>",
                                                                 "(II)V");

    jclass ViewGroup = env->FindClass("android/view/ViewGroup$LayoutParams");

    jint width = dp(env,obj,290);
    jint height = env->GetStaticIntField(ViewGroup,
                                         env->GetStaticFieldID(layoutParamsCls, "WRAP_CONTENT",
                                                               "I"));
    jobject layoutParams = env->NewObject(layoutParamsCls, layoutParamsConstructorMethodID, width,
                                          height);

    jmethodID setLayoutParamsMethodID = env->GetMethodID(cls, "setLayoutParams",
                                                         "(Landroid/view/ViewGroup$LayoutParams;)V");

    jclass colorClass = env->FindClass("android/graphics/Color");
    jmethodID parseColorMethod = env->GetStaticMethodID(colorClass, "parseColor", "(Ljava/lang/String;)I");

    env->CallVoidMethod(mExpanded, setLayoutParamsMethodID, layoutParams);

    jclass gradientDrawableClass = env->FindClass("android/graphics/drawable/GradientDrawable");
    jmethodID gradientDrawableConstructor = env->GetMethodID(gradientDrawableClass, "<init>", "()V");
    jobject gdMenuBody = env->NewObject(gradientDrawableClass, gradientDrawableConstructor);

    jmethodID cornerRadiusField = env->GetMethodID(gradientDrawableClass, "setCornerRadius", "(F)V");
    env->CallVoidMethod(gdMenuBody, cornerRadiusField, 4.0f);

    jint buttonBgColor = env->CallStaticIntMethod(colorClass, parseColorMethod, env->NewStringUTF("#EE1C2A35"));
    jint buttonBgColor2 = env->CallStaticIntMethod(colorClass, parseColorMethod, env->NewStringUTF("#32cb00"));

    jmethodID colorField = env->GetMethodID(gradientDrawableClass, "setColor", "(I)V");
    env->CallVoidMethod(gdMenuBody, colorField, buttonBgColor);

    jmethodID setStrokeMethod = env->GetMethodID(gradientDrawableClass, "setStroke", "(II)V");
    env->CallVoidMethod(gdMenuBody, setStrokeMethod, 1, buttonBgColor2);


    jmethodID setBackgroundColorMethodIDx = env->GetMethodID(cls, "setBackground", "(Landroid/graphics/drawable/Drawable;)V");

    env->CallVoidMethod(mExpanded, setBackgroundColorMethodIDx, gdMenuBody);

}

void MakeLogo(JNIEnv* env, jobject obj) {

    jclass Menu = env->GetObjectClass(obj);

    jfieldID startimage_field = env->GetFieldID(Menu, "startimage", "Landroid/widget/ImageView;");
    jobject startimage= env->GetObjectField(obj, startimage_field);

    jfieldID wView_field = env->GetFieldID(Menu, "wView", "Landroid/webkit/WebView;");
    jobject wView = env->GetObjectField(obj, wView_field);

    jfieldID settings_field = env->GetFieldID(Menu, "settings", "Landroid/widget/TextView;");
    jobject settings = env->GetObjectField(obj, settings_field);

    jfieldID titleText_field = env->GetFieldID(Menu, "titleText", "Landroid/widget/RelativeLayout;");
    jobject titleText = env->GetObjectField(obj, titleText_field);

    jfieldID title_field = env->GetFieldID(Menu, "title", "Landroid/widget/TextView;");
    jobject title = env->GetObjectField(obj, title_field);

    jfieldID heading_field = env->GetFieldID(Menu, "heading", "Landroid/widget/TextView;");
    jobject heading = env->GetObjectField(obj, heading_field);

    jfieldID scrollView_field = env->GetStaticFieldID(Menu, "scrollView", "Landroid/widget/ScrollView;");
    jobject scrollView = env->GetStaticObjectField(Menu, scrollView_field);

    jclass clsImageView = env->GetObjectClass(startimage);

    jmethodID midSetLayoutParams = env->GetMethodID(clsImageView, "setLayoutParams", "(Landroid/view/ViewGroup$LayoutParams;)V");

    jmethodID midGetLayoutParams = env->GetMethodID(clsImageView, "getLayoutParams", "()Landroid/view/ViewGroup$LayoutParams;");

    jclass clsRelativeLayoutParams = env->FindClass("android/widget/RelativeLayout$LayoutParams");

    jmethodID midRelativeLayoutParamsConstructor = env->GetMethodID(clsRelativeLayoutParams, "<init>", "(II)V");

    jclass ViewGroup = env->FindClass("android/view/ViewGroup$LayoutParams");

    jclass layoutParamsClass = env->FindClass("android/widget/LinearLayout$LayoutParams");

    jfieldID matchParentField = env->GetStaticFieldID(ViewGroup, "WRAP_CONTENT", "I");
    jint matchParent = env->GetStaticIntField(ViewGroup, matchParentField);

    jobject layoutParamsx = env->NewObject(clsRelativeLayoutParams, midRelativeLayoutParamsConstructor, matchParent, matchParent);

    env->CallVoidMethod(startimage, midSetLayoutParams, layoutParamsx);

    jclass clsDisplayMetrics = env->FindClass("android/util/DisplayMetrics");

    jmethodID midGetDisplayMetrics = env->GetMethodID(clsImageView, "getResources", "()Landroid/content/res/Resources;");

    jobject resources = env->CallObjectMethod(startimage, midGetDisplayMetrics);

    midGetDisplayMetrics = env->GetMethodID(env->GetObjectClass(resources), "getDisplayMetrics", "()Landroid/util/DisplayMetrics;");

    jobject displayMetrics = env->CallObjectMethod(resources, midGetDisplayMetrics);

    jclass TypedValue = env->FindClass("android/util/TypedValue");

    jmethodID midApplyDimension = env->GetStaticMethodID(TypedValue, "applyDimension","(IFLandroid/util/DisplayMetrics;)F");

    jfloat pixelValue = env->CallStaticFloatMethod(TypedValue, midApplyDimension, 1, 70.0f,displayMetrics);

    int applyDimension = (int) pixelValue;

    jfieldID fidHeight = env->GetFieldID(clsRelativeLayoutParams, "height", "I");

    env->SetIntField(layoutParamsx, fidHeight, applyDimension);

    jfieldID fidWidth = env->GetFieldID(clsRelativeLayoutParams, "width", "I");

    env->SetIntField(layoutParamsx, fidWidth, applyDimension);

    jmethodID midSetScaleType = env->GetMethodID(clsImageView, "setScaleType", "(Landroid/widget/ImageView$ScaleType;)V");

    jclass clsImageViewScaleType = env->FindClass("android/widget/ImageView$ScaleType");

    jfieldID fidFIT_XY = env->GetStaticFieldID(clsImageViewScaleType, "FIT_XY", "Landroid/widget/ImageView$ScaleType;");

    jobject objFIT_XY = env->GetStaticObjectField(clsImageViewScaleType, fidFIT_XY);

    env->CallVoidMethod(startimage, midSetScaleType, objFIT_XY);

    jmethodID midDecode = env->GetStaticMethodID(env->FindClass("android/util/Base64"), "decode", "(Ljava/lang/String;I)[B");

    jbyteArray byteArray = (jbyteArray) env->CallStaticObjectMethod(env->FindClass("android/util/Base64"), midDecode, Icon(env, obj), 0);

    jmethodID midDecodeByteArray = env->GetStaticMethodID(env->FindClass("android/graphics/BitmapFactory"), "decodeByteArray", "([BII)Landroid/graphics/Bitmap;");

    jobject bitmap = env->CallStaticObjectMethod(env->FindClass("android/graphics/BitmapFactory"), midDecodeByteArray, byteArray, 0, env->GetArrayLength(byteArray));

    jmethodID midSetImageBitmap = env->GetMethodID(clsImageView, "setImageBitmap", "(Landroid/graphics/Bitmap;)V");

    env->CallVoidMethod(startimage, midSetImageBitmap, bitmap);


    jfieldID mCollapsed_field = env->GetFieldID(env->GetObjectClass(obj), "mCollapsed", "Landroid/widget/RelativeLayout;");

    jobject mCollapsed = env->GetObjectField(obj, mCollapsed_field);

    jfieldID mExpanded_field = env->GetFieldID(Menu, "mExpanded", "Landroid/widget/LinearLayout;");
    jobject mExpanded = env->GetObjectField(obj, mExpanded_field);

    jobject globalmCollapsed = env->NewGlobalRef(mCollapsed);
    jobject globalmExpanded = env->NewGlobalRef(mExpanded);

    auto callback = new std::function<void(void)>([env,globalmCollapsed, globalmExpanded] {
        jclass viewClass = env->FindClass("android/view/View");
        jmethodID setVisibilityId = env->GetMethodID(viewClass, "setVisibility", "(I)V");
        jmethodID setAlphaId = env->GetMethodID(viewClass, "setAlpha", "(F)V");

        jfieldID visibleId = env->GetStaticFieldID(viewClass, "VISIBLE", "I");
        jfieldID goneId = env->GetStaticFieldID(viewClass, "GONE", "I");

        int visible = env->GetStaticIntField(viewClass, visibleId);
        int gone = env->GetStaticIntField(viewClass, goneId);


        env->CallVoidMethod(globalmCollapsed, setVisibilityId, gone);

        env->CallVoidMethod(globalmExpanded, setVisibilityId, visible);
    });

    jclass listener_clazz = env->FindClass("com/android/support/LGLClickListener");
    jmethodID new_listener = env->GetMethodID(listener_clazz, "<init>", "(J)V");
    jobject listener = env->NewObject(listener_clazz, new_listener, (jlong)callback);
    jmethodID set_onclicklistener = env->GetMethodID(clsImageView, "setOnClickListener", "(Landroid/view/View$OnClickListener;)V");
    env->CallVoidMethod(startimage, set_onclicklistener, listener);


    jfieldID topMargin = env->GetFieldID(clsRelativeLayoutParams, "topMargin", "I");


    env->SetIntField(layoutParamsx, topMargin, convertDipToPixels(env,obj,10));


    jclass webViewClass = env->GetObjectClass(wView);

    jmethodID loadDataMethodID = env->GetMethodID(webViewClass, "loadData", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");

    jstring data = env->NewStringUTF("<html><head></head><body style=\"margin: 0; padding: 0\"><img src=\"https://imgur.com/gallery/H8263Xs\" width=\"50\" height=\"50\" ></body></html>");
    jstring mimeType = env->NewStringUTF("text/html");
    jstring encoding = env->NewStringUTF("utf-8");
    env->CallVoidMethod(wView, loadDataMethodID, data, mimeType, encoding);

    jmethodID setBackgroundColorMethodID = env->GetMethodID(webViewClass, "setBackgroundColor", "(I)V");

    env->CallVoidMethod(wView, setBackgroundColorMethodID, 0x00000000);

    jmethodID alphaFieldID = env->GetMethodID(webViewClass, "setAlpha", "(F)V");

    env->CallVoidMethod(wView, alphaFieldID, 0.7f);

    jmethodID getSettings = env->GetMethodID(webViewClass, "getSettings", "()Landroid/webkit/WebSettings;");

    jobject webSettings = env->CallObjectMethod(wView, getSettings);

    jclass webSettingsClass = env->GetObjectClass(webSettings);

    jmethodID setAppCacheEnabledMethodID = env->GetMethodID(webSettingsClass, "setAppCacheEnabled", "(Z)V");

    env->CallVoidMethod(webSettings, setAppCacheEnabledMethodID, true);


    jclass clsTextView = env->FindClass("android/widget/TextView");
    jmethodID midSetText = env->GetMethodID(clsTextView, "setText", "(Ljava/lang/CharSequence;)V");
    jmethodID midSetTextColor = env->GetMethodID(clsTextView, "setTextColor", "(I)V");
    jmethodID midSetTypeface = env->GetMethodID(clsTextView, "setTypeface", "(Landroid/graphics/Typeface;)V");
    jmethodID midSetTextSize = env->GetMethodID(clsTextView, "setTextSize", "(F)V");

    jclass clsRelativeLayout = env->FindClass("android/widget/RelativeLayout$LayoutParams");
    jmethodID midAddRule = env->GetMethodID(clsRelativeLayout, "addRule", "(I)V");

    jclass clsTypeface = env->FindClass("android/graphics/Typeface");
    jfieldID fidDEFAULT_BOLD = env->GetStaticFieldID(clsTypeface, "DEFAULT_BOLD", "Landroid/graphics/Typeface;");

    jclass clsBuildVersion = env->FindClass("android/os/Build$VERSION");
    jfieldID fidSDK_INT = env->GetStaticFieldID(clsBuildVersion, "SDK_INT", "I");

    jstring settingsText = NULL;
    if (env->GetStaticIntField(clsBuildVersion, fidSDK_INT) >= 23) {
        settingsText = env->NewStringUTF("⚙");
    } else {
        settingsText = env->NewStringUTF("⚙");
    }

    jint bgColor = env->CallStaticIntMethod(env->FindClass("android/graphics/Color"), env->GetStaticMethodID(env->FindClass("android/graphics/Color"), "parseColor", "(Ljava/lang/String;)I"), env->NewStringUTF("#2F3D4C"));

    env->CallVoidMethod(settings, midSetText, settingsText);
    env->CallVoidMethod(settings, midSetTextColor, bgColor);
    env->CallVoidMethod(settings, midSetTypeface, env->GetStaticObjectField(clsTypeface, fidDEFAULT_BOLD));
    env->CallVoidMethod(settings, midSetTextSize, 20.0f);


    jobject globalObj = env->NewGlobalRef(obj);
    bool settingsOpen = false;
    auto callback2 = new std::function<void(void)>([env,&settingsOpen,globalObj] {
        settingsOpen = !settingsOpen;


        jclass Menu = env->GetObjectClass(globalObj);
        jfieldID scrollView_field = env->GetStaticFieldID(Menu, "scrollView", "Landroid/widget/ScrollView;");
        jobject scrollView_obj = env->GetStaticObjectField(Menu, scrollView_field);
        jclass ScrollView_class = env->GetObjectClass(scrollView_obj);

        jfieldID patches_field = env->GetStaticFieldID(Menu, "patches", "Landroid/widget/LinearLayout;");
        jobject patches_obj = env->GetStaticObjectField(Menu, patches_field);

        jfieldID mSettings_field = env->GetStaticFieldID(Menu, "mSettings", "Landroid/widget/LinearLayout;");
        jobject mSettings_obj = env->GetStaticObjectField(Menu, mSettings_field);

        jmethodID removeView_method = env->GetMethodID(ScrollView_class, "removeView", "(Landroid/view/View;)V");
        jmethodID removeallView_method = env->GetMethodID(ScrollView_class, "removeAllViews", "()V");
        jmethodID addView_method = env->GetMethodID(ScrollView_class, "addView", "(Landroid/view/View;)V");

        jclass scroll_view_class = env->FindClass("android/widget/ScrollView");
        jmethodID scroll_to_method = env->GetMethodID(scroll_view_class, "scrollTo", "(II)V");

        try {
            if (settingsOpen) {
                env->CallVoidMethod(scrollView_obj, removeallView_method);
                env->CallVoidMethod(scrollView_obj, removeView_method, patches_obj);
                env->CallVoidMethod(scrollView_obj, addView_method, mSettings_obj);
                env->CallVoidMethod(scrollView_obj, scroll_to_method, 0, 0);
            } else {
                env->CallVoidMethod(scrollView_obj, removeallView_method);
                env->CallVoidMethod(scrollView_obj, removeView_method, mSettings_obj);
                env->CallVoidMethod(scrollView_obj, addView_method, patches_obj);
            }
        } catch (jthrowable exception) {
            jclass exceptionClass = env->FindClass("java/lang/IllegalStateException");
            if (env->IsInstanceOf(exception, exceptionClass)) {
                env->ExceptionClear();
            } else {
                env->Throw(exception);
            }
            env->DeleteLocalRef(exceptionClass);
        }

    });

     jobject listener2 = env->NewObject(listener_clazz, new_listener, (jlong)callback2);
    jmethodID set_onclicklistener2 = env->GetMethodID(env->GetObjectClass(settings), "setOnClickListener", "(Landroid/view/View$OnClickListener;)V");
    env->CallVoidMethod(settings, set_onclicklistener2, listener2);


    jmethodID midRelativeLayoutConstructor = env->GetMethodID(clsRelativeLayout, "<init>", "(II)V");

    jobject rlsettings = env->NewObject(clsRelativeLayout, midRelativeLayoutConstructor, matchParent, matchParent);

    jclass RelativeLayout = env->FindClass("android/widget/RelativeLayout");

    jfieldID ALIGN_PARENT_RIGHTf = env->GetStaticFieldID(RelativeLayout, "ALIGN_PARENT_RIGHT", "I");
    jint ALIGN_PARENT_RIGHT = env->GetStaticIntField(RelativeLayout, ALIGN_PARENT_RIGHTf);
    env->CallVoidMethod(rlsettings, midAddRule, ALIGN_PARENT_RIGHT);
    env->CallVoidMethod(settings, midSetLayoutParams, rlsettings);


    jclass clsRelativeLayoutx = env->GetObjectClass(titleText);

    jmethodID midSetPadding = env->GetMethodID(clsRelativeLayoutx, "setPadding", "(IIII)V");

    env->CallVoidMethod(titleText, midSetPadding, 10, 5, 10, 5);

    jmethodID midSetVerticalGravity = env->GetMethodID(clsRelativeLayoutx, "setVerticalGravity", "(I)V");

    env->CallVoidMethod(titleText, midSetVerticalGravity, 16);

    jclass textViewClass = env->GetObjectClass(title);

    jmethodID setColorMethod = env->GetMethodID(textViewClass, "setTextColor", "(I)V");

    jint textPaintd = env->CallStaticIntMethod(env->FindClass("android/graphics/Color"), env->GetStaticMethodID(env->FindClass("android/graphics/Color"), "parseColor", "(Ljava/lang/String;)I"), env->NewStringUTF("#82CAFD"));

    env->CallVoidMethod(title, setColorMethod, textPaintd);

    jmethodID setTextSizeMethod = env->GetMethodID(textViewClass, "setTextSize", "(F)V");
    env->CallVoidMethod(title, setTextSizeMethod, 18.0f);

    jmethodID setGravityMethod = env->GetMethodID(textViewClass, "setGravity", "(I)V");

    jint Gravity = env->GetStaticIntField(env->FindClass("android/view/Gravity"), env->GetStaticFieldID(env->FindClass("android/view/Gravity"), "CENTER", "I"));

    env->CallVoidMethod(title, setGravityMethod, Gravity);

    jclass relativeLayoutParamsClass = env->FindClass("android/widget/RelativeLayout$LayoutParams");
    jmethodID relativeLayoutParamsConstructor = env->GetMethodID(relativeLayoutParamsClass, "<init>", "(II)V");
    jobject layoutParams = env->NewObject(relativeLayoutParamsClass, relativeLayoutParamsConstructor, matchParent, matchParent);
    jmethodID addRuleMethod = env->GetMethodID(relativeLayoutParamsClass, "addRule", "(I)V");

    jfieldID CENTER_HORIZONTALd = env->GetStaticFieldID(RelativeLayout, "CENTER_HORIZONTAL", "I");
    jint CENTER_HORIZONTAL = env->GetStaticIntField(RelativeLayout, CENTER_HORIZONTALd);

    env->CallVoidMethod(layoutParams, addRuleMethod, CENTER_HORIZONTAL);

    jmethodID setLayoutParamsMethod = env->GetMethodID(textViewClass, "setLayoutParams", "(Landroid/view/ViewGroup$LayoutParams;)V");
    env->CallVoidMethod(title, setLayoutParamsMethod, layoutParams);
    setTitleText(env,obj,title);



    jclass textViewheading= env->GetObjectClass(heading);

    jmethodID setEllipsizeMethod = env->GetMethodID(textViewheading, "setEllipsize", "(Landroid/text/TextUtils$TruncateAt;)V");

    jclass truncateAtClass = env->FindClass("android/text/TextUtils$TruncateAt");
    jfieldID marqueeField = env->GetStaticFieldID(truncateAtClass, "MARQUEE", "Landroid/text/TextUtils$TruncateAt;");
    jobject marquee = env->GetStaticObjectField(truncateAtClass, marqueeField);

    env->CallVoidMethod(heading, setEllipsizeMethod, marquee);

    jmethodID setMarqueeRepeatLimitMethod = env->GetMethodID(textViewheading, "setMarqueeRepeatLimit", "(I)V");

    env->CallVoidMethod(heading, setMarqueeRepeatLimitMethod, -1);

    jmethodID setSingleLineMethod = env->GetMethodID(textViewheading, "setSingleLine", "(Z)V");

    env->CallVoidMethod(heading, setSingleLineMethod, true);

    jmethodID setSelectedMethod = env->GetMethodID(textViewheading, "setSelected", "(Z)V");

    env->CallVoidMethod(heading, setSelectedMethod, true);

    jmethodID setTextColorMethod = env->GetMethodID(textViewheading, "setTextColor", "(I)V");

    jint colorheading = env->CallStaticIntMethod(env->FindClass("android/graphics/Color"), env->GetStaticMethodID(env->FindClass("android/graphics/Color"), "parseColor", "(Ljava/lang/String;)I"), env->NewStringUTF("#82CAFD"));


    env->CallVoidMethod(heading, setTextColorMethod, colorheading);

    jmethodID setTextSizeMethodg = env->GetMethodID(textViewheading, "setTextSize", "(F)V");

    env->CallVoidMethod(heading, setTextSizeMethodg, 10.0f);

    jmethodID setGravityMethodg = env->GetMethodID(textViewheading, "setGravity", "(I)V");

    env->CallVoidMethod(heading, setGravityMethodg, Gravity);

    jmethodID setPaddingMethod = env->GetMethodID(textViewheading, "setPadding", "(IIII)V");

    env->CallVoidMethod(heading, setPaddingMethod, 0, 0, 0, 5);

    setHeadingText(env,obj,heading);


    jclass scrollViewClass = env->GetObjectClass(scrollView);

    jclass linearLayoutLayoutParamsClass = env->FindClass("android/widget/LinearLayout$LayoutParams");
    jmethodID linearLayoutLayoutParamsConstructor = env->GetMethodID(linearLayoutLayoutParamsClass, "<init>", "(II)V");

    jfieldID MATCH_PARENTField = env->GetStaticFieldID(ViewGroup, "MATCH_PARENT", "I");
    jint MATCH_PARENT = env->GetStaticIntField(ViewGroup, MATCH_PARENTField);

    jobject scrlLL = env->NewObject(linearLayoutLayoutParamsClass, linearLayoutLayoutParamsConstructor, MATCH_PARENT, dp(env,obj,210));

    jmethodID setLayoutParamsMethodID = env->GetMethodID(scrollViewClass, "setLayoutParams", "(Landroid/view/ViewGroup$LayoutParams;)V");
    env->CallVoidMethod(scrollView, setLayoutParamsMethodID, scrlLL);

    jobject mExpanded_obj = env->GetObjectField(obj, mExpanded_field);

    jobject existingLayoutParams = env->CallObjectMethod(mExpanded_obj, env->GetMethodID(env->GetObjectClass(mExpanded_obj), "getLayoutParams", "()Landroid/view/ViewGroup$LayoutParams;"));

    jmethodID layoutParamsConstructor = env->GetMethodID(layoutParamsClass, "<init>", "(Landroid/view/ViewGroup$LayoutParams;)V");
    jobject newLayoutParams = env->NewObject(layoutParamsClass, layoutParamsConstructor, existingLayoutParams);

    jfieldID weightField = env->GetFieldID(layoutParamsClass, "weight", "F");
    env->SetFloatField(newLayoutParams, weightField, 1.0f);

    jfieldID scrlLLExpandedField = env->GetStaticFieldID(env->GetObjectClass(obj), "scrlLLExpanded", "Landroid/widget/LinearLayout$LayoutParams;");
    env->SetStaticObjectField(env->GetObjectClass(obj), scrlLLExpandedField, newLayoutParams);

    jmethodID setBackgroundColorMethodIDc = env->GetMethodID(scrollViewClass, "setBackgroundColor", "(I)V");

    jint MENU_FEATURE_BG_COLOR = env->CallStaticIntMethod(env->FindClass("android/graphics/Color"), env->GetStaticMethodID(env->FindClass("android/graphics/Color"), "parseColor", "(Ljava/lang/String;)I"), env->NewStringUTF("#DD141C22"));

    env->CallVoidMethod(scrollView, setBackgroundColorMethodIDc, MENU_FEATURE_BG_COLOR);

}

void MakeMenu(JNIEnv* env, jobject obj) {

    jclass Menu = env->GetObjectClass(obj);

    jfieldID relativeLayout_field = env->GetFieldID(Menu, "rLayout", "Landroid/widget/RelativeLayout;");
    jobject relativeLayout_obj = env->GetObjectField(obj, relativeLayout_field);
    jclass relativeLayoutClass = env->GetObjectClass(relativeLayout_obj);

    jfieldID patches_field = env->GetStaticFieldID(Menu, "patches", "Landroid/widget/LinearLayout;");
    jobject patches = env->GetStaticObjectField(Menu, patches_field);


    jfieldID hideBtn_field = env->GetFieldID(Menu, "hideBtn", "Landroid/widget/Button;");
    jobject hideBtn = env->GetObjectField(obj, hideBtn_field);

    jfieldID closeBtn_field = env->GetFieldID(Menu, "closeBtn", "Landroid/widget/Button;");
    jobject closeBtn = env->GetObjectField(obj, closeBtn_field);

    jmethodID relativeLayoutConstructor = env->GetMethodID(relativeLayoutClass, "<init>", "(Landroid/content/Context;)V");
    jobject relativeLayout_ = env->NewObject(relativeLayoutClass, relativeLayoutConstructor, obj);

    jmethodID setPaddingMethod = env->GetMethodID(relativeLayoutClass, "setPadding", "(IIII)V");
    env->CallVoidMethod(relativeLayout_, setPaddingMethod, 10, 3, 10, 3);

    jmethodID setVerticalGravityMethod = env->GetMethodID(relativeLayoutClass, "setVerticalGravity", "(I)V");
    jint Gravity_CENTER = env->GetStaticIntField(env->FindClass("android/view/Gravity"), env->GetStaticFieldID(env->FindClass("android/view/Gravity"), "CENTER", "I"));

    env->CallVoidMethod(relativeLayout_, setVerticalGravityMethod, Gravity_CENTER);


    jclass linearLayoutClass = env->GetObjectClass(patches);

    jclass tlinearLayoutClass = env->FindClass("android/widget/LinearLayout");

    jfieldID linearLayoutVerticalField = env->GetStaticFieldID(tlinearLayoutClass, "VERTICAL", "I");
    jint linearLayoutVertical = env->GetStaticIntField(tlinearLayoutClass, linearLayoutVerticalField);

    jmethodID setOrientationMethod = env->GetMethodID(linearLayoutClass, "setOrientation", "(I)V");
    env->CallVoidMethod(patches, setOrientationMethod, linearLayoutVertical);


    jclass hideBtnClass = env->GetObjectClass(hideBtn);

    jclass ViewGroup = env->FindClass("android/view/ViewGroup$LayoutParams");

    jfieldID matchParentField = env->GetStaticFieldID(ViewGroup, "WRAP_CONTENT", "I");
    jint WRAP_CONTENT = env->GetStaticIntField(ViewGroup, matchParentField);

    jclass RelativeLayout = env->FindClass("android/widget/RelativeLayout");

   jfieldID ALIGN_PARENT_RIGHTf = env->GetStaticFieldID(RelativeLayout, "ALIGN_PARENT_RIGHT", "I");
    jint ALIGN_PARENT_RIGHT = env->GetStaticIntField(RelativeLayout, ALIGN_PARENT_RIGHTf);

    jfieldID alignParentLeftField = env->GetStaticFieldID(RelativeLayout, "ALIGN_PARENT_LEFT", "I");
    jint alignParentLeft = env->GetStaticIntField(relativeLayoutClass, alignParentLeftField);

    jclass viewClass = env->FindClass("android/view/View");
    jfieldID layoutDirectionField = env->GetStaticFieldID(viewClass, "LAYOUT_DIRECTION_LTR", "I");
    jint layoutDirection = env->GetStaticIntField(viewClass, layoutDirectionField);


    jmethodID lParamsHideBtnField = env->GetMethodID(hideBtnClass, "setLayoutParams", "(Landroid/view/ViewGroup$LayoutParams;)V");

    jint Color_TRANSPARENT = env->CallStaticIntMethod(env->FindClass("android/graphics/Color"), env->GetStaticMethodID(env->FindClass("android/graphics/Color"), "parseColor", "(Ljava/lang/String;)I"), env->NewStringUTF("#0082CAFD"));
    jint Color_82CAFD= env->CallStaticIntMethod(env->FindClass("android/graphics/Color"), env->GetStaticMethodID(env->FindClass("android/graphics/Color"), "parseColor", "(Ljava/lang/String;)I"), env->NewStringUTF("#82CAFD"));


    jmethodID setBackgroundColorMethod = env->GetMethodID(hideBtnClass, "setBackgroundColor", "(I)V");
    env->CallVoidMethod(hideBtn, setBackgroundColorMethod, Color_TRANSPARENT);

    jmethodID setTextMethod = env->GetMethodID(hideBtnClass, "setText", "(Ljava/lang/CharSequence;)V");
    jstring hideBtnText = env->NewStringUTF("HIDE/KILL (Hold)");
    env->CallVoidMethod(hideBtn, setTextMethod, hideBtnText);

    jmethodID setTextColorMethod = env->GetMethodID(hideBtnClass, "setTextColor", "(I)V");
    env->CallVoidMethod(hideBtn, setTextColorMethod, Color_82CAFD);


    jfieldID mCollapsed_field = env->GetFieldID(env->GetObjectClass(obj), "mCollapsed", "Landroid/widget/RelativeLayout;");

    jobject mCollapsed = env->GetObjectField(obj, mCollapsed_field);

    jfieldID mExpanded_field = env->GetFieldID(Menu, "mExpanded", "Landroid/widget/LinearLayout;");
    jobject mExpanded = env->GetObjectField(obj, mExpanded_field);

    jclass PreferencesClass = env->FindClass("com/android/support/Preferences");
    jfieldID context_ok = env->GetStaticFieldID(PreferencesClass, "context","Landroid/content/Context;");

    jobject context = env->GetStaticObjectField(PreferencesClass,context_ok);

    jobject globalObj = env->NewGlobalRef(obj);
    jobject globalmCollapsed = env->NewGlobalRef(mCollapsed);
    jobject globalmExpanded = env->NewGlobalRef(mExpanded);
    jobject globalmcontext= env->NewGlobalRef(context);
    auto callback = new std::function<void(void)>([env,globalObj,globalmCollapsed, globalmExpanded,globalmcontext] {

        jclass viewClass = env->FindClass("android/view/View");
        jmethodID setVisibilityId = env->GetMethodID(viewClass, "setVisibility", "(I)V");
        jmethodID setAlphaId = env->GetMethodID(viewClass, "setAlpha", "(F)V");

        jfieldID visibleId = env->GetStaticFieldID(viewClass, "VISIBLE", "I");
        jfieldID goneId = env->GetStaticFieldID(viewClass, "GONE", "I");

        int visible = env->GetStaticIntField(viewClass, visibleId);
        int gone = env->GetStaticIntField(viewClass, goneId);


        env->CallVoidMethod(globalmCollapsed, setVisibilityId, visible);
        env->CallVoidMethod(globalmCollapsed, setAlphaId, 0.0f);

        env->CallVoidMethod(globalmExpanded, setVisibilityId, gone);
        Toastx(env,globalmcontext,OBFUSCATE("Icon hidden. Remember the hidden icon position --"),1);

    });

    jclass listener_clazz = env->FindClass("com/android/support/LGLClickListener");
    jmethodID new_listener = env->GetMethodID(listener_clazz, "<init>", "(J)V");
    jobject listener = env->NewObject(listener_clazz, new_listener, (jlong)callback);
    jmethodID set_onclicklistener = env->GetMethodID(hideBtnClass, "setOnClickListener", "(Landroid/view/View$OnClickListener;)V");
    env->CallVoidMethod(hideBtn, set_onclicklistener, listener);


    jclass buttonClass = env->GetObjectClass(closeBtn);
    jmethodID setLayoutParams = env->GetMethodID(buttonClass, "setLayoutParams", "(Landroid/view/ViewGroup$LayoutParams;)V");

    jclass relativeLayoutParamsClass = env->FindClass("android/widget/RelativeLayout$LayoutParams");
    jmethodID relativeLayoutParamsConstructor = env->GetMethodID(relativeLayoutParamsClass, "<init>", "(II)V");
    jobject lParamsCloseBtn = env->NewObject(relativeLayoutParamsClass, relativeLayoutParamsConstructor, WRAP_CONTENT, WRAP_CONTENT);

    jmethodID addRule = env->GetMethodID(relativeLayoutParamsClass, "addRule", "(I)V");
    env->CallVoidMethod(lParamsCloseBtn, addRule, ALIGN_PARENT_RIGHT);

    env->CallVoidMethod(closeBtn, setLayoutParams, lParamsCloseBtn);

    jmethodID setBackgroundColor = env->GetMethodID(buttonClass, "setBackgroundColor", "(I)V");
    env->CallVoidMethod(closeBtn, setBackgroundColor, Color_TRANSPARENT);

    jmethodID setText = env->GetMethodID(buttonClass, "setText", "(Ljava/lang/CharSequence;)V");
    jstring MINIMIZE = env->NewStringUTF("MINIMIZE");
    env->CallVoidMethod(closeBtn, setText, MINIMIZE);

    jmethodID setTextColor = env->GetMethodID(buttonClass, "setTextColor", "(I)V");
    env->CallVoidMethod(closeBtn, setTextColor, Color_82CAFD);

    auto callback1 = new std::function<void(void)>([env,globalObj,globalmCollapsed, globalmExpanded,globalmcontext] {

        jclass viewClass = env->FindClass("android/view/View");
        jmethodID setVisibilityId = env->GetMethodID(viewClass, "setVisibility", "(I)V");
        jmethodID setAlphaId = env->GetMethodID(viewClass, "setAlpha", "(F)V");

        jfieldID visibleId = env->GetStaticFieldID(viewClass, "VISIBLE", "I");
        jfieldID goneId = env->GetStaticFieldID(viewClass, "GONE", "I");

        int visible = env->GetStaticIntField(viewClass, visibleId);
        int gone = env->GetStaticIntField(viewClass, goneId);


        env->CallVoidMethod(globalmCollapsed, setVisibilityId, visible);
        env->CallVoidMethod(globalmCollapsed, setAlphaId, 0.7f);

        env->CallVoidMethod(globalmExpanded, setVisibilityId, gone);

    });

    jobject listener2 = env->NewObject(listener_clazz, new_listener, (jlong)callback1);
    jmethodID set_onclicklistener2 = env->GetMethodID(hideBtnClass, "setOnClickListener", "(Landroid/view/View$OnClickListener;)V");
   env->CallVoidMethod(closeBtn, set_onclicklistener2, listener2);

   /*
    *  env->DeleteGlobalRef(globalObj);
        env->DeleteGlobalRef(globalmCollapsed);
        env->DeleteGlobalRef(globalmExpanded);
        env->DeleteGlobalRef(globalmcontext);
        */
}

void MakeWindow(JNIEnv* env, jobject obj){
    InitializeField(env,obj);
    MakeWindowx(env,obj);
    MakeLogo(env,obj);
    MakeMenu(env,obj);
    AddViews(env,obj);
}

void *exit_thread(void *) {

    sleep(5);
    exit(0);
}

void CheckOverlayPermission(JNIEnv *env, jclass thiz, jobject ctx){

    LOGI(OBFUSCATE("Check overlay permission"));

    int sdkVer = api_level();
    if (sdkVer >= 23){
        jclass Settings = env->FindClass(OBFUSCATE("android/provider/Settings"));
        jmethodID canDraw =env->GetStaticMethodID(Settings, OBFUSCATE("canDrawOverlays"), OBFUSCATE("(Landroid/content/Context;)Z"));
        if (!env->CallStaticBooleanMethod(Settings, canDraw, ctx)){
            Toastx(env,ctx,OBFUSCATE("Overlay permission is required in order to show mod menu."),1);
            startActivityPermisson(env, ctx);

            pthread_t ptid;
            pthread_create(&ptid, NULL, exit_thread, NULL);
            return;
        }
    }


    LOGI(OBFUSCATE("Start service"));

    startService(env, ctx);

    if(libLoaded){
        startActivity(env, ctx);
    }
}

void Thread(JNIEnv* env, jobject obj, jobject rootFrame,jobject ctx) {

    if (rootFrame == NULL) {
        return;
    }

    jclass viewClass = env->FindClass("android/view/View");
    jfieldID visibleId = env->GetStaticFieldID(viewClass, "VISIBLE", "I");
    jfieldID INVISIBLEId = env->GetStaticFieldID(viewClass, "INVISIBLE", "I");
    int visible = env->GetStaticIntField(viewClass, visibleId);
    int INVISIBLE = env->GetStaticIntField(viewClass, INVISIBLEId);

    jclass rootFrameClass = env->GetObjectClass(rootFrame);
    jmethodID setVisibilityId = env->GetMethodID(rootFrameClass, "setVisibility", "(I)V");

    if (isNotInGame(env,ctx)) {
        env->CallVoidMethod(rootFrame, setVisibilityId, INVISIBLE);
    } else {
        env->CallVoidMethod(rootFrame, setVisibilityId, visible);
    }
}

void onDestroy(JNIEnv* env, jobject obj, jobject rootFrame,jobject mWindowManager) {

    if (rootFrame == NULL) {
        return;
    }

    jclass mWindowManagerClass = env->GetObjectClass(mWindowManager);
    jmethodID removeViewId = env->GetMethodID(mWindowManagerClass, "removeView", "(Landroid/view/View)V");

    env->CallVoidMethod(mWindowManager, removeViewId, rootFrame);

}

void FeatureLister(JNIEnv* env, jobject obj, jobjectArray listFT,jobject linearLayout) {
    jclass linearLayoutClass = env->GetObjectClass(linearLayout);
     jmethodID addView_method = env->GetMethodID(linearLayoutClass, "addView", "(Landroid/view/View;)V");

    jclass viewsClass = env->FindClass("com/android/support/Views");
    jmethodID switchMethod = env->GetStaticMethodID(viewsClass, "Switch","(ILjava/lang/String;Z)Landroid/view/View;");
    jmethodID ButtonMethod = env->GetStaticMethodID(viewsClass, "Button","(ILjava/lang/String;)Landroid/view/View;");
    jmethodID ButtonOnOffMethod = env->GetStaticMethodID(viewsClass, "ButtonOnOff","(ILjava/lang/String;Z)Landroid/view/View;");
    jmethodID SeekBarMethod = env->GetStaticMethodID(viewsClass, "SeekBar","(ILjava/lang/String;II)Landroid/view/View;");
    jmethodID SpinnerMethod = env->GetStaticMethodID(viewsClass, "Spinner","(ILjava/lang/String;Ljava/lang/String;)Landroid/view/View;");
    jmethodID RadioButtonMethod = env->GetStaticMethodID(viewsClass, "RadioButton","(ILjava/lang/String;Ljava/lang/String;)Landroid/view/View;");
    jmethodID InputTextMethod = env->GetStaticMethodID(viewsClass, "TextField","(ILjava/lang/String;ZI)Landroid/view/View;");
    jmethodID CheckBoxMethod = env->GetStaticMethodID(viewsClass, "CheckBox","(ILjava/lang/String;Z)Landroid/view/View;");
    jmethodID CollapseMethod = env->GetStaticMethodID(viewsClass, "Collapse","(Landroid/widget/LinearLayout;Ljava/lang/String;)V");

    jclass textUtilsClass = env->FindClass("android/text/TextUtils");
    jmethodID isDigitsOnlyMethod = env->GetStaticMethodID(textUtilsClass, "isDigitsOnly","(Ljava/lang/CharSequence;)Z");
    jmethodID isEmptyMethod = env->GetStaticMethodID(textUtilsClass, "isEmpty","(Ljava/lang/CharSequence;)Z");

    jclass stringClass = env->FindClass("java/lang/String");
    jmethodID splitMethod = env->GetMethodID(stringClass, "split","(Ljava/lang/String;)[Ljava/lang/String;");

    jclass booleanClass = env->FindClass("java/lang/Boolean");
    jmethodID parseBooleanMethod = env->GetStaticMethodID(booleanClass, "parseBoolean","(Ljava/lang/String;)Z");

    jfieldID mCollapse_field = env->GetStaticFieldID(env->GetObjectClass(obj), "mCollapse", "Landroid/widget/LinearLayout;");
    jobject mCollapse_obj = env->GetStaticObjectField(env->GetObjectClass(obj), mCollapse_field);


    jsize numFeatures = env->GetArrayLength(listFT);

    jint featNum, subFeat = 0;
    jobject llBak = linearLayout;
    for (int i = 0; i < numFeatures; i++) {
        llBak = linearLayout;
        jboolean switchedOn = false;
        jstring feature = (jstring) env->GetObjectArrayElement(listFT, i);

        const char *featureStr = env->GetStringUTFChars(feature, NULL);
        std::string featureCppStr(featureStr);
        env->ReleaseStringUTFChars(feature, featureStr);

        if (featureCppStr.find("True_") != std::string::npos) {
            switchedOn = true;
            featureCppStr = std::regex_replace(featureCppStr, std::regex("True_"), "");
        }
        if (featureCppStr.find("CollapseAdd_") != std::string::npos) {
            llBak = mCollapse_obj;
            featureCppStr = std::regex_replace(featureCppStr, std::regex("CollapseAdd_"), "");
        }
        std::vector<std::string> featureSplitx;
        std::istringstream featureStreamx(featureCppStr);
        std::string featurePartx;
        while (std::getline(featureStreamx, featurePartx, '_')) {
            featureSplitx.push_back(featurePartx);
        }

        if (std::regex_match(featureSplitx[0], std::regex("-?[0-9]+"))) {
            featNum = std::stoi(featureSplitx[0]);
            featureCppStr = std::regex_replace(featureCppStr, std::regex(featureSplitx[0] + "_"), "");
            subFeat++;
        } else {
            featNum = i;
        }
        std::vector<std::string> featureSplit;
        std::istringstream featureStream(featureCppStr);
        std::string featurePart;
        while (std::getline(featureStream, featurePart, '_')) {
            featureSplit.push_back(featurePart);
        }

        if (featureSplit[0] == "Toggle") {
            jstring labelStr = env->NewStringUTF(featureSplit[1].c_str());
            jobject switchView = env->CallStaticObjectMethod(viewsClass, switchMethod, featNum, labelStr, switchedOn);

            if(llBak == NULL){
                return;
            }
            env->CallVoidMethod(llBak, addView_method, switchView);
            env->DeleteLocalRef(labelStr);
            env->DeleteLocalRef(switchView);
        }
        if (featureSplit[0] == "Collapse") {
            subFeat++;
            jstring labelStr = env->NewStringUTF(featureSplit[1].c_str());
            env->CallStaticVoidMethod(viewsClass, CollapseMethod, llBak, labelStr);

            if(llBak == NULL){
                return;
            }
            env->DeleteLocalRef(labelStr);
        }
        if (featureSplit[0] == "CheckBox") {
            jstring labelStr = env->NewStringUTF(featureSplit[1].c_str());
            jobject CheckBoxView = env->CallStaticObjectMethod(viewsClass, CheckBoxMethod, featNum, labelStr, switchedOn);

            if(llBak == NULL){
                return;
            }
            env->CallVoidMethod(llBak, addView_method, CheckBoxView);
            env->DeleteLocalRef(labelStr);
            env->DeleteLocalRef(CheckBoxView);
        }
        if (featureSplit[0] == "ButtonLink") {
            subFeat++;
            jstring labelStr = env->NewStringUTF(featureSplit[1].c_str());
            jstring labelStr2 = env->NewStringUTF(featureSplit[2].c_str());
            jobject ButtonLinkView = ButtonLink(env,obj,labelStr,labelStr2);

            if(llBak == NULL){
                return;
            }
            env->CallVoidMethod(llBak, addView_method, ButtonLinkView);
            env->DeleteLocalRef(labelStr);
            env->DeleteLocalRef(labelStr2);
            env->DeleteLocalRef(ButtonLinkView);
        }
        if (featureSplit[0] == "Category") {
            subFeat++;
            jstring labelStr = env->NewStringUTF(featureSplit[1].c_str());
            jobject CategoryView = Category(env,obj,labelStr);//env->CallObjectMethod(obj, CategoryMethod,labelStr);

            if(llBak == NULL){
                return;
            }
            env->CallVoidMethod(llBak, addView_method, CategoryView);
            env->DeleteLocalRef(labelStr);
            env->DeleteLocalRef(CategoryView);
        }
        if (featureSplit[0] == "RadioButton") {
            jstring labelStr = env->NewStringUTF(featureSplit[1].c_str());
            jstring labelStr2 = env->NewStringUTF(featureSplit[2].c_str());
            jobject RadioButtonView = env->CallStaticObjectMethod(viewsClass, RadioButtonMethod, featNum, labelStr, labelStr2);

            if(llBak == NULL){
                return;
            }
            env->CallVoidMethod(llBak, addView_method, RadioButtonView);
            env->DeleteLocalRef(labelStr);
            env->DeleteLocalRef(RadioButtonView);
        }
        if (featureSplit[0] == "ButtonOnOff") {
            jstring labelStr = env->NewStringUTF(featureSplit[1].c_str());
            jobject ButtonOnOffView = env->CallStaticObjectMethod(viewsClass, ButtonOnOffMethod, featNum, labelStr, switchedOn);

            if(llBak == NULL){
                return;
            }
            env->CallVoidMethod(llBak, addView_method, ButtonOnOffView);
            env->DeleteLocalRef(labelStr);
            env->DeleteLocalRef(ButtonOnOffView);
        }
        if (featureSplit[0] == "Button") {
            jstring labelStr = env->NewStringUTF(featureSplit[1].c_str());
            jobject ButtonView = env->CallStaticObjectMethod(viewsClass, ButtonMethod, featNum, labelStr);

            if(llBak == NULL){
                return;
            }
            env->CallVoidMethod(llBak, addView_method, ButtonView);
            env->DeleteLocalRef(labelStr);
            env->DeleteLocalRef(ButtonView);
        }
        if (featureSplit[0] == "SeekBar") {
            jstring labelStr = env->NewStringUTF(featureSplit[1].c_str());
            jobject SeekBarView = env->CallStaticObjectMethod(viewsClass, SeekBarMethod, featNum, labelStr,std::stoi(featureSplit[2]),std::stoi(featureSplit[3]));

            if(llBak == NULL){
                return;
            }
            env->CallVoidMethod(llBak, addView_method, SeekBarView);
            env->DeleteLocalRef(labelStr);
            env->DeleteLocalRef(SeekBarView);
        }
        if (featureSplit[0] == "Spinner") {
            jstring labelStr = env->NewStringUTF(featureSplit[1].c_str());
            jobject RichTextViewView = RichTextView(env,obj,labelStr);//env->CallObjectMethod(obj, RichTextViewMethod, labelStr);
            jobject SpinnerView = env->CallStaticObjectMethod(viewsClass, SpinnerMethod, featNum, labelStr,env->NewStringUTF(featureSplit[2].c_str()));

            if(llBak == NULL){
                return;
            }
            env->CallVoidMethod(llBak, addView_method, RichTextViewView);
            env->CallVoidMethod(llBak, addView_method, SpinnerView);
            env->DeleteLocalRef(labelStr);
            env->DeleteLocalRef(SpinnerView);
            env->DeleteLocalRef(RichTextViewView);
        }

        if (featureSplit[0] == "RichTextView") {
            subFeat++;
            jstring labelStr = env->NewStringUTF(featureSplit[1].c_str());
            jobject RichTextViewView =  RichTextView(env,obj,labelStr);//env->CallObjectMethod(obj, RichTextViewMethod, labelStr);

            if(llBak == NULL){
                return;
            }
            env->CallVoidMethod(llBak, addView_method, RichTextViewView);
            env->DeleteLocalRef(labelStr);
            env->DeleteLocalRef(RichTextViewView);
        }
        if (featureSplit[0] == "RichWebView") {
            subFeat++;
            jstring labelStr = env->NewStringUTF(featureSplit[1].c_str());
            jobject RichWebViewView =  RichWebView(env,obj,labelStr);//env->CallObjectMethod(obj, RichWebViewMethod, labelStr);

            if(llBak == NULL){
                return;
            }
            env->CallVoidMethod(llBak, addView_method, RichWebViewView);
            env->DeleteLocalRef(labelStr);
            env->DeleteLocalRef(RichWebViewView);
        }
        if (featureSplit[0] == "InputText") {
            jstring labelStr = env->NewStringUTF(featureSplit[1].c_str());
            jobject InputTextView = env->CallStaticObjectMethod(viewsClass, InputTextMethod, featNum, labelStr,JNI_FALSE,0);

            if(llBak == NULL){
                return;
            }
            env->CallVoidMethod(llBak, addView_method, InputTextView);
            env->DeleteLocalRef(labelStr);
            env->DeleteLocalRef(InputTextView);
        }

        if (featureSplit[0] == "InputValue") {
            jobject InputTextView;
            jstring labelStr;
            if(featureSplit.size() == 3){
                labelStr = env->NewStringUTF(featureSplit[2].c_str());

                InputTextView = env->CallStaticObjectMethod(viewsClass, InputTextMethod, featNum, labelStr,JNI_FALSE,std::stoi(featureSplit[1]));

            }else if(featureSplit.size() == 2){
                labelStr = env->NewStringUTF(featureSplit[1].c_str());
                InputTextView = env->CallStaticObjectMethod(viewsClass, InputTextMethod, featNum, labelStr,JNI_FALSE,0);
            }

            if(llBak == NULL){
                return;
            }
            env->CallVoidMethod(llBak, addView_method, InputTextView);
            env->DeleteLocalRef(labelStr);
            env->DeleteLocalRef(InputTextView);
        }
        featureSplit.clear();
    }
}

#pragma clang diagnostic pop
