/*
 * Copyright 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package krustyfishgl.modelpreviewer;

import android.content.Context;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;
import android.util.Log;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.ScaleGestureDetector;

import java.io.IOException;
import java.util.Arrays;
import java.util.List;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

class GLES2JNIView extends GLSurfaceView {
    private static final String TAG = "GLES2JNI";
    private static final boolean DEBUG = true;
    private static AssetManager assets = null;
    private Renderer renderer = null;

    private String[] mFileNames;
    private int mFileIndex = -1;

    private ScaleGestureDetector mScaleDetector;
    private GestureDetector  mRotateDetector;

    public GLES2JNIView(Context context) {
        super(context);
        // Pick an EGLConfig with RGB8 color, 16-bit depth, no stencil,
        // supporting OpenGL ES 2.0 or later backwards-compatible versions.
        setEGLConfigChooser(8, 8, 8, 0, 16, 0);
        setEGLContextClientVersion(2);
        renderer = new Renderer();
        setRenderer(renderer);
        mScaleDetector = new ScaleGestureDetector(context, new ScaleListener());
        mRotateDetector = new GestureDetector(context, new RotateListener());
        assets = getContext().getAssets();

        FindAllModels();
    }

    private void FindAllModels() {
        try {
            mFileNames = assets.list("Models");
        } catch (IOException e) {
            //TODO: CRITICAL ERROR!
        }
    }

    public void onZoom(float zoom) {
        GLES2JNILib.zoom(zoom);
    }

    public void onRotate(float rotateX, float rotateY) {
        GLES2JNILib.rotate(rotateX, rotateY);
    }

    public void onLoadNext() {
        ++mFileIndex;
        mFileIndex %= mFileNames.length;

        queueEvent(new Runnable() {
            @Override
            public void run() {
                renderer.onLoadNext(assets, mFileNames[mFileIndex]);
            }
        });
    }

    private static class Renderer implements GLSurfaceView.Renderer {
        public void onDrawFrame(GL10 gl) {
            GLES2JNILib.step();
        }

        public void onSurfaceChanged(GL10 gl, int width, int height) {
            GLES2JNILib.resize(width, height);
        }

        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            GLES2JNILib.init();
        }

        public void onLoadNext(AssetManager assets, String file){
            GLES2JNILib.load(assets, file);
        }
    }

    @Override
    public boolean onTouchEvent(MotionEvent ev) {
        mRotateDetector.onTouchEvent(ev);
        mScaleDetector.onTouchEvent(ev);
        return true;
    }

    private class RotateListener extends GestureDetector.SimpleOnGestureListener {

        @Override
        public boolean onDown(MotionEvent e) {
            return true;
        }

        @Override
        public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY) {
            onRotate(distanceX, distanceY);

            return true;
        }

        @Override
        public void onLongPress(MotionEvent e) {
            onLoadNext();
        }
    }

    private class ScaleListener extends ScaleGestureDetector.SimpleOnScaleGestureListener {

        @Override
        public boolean onScale(ScaleGestureDetector detector) {
            onZoom(detector.getScaleFactor());

            return true;
        }
    }
}
