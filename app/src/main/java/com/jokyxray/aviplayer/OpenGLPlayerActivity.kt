package com.jokyxray.aviplayer

import android.opengl.GLES30
import android.opengl.GLSurfaceView
import android.os.Bundle
import com.jokyxray.aviplayer.databinding.ActivityOpenGlplayerBinding
import java.util.concurrent.atomic.AtomicBoolean
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class OpenGLPlayerActivity : AbstractPlayerActivity() {
    private lateinit var binding: ActivityOpenGlplayerBinding
    private var instance:Long = 0
    private val isPlaying = AtomicBoolean()
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityOpenGlplayerBinding.inflate(layoutInflater)
        setContentView(binding.root)
        binding.glSurfaceView.setRenderer(renderer)
        binding.glSurfaceView.renderMode = GLSurfaceView.RENDERMODE_WHEN_DIRTY
    }

    override fun onStart() {
        super.onStart()
        instance = init(avi)
    }

    override fun onResume() {
        super.onResume()
        binding.glSurfaceView.onResume()
    }

    override fun onPause() {
        super.onPause()
        binding.glSurfaceView.onPause()
    }

    override fun onStop() {
        super.onStop()
        free(instance)
        instance = 0
    }

    private val player = Runnable {
        val frameDelay:Long = (1000/ getFrameRate(avi)).toLong()
        while (isPlaying.get()){
            binding.glSurfaceView.requestRender()
            try {
                Thread.sleep(frameDelay)
            }catch (e:Exception){}
        }
    }

    private val renderer = object : GLSurfaceView.Renderer{
        override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
            initSurface(instance,avi)
            isPlaying.set(true)
            Thread(player).start()
        }

        override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {

        }

        override fun onDrawFrame(gl: GL10?) {
            if(!render(instance,avi)){
                isPlaying.set(false)
            }
        }

    }

    companion object{
        @JvmStatic
        private external fun init(avi:Long):Long
        @JvmStatic
        private external fun initSurface(instance:Long,avi:Long)
        @JvmStatic
        private external fun render(instance: Long,avi: Long):Boolean
        @JvmStatic
        private external fun free(instance: Long)
    }
}