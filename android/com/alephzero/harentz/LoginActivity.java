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
import com.facebook.CallbackManager;
import com.facebook.FacebookCallback;
import com.facebook.FacebookException;
import com.facebook.GraphRequest;
import com.facebook.login.LoginManager;
import com.facebook.login.LoginResult;

import org.json.JSONException;

public class LoginActivity extends AppCompatActivity {

    private EditText username;
    private EditText password;
    private ProgressBar progressBar;

    private CallbackManager callbackManager;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);

        username = findViewById(R.id.username);
        password = findViewById(R.id.password);
        progressBar = findViewById(R.id.progressBar);

        callbackManager = CallbackManager.Factory.create();

        password.setOnEditorActionListener((TextView v, int actionId, KeyEvent event) -> {
                    if (actionId == EditorInfo.IME_ACTION_DONE) {
                        doLogin(null);
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
                        logInUser(name + " " + lastName, Security.generateHmac(name + " " + lastName), true);
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
        callbackManager.onActivityResult(requestCode, resultCode, data);
        super.onActivityResult(requestCode, resultCode, data);
    }

    public void doLogin(View view) {
        logInUser(username.getText().toString(), password.getText().toString(), false);
    }

    private void logInUser(String username, String password, boolean withFb) {
        progressBar.setVisibility(View.VISIBLE);

        OnlineApiFacade.getInstance(getApplicationContext())
                .logIn(username, password,
                        token -> {
                            Toast.makeText(getApplicationContext(), "Login successful", Toast.LENGTH_LONG).show();
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
}
