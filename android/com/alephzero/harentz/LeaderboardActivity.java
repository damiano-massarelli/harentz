package com.alephzero.harentz;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.view.View;
import android.widget.ListView;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;


public class LeaderboardActivity extends AppCompatActivity {

    private RecyclerView leaderboard;
    private ProgressBar progressBar;
    private TextView errorMessage;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_leaderboard);

        leaderboard = findViewById(R.id.leaderboard);
        progressBar = findViewById(R.id.progressBar);
        errorMessage = findViewById(R.id.errorMessage);
        progressBar.setVisibility(View.VISIBLE);


        leaderboard.setLayoutManager(new LinearLayoutManager(this));

        OnlineApiFacade.getInstance(getApplicationContext())
                .loadLeaderboard(
                        (scores, index) -> {
                            leaderboard.setAdapter(new LeaderboardAdatpter(scores));
                            leaderboard.post(() -> leaderboard.smoothScrollToPosition(index));
                            progressBar.setVisibility(View.GONE);
                        },
                        errorMsg -> {
                            if (errorMsg == null)
                                errorMsg = "An error occurred, please try again later";
                            Toast.makeText(getApplicationContext(), errorMsg, Toast.LENGTH_LONG).show();
                            progressBar.setVisibility(View.GONE);
                            errorMessage.setVisibility(View.VISIBLE);
                        }
                );

    }

    @Override
    protected void onResume() {
        super.onResume();
        errorMessage.setVisibility(View.GONE);
    }
}
