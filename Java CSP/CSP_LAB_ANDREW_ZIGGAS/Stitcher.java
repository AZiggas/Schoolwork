import java.awt.image.BufferedImage;
import java.lang.Math.*;
import javax.imageio.*;
import java.util.*;
//import java.lang.*;

public class Stitcher implements Runnable{
	
	class Record{
		String filename;
		int[] img;
		boolean Prev;
		boolean Next;
		Record(String f, int[] i){
			filename=f;
			img=i;
		}
	}
	int numComparators;
	int numDecoders;

	int doneCount;
	BufferedChannel<Tuple> decoded;
	BufferedChannel<Tuple> compare;
	
	Stitcher(int numComparators, int numDecoders, BufferedChannel<Tuple> decoded, BufferedChannel<Tuple> compare){
		this.numComparators = numComparators;
		this.numDecoders = numDecoders;
		this.decoded = decoded;
		this.compare = compare;
		this.doneCount = 0;
	}
	
	@Override
	public void run(){
		Map<Integer, Record> map = new TreeMap<>();
		
		//Pull 1 frame. Pull another. Check if frame2 is next to frame1 if so compare. Else throw back in.
		//Only one stitcher. Get frame 0. have counter start at 0. 
		while(true){
			//This will first be placed into our main
			Tuple input = decoded.get();
			if(input.tag.equals("DecoderDone")){
				doneCount += 1;
				if(doneCount >= numDecoders){
					break;
				}
			}else{
				int[] buffArray = (int[])input.data[0];
				String name = (String)input.data[1];
				int count = (int)input.data[2];
				
				Record r = new Record(name, buffArray);
				if(count == 0){
					r.Prev=true;
				}
				
				if(map.containsKey(count-1)){
					Record rPrev = map.get(count-1);
					Object[] data = new Object[6];
					data[0] = rPrev.filename;
					data[1] = r.filename;
					data[2] = rPrev.img;
					data[3] = r.img;
					data[4] = (int)input.data[3];
					data[5] = (int)input.data[4];
					compare.put(new Tuple("pair",data));
					rPrev.Next = true;
					r.Prev = true;
					if(rPrev.Next && rPrev.Prev){
						map.remove(count-1);
					}
					
				}
				
				if(map.containsKey(count+1)){
					Record rNext = map.get(count+1);
					Object[] data = new Object[6];
					data[0] = r.filename;
					data[1] = rNext.filename;
					data[2] = r.img;
					data[3] = rNext.img;
					data[4] = (int)input.data[3];
					data[5] = (int)input.data[4];
					compare.put(new Tuple("pair",data));
					rNext.Prev=true;
					r.Next=true;
					if(rNext.Next && rNext.Prev){
						map.remove(count+1);
					}
				}
				
				if(!r.Next || !r.Prev){
					map.put(count, r);
				}
			}
		}
		Object[] obArr = new Object[1];
		for(int i = 0; i < numComparators; i++){
			compare.put(new Tuple("StitcherDone", obArr));
		}
		System.out.println("Stitcher DONE!!!!");
	}
}