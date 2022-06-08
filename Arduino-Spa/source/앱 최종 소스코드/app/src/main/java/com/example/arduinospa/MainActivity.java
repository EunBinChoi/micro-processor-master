package com.example.arduinospa;

import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.view.Menu;
import android.view.MenuItem;

import com.example.arduinospa.BluetoothSerialClient;
import com.example.arduinospa.BluetoothSerialClient.BluetoothStreamingHandler;
import com.example.arduinospa.BluetoothSerialClient.OnBluetoothEnabledListener;
import com.example.arduinospa.BluetoothSerialClient.OnScanListener;
import com.example.arduinospa.SeekBarListener;
import android.bluetooth.BluetoothDevice;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.Switch;
import android.widget.Toast;
import android.content.Intent;
import java.util.ArrayList;
import java.util.Set;
import android.bluetooth.*;
import android.content.*;
import android.util.*;
import java.io.*;
import java.util.*;
import android.app.*;
import android.widget.EditText;
import android.widget.TextView;
import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.Field;
import java.nio.ByteBuffer;
import java.util.LinkedList;
import java.util.Set;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.bluetooth.BluetoothDevice;
import android.content.DialogInterface;
import android.content.DialogInterface.OnCancelListener;
import android.graphics.Color;
import android.os.Bundle;
import android.text.Html;
import android.text.method.ScrollingMovementMethod;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewConfiguration;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.SeekBar;


public class MainActivity extends AppCompatActivity {
    private LinkedList<BluetoothDevice> mBluetoothDevices = new LinkedList<BluetoothDevice>();
    private ArrayAdapter<String> mDeviceArrayAdapter;

    private EditText mEditTextInput;
    private TextView mTextView;
    private Button mButtonSend;
    private ProgressDialog mLoadingDialog;
    private AlertDialog mDeviceListDialog;
    private Menu mMenu;
    private BluetoothSerialClient mClient;
    private LinearLayout pk_LO;
    private LinearLayout main_LO;
    private TextView temp_TV;

    private TextView min_TV;
    private SeekBar minSeekbar;
    private SeekBarListener minSBListener;


    private TextView max_TV;
    private SeekBar maxSeekbar;
    private SeekBarListener maxSBListener;

//    private TextView light_TV;
//    private SeekBar lightSeekbar;
//    private SeekBarListener lightSBListener;

    private TextView fan_TV;
    private SeekBar fanSeekbar;
    private SeekBarListener fanSBListener;

    private Switch vib_SW;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        mClient = BluetoothSerialClient.getInstance();
        pk_LO = (LinearLayout) findViewById(R.id.packet_Test_layout);
        main_LO = (LinearLayout) findViewById(R.id.main_layout);
        temp_TV = (TextView) findViewById(R.id.temp_textview);

        //min
        min_TV = (TextView) findViewById(R.id.min_textview);
        minSeekbar =(SeekBar) findViewById(R.id.min_seekbar);
        minSBListener = new SeekBarListener(minSeekbar,min_TV,mBTHandler);

        //max
        max_TV = (TextView) findViewById(R.id.max_textview);
        maxSeekbar =(SeekBar) findViewById(R.id.max_seekbar);
        maxSBListener = new SeekBarListener(maxSeekbar,max_TV,mBTHandler);

        //light
//        light_TV = (TextView) findViewById(R.id.light_textview);
//        lightSeekbar =(SeekBar) findViewById(R.id.light_seekbar);
//        lightSBListener = new SeekBarListener(lightSeekbar,light_TV,mBTHandler);

        //fan
        fan_TV = (TextView) findViewById(R.id.fan_textview);
        fanSeekbar =(SeekBar) findViewById(R.id.fan_seekbar);
        fanSBListener = new SeekBarListener(fanSeekbar,fan_TV,mBTHandler);

        //Vibration
        vib_SW = (Switch) findViewById(R.id.vibration_switch);
        vib_SW.setOnCheckedChangeListener(new Switch.OnCheckedChangeListener(){
            public void onCheckedChanged(CompoundButton cb , boolean isCheking){
                //string
                if(isCheking){
                    mBTHandler.write("v1$\n".getBytes());
                }else {
                    mBTHandler.write("v0$\n".getBytes());
                }
            }
        });

