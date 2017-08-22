package fr.bmartel.android.curlndk;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

public class CurlActivity extends Activity {

    private String TAG = "ARUNKKK";
      TextView tv1  ;//= (TextView) findViewById(R.id.textView1);
    private ThreadPoolExecutor executor = null;

    // the java declaration for your wrapper test function
    public native void initJNI();
    public native String getCurlResponse(String url);
    public native String getJniString();
    // tell java which library to load
    static {
        System.loadLibrary("curl-ndk");
    }

    public  String messageMe(final String text) {

        Log.e("ARUNKKK","messageMe---:"+ text+": ---Tid =" +Thread.currentThread().getId());
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Log.e("ARUNKKK","Runnable messageMe---:"+ text+": ---Tid =" +Thread.currentThread().getId());

             //   CurlActivity.this.tv1.setText(text);
            }
        });
        Toast.makeText(CurlActivity.this," "+text,Toast.LENGTH_SHORT).show();
        return text;
    }


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        final TextView tv= (TextView) findViewById(R.id.textView1);
        Log.e(TAG, "Create CurlActivity  Tid = " + Thread.currentThread().getId());



        //this.initJNI();
        tv.setText(this.getJniString());
        executor = new ThreadPoolExecutor(1, 1, 1, TimeUnit.MINUTES, new ArrayBlockingQueue<Runnable>(1, true), new ThreadPoolExecutor.CallerRunsPolicy());


        Button button = (Button) findViewById(R.id.curl_request_btn);

        final TextView text = (TextView) findViewById(R.id.curl_response_txt);

        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                //run that in separate thread to avoid ui blocking
                executor.execute(new Runnable() {

                    @Override
                    public void run() {

                        String URL = "http://172.16.0.141/test/Sample.ts";//
                        //final String response = getCurlResponse("http://www.eenadu.net/homeinner.aspx?category=home&item=break89");
                        Log.i(TAG, "http request from ndk");
                        final String response = getCurlResponse(URL);

                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                Log.e("ARUNKKK","runOnUiThread Tid =" +Thread.currentThread().getId());

                                text.setText(response);
                            }
                        });
                    }

                    ;
                });
            }
        });
    }


}
