package com.android.support;

import android.view.View;


public class LGLClickListener implements View.OnClickListener {
    private final long nativePeer;

    public LGLClickListener(final long nativePeer) {
        this.nativePeer = nativePeer;
    }

    @Override
    public void onClick(View v) {
        OnClick(nativePeer);
    }

    @Override
    public void finalize() {
        Release(nativePeer);
    }

    private static native void OnClick(final long peer);
    private static native void Release(final long peer);
}