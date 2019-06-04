package com.alephzero.harentz;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.inputmethod.EditorInfo;
import android.widget.EditText;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

import com.facebook.AccessToken;
import com.facebook.AccessTokenTracker;
import com.facebook.CallbackManager;
import com.facebook.FacebookCallback;
import com.facebook.FacebookException;
import com.facebook.GraphRequest;
import com.facebook.GraphResponse;
import com.facebook.login.LoginManager;
import com.facebook.login.LoginResult;
import com.facebook.login.widget.LoginButton;

import org.json.JSONException;
import org.json.JSONObject;

public class LightRegistrationActivity extends AppCompatActivity {

    private static final String TAG = "LRA";

    private CallbackManager callbackManager;

    private EditText username;
    private ProgressBar progressBar;
    private TextView scoreView;

    private int score = 0;

    private OnlineApiFacade onlineApiFacade;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_light_registration);

        username = findViewById(R.id.username);
        progressBar = findViewById(R.id.progressBar);
        scoreView = findViewById(R.id.score_preview);

        score = getIntent().getIntExtra("score", 0);
        scoreView.setText(Integer.toString(score));

        onlineApiFacade = OnlineApiFacade.getInstance(getApplicationContext());

        username.setOnEditorActionListener((TextView v, int actionId, KeyEvent event) -> {
                    if (actionId == EditorInfo.IME_ACTION_DONE) {
                        doLightRegistration(null);
                        return true;
                    }
                    return false;
                }
        );

        callbackManager = CallbackManager.Factory.create();

        LoginButton loginButton = findViewById(R.id.login_button);
        loginButton.setPermissions("public_profile");

        LoginManager.getInstance().registerCallback(callbackManager, new FacebookCallback<LoginResult>() {
            @Override
            public void onSuccess(LoginResult loginResult) {
                AccessToken accessToken = loginResult.getAccessToken();


                GraphRequest req = GraphRequest.newMeRequest(accessToken, (object, response) -> {
                    if (object == null) return;
                    try {
                        String lastName = object.getString("last_name");
                        String name = object.getString("first_name");
                        doFbSignUp(name + " " + lastName);
                    } catch (JSONException e) {
                    }
                });

                Bundle params = new Bundle();
                params.putString("fields", "first_name,last_name");
                req.setParameters(params);
                req.executeAsync();
            }

            @Override
            public void onCancel() {
            }

            @Override
            public void onError(FacebookException e) {
            }
        });
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        callbackManager.onActivityResult(requestCode, resultCode, data);
    }

    public void doLightRegistration(View view) {
        progressBar.setVisibility(View.VISIBLE);

        String chosenUsername = username.getText().toString();

        onlineApiFacade.lightRegistration(chosenUsername,
                token -> {
                    Toast.makeText(getApplicationContext(), "Registration successful", Toast.LENGTH_LONG).show();
                    User.getInstance().saveUser(getApplicationContext(), chosenUsername, token);
                    User.getInstance().saveIsSignedUpWithFb(getApplicationContext(), false);

                    onlineApiFacade.updateScore(score,
                            () -> {
                                // go to leaderboard
                                Intent gotoLeaderboard = new Intent(this, LeaderboardActivity.class);
                                startActivity(gotoLeaderboard);
                                progressBar.setVisibility(View.GONE);
                            },
                            errorMsg -> {
                                if (errorMsg == null)
                                    errorMsg = "An error occurred, please try again later";
                                Toast.makeText(getApplicationContext(), errorMsg, Toast.LENGTH_LONG).show();
                                progressBar.setVisibility(View.GONE);
                            });
                },
                errorMsg -> {
                    if (errorMsg == null)
                        errorMsg = "An error occurred, please try again later";
                    Toast.makeText(getApplicationContext(), errorMsg, Toast.LENGTH_LONG).show();
                    progressBar.setVisibility(View.GONE);
                }
        );
    }

    private void doFbSignUp(String username) {
        progressBar.setVisibility(View.VISIBLE);
        onlineApiFacade.signUp(username, Security.generateHmac(username),
                token -> {
                    Toast.makeText(getApplicationContext(), "Registration successful", Toast.LENGTH_LONG).show();
                    User.getInstance().saveUser(getApplicationContext(), username, token);
                    User.getInstance().saveIsSignedUp(getApplicationContext());
                    User.getInstance().saveIsSignedUpWithFb(getApplicationContext(), true);

                    // now update score
                    onlineApiFacade.updateScore(score,
                            () -> {
                                // go to leaderboard
                                Intent gotoLeaderboard = new Intent(this, LeaderboardActivity.class);
                                startActivity(gotoLeaderboard);
                                progressBar.setVisibility(View.GONE);
                            },
                            errorMsg -> {
                                if (errorMsg == null)
                                    errorMsg = "An error occurred, please try again later";
                                Toast.makeText(getApplicationContext(), errorMsg, Toast.LENGTH_LONG).show();
                                progressBar.setVisibility(View.GONE);
                            });
                }, errorMsg -> {
                    // force log out if error occurred on server
                    LoginManager.getInstance().logOut();

                    if (errorMsg == null)
                        errorMsg = "An error occurred, please try again later";
                    Toast.makeText(getApplicationContext(), errorMsg, Toast.LENGTH_LONG).show();
                    progressBar.setVisibility(View.GONE);
                });
    }

    public void playAgain(View view) {
        onBackPressed();
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (User.getInstance().isRegistered(getApplicationContext()))
            onBackPressed();
    }

    public void gotoSignup(View view) {
        Intent gotoSignup = new Intent(this, SignupActivity.class);
        startActivity(gotoSignup);
    }

    public void gotoLogin(View view) {
        Intent gotoLogin = new Intent(this, LoginActivity.class);
        startActivity(gotoLogin);
    }
}
