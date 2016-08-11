
/*
 * Copyright (c) 2015, Nordic Semiconductor
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

package com.example.blockworks.airui;

import java.io.UnsupportedEncodingException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.IntBuffer;
import java.util.ArrayList;
import java.util.List;

import android.app.Activity;
import android.app.AlertDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothManager;
import android.bluetooth.le.BluetoothLeScanner;
import android.bluetooth.le.ScanCallback;
import android.bluetooth.le.ScanFilter;
import android.bluetooth.le.ScanResult;
import android.bluetooth.le.ScanSettings;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.content.res.Configuration;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Looper;
import android.os.Message;
import android.os.PowerManager;
import android.support.v4.content.LocalBroadcastManager;
import android.util.Log;
import android.widget.RadioGroup;
import android.widget.Toast;



public class MainActivity extends Activity implements RadioGroup.OnCheckedChangeListener
{
    private BluetoothAdapter mBluetoothAdapter;

    private static final int REQUEST_ENABLE_BT = 2;
    public static final String TAG = "AirUI";
    private static final int UART_PROFILE_CONNECTED = 20;
    private static final int UART_PROFILE_DISCONNECTED = 21;
    private int mState = UART_PROFILE_DISCONNECTED;
    private UartService mService = null;
    private BluetoothAdapter mBtAdapter = null;
    private Intent uiIntent;
    private DeviceServer    deviceServer    = null;

    private BluetoothLeScanner mLEScanner;
    //private ScanSettings settings;
    //private ScanSettings settings;
    //private List<ScanFilter> filters;


    //
    //
    //
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        mBtAdapter = BluetoothAdapter.getDefaultAdapter();

        service_init();

        //
        //
        //
        deviceServer    = new DeviceServer();

        //
        // Initializes a Bluetooth adapter.  For API level 18 and above, get a reference to
        // BluetoothAdapter through BluetoothManager.
        //
        final BluetoothManager bluetoothManager = (BluetoothManager) getSystemService(Context.BLUETOOTH_SERVICE);
        mBluetoothAdapter = bluetoothManager.getAdapter();
        mLEScanner = mBluetoothAdapter.getBluetoothLeScanner();

        //
        //
        //
        //settings = new ScanSettings.Builder()
        //        .setScanMode(ScanSettings.SCAN_MODE_LOW_LATENCY)
        //        .build();
        //filters = new ArrayList<ScanFilter>();

        //
        // Start the scan.
        //
        mLEScanner.startScan(mScanCallback);
    }


    private ScanCallback mScanCallback = new ScanCallback()
    {
        @Override
        public void onScanResult(int callbackType, ScanResult result)
        {
            //Log.i("callbackType", String.valueOf(callbackType));
            //Log.i("result", result.toString());
            final BluetoothDevice device = result.getDevice();
            int rssi    = result.getRssi();

            Log.i( "Main", "Device with RSSI of "+rssi+" found");


            //
            // Check the proximity to the target device.
            // If we're close enough, connect and interrogate it.
            //
            if( (rssi >= -60) && (mService != null))
            {
                //
                // Stop the scan.
                //
                mLEScanner.stopScan(mScanCallback);

                Log.i( "Main", "Device within range! "+rssi);

                //
                //
                //
                Log.i(TAG, "Connecting device: "+ device.getAddress() );

                runOnUiThread(new Runnable()
                {
                    public void run()
                    {
                        //
                        // Connect to the device.
                        //
                        mService.connect( device.getAddress() );
                    }
                });

            }
        }

        @Override
        public void onBatchScanResults(List<ScanResult> results)
        {
            for (ScanResult sr : results)
            {
                Log.i("ScanResult - Results", sr.toString());
            }
        }

        @Override
        public void onScanFailed(int errorCode)
        {
            Log.e("Scan Failed", "Error Code: " + errorCode);
        }
    };

    //
    // UART service connected/disconnected
    //
    private ServiceConnection mServiceConnection = new ServiceConnection()
    {
        public void onServiceConnected(ComponentName className, IBinder rawBinder)
        {
        		mService = ((UartService.LocalBinder) rawBinder).getService();
        		Log.d(TAG, "onServiceConnected mService= " + mService);
        		if (!mService.initialize())
                {
                    Log.e(TAG, "Unable to initialize Bluetooth");
                    finish();
                }
        }

        public void onServiceDisconnected(ComponentName classname)
        {
            mService = null;
        }
    };


    int     timestamp   = 0;

    //
    //
    //
    private final void TransmitHaloEvent( int type, byte[] payload)
    {
        try
        {
            //
            // Get the timestamp;
            //
            timestamp++;

            //
            // form the HaloEvent data.
            //
            int[]  event   = {timestamp, type, 987654321};
            ByteBuffer  byteBuffer  = ByteBuffer.allocate( event.length * 4 );
            byteBuffer.order( ByteOrder.LITTLE_ENDIAN );
            IntBuffer   intBuffer   = byteBuffer.asIntBuffer();
            intBuffer.put(event);
            byte[]  eventData   = byteBuffer.array();

            //
            // Send the HaloEvent.
            //
            mService.writeRXCharacteristic(eventData);
        }
        catch(Exception e)
        {
            e.printStackTrace();
        }

    }

    //
    //
    //
    private final BroadcastReceiver UARTStatusChangeReceiver = new BroadcastReceiver()
    {
        public void onReceive(Context context, Intent intent)
        {
            String action = intent.getAction();

            final Intent mIntent = intent;
            if (action.equals(UartService.ACTION_GATT_CONNECTED))
            {

                //
                //
                //
                Log.d(TAG, "UART_CONNECT_MSG");
            }
           
            if (action.equals(UartService.ACTION_GATT_DISCONNECTED))
            {
                //
                // Close the sub activity.
                //
                finishActivity(123);

                Log.d(TAG, "UART_DISCONNECT_MSG");

                //
                // Start the scan.
                //
                Handler handler = new Handler(Looper.getMainLooper());
                handler.postDelayed(new Runnable()
                {
                    public void run()
                    {
                        mLEScanner.startScan(mScanCallback);
                    }
                }, 2000);

            }
            
          
            if (action.equals(UartService.ACTION_GATT_SERVICES_DISCOVERED))
            {
                //
                //
                //
                mService.enableTXNotification();


                //
                // Send an identify message to the device after 1 second.
                //
                runOnUiThread(new Runnable()
                {
                    public void run()
                    {
                        //
                        // Wait a little while for the device to settle before sending the request.
                        //
                        try
                        {
                            Thread.sleep(100);
                        }
                        catch(Exception e)
                        {
                            e.printStackTrace();
                        }

                        //
                        // Send the request.
                        //
                        /*
                        String message     = "<Identify>";
                        try
                        {
                            byte[] value = message.getBytes("UTF-8");
                            mService.writeRXCharacteristic(value);
                        }
                        catch(UnsupportedEncodingException e)
                        {
                            e.printStackTrace();
                        }
                        */

                        byte[]  payload     = {};
                        TransmitHaloEvent(0x00000001, payload);
                    }
                });


            }


            if (action.equals(UartService.ACTION_DATA_AVAILABLE))
            {
                //
                // Get the data from the UartService
                //
                final byte[] txValue = intent.getByteArrayExtra(UartService.EXTRA_DATA);
                runOnUiThread(new Runnable()
                {
                     public void run()
                     {
                         try
                         {
                            String text = new String(txValue, "UTF-8");

                             //
                             // Parse the data into a HaloEvent.
                             //
                             ByteBuffer  byteBuffer  = ByteBuffer.allocate( txValue.length );
                             byteBuffer.put(txValue);
                             byteBuffer.order( ByteOrder.LITTLE_ENDIAN );
                             int    timestamp       = byteBuffer.getInt(0);
                             int    type            = byteBuffer.getInt(4);
                             int    bytesInPayload  = byteBuffer.getInt(8);

                             Log.i("Halo", "Received HaloEvent: "+timestamp+" "+type+" "+bytesInPayload );


                             //
                             // Start the DeviceUIActivity intent.
                             //
                             uiIntent = new Intent(MainActivity.this, DeviceUIActivity.class);
                             Bundle b = new Bundle();
                             b.putString("Identity", text);
                             uiIntent .putExtras(b);
                             startActivityForResult(uiIntent , 123);
                         }
                         catch (Exception e)
                         {
                             Log.e(TAG, e.toString());
                         }
                     }
                 });
             }


            if (action.equals(UartService.DEVICE_DOES_NOT_SUPPORT_UART))
            {
            	showMessage("Device doesn't support UART. Disconnecting");
            	mService.disconnect();
            }
            
            
        }
    };



    //
    //
    //
    private void service_init()
    {
        Intent bindIntent = new Intent(this, UartService.class);
        bindService(bindIntent, mServiceConnection, Context.BIND_AUTO_CREATE);
  
        LocalBroadcastManager.getInstance(this).registerReceiver(UARTStatusChangeReceiver, makeGattUpdateIntentFilter());
    }


    //
    //
    //
    private static IntentFilter makeGattUpdateIntentFilter()
    {
        final IntentFilter intentFilter = new IntentFilter();
        intentFilter.addAction(UartService.ACTION_GATT_CONNECTED);
        intentFilter.addAction(UartService.ACTION_GATT_DISCONNECTED);
        intentFilter.addAction(UartService.ACTION_GATT_SERVICES_DISCOVERED);
        intentFilter.addAction(UartService.ACTION_DATA_AVAILABLE);
        intentFilter.addAction(UartService.DEVICE_DOES_NOT_SUPPORT_UART);
        return intentFilter;
    }



    //
    //
    //
    @Override
    public void onStart()
    {
        super.onStart();
    }


    //
    //
    //
    @Override
    public void onDestroy()
    {
    	 super.onDestroy();
        Log.d(TAG, "onDestroy()");
        
        try
        {
        	LocalBroadcastManager.getInstance(this).unregisterReceiver(UARTStatusChangeReceiver);
        }
        catch (Exception ignore)
        {
            Log.e(TAG, ignore.toString());
        } 
        unbindService(mServiceConnection);
        mService.stopSelf();
        mService= null;
    }


    //
    //
    //
    @Override
    protected void onStop()
    {
        Log.d(TAG, "onStop");
        super.onStop();
    }


    //
    //
    //
    @Override
    protected void onPause()
    {
        Log.d(TAG, "onPause");
        super.onPause();
    }


    //
    //
    //
    @Override
    protected void onRestart()
    {
        super.onRestart();
        Log.d(TAG, "onRestart");
    }


    //
    //
    //
    @Override
    public void onResume()
    {
        super.onResume();
        Log.d(TAG, "onResume");
        if (!mBtAdapter.isEnabled())
        {
            Log.i(TAG, "onResume - BT not enabled yet");
            Intent enableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableIntent, REQUEST_ENABLE_BT);
        }
 
    }

    //
    //
    //
    @Override
    public void onConfigurationChanged(Configuration newConfig)
    {
        super.onConfigurationChanged(newConfig);
    }


    //
    //
    //
    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data)
    {
    }


    //
    //
    //
    @Override
    public void onCheckedChanged(RadioGroup group, int checkedId)
    {
       
    }


    //
    //
    //
    private void showMessage(String msg)
    {
        Toast.makeText(this, msg, Toast.LENGTH_SHORT).show();
    }


    //
    //
    //
    @Override
    public void onBackPressed()
    {
        if (mState == UART_PROFILE_CONNECTED)
        {
            Intent startMain = new Intent(Intent.ACTION_MAIN);
            startMain.addCategory(Intent.CATEGORY_HOME);
            startMain.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            startActivity(startMain);
            showMessage("nRFUART's running in background.\n             Disconnect to exit");
        }
        else
        {
            new AlertDialog.Builder(this)
            .setIcon(android.R.drawable.ic_dialog_alert)
            .setTitle(R.string.popup_title)
            .setMessage(R.string.popup_message)
            .setPositiveButton(R.string.popup_yes, new DialogInterface.OnClickListener()
                {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
   	                finish();
                }
            })
            .setNegativeButton(R.string.popup_no, null)
            .show();
        }
    }



}
