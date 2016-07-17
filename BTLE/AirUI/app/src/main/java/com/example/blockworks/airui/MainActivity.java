
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
import java.text.DateFormat;
import java.util.Date;
import android.app.Activity;
import android.app.AlertDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothManager;
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
import android.os.Message;
import android.support.v4.content.LocalBroadcastManager;
import android.util.Log;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.RadioGroup;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends Activity implements RadioGroup.OnCheckedChangeListener
{
    private BluetoothAdapter mBluetoothAdapter;

    private static final int REQUEST_SELECT_DEVICE = 1;
    private static final int REQUEST_ENABLE_BT = 2;
    private static final int UART_PROFILE_READY = 10;
    public static final String TAG = "AirUI";
    private static final int UART_PROFILE_CONNECTED = 20;
    private static final int UART_PROFILE_DISCONNECTED = 21;
    private int mState = UART_PROFILE_DISCONNECTED;
    private UartService mService = null;
    private BluetoothDevice mDevice = null;
    private BluetoothAdapter mBtAdapter = null;
    private ListView messageListView;
    private ArrayAdapter<String> listAdapter;
    private Button btnConnectDisconnect,btnSend;
    private EditText edtMessage;
    private Intent uiIntent;




    //
    //
    //
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        mBtAdapter = BluetoothAdapter.getDefaultAdapter();

        messageListView = (ListView) findViewById(R.id.listMessage);
        listAdapter = new ArrayAdapter<String>(this, R.layout.message_detail);
        messageListView.setAdapter(listAdapter);
        messageListView.setDivider(null);
        btnConnectDisconnect=(Button) findViewById(R.id.btn_select);
        btnSend=(Button) findViewById(R.id.sendButton);
        edtMessage = (EditText) findViewById(R.id.sendText);
        service_init();

        //
        // Start the DeviceListActivity intent.
        //
        //Intent newIntent = new Intent(MainActivity.this, DeviceListActivity.class);
        //startActivityForResult(newIntent, REQUEST_SELECT_DEVICE);

        //
        // Initializes a Bluetooth adapter.  For API level 18 and above, get a reference to
        // BluetoothAdapter through BluetoothManager.
        //
        final BluetoothManager bluetoothManager = (BluetoothManager) getSystemService(Context.BLUETOOTH_SERVICE);
        mBluetoothAdapter = bluetoothManager.getAdapter();

        //
        // Start the scan.
        //
        mBluetoothAdapter.startLeScan(mLeScanCallback);

    }


    private BluetoothAdapter.LeScanCallback mLeScanCallback =
            new BluetoothAdapter.LeScanCallback() {

                @Override
                public void onLeScan(final BluetoothDevice device, final int rssi, byte[] scanRecord) {
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {

                            Log.i( "DeviceListActivity", "Device with RSSI of "+rssi+" found");


                            //
                            // Check the proximity to the target device.
                            // If we're close enough, connect and interrogate it.
                            //
                            if(rssi >= -60)
                            {
                                //
                                // Stop the scan.
                                //
                                mBluetoothAdapter.stopLeScan(mLeScanCallback);

                                Log.i( "DeviceListActivity", "Device within range! "+rssi);

                                //
                                // return to connect and interrogate this device.
                                //
                                Bundle b = new Bundle();
                                b.putString(BluetoothDevice.EXTRA_DEVICE, device.getAddress());

                                Intent result = new Intent();
                                result.putExtras(b);
                                setResult(Activity.RESULT_OK, result);
                                finish();
                            }

                        }
                    });
                }
            };




    //
    //UART service connected/disconnected
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


    //
    //
    //
    private Handler mHandler = new Handler()
    {
        @Override
        //Handler events that received from UART service 
        public void handleMessage(Message msg)
        {
  
        }
    };


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
                runOnUiThread(new Runnable() {
                public void run() {
                    String currentDateTimeString = DateFormat.getTimeInstance().format(new Date());
                    Log.d(TAG, "UART_CONNECT_MSG");
                    btnConnectDisconnect.setText("Disconnect");
                    edtMessage.setEnabled(true);
                    btnSend.setEnabled(true);
                    ((TextView) findViewById(R.id.deviceName)).setText(mDevice.getName()+ " - ready");
                    listAdapter.add("["+currentDateTimeString+"] Connected to: "+ mDevice.getName());
                    messageListView.smoothScrollToPosition(listAdapter.getCount() - 1);
                    mState = UART_PROFILE_CONNECTED;
                }
                });
            }
           
            if (action.equals(UartService.ACTION_GATT_DISCONNECTED))
            {
                //
                // Close the sub activity.
                //
                finishActivity(123);

                //
                // Start the DeviceList activity again.
                //
                Intent newIntent = new Intent(MainActivity.this, DeviceListActivity.class);
                startActivityForResult(newIntent, REQUEST_SELECT_DEVICE);


                //
                // Update the UI.
                //
                runOnUiThread(new Runnable() {
                     public void run() {
                    	 	 String currentDateTimeString = DateFormat.getTimeInstance().format(new Date());
                             Log.d(TAG, "UART_DISCONNECT_MSG");
                             btnConnectDisconnect.setText("Connect");
                             edtMessage.setEnabled(false);
                             btnSend.setEnabled(false);
                             ((TextView) findViewById(R.id.deviceName)).setText("Not Connected");
                             listAdapter.add("["+currentDateTimeString+"] Disconnected to: "+ mDevice.getName());
                             mState = UART_PROFILE_DISCONNECTED;
                             mService.close();
                     }
                });
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
                Handler handler = new Handler();

                handler.postDelayed(new Runnable()
                {
                    public void run()
                    {
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
                    }
                }, 1000);

            }


            if (action.equals(UartService.ACTION_DATA_AVAILABLE))
            {
                //
                // Get the data from the UartService
                //
                final byte[] txValue = intent.getByteArrayExtra(UartService.EXTRA_DATA);
                runOnUiThread(new Runnable() {
                     public void run() {
                     try
                     {
                        String text = new String(txValue, "UTF-8");
                        String currentDateTimeString = DateFormat.getTimeInstance().format(new Date());
                        listAdapter.add("["+currentDateTimeString+"] RX: "+text);
                        messageListView.smoothScrollToPosition(listAdapter.getCount() - 1);

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
        if (resultCode == Activity.RESULT_CANCELED && data == null)
        {
            //
            // Start the DeviceList activity again.
            //
            Intent newIntent = new Intent(MainActivity.this, DeviceListActivity.class);
            startActivityForResult(newIntent, REQUEST_SELECT_DEVICE);
        }


        switch (requestCode)
        {


            case REQUEST_SELECT_DEVICE:
                //When the DeviceListActivity return, with the selected device address
                if (resultCode == Activity.RESULT_OK && data != null)
                {
                    //
                    // Get the selected device and connect to it.
                    //
                    String deviceAddress = data.getStringExtra(BluetoothDevice.EXTRA_DEVICE);
                    mDevice = BluetoothAdapter.getDefaultAdapter().getRemoteDevice(deviceAddress);

                    Log.d(TAG, "... onActivityResultdevice.address==" + mDevice + "mserviceValue" + mService);

                    Log.i(TAG, "Connecting device: "+deviceAddress);
                    mService.connect(deviceAddress);
                }
                break;

            case REQUEST_ENABLE_BT:
                // When the request to enable Bluetooth returns
                if (resultCode == Activity.RESULT_OK)
                {
                    Toast.makeText(this, "Bluetooth has turned on ", Toast.LENGTH_SHORT).show();
                }
                else
                {
                    // User did not enable Bluetooth or an error occurred
                    Log.d(TAG, "BT not enabled");
                    Toast.makeText(this, "Problem in BT Turning ON ", Toast.LENGTH_SHORT).show();
                    finish();
                }
                break;

            default:
                Log.e(TAG, "wrong request code");
                break;
        }


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
