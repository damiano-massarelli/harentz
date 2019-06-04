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
import com.facebook.login.Login;
import com.facebook.login.LoginManager;
import com.facebook.login.LoginResult;

import org.json.JSONException;

public class SignupActivity extends AppCompatActivity {

    private EditText username;
    private EditText password;
    private ProgressBar progressBar;

    private CallbackManager callbackManager;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_signup);

        username = findViewById(R.id.username);
        password = findViewById(R.id.password);
        progressBar = findViewById(R.id.progressBar);

        String name = User.getInstance().getUsername(getApplicationContext());
        if (username != null)
            this.username.setText(name);

        callbackManager = CallbackManager.Factory.create();

        password.setOnEditorActionListener((TextView v, int actionId, KeyEvent event) -> {
            if (actionId == EditorInfo.IME_ACTION_DONE) {
                doSignup(null);
                return true;
            }
            return false;
        }
        );

        LoginManager.getInstance().registerCallback(callbackManager, new FacebookCallback<LoginResult>() {
            @Override
            public void onSuccess(LoginResult loginResult) {
                AccessToken accessToken = loginResult.getAccessToken();

                GraphRequest req = GraphRequest.newMeRequest(accessToken, (object, response) -> {
                    if (object == null) return;
                    try {
                        String lastName = object.getString("last_name");
                        String name = object.getString("first_name");
                        signUpUser(name + " " + lastName, Security.generateHmac(name + " " + lastName), true);
                    } catch (JSONException e) {
                    }
                });

                Bundle params = new Bundle();
                params.putString("fields", "first_name,last_name");
                req.setParameters(params);
                req.executeAsync();
            }

            @Override
            public void onCancel() {}

            @Override
            public void onError(FacebookException e) {}
        });
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        callbackManager.onActivityResult(requestCode, resultCode, data);
        super.onActivityResult(requestCode, resultCode, data);
    }

    public void doSignup(View view) {
        signUpUser(username.getText().toString(), password.getText().toString(), false);
    }

    private void signUpUser(String username, String password, boolean withFb) {
        progressBar.setVisibility(View.VISIBLE);

        OnlineApiFacade.getInstance(getApplicationContext())
                .signUp(username, password,
                        token -> {
                            Toast.makeText(getApplicationContext(), "Registration successful", Toast.LENGTH_LONG).show();
                            User.getInstance().saveUser(getApplicationContext(), username, token);
                            User.getInstance().saveIsSignedUp(getApplicationContext());
                            User.getInstance().saveIsSignedUpWithFb(getApplicationContext(), withFb);
                            progressBar.setVisibility(View.GONE);
                            onBackPressed();
                        },
                        errorMsg -> {
                            if (errorMsg == null)
                                errorMsg = "An error occurred, please try again later";

                            if (withFb)
                                LoginManager.getInstance().logOut();

                            Toast.makeText(getApplicationContext(), errorMsg, Toast.LENGTH_LONG).show();
                            progressBar.setVisibility(View.GONE);
                        }
                );
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (User.getInstance().isRegistered(getApplicationContext()))
            onBackPressed();
    }

    public void gotoLogin(View view) {
        Intent gotoLogin = new Intent(this, LoginActivity.class);
        startActivity(gotoLogin);
    }
}
