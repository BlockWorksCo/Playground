package com.example.blockworks.airui;

import android.app.IntentService;
import android.bluetooth.BluetoothClass;
import android.content.ComponentName;
import android.content.Intent;
import android.content.Context;
import android.content.ServiceConnection;
import android.os.IBinder;
import android.util.Log;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.Reader;
import java.net.ServerSocket;
import java.net.Socket;

/**
 * An {@link IntentService} subclass for handling asynchronous task requests in
 * a service on a separate handler thread.
 * <p>
 * TODO: Customize class - update intent actions, extra parameters and static
 * helper methods.
 */
public class DeviceServer extends IntentService
{
    // TODO: Rename actions, choose action names that describe tasks that this
    // IntentService can perform, e.g. ACTION_FETCH_NEW_ITEMS
    private static final String ACTION_FOO = "com.example.blockworks.airui.action.FOO";
    private static final String ACTION_BAZ = "com.example.blockworks.airui.action.BAZ";

    // TODO: Rename parameters
    private static final String EXTRA_PARAM1 = "com.example.blockworks.airui.extra.PARAM1";
    private static final String EXTRA_PARAM2 = "com.example.blockworks.airui.extra.PARAM2";




    private  UartService mService = null;
    boolean mBound         = false;




    //
    // UART service connected/disconnected
    //
    private ServiceConnection mServiceConnection = new ServiceConnection()
    {
        public void onServiceConnected(ComponentName className, IBinder rawBinder)
        {
            mService = ((UartService.LocalBinder) rawBinder).getService();
            //Log.d(TAG, "onServiceConnected mService= " + mService);
            if (!mService.initialize())
            {
                //Log.e(TAG, "Unable to initialize Bluetooth");
                //finish();
            }
        }

        public void onServiceDisconnected(ComponentName classname)
        {
            mService = null;
        }
    };



    Thread          serverThread    = null;
    private int     readCounter     = 0;
    private Context context         = null;

    //
    //
    //
    public DeviceServer(UartService _uartService)
    {
        super("DeviceServer");

        //
        //
        //
        mService    = _uartService;

        ServerThread t = new ServerThread();
        this.serverThread = new Thread(t);
        this.serverThread.start();
    }

    //
    //
    //
    public void initialise()
    {
        //
        //
        //
        Intent intent = new Intent(context, UartService.class);
        bindService(intent, mServiceConnection, Context.BIND_AUTO_CREATE);
        //bindService(intent, mServiceConnection, 0 );
    }



    /**
     * Starts this service to perform action Foo with the given parameters. If
     * the service is already performing a task this action will be queued.
     *
     * @see IntentService
     */
    // TODO: Customize helper method
    public static void startActionFoo(Context context, String param1, String param2) {
        Intent intent = new Intent(context, DeviceServer.class);
        intent.setAction(ACTION_FOO);
        intent.putExtra(EXTRA_PARAM1, param1);
        intent.putExtra(EXTRA_PARAM2, param2);
        context.startService(intent);
    }

    /**
     * Starts this service to perform action Baz with the given parameters. If
     * the service is already performing a task this action will be queued.
     *
     * @see IntentService
     */
    // TODO: Customize helper method
    public static void startActionBaz(Context context, String param1, String param2) {
        Intent intent = new Intent(context, DeviceServer.class);
        intent.setAction(ACTION_BAZ);
        intent.putExtra(EXTRA_PARAM1, param1);
        intent.putExtra(EXTRA_PARAM2, param2);
        context.startService(intent);
    }

    @Override
    protected void onHandleIntent(Intent intent) {
        if (intent != null) {
            final String action = intent.getAction();
            if (ACTION_FOO.equals(action)) {
                final String param1 = intent.getStringExtra(EXTRA_PARAM1);
                final String param2 = intent.getStringExtra(EXTRA_PARAM2);
                handleActionFoo(param1, param2);
            } else if (ACTION_BAZ.equals(action)) {
                final String param1 = intent.getStringExtra(EXTRA_PARAM1);
                final String param2 = intent.getStringExtra(EXTRA_PARAM2);
                handleActionBaz(param1, param2);
            }
        }
    }

    /**
     * Handle action Foo in the provided background thread with the provided
     * parameters.
     */
    private void handleActionFoo(String param1, String param2) {
        // TODO: Handle action Foo
        throw new UnsupportedOperationException("Not yet implemented");
    }

    /**
     * Handle action Baz in the provided background thread with the provided
     * parameters.
     */
    private void handleActionBaz(String param1, String param2) {
        // TODO: Handle action Baz
        throw new UnsupportedOperationException("Not yet implemented");
    }


    @Override
    public void onCreate()
    {
        initialise();
        //bindService(this);
    }


    //
    //
    //
    class ServerThread implements Runnable
    {
        private InputStream input;


        //
        //
        //
        public ServerThread()
        {
        }



        //
        //
        //
        public void run()
        {
            ServerSocket serverSocket     = null;


            try
            {
                serverSocket = new ServerSocket(8080);
            }
            catch (IOException e)
            {
                e.printStackTrace();
            }


            while (!Thread.currentThread().isInterrupted())
            {
                try
                {
                    //
                    // Open the socket for this connection.
                    //
                    Socket socket = serverSocket.accept();

                    //
                    // Read the text from the browser.
                    //
                    input = socket.getInputStream();

                    reader();

                    //
                    // Write the output.
                    //
                    OutputStreamWriter output = new OutputStreamWriter(socket.getOutputStream());
                    output.write(   "HTTP/1.1 200 OK\n" +
                                    "Date: Mon, 27 Jul 2009 12:28:53 GMT\n" +
                                    "Server: Apache/2.2.14 (Win32)\n" +
                                    "Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT\n" +
                                    //"Content-Length: 88\n" +
                                    "Content-Type: text/html\n" +
                                    "Access-Control-Allow-Origin: *\n"+
                                    "Cache-Control: no-cache, no-store, must-revalidate\n" +
                                    "Pragma: no-cache\n" +
                                    "Expires: 0"+
                                    "Connection: Closed.\n\n"+
                                    "<html>\n" +
                                    "<body>\n" +
                                    "<h1>Hello, World("+readCounter+")!</h1>\n" +
                                    "</body>\n" +
                                    "</html>\n\n");
                    readCounter++;
                    output.flush();
                    socket.close();

                    //
                    //
                    //
                    byte[]  payload     = {};
                    mService.TransmitHaloEvent(0x00000001, payload);
                }
                catch (IOException e)
                {
                    e.printStackTrace();
                }
            }
        }


        //
        //
        //
        public void reader()
        {
            String  request     = "";
            try {
                do
                {
                    try
                    {
                        byte[]  requestBytes     = new byte[128];

                        int numberOfBytes = input.read(requestBytes);
                        if(numberOfBytes > 0)
                        {
                            String str = new String(requestBytes, "UTF-8");
                            request     = request + str;
                        }
                    }
                    catch (IOException e)
                    {
                        e.printStackTrace();
                    }

                } while(input.available() > 0);
            }
            catch (IOException e)
            {
                e.printStackTrace();
            }

            Log.i("Reader",request);
        }

    }





}
