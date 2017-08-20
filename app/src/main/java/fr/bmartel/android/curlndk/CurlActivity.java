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

public class CurlActivity extends Activity{

    private String TAG = CurlActivity.this.getClass().getName();

    private ThreadPoolExecutor executor = null;

    // the java declaration for your wrapper test function
    public native String  getCurlResponse(String url);

    public native String getJniString();

    // please, let me live even though I used this dark programming technique
    public  String messageMe(String text) {
        Log.e("ARUNKKK",text);
        Toast.makeText(CurlActivity.this," "+text,Toast.LENGTH_SHORT).show();
        return text;
    }

    // tell java which library to load
    static {
        System.loadLibrary("curl-ndk");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        TextView tv = (TextView) findViewById(R.id.textView);
        tv.setText(this.getJniString());
        executor=new ThreadPoolExecutor(1,1,1, TimeUnit.MINUTES,new ArrayBlockingQueue<Runnable>(1,true),new ThreadPoolExecutor.CallerRunsPolicy());

        Log.i(TAG,"Create CurlActivity");

        Button button = (Button) findViewById(R.id.curl_request_btn);

        final TextView text =(TextView) findViewById(R.id.curl_response_txt);

        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                //run that in separate thread to avoid ui blocking
                executor.execute(new Runnable() {

                    @Override
                    public void run() {

                        //final String response = getCurlResponse("http://jsonplaceholder.typicode.com/posts/1");
                        final String response = getCurlResponse("http://www.eenadu.net/homeinner.aspx?category=home&item=break89");
                        Log.i(TAG, "http request from ndk");

                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                text.setText(response);
                            }
                        });
                    };
                });
            }
        });
    }


}
