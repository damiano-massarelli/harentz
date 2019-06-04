package com.alephzero.harentz;

import android.content.Context;
import android.util.Log;

import com.android.volley.DefaultRetryPolicy;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonArrayRequest;
import com.android.volley.toolbox.JsonObjectRequest;
import com.android.volley.toolbox.Volley;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import java9.util.function.BiConsumer;
import java9.util.function.Consumer;

public class OnlineApiFacade {

    private static final String BASE = "https://harentz.herokuapp.com";

    private static final String LIGHT_REGISTRATION_URL = BASE + "/lightRegistration";
    private static final String LEADERBOARD_URL = BASE + "/leaderboard";
    private static final String UPDATE_SCORE_URL = BASE + "/updateScore";
    private static final String REGISTRATION_URL = BASE + "/registration";
    private static final String AUTH_URL = BASE + "/auth";

    private static OnlineApiFacade instance = null;

    private RequestQueue requestQueue;
    private Context context;

    private OnlineApiFacade(Context context) {
        requestQueue = Volley.newRequestQueue(context);
        this.context = context;
    }

    public static OnlineApiFacade getInstance(Context context) {
        if (instance == null) {
            System.setProperty("http.keepAlive", "false");

            instance = new OnlineApiFacade(context);
        }

        return instance;
    }

    private void setRetryPolicy(Request<?> req) {
        req.setRetryPolicy((new DefaultRetryPolicy(
                0,
                -1,
                0)));
    }

    private void handleError(VolleyError error, Consumer<String> onError) {
        if (error != null && error.networkResponse != null && error.networkResponse.data != null) {
            try {
                JSONObject errorData = new JSONObject(new String(error.networkResponse.data, "UTF-8"));
                String errorMsg = errorData.getString("message");
                onError.accept(errorMsg);
            } catch (Exception e) {
                onError.accept(null);
            }
        } else
            onError.accept(null);
    }
    /**
     * Send light registration data to the server and returns the response data
     * @param username the chosen username
     * @param onSuccess the method called when a response is returned by the server (token is passed)
     * @param onError the method called when an error is returned by the server (error msg is passed)
     */
    void lightRegistration(String username, Consumer<String> onSuccess, Consumer<String> onError) {
        Map<String, String> body = new HashMap<String, String>();
        body.put("username", username);

        JsonObjectRequest jsonObjectRequest = new JsonObjectRequest(
                Request.Method.POST,
                LIGHT_REGISTRATION_URL, new JSONObject(body),

                (JSONObject response) -> {
                    try {
                        String token = response.getString("token");
                        onSuccess.accept(token);
                    } catch (JSONException e) {
                        onError.accept(null);
                    }
                },
                error -> {
                    handleError(error, onError);
                });

        setRetryPolicy(jsonObjectRequest);
        requestQueue.add(jsonObjectRequest);
    }

    /**
     * Fetches the online leaderboard.
     * On success the onSuccess method is called passing the whole leaderboard and the position
     * of the registered user in the list so that scroll can be performed (0 if not available).
     * On error the on error method is called.
     * @param onSuccess
     * @param onError
     */
    void loadLeaderboard(BiConsumer<JSONArray, Integer> onSuccess, Consumer<String> onError) {
        JsonArrayRequest leaderboardRequest = new JsonArrayRequest(Request.Method.GET, LEADERBOARD_URL, null,
                (JSONArray response) -> {
                    int index = 0;
                    try {
                        for (int i = 0; i < response.length(); ++i)
                            if (response.getJSONObject(i).getString("username").equals(User.getInstance().getUsername(context)))
                                index = i;
                    }  catch (JSONException e) {
                        e.printStackTrace();
                    }
                    onSuccess.accept(response, index);
                },
                error -> {
                    handleError(error, onError);
                });

        setRetryPolicy(leaderboardRequest);
        requestQueue.add(leaderboardRequest);
    }

    public void updateScore(int score, Runnable onSuccess, Consumer<String> onError) {
        String token = User.getInstance().getToken(context);
        if (token == null) {
            onError.accept("Cannot update score, not registered");
            return;
        }

        // body data
        Map<String, String> body = new HashMap<>();
        body.put("score", Integer.toString(score));
        body.put("hmac", Security.generateHmac(Integer.toString(score)));

        JsonObjectRequest updateScoreRequest = new JsonObjectRequest(Request.Method.PUT, UPDATE_SCORE_URL, new JSONObject(body),
                (JSONObject response) -> {
                    onSuccess.run();
                },
                error -> {
                    handleError(error, onError);
                }) {
                    @Override
                    public Map<String, String> getHeaders() {
                        Map<String, String>  params = new HashMap<>();
                        params.put("Authorization", "jwt " + token);
                        return params;
            }
        };

        setRetryPolicy(updateScoreRequest);
        requestQueue.add(updateScoreRequest);
    }

    public void signUp(String username, String password, Consumer<String> onSuccess, Consumer<String> onError) {
        Log.i("qui", "core de crishto");
        Map<String, String> body = new HashMap<>();
        body.put("username", username);
        body.put("password", password);

        String sendToken = User.getInstance().getToken(context);

        JsonObjectRequest jsonObjectRequest = new JsonObjectRequest(
                Request.Method.POST,
                REGISTRATION_URL, new JSONObject(body),

                (JSONObject response) -> {
                    try {
                        String token = response.getString("token");
                        onSuccess.accept(token);
                    } catch (JSONException e) {
                        onError.accept(null);
                    }
                },
                error -> {
                    handleError(error, onError);
                }) {
                    @Override
                    public Map<String, String> getHeaders() {
                        Map<String, String>  params = new HashMap<String, String>();
                        if (sendToken != null)
                            params.put("Authorization", "jwt " + sendToken);
                        return params;
            }};

        setRetryPolicy(jsonObjectRequest);
        requestQueue.add(jsonObjectRequest);
    }

    public void logIn(String username, String password, Consumer<String> onSuccess, Consumer<String> onError) {
        Map<String, String> body = new HashMap<>();
        body.put("username", username);
        body.put("password", password);


        JsonObjectRequest jsonObjectRequest = new JsonObjectRequest(
                Request.Method.POST,
                AUTH_URL, new JSONObject(body),

                (JSONObject response) -> {
                    try {
                        String token = response.getString("token");
                        onSuccess.accept(token);
                    } catch (JSONException e) {
                        onError.accept(null);
                    }
                },
                error -> {
                    handleError(error, onError);
                });

        setRetryPolicy(jsonObjectRequest);
        requestQueue.add(jsonObjectRequest);
    }

}
