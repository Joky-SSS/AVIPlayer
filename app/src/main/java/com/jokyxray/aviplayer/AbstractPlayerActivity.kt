package com.jokyxray.aviplayer

import androidx.appcompat.app.AlertDialog
import androidx.appcompat.app.AppCompatActivity
import java.io.IOException

open class AbstractPlayerActivity : AppCompatActivity() {

    companion object {
        const val EXTRA_FILE_NAME = "EXTRA_FILE_NAME"
        @JvmStatic
        protected external fun open(fileName:String):Long
        @JvmStatic
        protected external fun getWidth(avi:Long):Int
        @JvmStatic
        protected external fun getHeight(avi:Long):Int
        @JvmStatic
        protected external fun getFrameRate(avi:Long):Double
        @JvmStatic
        protected external fun close(avi:Long)

        init {
            System.loadLibrary("aviplayer")
        }
    }

    protected var avi = 0L

    override fun onStart() {
        super.onStart()
        try {
            avi = open(getFileName())
        } catch (ioException: IOException) {
            AlertDialog.Builder(this).setTitle(R.string.error_alert_title)
                .setMessage(ioException.message).show()
        }
    }

    override fun onStop() {
        super.onStop()
        if (0L != avi) {
            close(avi)
            avi = 0
        }
    }

    private fun getFileName():String{
        return intent.extras?.getString(EXTRA_FILE_NAME,"")?:""
    }


}