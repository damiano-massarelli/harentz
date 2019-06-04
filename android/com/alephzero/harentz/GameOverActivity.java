package com.alephzero.harentz;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

public class GameOverActivity extends AppCompatActivity {

    private ProgressBar progressBar;
    private TextView scoreView;

    int score = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_game_over);

        progressBar = findViewById(R.id.progressBar);
        scoreView = findViewById(R.id.score_preview);

        score = getIntent().getIntExtra("score", 0);
        scoreView.setText(Integer.toString(score));
        int pendingScore = User.getInstance().getPendingScore(getApplicationContext());
        if (pendingScore > score)
            score = pendingScore;

        updateLeaderboard();
        displayLoggedInAs();
    }

    @Override
    protected void onResume() {
        super.onResume();
        displayLoggedInAs();
    }

    private void updateLeaderboard() {
        progressBar.setVisibility(View.VISIBLE);
        OnlineApiFacade.getInstance(getApplicationContext()).updateScore(score,
                () -> {
                    Toast.makeText(getApplicationContext(), "Leaderboard updated!", Toast.LENGTH_LONG).show();
                    progressBar.setVisibility(View.GONE);
                    User.getInstance().removePendingScore(getApplicationContext());
                },
                errorMsg -> {
                    Toast.makeText(getApplicationContext(), "Cannot update leaderboard, your score will be updated as soon as possible", Toast.LENGTH_LONG).show();
                    User.getInstance().savePendingScore(getApplicationContext(), score);
                    progressBar.setVisibility(View.GONE);
                });
    }

    public void gotoLeaderboard(View view) {
        Intent leaderboardActivity = new Intent(this, LeaderboardActivity.class);
        startActivity(leaderboardActivity);
    }

    public void playAgain(View view) {
        onBackPressed();
    }

    public void gotoSignup(View view) {
        Intent gotoSignup = new Intent(this, SignupActivity.class);
        startActivity(gotoSignup);
    }

    public void displayLoggedInAs() {
        if (!User.getInstance().isSignedUp(getApplicationContext()))
            return;

        findViewById(R.id.signup).setVisibility(View.GONE);
        findViewById(R.id.loggedinasPanel).setVisibility(View.VISIBLE);

        TextView username = findViewById(R.id.username);
        username.setText(User.getInstance().getUsername(getApplicationContext()));
    }
}
