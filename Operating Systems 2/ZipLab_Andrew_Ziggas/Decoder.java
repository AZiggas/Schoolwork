import java.awt.image.BufferedImage;
import javax.imageio.*;
import java.io.*;

public class Decoder implements Runnable{
	int startNum;
	int numDecoders;
	
	Decoder(int startNum, int numDecoders){
		this.startNum = startNum;
		this.numDecoders = numDecoders;
	}
	
	@Override
	public void run(){
		while(true){
			try{
				Tuple t = Blackboard.get("Frame"+ Integer.toString(startNum), "ReaderDone");
				if(t.tag.equals("ReaderDone")){
					break;
				}
				//System.out.println("DECODING!!!!");
				byte[] b = (byte[])t.data[0];
				BufferedImage bImg = ImageIO.read(new ByteArrayInputStream(b));
				
				int[] buffArray = bImg.getRGB(0,0, bImg.getWidth(), bImg.getHeight(), null, 0, bImg.getWidth());
				
				if(t.tag.equals("Frame0")){
					//System.out.println("YES");
					Blackboard.put("DecodedFrame" + Integer.toString(startNum), buffArray,t.data[1], bImg.getWidth(), bImg.getHeight());
				}
				else{
					Blackboard.put("DecodedFrame" + Integer.toString(startNum), buffArray, t.data[1], bImg.getWidth(), bImg.getHeight());
					Blackboard.put("DecodedFrame" + Integer.toString(startNum), buffArray, t.data[1], bImg.getWidth(), bImg.getHeight());
				}
				startNum = startNum + numDecoders;
			}catch(IOException IOE){
				System.out.println(IOE);
			}
		}
		System.out.println("DECODER DONE!!!!");
		Blackboard.put("DecoderDone");
	}
}