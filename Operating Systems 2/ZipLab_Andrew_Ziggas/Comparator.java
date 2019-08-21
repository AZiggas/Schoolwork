import java.awt.image.BufferedImage;
import java.lang.Math.*;
import javax.imageio.*;
//import java.lang.*;

public class Comparator implements Runnable{
	int numComparators;
	int numDecoders;
	int startNum;
	Comparator(int numComparators, int numDecoders, int startNum){
		this.numComparators = numComparators;
		this.numDecoders = numDecoders;
		this.startNum = startNum;
	}
	
	@Override
	public void run(){
		Tuple t1;
		Tuple t2;
		int start = startNum;
		while(true){
			//This will first be placed into our main
			Tuple count = Blackboard.get("doneCount");
			t2 = Blackboard.get("DecodedFrame" + Integer.toString(startNum+1), "DecoderDone");
			if(t2.tag.equals("DecoderDone")){
				int value = ((int)count.data[0]) + 1;
				Blackboard.put("doneCount", value);
				if(value >= numDecoders){
					break;
				}
			}
			else{
				Blackboard.put("doneCount", (int)count.data[0]);
				t1 = Blackboard.get("DecodedFrame" + Integer.toString(startNum));
				
				//System.out.println("COMPARING!!!!");
				
				int[] pixelArray1 = (int[])t1.data[0];
				int[] pixelArray2 = (int[])t2.data[0];
				
				int pixelChange = 0;
				int pixelChangeThreshold = ((int)t1.data[2]*(int)t1.data[3])/100;
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
					//System.out.println(start);
					Blackboard.put("Out", (String)t1.data[1], (String)t2.data[1]);
				}
				Blackboard.put("OK");
				//System.out.println((String)t1.data[1]);
				startNum = startNum + numComparators;
			}
			if((int)count.data[0]+1 >= numDecoders){
				break;
			}
		}
		Blackboard.put("ComparatorDone");
		System.out.println("COMPARATOR DONE!!!!");
	}
}