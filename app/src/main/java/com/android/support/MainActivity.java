package com.android.support;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.provider.Settings;
import android.widget.Toast;

public class MainActivity extends Activity {

    static {
        System.loadLibrary("MyLibName");
    }

    static native void CheckOverlayPermission(Context ctx);
    static Context mContext;

    public static void Start(Context nContext){
        mContext = nContext;
        CheckOverlayPermission(nContext);
    }
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Start(this);
    }
}
