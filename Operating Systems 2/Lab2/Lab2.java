
import javax.swing.*;
import java.awt.image.*;
import javax.imageio.*;
import java.io.*;
import java.awt.*;
import java.util.*;

class tClass implements Runnable{
	private final BufferedImage img;
	private final int w;
	private final int h;
	private final int h2;
	int count;
	
	tClass(BufferedImage img, int w, int h, int h2){
		this.img = img;
		this.w = w;
		this.h = h;
		this.h2 = h2;
		this.count = 0;
	}
	
	public void run(){
		for(int j = 0; j < w; j++){
			for(int k = h; k < h2; k++){
				Color c = new Color(img.getRGB(j,k));
				if(c.equals(Color.WHITE)){
					count++;
				}
				//System.out.println("Thread: " + getName() + " running");
			}
		}
	}
	
	public int getCount(){
		return count;
	}
	
}


public class Lab2{
	public static void main(String[] args){
		
		 int numthreads = Integer.parseInt(JOptionPane.showInputDialog(null, "Num threads?"));
		 
        JFileChooser jf = new JFileChooser(".");
		
        if( jf.showOpenDialog(null) != JFileChooser.APPROVE_OPTION)
            System.exit(0);
		
		try{
			BufferedImage img = ImageIO.read(jf.getSelectedFile());
			int w = img.getWidth();
			int h = img.getHeight();
			int h2 = (h/numthreads) + 1;
		
			ArrayList<Thread> tList = new ArrayList<>();
			ArrayList<tClass> cList = new ArrayList<>();
			int whiteCount = 0;
			int i = 0;
			
			//Time start
			double start = System.nanoTime();
			
			for(i = 0; i < numthreads; i++){
				if(((h2*(i+1))) < h){
					cList.add(i, new tClass(img,w,h2*i,(h2*(i+1))));
				}else{
					cList.add(i, new tClass(img,w,h2*i,h));
				}
				tList.add(i, new Thread(cList.get(i)));
			}
			
			for(i = 0; i < numthreads; i++){
				tList.get(i).start();
			}
			for(i = 0; i < numthreads; i++){
				tList.get(i).join();
				whiteCount += cList.get(i).getCount();
			}
			
			double end = System.nanoTime();
				
			double total = end - start;
				
			total = (double)(total*0.000000001);
			
			System.out.println("The amount of threads is: " + numthreads);
			
			System.out.println("The amount of white pixels is: " + whiteCount);
			
			System.out.println("Result:" + total + " seconds");
			
		}catch(IOException e){
			
		}catch(InterruptedException err){
			
		}
		
	}
}