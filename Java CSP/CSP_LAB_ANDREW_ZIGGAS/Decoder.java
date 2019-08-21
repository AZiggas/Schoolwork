import java.awt.image.BufferedImage;
import javax.imageio.*;
import java.io.*;

public class Decoder implements Runnable{

	BufferedChannel<Tuple> decoderUnread;
	BufferedChannel<Tuple> decoded;
	
	Decoder(BufferedChannel<Tuple> decoderUnread, BufferedChannel<Tuple> decoded){
		this.decoderUnread = decoderUnread;
		this.decoded = decoded;
	}
	
	@Override
	public void run(){
		while(true){
			Tuple t = decoderUnread.get();
			if(t.tag.equals("ReaderDone")){
				break;
			}
			//Do stuff
			//System.out.println("DECODING!!!!");
			byte[] b = (byte[])t.data[0];
			String name = (String)t.data[1];
			int count = (int)t.data[2];
			
			try{
				BufferedImage bImg = ImageIO.read(new ByteArrayInputStream(b));
				
				Object[] data = new Object[5];
				
				data[0] = bImg.getRGB(0,0, bImg.getWidth(), bImg.getHeight(), null, 0, bImg.getWidth());
				data[1] = name;
				data[2] = count;
				data[3] = bImg.getWidth();
				data[4]  = bImg.getHeight();
				//System.out.println("YES");
				
				decoded.put(new Tuple(t.tag, data));
				
			}catch(IOException IOE){
				System.out.println(IOE);
			}
		}
		System.out.println("DECODER DONE!!!!");
		Object[] obArr = new Object[1];
		decoded.put(new Tuple("DecoderDone",obArr));
	}
}