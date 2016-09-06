package com.example.blockworks.airui;

import android.annotation.SuppressLint;
import android.app.ActionBar;
import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Looper;
import android.os.PowerManager;
import android.util.Log;
import android.webkit.WebChromeClient;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import java.util.Locale;


/**
 * An example full-screen activity that shows and hides the system UI (i.e.
 * status bar and navigation/system bar) with user interaction.
 */
public class DeviceUIActivity extends Activity
{

    /**
     * Some older devices needs a small delay between UI widget updates
     * and a change of the status and navigation bar.
     */
    private static final int UI_ANIMATION_DELAY = 300;

    private final Handler mHideHandler = new Handler();

    private final Runnable mHidePart2Runnable = new Runnable()
    {
        @SuppressLint("InlinedApi")
        @Override
        public void run()
        {
        }
    };


    //
    //private View mControlsView;
    //
    private final Runnable mShowPart2Runnable = new Runnable()
    {
        @Override
        public void run()
        {
            /*
            // Delayed display of UI elements
            ActionBar actionBar = getActionBar();
            if (actionBar != null)
            {
                actionBar.show();
            }
            */
        }
    };



    //
    //
    //
    private final Runnable mHideRunnable = new Runnable()
    {
        @Override
        public void run()
        {
            hide();
        }
    };
    private PowerManager.WakeLock   wakeLock    = null;


    @Override
    protected void onStop()
    {
        super.onStop();  // Always call the superclass method first

        //
        //
        //
        WebView webView     = (WebView)findViewById(R.id.webview);
        webView.destroy();

    }

    //
    //
    //
    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_device_ui);

        WebView webView     = (WebView)findViewById(R.id.webview);
        webView.setWebChromeClient(new WebChromeClient());
        webView.getSettings().setJavaScriptEnabled(true);

        //
        // Disable caching.
        //
        webView.getSettings().setAppCacheEnabled(false);
        webView.getSettings().setCacheMode(WebSettings.LOAD_NO_CACHE);

        //
        //
        //
        webView.setWebViewClient(new WebViewClient()
        {
            @Override
            public boolean shouldOverrideUrlLoading(WebView view, String url)
            {
                view.loadUrl(url);
                System.out.println("hello");
                return true;
            }
        });
        //webView.loadUrl(url);

        //
        // Get the device identity.
        //
        Bundle b = getIntent().getExtras();

        //
        // Form the URL for the device UI.
        //
        String  url     = String.format( Locale.UK, "http://ZooKoo.com/DeviceUI/%08d", b.getInt("Identity") );
        Log.i("DeviceUI","loading URL: "+url);


        //
        // turn on the backlight.
        //
        PowerManager powerManager = (PowerManager) getSystemService(POWER_SERVICE);
        wakeLock = powerManager.newWakeLock(PowerManager.SCREEN_BRIGHT_WAKE_LOCK, "MyWakelockTag");
        wakeLock.acquire();

        //
        // Release the wakelock after 2sec.
        //
        Handler handler = new Handler(Looper.getMainLooper());
        handler.postDelayed(new Runnable()
        {
            public void run()
            {
                wakeLock.release();
            }
        }, 2000);

        //
        // Hide the action bar.
        //
        ActionBar actionBar = getActionBar();
        if (actionBar != null)
        {
            actionBar.hide();
        }


        //
        // Show the webview with the UI on it.
        //
        webView.loadUrl(url);
    }




    //
    //
    //
    @Override
    protected void onPostCreate(Bundle savedInstanceState)
    {
        super.onPostCreate(savedInstanceState);

        // Trigger the initial hide() shortly after the activity has been
        // created, to briefly hint to the user that UI controls
        // are available.
        //delayedHide(100);
    }



    //
    //
    //
    private void hide()
    {
        /*
        // Hide UI first
        ActionBar actionBar = getActionBar();
        if (actionBar != null)
        {
            actionBar.hide();
        }
        */
        // Schedule a runnable to remove the status and navigation bar after a delay
        mHideHandler.removeCallbacks(mShowPart2Runnable);
        mHideHandler.postDelayed(mHidePart2Runnable, UI_ANIMATION_DELAY);

    }



    /**
     * Schedules a call to hide() in [delay] milliseconds, canceling any
     * previously scheduled calls.
     */
    private void delayedHide(int delayMillis)
    {
        mHideHandler.removeCallbacks(mHideRunnable);
        mHideHandler.postDelayed(mHideRunnable, delayMillis);
    }





    //
    //
    //
    @Override
    public void onBackPressed()
    {
        setResult(Activity.RESULT_CANCELED, null);
        finish();
    }




}
