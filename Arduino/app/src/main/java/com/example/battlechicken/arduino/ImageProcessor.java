package com.example.battlechicken.arduino;

import java.util.Arrays;

//Read more: http://javarevisited.blogspot.com/2011/12/read-write-image-in-java-example.html#ixzz3zg8PTHgo


/**
 * Created by Battlechicken on 2016-01-22.
 */
public class ImageProcessor {
	public class NV21Image {
		public byte[] data;
		private int width;
		private int height;
		public NV21Image(byte[] data, int w, int h) {
			this.data = data;
			this.height = h;
			this.width = w;
		}
		public int getGray(int x, int y) {
	        //byte signedGray = data[y + width*x];
	        byte signedGray = data[x + width*y];
	        //int gray = signedGray >= 0 ? signedGray : 256 + signedGray;
			int gray = signedGray + 128;
	        return gray;
	    }
		
		public void setGray(int x, int y, int gray) {
			data[x + width*y] = (byte) (gray);
		}
		public int clamp(int x, int a, int b) {
			if( x < a) return a;
			if( x > b) return b;
			return x;
		}
		
		public int getMedian(int x, int y, int size) {
			
			//int[] values = new int[(size*2 + 1)*(2*size+1)];
			//int current = 0;
			int whites = 0;
			int blacks = 0;
			for( int i = - size; i <= size; ++i) {
				for( int j = - size; j <= size; ++j) {
					int _x = clamp(x+i, 0, width-1);
					int _y = clamp(y+j, 0, height-1);
					//values[current++] = getGray(_x, _y);
					if( getGray(_x, _y) < 128 ) blacks++;
					else whites++;
				}
			}
			//Arrays.sort(values);
			//return values[values.length/2];
			return ( whites > blacks ? 255 : 0); 
		}
		
		public void ApplyMedian( int size ) {
			byte[] copy = data.clone();
			NV21Image original = new NV21Image(copy, width, height);
			
			for (int i = 0; i < width; i++) {
				for (int j = 0; j < height; j++) {
					int gray = original.getMedian(i, j, size);
					this.setGray(i, j, gray);
				}
			}
		}
		
		void TryFindTank( Result result) {
			int blackPixels = 0;
			double sumX = 0;
			double sumY = 0;
			
			for (int i = 0; i < width; i++) {
				for (int j = 0; j < height; j++) {
					int gray = getGray(i, j);
					if( gray < 128 ) {
						blackPixels++;
						sumX += (double)i/(double)width;
						sumY += (double)j/(double)height;
					}
				}
			}
			result.blackPixels = blackPixels;
			result.x = sumX / blackPixels;
			result.y = sumY / blackPixels;
			result.success = blackPixels > 600;
		}

	}

    public static class Result {
    	public int blackPixels = 0;
        public double y = 0;
        public double x = 0;
        public boolean success = false;
        public NV21Image image;
    }

    public Result process(byte[] data, int width, int height) {
        Result result = new Result();
        
        NV21Image image = new NV21Image(data, width, height); 
        result.image = image;
        //YuvImage yuv = new YuvImage(image, ImageFormat.NV21, width, height, null);

        result.success = true;
        result.x = 0.0f;
        result.y = 0.0f;
        
        
        /*IntBuffer intBuf =
		   ByteBuffer.wrap(data)
		     .order(ByteOrder.BIG_ENDIAN)
		     .asIntBuffer();
		 int[] array = new int[intBuf.remaining()];*/
		 //intBuf.get(array);
		 
        // k�s�b�l�s
        for (int i = 0; i < data.length; i++) {
        	//if( (int)(data[i] < 0 ? data[i] + 255 : data[i]) < 35 ) {
        	//if( (int)(data[i] & 0xff) < 35 )
        	
        	int gray = (int)data[i];
        	if( ((gray) < 0 ? gray + 256 : gray)  < 35 ) {
        		data[i] = -128;
        	} else {
        		data[i] = 127;
        	}
		}
        
        //image.ApplyMedian(10);
        
        image.TryFindTank(result);
        
        //result.bitmap = bitmap;
        //FileOutputStream fos = new FileOutputStream()
        //bitmap.compress(Bitmap.CompressFormat.PNG, null, out)

        return result;
        //throw Exception("hah");
    }

}
