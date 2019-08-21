import java.awt.image.BufferedImage;
import java.io.*;
import java.util.zip.*;
import java.util.Enumeration;
import javax.imageio.*;
//import java.lang.*;


public class Reader implements Runnable{
	File inputFile;
	int numDecoders;
	
	Reader(File file, int numDecoders){
		this.inputFile = file;
		this.numDecoders = numDecoders;
	}
	
	@Override
	public void run(){
		try{
			ZipFile zFile = new ZipFile(inputFile);
			Enumeration<? extends ZipEntry> entries = zFile.entries();
			int count = 0;
			while(entries.hasMoreElements()){
				Tuple t = Blackboard.get("OK");
				ZipEntry ze = entries.nextElement();
				String name = ze.getName();
				
				//System.out.println("READING!!!!");
				BufferedInputStream is = new BufferedInputStream(zFile.getInputStream(ze));
				long size = ze.getCompressedSize();
				byte[] b = new byte[(int)size];
				int offset = 0;
				
				int n = 0;
				while((n = is.read(b, 0, (int)size)) != -1){
					offset += n;
				}
				
				/*
				BufferedImage bImg = ImageIO.read(new ByteArrayInputStream(b));
				try{
					ImageIO.write(bImg,"png",new java.io.File("pic1.png"));
				}catch(Exception e){
					System.exit(9);
				}
				*/
				
				Blackboard.put("Frame" + Integer.toString(count),b,name);
				is.close();
				//System.out.println(Integer.toString(count));
				count = count + 1;
				
			}
			for(int i = 0; i < numDecoders; i++){
				System.out.println("READER DONE!!!!");
				Blackboard.put("ReaderDone");
			}
			zFile.close();
		}catch(FileNotFoundException FNFE){
			System.out.println(FNFE);
		}catch(IOException IOE){
			System.out.println(IOE);
		}
	}
}