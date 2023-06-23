//Please don't replace listeners with lambda!

package com.android.support;

import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.graphics.Color;
import android.os.Handler;
import android.os.IBinder;
import android.text.TextUtils;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;
import android.webkit.WebView;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.ScrollView;
import android.widget.TextView;

public class Menu extends Service {
    public static final String TAG = "Mod_Menu"; //Tag for logcat
    static int BTN_COLOR;
    static int MENU_FEATURE_BG_COLOR;
    static int ToggleON;
    static int ToggleOFF = Color.RED;
    static int BtnON;
    static int BtnOFF;
    static int SeekBarColor;
    static int SeekBarProgressColor;
    static int CheckBoxColor;
    static int RadioColor;
    static String NumberTxtColor;
    static LinearLayout patches;
    static LinearLayout mSettings;
    static LinearLayout mCollapse;
    static LinearLayout.LayoutParams scrlLLExpanded;
    static LinearLayout.LayoutParams scrlLL;
    static ScrollView scrollView;
    static boolean stopChecking;
    static float ICON_ALPHA;

    RelativeLayout mCollapsed, mRootContainer;
    LinearLayout mExpanded;
    WindowManager mWindowManager;
    WindowManager.LayoutParams params;
    ImageView startimage;
    FrameLayout rootFrame;
    WebView wView;
    TextView settings;

    RelativeLayout titleText;

    TextView title;

    TextView heading;

    RelativeLayout rLayout;
    Button hideBtn;
    Button closeBtn;


    ////////////////////native section
    static native void Thread(FrameLayout rootFrame, Context ctx);
    static native void onDestroy(FrameLayout rootFrame, WindowManager mWindowManager);
    public static native int convertDipToPixels(int i);
    native void setTitleText(TextView textView);
    native void setHeadingText(TextView textView);
    native String Icon();
    native String IconWebViewData();
    native String[] getFeatureList();
    native String[] settingsList();
    native boolean isGameLibLoaded();
    native boolean isNotInGame();
    native boolean InitializeField();
    native void AddViews();
    native void MakeWindow();
    native void MakeLogo();
    native void MakeMenu();
    native void FeatureLister(String[] bag, LinearLayout linearLayout);

    @Override
    public void onCreate() {
        super.onCreate();
        Preferences.context = this;
        initFloating();

        final Handler handler = new Handler();
        handler.post(new Runnable() {
            public void run() {
                Thread(rootFrame, Menu.this);
                handler.postDelayed(this, 1000);
            }
        });
    }

    public Context getContext() {
        return Preferences.context;
    }

    private void initFloating() {
        mWindowManager = (WindowManager) getSystemService(WINDOW_SERVICE);

        MakeWindow();

        FeatureLister(settingsList(), mSettings);

        rootFrame.setOnTouchListener(onTouchListener());
        startimage.setOnTouchListener(onTouchListener());
        wView.setOnTouchListener(onTouchListener());

        final Handler handler = new Handler();
        handler.postDelayed(new Runnable() {
            boolean viewLoaded = false;

            @Override
            public void run() {
                if (Preferences.loadPref && !isGameLibLoaded() && !stopChecking) {
                    if (!viewLoaded) {
                        patches.addView(Category("Save preferences was been enabled. Waiting for game lib to be loaded...\n\nForce load menu may not apply mods instantly. You would need to reactivate them again"));
                        patches.addView(Views.Button(-100, "Force load menu"));
                        viewLoaded = true;
                    }
                    handler.postDelayed(this, 600);
                } else {
                    patches.removeAllViews();
                    FeatureLister(getFeatureList(), patches);
                }
            }
        }, 500);
    }

    private View.OnTouchListener onTouchListener() {
        return new View.OnTouchListener() {
            final View collapsedView = mCollapsed;
            final View expandedView = mExpanded;
            private float initialTouchX, initialTouchY;
            private int initialX, initialY;

            public boolean onTouch(View view, MotionEvent motionEvent) {
                switch (motionEvent.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                        initialX = params.x;
                        initialY = params.y;
                        initialTouchX = motionEvent.getRawX();
                        initialTouchY = motionEvent.getRawY();
                        return true;
                    case MotionEvent.ACTION_UP:
                        int rawX = (int) (motionEvent.getRawX() - initialTouchX);
                        int rawY = (int) (motionEvent.getRawY() - initialTouchY);
                        mExpanded.setAlpha(1f);
                        mCollapsed.setAlpha(1f);
                        if (rawX < 10 && rawY < 10 && isViewCollapsed()) {
                            try {
                                collapsedView.setVisibility(View.GONE);
                                expandedView.setVisibility(View.VISIBLE);
                            } catch (NullPointerException e) {

                            }
                        }
                        return true;
                    case MotionEvent.ACTION_MOVE:
                        mExpanded.setAlpha(0.5f);
                        mCollapsed.setAlpha(0.5f);
                        params.x = initialX + ((int) (motionEvent.getRawX() - initialTouchX));
                        params.y = initialY + ((int) (motionEvent.getRawY() - initialTouchY));
                        mWindowManager.updateViewLayout(rootFrame, params);
                        return true;
                    default:
                        return false;
                }
            }
        };
    }

    native View ButtonLink(final String featName, final String url);

    native View Category(String text);

    native View RichTextView(String text);

    native View RichWebView(String text);

    native int onStartCommandn(Intent intent, int i, int i2);

    public int onStartCommand(Intent intent, int i, int i2) {
        return onStartCommandn(intent, i, i2);
    }

    native boolean isViewCollapsed();

    native int dp(int i);


    public void onDestroy() {
        super.onDestroy();
        onDestroy(rootFrame, mWindowManager);
    }

    //Same as above so it wont crash in the background and therefore use alot of Battery life
    public void onTaskRemoved(Intent intent) {
        super.onTaskRemoved(intent);
        try {
            Thread.sleep(100);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        stopSelf();
    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    public static class EditTextString {
        private String text;

        public String getString() {
            return text;
        }

        public void setString(String s) {
            text = s;
        }
    }

    public static class EditTextNum {
        private int val;

        public int getNum() {
            return val;
        }

        public void setNum(int i) {
            val = i;
        }
    }
}