package com.example.battlechicken.arduino;

import android.app.ActionBar;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.hardware.Camera;
import android.media.Image;
import android.os.Build;
import android.os.Environment;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.text.format.Time;
import android.text.method.CharacterPickerDialog;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.ByteBuffer;
import java.util.UUID;


public class MainActivity extends ActionBarActivity {
    private String TAG = "Kamera";
    private final static int REQUEST_ENABLE_BT = 1;
    private BluetoothAdapter mBluetoothAdapter;
    LinearLayout devicesLayout;
    private static final UUID ARDUINO_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    private OutputStream bthOutputStream;
    Camera mCamera;
    CameraPreview mPreview;
    ImageProcessor imageProcessor = new ImageProcessor();
    CustomView mCustomView;
    boolean saveNextFrame = false;
    boolean cameraMode = false;

    //int lastTouchPosX = 0;
    //int lastTouchPosY = 0;
    /*
    View.OnClickListener onScreenTap = new View.OnClickListener() {
        @Override
        public void onClick(View view) {
            saveNextFrame = true;

        }
    };
    */
    //private InputStream bthInputStream;
    //private ListView myListView;

    private class BluetoothDeviceView extends TextView{
        public BluetoothDevice getDevice() {
            return device;
        }

        private BluetoothDevice device;

        BluetoothDeviceView(Context context, BluetoothDevice device) {
            super(context);
            this.device = device;
        }
    }


    Camera.PreviewCallback previewCallback = new Camera.PreviewCallback() {
        volatile boolean locked = false;
        @Override
        public void onPreviewFrame(byte[] bytes, Camera camera) {
            if( !locked ) {
                locked = true;
                Camera.Parameters parameters = camera.getParameters();
                Camera.Size size = parameters.getPreviewSize();
                ImageProcessor.Result result = imageProcessor.process(bytes, size.width, size.height);
                //Toast.makeText(mCustomView.getContext(), "lefutott", Toast.LENGTH_SHORT).show();
                //mCustomView.setCirclePos(result.x, result.y);
                //mCustomView.setResult(result);
                //mCustomView.invalidate();
                processResult(result);
                locked = false;
            }

        }
    };

    class CustomView extends View {
        private double y = 0.5;
        private double x = 0.5;
        private int color = Color.RED;
        ImageProcessor.Result lastResult;

        String text = "Semmi";

        public CustomView(Context context) {
            super(context);
        }

        public void setText(String text){
            this.text = text;
            invalidate();
        }

        // 0 es 1 kozott!
        public void setCirclePos(double x, double y) {
            this.x = x;
            this.y = y;
        }

        public void setResult(ImageProcessor.Result result) {
            if(result.success) {
                setCirclePos(result.x, result.y);
                color = Color.GREEN;
            } else {
                setCirclePos(0.5, 0.5);
                color = Color.RED;
            }
            lastResult = result;

            invalidate();
            /*
            if(lastResult.bitmap != null) {
                color = Color.BLUE;
            } else {
                color = Color.YELLOW;
            }*/
        }
        @Override
        protected void onDraw(Canvas canvas) {
            super.onDraw(canvas);

            Paint paint = new Paint();
            paint.setARGB(100, Color.red(color), Color.green(color), Color.blue(color));
            //paint.setColor(color);
            int r = canvas.getHeight() / 8;

            canvas.drawCircle((int) (x * canvas.getWidth()), (int) (y * canvas.getHeight()), r, paint);

            Paint textPaint = new Paint();
            textPaint.setColor(Color.GREEN);
            canvas.drawText("Bejovo : " + text, 30, 30, textPaint);

            if (lastResult != null && lastResult.image != null ) {
                if( saveNextFrame && cameraMode ) {
                    saveNextFrame = false;
                    //canvas.drawBitmap(lastResult.image.ToGrayBitmap(), 0, 0, null);

                    String fileName = Environment.getExternalStorageDirectory() + "/frame" + System.currentTimeMillis() / 1000 + ".png";
                    try {
                        FileOutputStream fos = new FileOutputStream(fileName);
                        //lastResult.image.ToGrayBitmap().compress(Bitmap.CompressFormat.PNG, 0, fos);
                        throw new RuntimeException("nincs megirva");
                        //fos.close();
                        //Toast.makeText(this.getContext(), fileName, Toast.LENGTH_SHORT).show();
                    } catch (IOException e) {
                        Toast.makeText(this.getContext(), "Fail: " + fileName, Toast.LENGTH_SHORT).show();
                        e.printStackTrace();
                    }

                }

            }
            //canvas.drawARGB(128, 0, 255, 255);
            //canvas.drawPoint();
            /*
            for (int x = 0; x < canvas.getWidth(); ++x) {
                for (int y = 0; y < canvas.getHeight() ; ++y) {
                    canvas.drawARGB(128, 0, 255, 255);
                }
            }*/

        }
    }

