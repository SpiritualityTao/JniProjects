package com.peter.study.jniprojects;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.Looper;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private int count = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = findViewById(R.id.sample_text);
        // jni 新建Java类对象返回
        tv.setText(getPerson().toString());

        // java传递给Jni值
        nativeTransmit(false,
                (byte) 1,
                ';',
                (short) 2,
                5,
                7.7f,
                8.8d,
                "Peter",
                25,
                new int[]{1, 2, 3, 4, 5, 6, 7},
                new String[]{"abc", "def", "xyz"},
                new Person("宙斯"),
                new boolean[]{true, false}
        );

    }

    public void testJniThread(View view) {
        testOriginalThread();
    }

    public void testSync(View view) {
        for (int i = 0; i < 10; i++) {
            new Thread(new Runnable() {
                @Override
                public void run() {
                    count();
                    nativeCount();
                }
            }).start();
        }
    }

    public void testJniException(View view) {
        // jni捕获，跑出异常
        handleException("异常处理");
    }

    public void testException() {
        throw new NullPointerException("MainActivity throw NullPointerException in function(testException)");
    }

    private void count() {
        synchronized (this) {
            count++;
            Log.d("Java", "count: " + count);
        }
    }

    public native void nativeCount();

    public native void dynamicRegister(String name);

    public native void handleException(String name);

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native Person getPerson();

    public native void nativeTransmit(boolean b,
                                      byte b1,
                                      char c,
                                      short s,
                                      long l,
                                      float f,
                                      double d,
                                      String name,
                                      int age,
                                      int[] i,
                                      String[] strs,
                                      Person person,
                                      boolean[] booleans);

    // AndroidUI操作，让C++线程里面来调用
    public void updateUI() {
        if (Looper.getMainLooper() == Looper.myLooper()) {
            new AlertDialog.Builder(MainActivity.this)
                    .setTitle("UI")
                    .setMessage("native 运行在主线程，直接更新 UI ...")
                    .setPositiveButton("确认", null)
                    .show();
        } else {
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    new AlertDialog.Builder(MainActivity.this)
                            .setTitle("UI")
                            .setMessage("native运行在子线程切换为主线程更新 UI ...")
                            .setPositiveButton("确认", null)
                            .show();
                }
            });
        }
    }

    public native void testOriginalThread();

    public native void unThread();

    @Override
    protected void onDestroy() {
        super.onDestroy();
        unThread();
    }


}
