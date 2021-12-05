package com.jokyxray.aviplayer

import android.os.Bundle
import android.view.Surface
import android.view.SurfaceHolder
import com.jokyxray.aviplayer.databinding.ActivityNativeWindowPlayerBinding
import java.util.concurrent.atomic.AtomicBoolean

class NativeWindowPlayerActivity : AbstractPlayerActivity() {
    private lateinit var binding: ActivityNativeWindowPlayerBinding
    private val isPlaying = AtomicBoolean()
    private lateinit var surfaceHolder:SurfaceHolder
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityNativeWindowPlayerBinding.inflate(layoutInflater)
        setContentView(binding.root)
        surfaceHolder = binding.surfaceView.holder
        surfaceHolder.addCallback(surfaceHolderCallback)
    }

    private val surfaceHolderCallback = object :SurfaceHolder.Callback{
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
        val surface = surfaceHolder.surface
        init(avi,surface)
        val frameDelay:Long = (1000/ getFrameRate(avi)).toLong()
        while (isPlaying.get()){
            render(avi,surface)
            try {
                Thread.sleep(frameDelay)
            }catch (e:Exception){
                break
            }
        }
    }
    private companion object{
        @JvmStatic
        private external fun init(avi:Long,surface: Surface)
        @JvmStatic
        private external fun render(avi:Long,surface: Surface):Boolean
    }
}