    void hideTaskBar() {
        if (Build.VERSION.SDK_INT < 16) {
            getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                    WindowManager.LayoutParams.FLAG_FULLSCREEN);
        } else {
            View decorView = getWindow().getDecorView();
            // Hide the status bar.
            int uiOptions = View.SYSTEM_UI_FLAG_FULLSCREEN;
            decorView.setSystemUiVisibility(uiOptions);
            // Remember that you should never show the action bar if the
            // status bar is hidden, so hide that too if necessary.
            ActionBar actionBar = getActionBar();
            if (actionBar != null) {
                actionBar.hide();
            } else {
                android.support.v7.app.ActionBar supportActionBar = getSupportActionBar();
                if( supportActionBar != null ) {
                    supportActionBar.hide();
                }
            }




        }

    }
    void EnterCameraMode() {
        hideTaskBar();
        //FrameLayout frameLayout = new FrameLayout(this);
        RelativeLayout frameLayout = new RelativeLayout(this);



        //mCamera.startPreview();

        mPreview = new CameraPreview(this, mCamera);
        frameLayout.addView(mPreview);
        mPreview.setPreviewCallback(previewCallback);

        //mCustomView.setOnClickListener(onScreenTap);
        mCustomView.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                saveNextFrame = true;
                //lastTouchPosX = (int)motionEvent.getX();
                //lastTouchPosY = (int)motionEvent.getY();
                ImageProcessor.Result fakeResult = new ImageProcessor.Result();

                fakeResult.x = motionEvent.getX() / view.getWidth();
                fakeResult.y = motionEvent.getY() / view.getHeight();
                fakeResult.success = true;

                processResult(fakeResult);
                //mCustomView.setResult(fakeResult);

                return true;
            }
        });
        frameLayout.addView(mCustomView);

        setContentView(frameLayout);
    }

    boolean bluetoothWarningDisplayed = false;

    private void processResult(ImageProcessor.Result result) {
        mCustomView.setResult(result);
        if (bthOutputStream == null) {
            if( !bluetoothWarningDisplayed ) {
                Toast.makeText(this, "Bluetooth kapcsolat nincs inicializalva", Toast.LENGTH_LONG).show();
                bluetoothWarningDisplayed = true;
            }
            return;
        }
        final float csodakonstans = 0.466307f;
        double p = (result.x - 0.5f)*2;
        double angle = Math.atan(p * csodakonstans);
        try {
            bthOutputStream.write(result.success ? 1 : 0);
            //bthOutputStream.write( float2ByteArray(angle) );
            bthOutputStream.write((byte) angle);
        } catch (IOException e) {
            e.printStackTrace();
            Toast.makeText(this, "Bluetooth send fail", Toast.LENGTH_LONG).show();
            System.exit(0);
        }

    }

    public static byte [] float2ByteArray (float value)
    {
        return ByteBuffer.allocate(4).putFloat(value).array();
    }

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mCustomView = new CustomView(this);
        // Create an instance of Camera
        mCamera = getCameraInstance();

        // Create our Preview view and set it as the content of our activity.
        if( cameraMode ) {
            EnterCameraMode();
            return;
        }


        //FrameLayout preview = (FrameLayout) findViewById(R.id.camera_preview);
        //preview.addView(mPreview);



        //LinearLayout devicesListView = (LinearLayout) findViewById(R.id.DevicesList);
        //myListView = (ListView)findViewById(R.id.bluetooth_devices_list);
        devicesLayout = (LinearLayout) findViewById(R.id.devices_layout);


        //TextView textView = new TextView(this.getApplicationContext());
        //textView.setText("asd");
        //textView.setHeight(View.);
        //devicesLayout.addView(textView);
        //devicesLayout.addView(textView);
        //if("asd" != "3252") return;

        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if (mBluetoothAdapter == null) {
            Toast.makeText(getApplicationContext(), "Device does not support Bluetooth", Toast.LENGTH_LONG).show();
            return;
        }

        if (!mBluetoothAdapter.isEnabled()) {
            Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
        } else {
            onBluetoothEnabled();

        }
    }

    private final BroadcastReceiver mReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();


            // When discovery finds a device
            if (BluetoothDevice.ACTION_FOUND.equals(action)) {

                // Get the BluetoothDevice object from the Intent
                BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                Toast.makeText(context, device.getName(), Toast.LENGTH_SHORT).show();
                // Add the name and address to an array adapter to show in a ListView
                //mArrayAdapter.add(device.getName() + "\n" + device.getAddress());

                BluetoothDeviceView textView = new BluetoothDeviceView(context, device);
                textView.setText(device.getName());
                textView.setTextSize(30);
                textView.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        BluetoothDeviceView view = (BluetoothDeviceView)v;
                        BluetoothDevice device = view.getDevice();

                        connectToBthDevice(device);
                    }
                });
                //myListView.addView(textView);
                devicesLayout.addView(textView);

            }
        }
    };

    void connectToBthDevice(BluetoothDevice device){

        try {
            final BluetoothSocket bthSocket = device.createRfcommSocketToServiceRecord(ARDUINO_UUID);
            mBluetoothAdapter.cancelDiscovery();
            //connectToDevice(bthSocket);
            //serverSocket = bthSocket;

            bthSocket.connect();
            //EnterCameraMode();
            /*
            setContentView(R.layout.activity_communication);

            Button sendButton = (Button) findViewById(R.id.button_send);
            sendButton.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    try {
                        bthOutputStream.write("Hello".getBytes("US-ASCII"));
                        bthOutputStream.flush();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            });
               */


            //bthInputStream = bthSocket.getInputStream();
            bthOutputStream = bthSocket.getOutputStream();



            final TextView outputView = (TextView)findViewById(R.id.arduino_output);

            Thread thread = new Thread(new Runnable() {
                @Override
                public void run() {
                    try {
                        InputStream bthInputStream = bthSocket.getInputStream();

                        while(true) {
                            byte[] buffer = new byte[100];
                            bthInputStream.read(buffer);
                            final String input = new String(buffer, "US-ASCII");

                            Toast.makeText(getApplicationContext(), "jott: " + input, Toast.LENGTH_LONG).show();

                            runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                    //outputView.append(input);
                                    mCustomView.setText(input);
                                }
                            });

                            //
                        }

                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            });
            thread.start();

            EnterCameraMode();

        } catch (Exception e) {
            Log.e("BTH", "IO exc. @ connecttobthdevice");
            e.printStackTrace();
        }

    }


    void onBluetoothEnabled() {
        // Create a BroadcastReceiver for ACTION_FOUND

        // Register the BroadcastReceiver
        IntentFilter filter = new IntentFilter(BluetoothDevice.ACTION_FOUND);
        registerReceiver(mReceiver, filter); // Don't forget to unregister during onDestroy

        Toast.makeText(getApplicationContext(), "Searching", Toast.LENGTH_LONG).show();
        mBluetoothAdapter.startDiscovery();
    }

    @Override
    protected void onDestroy() {
        unregisterReceiver(mReceiver);
        super.onDestroy();
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        switch(resultCode) {
            case REQUEST_ENABLE_BT:
                onBluetoothEnabled();
                break;
        }

        //super.onActivityResult(requestCode, resultCode, data);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        if( id == R.id.action_restart) {
            System.exit(0);
        }

        if( id == R.id.action_enter_camera_mode) {
            EnterCameraMode();
        }

        return super.onOptionsItemSelected(item);
    }



    /** A basic Camera preview class */
    public class CameraPreview extends SurfaceView implements SurfaceHolder.Callback {
        private SurfaceHolder mHolder;
        private Camera mCamera;
        private Camera.PreviewCallback mPreviewCallback;



        public CameraPreview(Context context, Camera camera) {
            super(context);
            mCamera = camera;

            // Install a SurfaceHolder.Callback so we get notified when the
            // underlying surface is created and destroyed.
            mHolder = getHolder();
            mHolder.addCallback(this);
            // deprecated setting, but required on Android versions prior to 3.0
            mHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
        }
        public void setPreviewCallback(Camera.PreviewCallback previewCallback) {
            this.mPreviewCallback = previewCallback;
            mCamera.setPreviewCallback(previewCallback);
        }
        public void surfaceCreated(SurfaceHolder holder) {
            // The Surface has been created, now tell the camera where to draw the preview.
            try {
                mCamera.setPreviewDisplay(holder);
                mCamera.startPreview();
                mCamera.setPreviewCallback(previewCallback);
            } catch (IOException e) {
                Log.d("Kamera ", "Error setting camera preview: " + e.getMessage());
            }
        }

        public void surfaceDestroyed(SurfaceHolder holder) {
            // empty. Take care of releasing the Camera preview in your activity.
        }

        public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
            // If your preview can change or rotate, take care of those events here.
            // Make sure to stop the preview before resizing or reformatting it.

            if (mHolder.getSurface() == null){
                // preview surface does not exist
                return;
            }

            // stop preview before making changes
            try {
                mCamera.stopPreview();
            } catch (Exception e){
                // ignore: tried to stop a non-existent preview
            }

            // set preview size and make any resize, rotate or
            // reformatting changes here

            // start preview with new settings
            try {
                mCamera.setPreviewDisplay(mHolder);
                mCamera.startPreview();
                mCamera.setPreviewCallback( mPreviewCallback);
            } catch (Exception e){
                Log.d(TAG, "Error starting camera preview: " + e.getMessage());
            }
        }
    }


    /** A safe way to get an instance of the Camera object. */
    public static Camera getCameraInstance(){
        Camera c = null;
        try {
            c = Camera.open(); // attempt to get a Camera instance
        }
        catch (Exception e){
            // Camera is not available (in use or does not exist)
            // Inkabb fagyjon ki minthogy lenyelje ugyse tudunk vele mit kezdeni
            throw e;
        }
        return c; // returns null if camera is unavailable
    }
}