        if(mClient == null) {
            Toast.makeText(getApplicationContext(), "Cannot use the Bluetooth device.", Toast.LENGTH_SHORT).show();
            finish();
        }
        overflowMenuInActionBar();
        initProgressDialog();
        initDeviceListDialog();
        initWidget();

    }
    private void initProgressDialog() {
        mLoadingDialog = new ProgressDialog(this);
        mLoadingDialog.setCancelable(false);
    }

    private void initWidget() {
        mTextView = (TextView) findViewById(R.id.textViewTerminal);
        mTextView.setMovementMethod(new ScrollingMovementMethod());
        mEditTextInput = (EditText) findViewById(R.id.editTextInput);
        mButtonSend = (Button) findViewById(R.id.buttonSend);
        mButtonSend.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                sendStringData(mEditTextInput.getText().toString());
                mEditTextInput.setText("");
            }
        });

        min_TV.setText("Min : 10℃");
        max_TV.setText("Max : 30℃");
        temp_TV.setText("Temperature : 0℃");
        minSeekbar.setProgress(10);
        maxSeekbar.setProgress(10);
    }
    private void initDeviceListDialog() {
        mDeviceArrayAdapter = new ArrayAdapter<String>(getApplicationContext(), R.layout.item_device);
        ListView listView = new ListView(getApplicationContext());
        listView.setAdapter(mDeviceArrayAdapter);
        listView.setOnItemClickListener(new OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                String item =  (String) parent.getItemAtPosition(position);
                for(BluetoothDevice device : mBluetoothDevices) {
                    if(item.contains(device.getAddress())) {
                        connect(device);
                        mDeviceListDialog.cancel();
                    }
                }
            }
        });
        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setTitle("Select bluetooth device");
        builder.setView(listView);
        builder.setPositiveButton("Scan",
                new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        scanDevices();
                    }
                });
        mDeviceListDialog = builder.create();
        mDeviceListDialog.setCanceledOnTouchOutside(false);
    }

    private void overflowMenuInActionBar(){
        try {
            ViewConfiguration config = ViewConfiguration.get(this);
            Field menuKeyField = ViewConfiguration.class.getDeclaredField("sHasPermanentMenuKey");
            if(menuKeyField != null) {
                menuKeyField.setAccessible(true);
                menuKeyField.setBoolean(config, false);
            }
        } catch (Exception ex) {
            // 무시한다. 3.x 이 예외가 발생한다.
            // 또, 타블릿 전용으로 만들어진 3.x 버전의 디바이스는 보통 하드웨어 버튼이 존재하지 않는다.
        }
    }
    private void addDeviceToArrayAdapter(BluetoothDevice device) {
        if(mBluetoothDevices.contains(device)) {
            mBluetoothDevices.remove(device);
            mDeviceArrayAdapter.remove(device.getName() + "\n" + device.getAddress());
        }
        mBluetoothDevices.add(device);
        mDeviceArrayAdapter.add(device.getName() + "\n" + device.getAddress() );
        mDeviceArrayAdapter.notifyDataSetChanged();

    }

    private void enableBluetooth() {
        BluetoothSerialClient btSet =  mClient;
        btSet.enableBluetooth(this, new OnBluetoothEnabledListener() {
            @Override
            public void onBluetoothEnabled(boolean success) {
                if(success) {
                    getPairedDevices();
                } else {
                    finish();
                }
            }
        });
    }

    private void addText(String text) {
        mTextView.append(text);
        final int scrollAmount = mTextView.getLayout().getLineTop(mTextView.getLineCount()) - mTextView.getHeight();
        if (scrollAmount > 0)
            mTextView.scrollTo(0, scrollAmount);
        else
            mTextView.scrollTo(0, 0);
    }


    private void getPairedDevices() {
        Set<BluetoothDevice> devices =  mClient.getPairedDevices();
        for(BluetoothDevice device: devices) {
            addDeviceToArrayAdapter(device);
        }
    }

    private void scanDevices() {
        BluetoothSerialClient btSet = mClient;
        btSet.scanDevices(getApplicationContext(), new OnScanListener() {
            String message = "";

            @Override
            public void onStart() {
                Log.d("Test", "Scan Start.");
                mLoadingDialog.show();
                message = "Scanning....";
                mLoadingDialog.setMessage("Scanning....");
                mLoadingDialog.setCancelable(true);
                mLoadingDialog.setCanceledOnTouchOutside(false);
                mLoadingDialog.setOnCancelListener(new OnCancelListener() {
                    @Override
                    public void onCancel(DialogInterface dialog) {
                        BluetoothSerialClient btSet = mClient;
                        btSet.cancelScan(getApplicationContext());
                    }
                });
            }

            @Override
            public void onFoundDevice(BluetoothDevice bluetoothDevice) {
                addDeviceToArrayAdapter(bluetoothDevice);
                message += "\n" + bluetoothDevice.getName() + "\n" + bluetoothDevice.getAddress();
                mLoadingDialog.setMessage(message);
            }

            @Override
            public void onFinish() {
                Log.d("Test", "Scan finish.");
                message = "";
                mLoadingDialog.cancel();
                mLoadingDialog.setCancelable(false);
                mLoadingDialog.setOnCancelListener(null);
                mDeviceListDialog.show();
            }
        });
    }


    private void connect(BluetoothDevice device) {
        mLoadingDialog.setMessage("Connecting....");
        mLoadingDialog.setCancelable(false);
        mLoadingDialog.show();
        BluetoothSerialClient btSet =  mClient;
        btSet.connect(getApplicationContext(), device, mBTHandler);
    }

    private BluetoothStreamingHandler mBTHandler = new BluetoothStreamingHandler() {
        ByteBuffer mmByteBuffer = ByteBuffer.allocate(1024);

        @Override
        public void onError(Exception e) {
            mLoadingDialog.cancel();
            addText("Messgae : Connection error - " +  e.toString() + "\n");
            mMenu.getItem(0).setTitle(R.string.action_connect);
        }

        @Override
        public void onDisconnected() {
            mMenu.getItem(0).setTitle(R.string.action_connect);
            mLoadingDialog.cancel();
            addText("Messgae : Disconnected.\n");
        }
        @Override
        public void onData(byte[] buffer, int length) {
            if(length == 0) return;
            if(mmByteBuffer.position() + length >= mmByteBuffer.capacity()) {
                ByteBuffer newBuffer = ByteBuffer.allocate(mmByteBuffer.capacity() * 2);
                newBuffer.put(mmByteBuffer.array(), 0,  mmByteBuffer.position());
                mmByteBuffer = newBuffer;
            }
            mmByteBuffer.put(buffer, 0, length);

            if(main_LO.getVisibility() == View.VISIBLE){
                if(buffer[length - 1] == '\n') {
                    String s = new String(mmByteBuffer.array(), 0, mmByteBuffer.position());
                    String t = "";
                    if(s.charAt(0) == 'c'){
                        t = s.substring(1);
                        //s.
                    }
                    t = t.substring(0,t.length()-2);
                    temp_TV.setText("Temperature : "+ t + "℃");
                    mmByteBuffer.clear();
                }
            }else{
                if(buffer[length - 1] == '\n') {
                    addText(mClient.getConnectedDevice().getName() + " : " +
                            new String(mmByteBuffer.array(), 0, mmByteBuffer.position()) + '\n');
                    mmByteBuffer.clear();
                }
            }
        }

        @Override
        public void onConnected() {
            addText("Messgae : Connected. " + mClient.getConnectedDevice().getName() + "\n");
            mLoadingDialog.cancel();
            mMenu.getItem(0).setTitle(R.string.action_disconnect);
        }
    };

    public void sendStringData(String data) {
        data += '\0';
        byte[] buffer = data.getBytes();
        if(mBTHandler.write(buffer)) {
            addText("Me : " + data + '\n');
        }
    }

    protected void onDestroy() {
        super.onDestroy();
        mClient.claer();
    };
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        mMenu = menu;
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
       // int id = item.getItemId();

        boolean connect = mClient.isConnection();
        if(item.getItemId() == R.id.action_connect) {
            if (!connect) {
                mDeviceListDialog.show();
            } else {
                mBTHandler.close();
            }
            return true;
        }else if(item.getItemId() == R.id.change_mode){ //화면 전환

            if(pk_LO.getVisibility() ==View.VISIBLE)
            {
                pk_LO.setVisibility(View.INVISIBLE);
                main_LO.setVisibility(View.VISIBLE);
            }
            else{
                pk_LO.setVisibility(View.VISIBLE);
                main_LO.setVisibility(View.INVISIBLE);
            }

            return true;
        }
        else {
            showCodeDlg();
            return true;
        }

        //noinspection SimplifiableIfStatement
      //  if (id == R.id.action_settings) {
       //     return true;
       // }

       // return super.onOptionsItemSelected(item);
    }

    public void TurnON(View view) {
//        if (!mBluetoothAdapter.isEnabled()) {
//            Intent turnOn = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
//            startActivityForResult(turnOn, 0);
//            Toast.makeText(getApplicationContext(), "Bluetooth is Turn ON", Toast.LENGTH_LONG).show();
//        } else {
//            Toast.makeText(getApplicationContext(), "Bluetooth is ON already !!", Toast.LENGTH_LONG).show();
//        }
      //  public void sendStringData(String data) {
      //      data += '\0';
       // mBTHandler.
         //   byte[] buffer = data.getBytes();
        String s = "tet";
        if(mBTHandler.write(s.getBytes())) {

        }
    }

    private void showCodeDlg() {
        TextView codeView = new TextView(this);
        codeView.setText(Html.fromHtml(readCode()));
        codeView.setMovementMethod(new ScrollingMovementMethod());
        codeView.setBackgroundColor(Color.parseColor("#202020"));
        new AlertDialog.Builder(this, android.R.style.Theme_Holo_Light_DialogWhenLarge)
                .setView(codeView)
                .setPositiveButton("OK", new AlertDialog.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        dialog.cancel();
                    }
                }).show();
    }

    private String readCode() {
        try {
            InputStream is = getAssets().open("HC_06_Echo.txt");
            int length = is.available();
            byte[] buffer = new byte[length];
            is.read(buffer);
            is.close();
            String code = new String(buffer);
            buffer = null;
            return code;
        } catch (IOException e) {
            e.printStackTrace();
        }
        return "";
    }
}
