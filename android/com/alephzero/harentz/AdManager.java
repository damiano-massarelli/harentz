package com.alephzero.harentz;

import android.content.Context;

import com.google.android.gms.ads.AdListener;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.InterstitialAd;
import com.google.android.gms.ads.MobileAds;

public class AdManager {
    private static String GAME_OVER_INTERSTITIAL_ID = "ca-app-pub-3663257795267877/8751103663";

    InterstitialAd interstitial;

    public AdManager(Context context) {
        // Sample AdMob app ID: ca-app-pub-3940256099942544~3347511713
        MobileAds.initialize(context, context.getString(R.string.app_ads_id));

        interstitial = new InterstitialAd(context);
        interstitial.setAdUnitId(GAME_OVER_INTERSTITIAL_ID);

        interstitial.setAdListener(new AdListener(){
            @Override
            public void onAdClosed() {
                AdManager.this.loadInterstitial();
            }
        });

        loadInterstitial();
    }

    public void loadInterstitial() {
        AdRequest request = new AdRequest.Builder()
                //.addTestDevice(AdRequest.DEVICE_ID_EMULATOR)
                .build();

        interstitial.loadAd(request);
    }

    public void showAd() {
        if (interstitial.isLoaded())
            interstitial.show();
        else
            loadInterstitial();
    }
}
