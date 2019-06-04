package com.alephzero.harentz;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;

import org.libsdl.app.SDLActivity;

public class MainActivity extends SDLActivity {

    private void goFullScreen() {
        /* Forces full screen */
        Window window = getWindow();
        if (window != null) {
            int flags = View.SYSTEM_UI_FLAG_FULLSCREEN |
                    View.SYSTEM_UI_FLAG_HIDE_NAVIGATION |
                    View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY |
                    View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN |
                    View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION |
                    View.SYSTEM_UI_FLAG_LAYOUT_STABLE | View.INVISIBLE;
            window.getDecorView().setSystemUiVisibility(flags);
            window.addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
            window.clearFlags(WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN);
        }
    }

    AdManager adManager;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        adManager = new AdManager(this);

        goFullScreen();
    }

    public void gameOver(int score) {
        Intent gotoNextActivity = null;

        // User is not registered, go to light registration
        if (!User.getInstance().isRegistered(getApplicationContext()))
            gotoNextActivity = new Intent(this, LightRegistrationActivity.class);
        else
            gotoNextActivity = new Intent(this, GameOverActivity.class);
        gotoNextActivity.putExtra("score", score);
        startActivity(gotoNextActivity);
    }

    @Override
    protected void onResume() {
        super.onResume();
        goFullScreen();
    }

    public void showAd() {
        this.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                adManager.showAd();
            }
        });
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        Log.i("qui", "destroyyyy");
    }
}
