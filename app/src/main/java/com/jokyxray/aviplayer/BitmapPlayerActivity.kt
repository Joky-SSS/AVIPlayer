package com.jokyxray.aviplayer

import android.graphics.Bitmap
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.SurfaceHolder
import com.jokyxray.aviplayer.databinding.ActivityBitmapPlayerBinding
import java.lang.Exception
import java.util.concurrent.atomic.AtomicBoolean

class BitmapPlayerActivity : AbstractPlayerActivity() {
    private lateinit var binding: ActivityBitmapPlayerBinding
    private lateinit var surfaceHolder: SurfaceHolder
    private val isPlaying = AtomicBoolean()
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityBitmapPlayerBinding.inflate(layoutInflater)
        setContentView(binding.root)
        surfaceHolder = binding.surfaceView.holder
        surfaceHolder.addCallback(surfaceHolderCallback)
    }

    private val surfaceHolderCallback = object : SurfaceHolder.Callback {
        override fun surfaceCreated(holder: SurfaceHolder) {
            isPlaying.set(true)
            Thread(renderer).start()
        }

        override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int) {

        }

        override fun surfaceDestroyed(holder: SurfaceHolder) {
            isPlaying.set(false)
        }
    }

    private val renderer = Runnable {
        val bitmap = Bitmap.createBitmap(getWidth(avi), getHeight(avi), Bitmap.Config.RGB_565)
        val frameDelay: Long = (1000 / getFrameRate(avi)).toLong()
        while (isPlaying.get()) {
            render(avi, bitmap)
            val canvas = surfaceHolder.lockCanvas()
            canvas.drawBitmap(bitmap, 0F, 0F, null)
            surfaceHolder.unlockCanvasAndPost(canvas)
            try {
                Thread.sleep(frameDelay)
            } catch (e: Exception) {
            }
        }
    }

    companion object {
        @JvmStatic
        private external fun render(avi: Long, bitmap: Bitmap): Boolean
    }
}