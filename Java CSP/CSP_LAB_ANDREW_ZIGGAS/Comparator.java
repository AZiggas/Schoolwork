import java.awt.image.BufferedImage;
import java.lang.Math.*;
import javax.imageio.*;
//import java.lang.*;

public class Comparator implements Runnable{
	BufferedChannel<Tuple> compare;
	BufferedChannel<Tuple> output;
	BufferedChannel<String> readerOk;
	
	Comparator(BufferedChannel<Tuple> compare, BufferedChannel<Tuple> output, BufferedChannel<String> readerOk){
		this.compare = compare;
		this.output = output;
		this.readerOk = readerOk;
	}
	
	@Override
	public void run(){
		while(true){
			//This will first be placed into our main
			Tuple input = compare.get();
			if(input.tag.equals("StitcherDone")){
				break;
			}else{
				//System.out.println("COMPARING!!!!");
				int[] pixelArray1 = (int[])input.data[2];
				int[] pixelArray2 = (int[])input.data[3];
				
				int pixelChange = 0;
				int pixelChangeThreshold = ((int)input.data[4]*(int)input.data[5])/100;
				int changeThreshold = 35; //Amount the value has changed before it is counted.
				int change = 0;
				int pixel1;
				int pixel2;
				for(int y = 0; y < pixelArray1.length; y++){
						pixel1 = pixelArray1[y];
						pixel2 = pixelArray2[y];
						
						change = Math.abs(((pixel1 >> 16) & 0xFF) - ((pixel2 >> 16) & 0xFF)); //Difference of red values
						if(change >= changeThreshold){
							pixelChange = pixelChange + 1;
						}
				}
				if(pixelChange >= pixelChangeThreshold){
					//System.out.println("IMAGE CHANGE!!!!");
					Object[] data = new Object[2];
					data[0] = (String)input.data[0];
					data[1] = (String)input.data[1];

					output.put(new Tuple("Out", data));
				}
				readerOk.put("OK");
			}
		}
		Object[] obArr = new Object[1];
		output.put(new Tuple("ComparatorDone", obArr));
		System.out.println("COMPARATOR DONE!!!!");
	}
}