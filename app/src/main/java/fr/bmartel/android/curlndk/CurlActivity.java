package fr.bmartel.android.curlndk;

import android.app.Activity;
import android.graphics.Color;
import android.net.Uri;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.MediaController;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.VideoView;

import java.util.concurrent.ThreadPoolExecutor;

public class CurlActivity extends Activity {

    private String TAG = "ARUNKKK";
    TextView msg,progress;
    private ProgressBar spinner;
    EditText url;
    Button download_File,play_Media;
    VideoView videoView;
    String gFilePath=null ;
    long  gFileSize=0 ;

    // the java declaration for your wrapper test function
    public native void init();
    public native String getCurlResponse(String url);
    public native String getJniString();
    public native int getdownloadProgress();
    public native boolean getisDownLoaded();
    public native int getTotalFileSize();
    public native String getFilePath();

    // tell java which library to load
    static {
        System.loadLibrary("curl-ndk");
    }

    public  String messageMe(final String text) {
        Log.e(TAG,"messageMe callback From CPP-JAVA :   " +text);
        //updateMsg(text);
        new Thread(new Runnable() {
            @Override
            public void run() {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                       progress.setText(" -> "+text);
                    }
                });
            }
        }).start();

        Toast.makeText(CurlActivity.this," "+text,Toast.LENGTH_SHORT).show();
        return text;
    }


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        progress = (TextView) findViewById(R.id.progress);
        spinner = (ProgressBar) findViewById(R.id.progressBar);
        spinner.setVisibility(View.INVISIBLE);
        url = (EditText) findViewById(R.id.url);
        url = (EditText) findViewById(R.id.url);
        videoView = (VideoView) findViewById(R.id.videoView);
        init();

        download_File = (Button) findViewById(R.id.downloadReq);
        play_Media = (Button) findViewById(R.id.play);

       // final TextView text = (TextView) findViewById(R.id.curl_response_txt);

        download_File.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String URL = url.getText().toString();
                downloadFile(URL);

            }
        });

    }

    private void downloadFile(final String url){
        new Thread(new Runnable() {
            final String URL = url;//"http://172.16.0.141/test/Sample.ts";
            @Override
            public void run() {
                Log.e(TAG, " getCurlResponse = "+URL);
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        spinner.setVisibility(View.VISIBLE);
                        play_Media.setVisibility(View.INVISIBLE);
                        play_Media.setClickable(false);
                        videoView.setVisibility(View.INVISIBLE);
                        progress.setText("Download in progress....");
                    }
                });

                final String response = getCurlResponse(URL); // From Native NDK code

                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        gFilePath = getFilePath(); // From Native NDK code
                        gFileSize = getTotalFileSize(); // From Native NDK code
                    }
                }).start();

                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        Log.e(TAG, "Response = "+response);

                        spinner.setVisibility(View.INVISIBLE);
                        spinner.setBackgroundColor(Color.MAGENTA);
                        play_Media.setTextColor(Color.GREEN);
                        play_Media.setClickable(true);
                        play_Media.setVisibility(View.VISIBLE);
                        progress.setTextColor(Color.GREEN);
                        progress.setTextSize(20);
                        progress.setText("Download Completed" + " \n Size = "+ gFileSize/( 1024* 1024 )+" MBs" );

                    }
                });
            }
        }).start();
    }

    public void PlayVideo(View view) {
        progress.setVisibility(View.INVISIBLE);
        videoView.setVisibility(View.VISIBLE);

        MediaController mediaController= new MediaController(this);
        mediaController.setAnchorView(videoView);
String filePath =getFilePath();
        Log.e(TAG,"File Path "+filePath);
        Uri uri = Uri.parse(filePath);

        videoView.setMediaController(mediaController);

        videoView.setVideoURI(uri);

        videoView.requestFocus();
        videoView.start();
    }
}
