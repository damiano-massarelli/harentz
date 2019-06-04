package com.alephzero.harentz;

import android.app.Activity;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;

import com.facebook.FacebookSdk;
import com.facebook.login.LoginManager;

public class MainMenuActivity extends AppCompatActivity {

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

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main_menu);

       goFullScreen();
    }

    @Override
    protected void onResume() {
        super.onResume();
        goFullScreen();
    }

    public void play(View view) {
        view.setEnabled(false);
        Intent playActivity = new Intent(this, MainActivity.class);
        startActivity(playActivity);
    }


    public void gotoLeaderboard(View view) {
        Intent leaderboardActivity = new Intent(this, LeaderboardActivity.class);
        startActivity(leaderboardActivity);
    }

    public void gotoCredits(View view) {
        Intent leaderboardActivity = new Intent(this, CreditsActivity.class);
        startActivity(leaderboardActivity);
    }
}
