package com.nan.voicechange;

import android.Manifest;
import android.os.Build;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;

import java.io.File;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            requestPermissions(new String[]{Manifest.permission.READ_EXTERNAL_STORAGE, Manifest.permission.WRITE_EXTERNAL_STORAGE, Manifest.permission.MOUNT_UNMOUNT_FILESYSTEMS}, 0);
        }

    }

    public void mFix(View view) {

        String voicePath = Environment.getExternalStorageDirectory().getAbsolutePath() + File.separatorChar
                + "love.mp3";
        int type = VoiceEffectUtils.TYPE_NORMAL;
        switch (view.getId()) {
            case R.id.btn_record:
                type = VoiceEffectUtils.TYPE_NORMAL;
                break;
            case R.id.btn_luoli:
                type = VoiceEffectUtils.TYPE_LUOLI;
                break;
            case R.id.btn_dashu:
                type = VoiceEffectUtils.TYPE_DASHU;
                break;
            case R.id.btn_jingsong:
                type = VoiceEffectUtils.TYPE_JINGSONG;
                break;
            case R.id.btn_gaoguai:
                type = VoiceEffectUtils.TYPE_GAOGUAI;
                break;
            case R.id.btn_kongling:
                type = VoiceEffectUtils.TYPE_KONGLING;
                break;
        }
        play(voicePath, type);
    }

    private void play(final String voicePath, final int type) {
        new Thread(new Runnable() {
            @Override
            public void run() {
                VoiceEffectUtils.playFixVoice(voicePath, type);
            }
        }).start();
    }

}
