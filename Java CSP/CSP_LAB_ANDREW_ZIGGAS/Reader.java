import java.awt.image.BufferedImage;
import java.io.*;
import java.util.zip.*;
import java.util.Enumeration;
import javax.imageio.*;
//import java.lang.*;


public class Reader implements Runnable{
	File inputFile;
	int numDecoders;
	BufferedChannel<String> readerOk;
	BufferedChannel<Tuple> decoderUnread;
	
	Reader(File file, int numDecoders, BufferedChannel<String> readerOk, BufferedChannel<Tuple> decoderUnread){
		this.inputFile = file;
		this.numDecoders = numDecoders;
		this.readerOk = readerOk;
		this.decoderUnread = decoderUnread;
	}
	
	@Override
	public void run(){
		try{
			ZipFile zFile = new ZipFile(inputFile);
			Enumeration<? extends ZipEntry> entries = zFile.entries();
			int count = 0;
			while(entries.hasMoreElements()){
				readerOk.get();
				
				ZipEntry ze = entries.nextElement();
				String name = ze.getName();
				
				//System.out.println("READING!!!!");
				BufferedInputStream is = new BufferedInputStream(zFile.getInputStream(ze));
				long size = ze.getSize();
				byte[] b = new byte[(int)size];
				int offset = 0;
				
				is.read(b);
				/*
				int n = 0;
				while((n = is.read(b, 0, (int)size)) != -1){
					offset += n;
				}
				*/
				
				Object[] data = new Object[3];
				data[0] = b;
				data[1] = name;
				data[2] = count;
				
				decoderUnread.put(new Tuple(Integer.toString(count), data));
				
				
				is.close();
				//System.out.println(Integer.toString(count));
				count = count + 1;
				
			}
			for(int i = 0; i < numDecoders; i++){
				System.out.println("READER DONE!!!!");
				Object[] obArr = new Object[1];
				
				decoderUnread.put(new Tuple("ReaderDone",obArr));
			}
			zFile.close();
		}catch(FileNotFoundException FNFE){
			System.out.println(FNFE);
		}catch(IOException IOE){
			System.out.println(IOE);
		}
	}
}