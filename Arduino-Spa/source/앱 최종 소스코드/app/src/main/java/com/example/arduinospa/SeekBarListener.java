package com.example.arduinospa;

import android.widget.SeekBar;
import android.widget.TextView;
import com.example.arduinospa.BluetoothSerialClient.BluetoothStreamingHandler;

public class SeekBarListener {
    private int progressVal;
    public int getProgressVal(){
        return progressVal;
    }
    public void setProgressVal(int val){
        progressVal =val;
    }
    public SeekBarListener(SeekBar tSB, final TextView display, final BluetoothStreamingHandler t){
        tSB.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            //값이 바뀔때,
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                if(seekBar.getId() == R.id.min_seekbar){
                    display.setText("Min : "+ (progress+10)+"℃");
                }else if(seekBar.getId() == R.id.max_seekbar){
                    display.setText("Max : "+ (progress+30)+"℃");
                }
                progressVal = progress;
            }

            @Override
            //터치를 시작할때,
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            //터치가 끝날때,
            public void onStopTrackingTouch(SeekBar seekBar) {
                String s="";
                if(seekBar.getId() == R.id.min_seekbar) {
                    s = "m" + (progressVal+10) + "$";
                }else if(seekBar.getId() == R.id.max_seekbar) {
                    s = "M" + (progressVal+30) + "$";
                }
                else if(seekBar.getId() == R.id.fan_seekbar) {
                    s = "f" + (progressVal)+"$";
                }
                s+='\n';
                if(t.write(s.getBytes())) {

                }
            }
        });

    }
}
