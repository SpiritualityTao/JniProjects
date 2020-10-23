package com.peter.study.jniprojects;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = findViewById(R.id.sample_text);
        tv.setText(getPerson().toString());

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
}
