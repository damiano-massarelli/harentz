package com.alephzero.harentz;

import android.content.Context;
import android.util.Log;

import com.facebook.AccessToken;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStreamReader;

public class User {
    private static final String TOKEN_PATH = "token.tkn";
    private static final String USERNAME_PATH = "username.usr";
    private static final String PENDING_SCORE_PATH = "score.scr";
    private static final String SIGNED_UP_PATH = "signedUp";

    private static final String USER_DATA_PATH = "user.json";

    private static User instance = null;

    private User() {}

    public static User getInstance() {
        if (instance != null)
            return instance;

        instance = new User();
        return instance;
    }

    public boolean reset(Context context) {
        try {
            return saveFile(context, USER_DATA_PATH, new JSONObject("{}"));
        } catch (JSONException e) {
            return false;
        }
    }

    private boolean saveFile(Context context, String path, JSONObject data) {
        FileOutputStream outputStream;

        try {
            outputStream = context.openFileOutput(path, Context.MODE_PRIVATE);
            outputStream.write(data.toString().getBytes());
            outputStream.close();
            return true;
        } catch (Exception e) {
            return false;
        }
    }

    private JSONObject readFile(Context context, String path) {
        FileInputStream inputStream;
        StringBuilder sb = new StringBuilder();
        try {
            inputStream = context.openFileInput(path);
            InputStreamReader inputStreamReader = new InputStreamReader(inputStream);
            BufferedReader bufferedReader = new BufferedReader(inputStreamReader);
            String line;
            while ((line = bufferedReader.readLine()) != null) {
                sb.append(line);
            }
        } catch (java.io.IOException e) {
            return new JSONObject();
        }
        try {
            return new JSONObject(sb.toString());
        } catch (JSONException e) {
            return new JSONObject();
        }
    }

    public boolean saveUser(Context context, String username, String token) {
        JSONObject data = readFile(context, USER_DATA_PATH);
        try {
            data.put("token", token);
            data.put("username", username);
            saveFile(context, USER_DATA_PATH, data);
            return true;
        } catch (JSONException e) {
            return false;
        }
    }

    public String getUsername(Context context) {
        JSONObject data = readFile(context, USER_DATA_PATH);
        try {
            return data.getString("username");
        } catch (JSONException e) {
            return null;
        }
    }

    public String getToken(Context context) {
        JSONObject data = readFile(context, USER_DATA_PATH);
        try {
            return data.getString("token");
        } catch (JSONException e) {
            return null;
        }
    }

    public boolean savePendingScore(Context context, int score) {
        JSONObject data = readFile(context, USER_DATA_PATH);
        try {
            data.put("pendingScore", score);
            data.put("pendingScoreDigest", Security.generateHmac(Integer.toString(score)));
            saveFile(context, USER_DATA_PATH, data);
            return true;
        } catch (JSONException e) {
            return false;
        }
    }

    public int getPendingScore(Context context) {
        JSONObject data = readFile(context, USER_DATA_PATH);
        try {
            int score = data.getInt("pendingScore");
            String digest = data.getString("pendingScoreDigest");
            if (Security.generateHmac(Integer.toString(score)).equals(digest))
                return score;

            return 0;
        } catch (JSONException e) {
            return 0;
        }
    }

    public void removePendingScore(Context context) {
        savePendingScore(context, 0);
    }

    public boolean saveIsSignedUp(Context context) {
        JSONObject data = readFile(context, USER_DATA_PATH);
        try {
            data.put("signedUp", true);
            saveFile(context, USER_DATA_PATH, data);
            return true;
        } catch (JSONException e) {
            return false;
        }
    }

    public boolean saveIsSignedUpWithFb(Context context, boolean signedUpWithFb) {
        JSONObject data = readFile(context, USER_DATA_PATH);
        try {
            data.put("signedUpWithFb", signedUpWithFb);
            saveFile(context, USER_DATA_PATH, data);
            return true;
        } catch (JSONException e) {
            return false;
        }
    }

    public boolean isSignedUp(Context context) {
        JSONObject data = readFile(context, USER_DATA_PATH);
        try {
            return data.getBoolean("signedUp");
        } catch (JSONException e) {
            return false;
        }
    }

    public boolean isSignedUpWithFb(Context context) {
        JSONObject data = readFile(context, USER_DATA_PATH);
        try {
            return data.getBoolean("signedUpWithFb");
        } catch (JSONException e) {
            return false;
        }
    }

    /**
     * Returns whether or not the user is registered.
     * Light registered or signed up users are the same for
     * this method.
     * @return true if user is registered, false otherwise
     */
    public boolean isRegistered(Context context) {
        String username = getUsername(context);
        boolean usesFb = isSignedUpWithFb(context);

        if (!usesFb)
            return username != null;

        AccessToken accessToken = AccessToken.getCurrentAccessToken();
        boolean isLoggedIn = accessToken != null && !accessToken.isExpired();

        return username != null && isLoggedIn;
    }

}
