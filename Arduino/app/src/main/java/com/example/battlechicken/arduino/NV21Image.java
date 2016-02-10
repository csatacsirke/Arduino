package com.example.battlechicken.arduino;

import android.graphics.Bitmap;
import android.graphics.Color;


/**
 * Created by Battlechicken on 2016-02-09.
 */

public class NV21Image {
    byte[] data;
    int width;
    int height;

    public NV21Image(byte[] data, int width, int height){
        this.data = data;
        this.width = width;
        this.height = height;
    }

    public int getGray(int x, int y) {
        //byte signedGray = data[y + width*x];
        byte signedGray = data[x + width*y];
        int gray = signedGray >= 0 ? signedGray : 256 + signedGray;
        return gray;
    }

    public Bitmap ToGrayBitmap() {
        //Bitmap bitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
        Bitmap bitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);

        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                int gray = getGray(i, j);
                bitmap.setPixel(i,j, Color.argb(255, gray, gray, gray));
            }
        }
        return bitmap;
    }

}

